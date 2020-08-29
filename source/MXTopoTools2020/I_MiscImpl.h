#pragma once

#include "Util.h"
#include "Codes.h"
#include "DiffPFOR.h"
#include "Helpers.h"

class I_MiscImpl {


private:
	static inline i32 _compressIntArray(
		DiffPFOR& c,
		const List<i32>& src,
		const i32 srcLen,
		List<i32>& dst
	) {
		DbgAssert(srcLen >= 0 && srcLen <= src.length());
		// Make sure the src can be stored at all.
		const i32 minLen = DiffPFOR::minCapacity(srcLen);
		if (minLen < 0) {
			// Too large.
			return minLen;
		}
		const i32 oldLen = dst.length();
		const i32 minCap = oldLen + minLen;
		if (minCap < 0) {
			// not enough space can be made available for a worst case
			return DiffPFOR::Errors::dst_overfull;
		}
		dst.setLength(minCap);
		// compress everything
		const i32 num = c.compress(src, 0, srcLen, dst, oldLen, dst.length() - oldLen);
		if (num < 0) {
			dst.setLength(oldLen);	// revert change
			return num;
		}
		dst.setLength(oldLen + num);
		return num;
	}

	static inline i32 _decompressIntArray(
		DiffPFOR& c,
		const List<i32>& src,
		const i32 off,
		List<i32>& dst
	) {
		// Make sure dst has the space.
		const i32 oldLen = dst.length();
		const i32 rawLen = DiffPFOR::decompressedLength(src, off);
		if (rawLen < 0) {
			return rawLen;
		}
		dst.setLength(rawLen);
		// Decompress into dst.
		return c.decompress(src, off, src.length() - off, dst, 0, rawLen);
	}

	static inline i32 tryCompress(const i32 r, DiffPFOR& c, const List<i32>& src, List<i32>& dst) {
		if (r < 0) {
			return r;
		}
		const i32 x = _compressIntArray(c, src, src.length(), dst);
		if (x < 0) {
			return x;
		}
		return r + x;
	}
	static inline i32 tryDecompress(const i32 r, DiffPFOR& c, const List<i32>& src, List<i32>& dst) {
		if (r < 0) {
			return r;
		}
		const i32 x = _decompressIntArray(c, src, r, dst);
		if (x < 0) {
			return x;
		}
		return r + x;
	}

public:
	static inline i32 compressIntArray(
		const List<i32>& src,
		List<i32>& dst
	) {
		using ERR = Codes::I_Misc_compressIntArray;
		// Allocate workspace once.
		DiffPFOR c;
		// Compress with same workspace.
		const i32 x = dst.length();
		i32 r = 0;
		r = tryCompress(r, c, src, dst);
		if (r >= 0) {
			DbgAssert(r == dst.length() - x);	// this is just for my own peace of mind
			return r;
		}
		return ERR::resolve(r);
	}

public:
	static inline i32 compressIntArrays(
		const List<FPValue*>& src,
		List<i32>& dst
	) {
		using ERR = Codes::I_Misc_compressIntArrays;
		List<Tab<i32>*> tabs(src.length());
		// check for all int tabs
		i32 r = Helpers::Misc::extractIntTabs(src, tabs);
		if (r < 0) {
			return ERR::resolveTabs(r);
		}
		// Allocate workspace once.
		DiffPFOR c;
		// Compress with same workspace.
		const i32 x = dst.length();
		r = 0;
		for (i32 i = 0; i < tabs.length() && r >= 0; ++i) {
			const List<i32> list(*tabs[i]);
			r = tryCompress(r, c, list, dst);
		}
		if (r >= 0) {
			DbgAssert(r == dst.length() - x);
			return r;
		}
		return ERR::resolveComp(r);
	}
	//public:
	//	static inline i32 compressIntArray1(
	//		const List<i32>& src1,
	//		List<i32>& dst
	//	) {
	//		using ERR = Codes::I_Misc_compressIntArray1;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Compress with same workspace.
	//		const i32 x = dst.length();
	//		i32 r = 0;
	//		r = tryCompress(r, c, src1, dst);
	//		if (r >= 0) {
	//			DbgAssert(r == dst.length() - x);	// this is just for my own peace of mind
	//			return r;
	//		}
	//		return ERR::resolve(r);
	//	}

	//public:
	//	static inline i32 compressIntArray2(
	//		const List<i32>& src1,
	//		const List<i32>& src2,
	//		List<i32>& dst
	//	) {
	//		using ERR = Codes::I_Misc_compressIntArray2;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Compress with same workspace.
	//		const i32 x = dst.length();
	//		i32 r = 0;
	//		r = tryCompress(r, c, src1, dst);
	//		r = tryCompress(r, c, src2, dst);
	//		if (r >= 0) {
	//			DbgAssert(r == dst.length() - x);
	//			return r;
	//		}
	//		return ERR::resolve(r);
	//	}

	//public:
	//	static inline i32 compressIntArray4(
	//		const List<i32>& src1,
	//		const List<i32>& src2,
	//		const List<i32>& src3,
	//		const List<i32>& src4,
	//		List<i32>& dst
	//	) {
	//		using ERR = Codes::I_Misc_compressIntArray4;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Compress with same workspace.
	//		const i32 x = dst.length();
	//		i32 r = 0;
	//		r = tryCompress(r, c, src1, dst);
	//		r = tryCompress(r, c, src2, dst);
	//		r = tryCompress(r, c, src3, dst);
	//		r = tryCompress(r, c, src4, dst);
	//		if (r >= 0) {
	//			DbgAssert(r == dst.length() - x);
	//			return r;
	//		}
	//		return ERR::resolve(r);
	//	}

	//public:
	//	static inline i32 compressIntArray8(
	//		const List<i32>& src1,
	//		const List<i32>& src2,
	//		const List<i32>& src3,
	//		const List<i32>& src4,
	//		const List<i32>& src5,
	//		const List<i32>& src6,
	//		const List<i32>& src7,
	//		const List<i32>& src8,
	//		List<i32>& dst
	//	) {
	//		using ERR = Codes::I_Misc_compressIntArray8;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Compress with same workspace.
	//		const i32 x = dst.length();
	//		i32 r = 0;
	//		r = tryCompress(r, c, src1, dst);
	//		r = tryCompress(r, c, src2, dst);
	//		r = tryCompress(r, c, src3, dst);
	//		r = tryCompress(r, c, src4, dst);
	//		r = tryCompress(r, c, src5, dst);
	//		r = tryCompress(r, c, src6, dst);
	//		r = tryCompress(r, c, src7, dst);
	//		r = tryCompress(r, c, src8, dst);
	//		if (r >= 0) {
	//			DbgAssert(r == dst.length() - x);
	//			return r;
	//		}
	//		return ERR::resolve(r);
	//	}

public:
	static inline i32 decompressIntArray(
		const List<i32>& src,
		const i32 off,
		List<i32>& dst
	) {
		using ERR = Codes::I_Misc_decompressIntArray;
		// Allocate workspace once.
		DiffPFOR c;
		// Decompress with same workspace.
		i32 r = off;
		r = tryDecompress(r, c, src, dst);
		return r < 0 ? ERR::resolve(r) : (r - off);
	}

public:
	static inline i32 decompressIntArrays(
		const List<i32>& src,
		const i32 off,
		const List<FPValue*>& dst
	) {
		using ERR = Codes::I_Misc_decompressIntArrays;
		List<Tab<i32>*> tabs(dst.length());
		// check for all int tabs
		i32 r = Helpers::Misc::extractIntTabs(dst, tabs);
		if (r < 0) {
			return ERR::resolveTabs(r);
		}
		// Allocate workspace once.
		DiffPFOR c;// Decompress with same workspace.
		r = off;
		for (i32 i = 0; i < tabs.length() && r >= 0; ++i) {
			List<i32> list(*tabs[i]);
			r = tryDecompress(r, c, src, list);
		}
		return r < 0 ? ERR::resolveComp(r) : (r - off);
	}

	//public:
	//	static inline i32 decompressIntArray1(
	//		const List<i32>& src,
	//		const i32 off,
	//		List<i32>& dst1
	//	) {
	//		using ERR = Codes::I_Misc_decompressIntArray1;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Decompress with same workspace.
	//		i32 r = off;
	//		r = tryDecompress(r, c, src, dst1);
	//		return r < 0 ? ERR::resolve(r) : (r - off);
	//	}

	//public:
	//	static inline i32 decompressIntArray2(
	//		const List<i32>& src,
	//		const i32 off,
	//		List<i32>& dst1,
	//		List<i32>& dst2
	//	) {
	//		using ERR = Codes::I_Misc_decompressIntArray2;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Decompress with same workspace.
	//		i32 r = off;
	//		r = tryDecompress(r, c, src, dst1);
	//		r = tryDecompress(r, c, src, dst2);
	//		return r < 0 ? ERR::resolve(r) : (r - off);
	//	}

	//public:
	//	static inline i32 decompressIntArray4(
	//		const List<i32>& src,
	//		const i32 off,
	//		List<i32>& dst1,
	//		List<i32>& dst2,
	//		List<i32>& dst3,
	//		List<i32>& dst4
	//	) {
	//		using ERR = Codes::I_Misc_decompressIntArray4;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Decompress with same workspace.
	//		i32 r = off;
	//		r = tryDecompress(r, c, src, dst1);
	//		r = tryDecompress(r, c, src, dst2);
	//		r = tryDecompress(r, c, src, dst3);
	//		r = tryDecompress(r, c, src, dst4);
	//		return r < 0 ? ERR::resolve(r) : (r - off);
	//	}

	//public:
	//	static inline i32 decompressIntArray8(
	//		const List<i32>& src,
	//		const i32 off,
	//		List<i32>& dst1,
	//		List<i32>& dst2,
	//		List<i32>& dst3,
	//		List<i32>& dst4,
	//		List<i32>& dst5,
	//		List<i32>& dst6,
	//		List<i32>& dst7,
	//		List<i32>& dst8
	//	) {
	//		using ERR = Codes::I_Misc_decompressIntArray8;
	//		// Allocate workspace once.
	//		DiffPFOR c;
	//		// Decompress with same workspace.
	//		i32 r = off;
	//		r = tryDecompress(r, c, src, dst1);
	//		r = tryDecompress(r, c, src, dst2);
	//		r = tryDecompress(r, c, src, dst3);
	//		r = tryDecompress(r, c, src, dst4);
	//		r = tryDecompress(r, c, src, dst5);
	//		r = tryDecompress(r, c, src, dst6);
	//		r = tryDecompress(r, c, src, dst7);
	//		r = tryDecompress(r, c, src, dst8);
	//		return r < 0 ? ERR::resolve(r) : (r - off);
	//	}

public:
	static inline i32 toBitset(
		BitArray& src,
		Bitset& dst
	) {
		dst.readBits(src);
		return Codes::OK;
	}

public:
	static inline i32 toBitArray(
		Bitset& src,
		BitArray& dst
	) {
		src.writeBits(dst);
		return Codes::OK;
	}

	// make naturally 0-indexed input/output 1-indexed
private:
	static constexpr i32 ONE_INDEX = 1;

public:
	static inline i32 buildSwapSequence(
		const List<const wchar_t*>& values,
		const i32 from,
		const i32 count,
		List<i32>& sequence,
		List<i32>& trans,
		const bool ascending
	) {
		return buildSwapSequenceImpl(values, from - ONE_INDEX, count, sequence, trans, ascending);
	}

private:
	static inline i32 buildSwapSequenceImpl(
		const List<const wchar_t*>& values,
		const i32 from,
		const i32 count,
		List<i32>& sequence,
		List<i32>& _trans,
		const bool ascending
	) {
		using ERR = Codes::I_Misc_buildSwapSequence;
		if (from < 0 || count < 0 || from > values.length() - count) {
			return ERR::bad_bounds;
		}
		if (count > 0x20000000) {
			// so that if doubled it's still purely non-netative. may be a bit too cautious here...
			return ERR::too_large;
		}
		sequence.setLength(count);
		_trans.setLength(count);
		if (count > 0) {
			// setup arrays
			const i32 adjust = from + ONE_INDEX;

			// working with pointers is actually easier here.
			i32* swap = sequence.addr();
			i32* trans = _trans.addr();

			// allocate a helper array
			List<i32> _invert(count);
			i32* invert = _invert.addr();

			// pre-fill swap and invert with self-matching values for range
			for (i32 i = 0; i < count; ++i) {
				swap[i] = i + adjust;
			}
			for (i32 i = 0; i < count; ++i) {
				invert[i] = i + from;
			}
			i32* swap_b = trans;
			i32* swap_a = swap;
			if (ascending) {
				// ascending merge-sort. 
				for (i32 w = 1, u = 2; w < count; w = u, u += u) {
					for (i32 l = 0; l < count; ) {
						const i32 r = Util::mathMin(l + w, count);
						const i32 e = Util::mathMin(l + u, count);
						for (i32 i = l, j = r; l < e; ++l) {
							if (i < r && (j >= e || Util::cmpCase(values[swap_a[i] - ONE_INDEX], values[swap_a[j] - ONE_INDEX]) <= 0)) {
								swap_b[l] = swap_a[i];
								invert[swap_a[i++] - adjust] = l;
							}
							else {
								swap_b[l] = swap_a[j];
								invert[swap_a[j++] - adjust] = l;
							}
						}
					}
					// swap the work arrays
					i32* t = swap_a;
					swap_a = swap_b;
					swap_b = t;
				}
			}
			else {
				// descending stable order, as if ascending order was fully reversed
				for (i32 w = 1, u = 2; w < count; w = u, u += u) {
					for (i32 r = count; r > 0; ) {
						const i32 l = Util::mathMax(0, r - w);
						const i32 e = Util::mathMax(0, r - u);
						for (i32 i = r - 1, j = l - 1; r > e; ) {
							if (i >= l && (j < e || Util::cmpCase(values[swap_a[i] - ONE_INDEX], values[swap_a[j] - ONE_INDEX]) <= 0)) {
								swap_b[--r] = swap_a[i];
								invert[swap_a[i--] - adjust] = r;
							}
							else {
								swap_b[--r] = swap_a[j];
								invert[swap_a[j--] - adjust] = r;
							}
						}
					}
					// swap the work arrays
					i32* t = swap_a;
					swap_a = swap_b;
					swap_b = t;
				}
			}
			// complete the array swap with correct destination if needed
			if (swap != swap_a) {
				std::copy(trans, trans + count, swap);
			}
			// generate trans table from inverted helper
			for (i32 i = 0; i < count; ++i) {
				trans[i] = invert[i] + adjust;
			}
			// finally, transform the swap table into a swap sequence using the inverted helper
			for (i32 i = 0; i < count; ++i) {
				const i32 y = invert[i];
				const i32 x = swap[i];
				swap[y] = x;
				invert[x - adjust] = y;
			}
		}
		return Codes::OK;
	}
};
