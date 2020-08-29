#pragma once

#include "I_MiscImpl.h"

extern const Interface_ID I_MISC_ID;

class I_Misc : public FPInterfaceDesc
{

public:


	//	i32 compressIntArray1(
	//		IntTab& src1,
	//		IntTab& dst
	//	) {
	//		const List<i32> _src1(src1);
	//		List<i32> _dst(dst);
	//		return I_MiscImpl::compressIntArray1(_src1, _dst);
	//	}

		// optimization, so that 1/2th of the workspace is needed overall
	//	i32 compressIntArray2(
	//		IntTab& src1,
	//		IntTab& src2,
	//		IntTab& dst
	//	) {
	//		const List<i32> _src1(src1);
	//		const List<i32> _src2(src2);
	//		List<i32> _dst(dst);
	//		return I_MiscImpl::compressIntArray2(_src1, _src2, _dst);
	//	}

		// optimization, so that 1/4th of the workspace is needed overall
	//	i32 compressIntArray4(
	//		IntTab& src1,
	//		IntTab& src2,
	//		IntTab& src3,
	//		IntTab& src4,
	//		IntTab& dst
	//	) {
	//		const List<i32> _src1(src1);
	//		const List<i32> _src2(src2);
	//		const List<i32> _src3(src3);
	//		const List<i32> _src4(src4);
	//		List<i32> _dst(dst);
	//		return I_MiscImpl::compressIntArray4(_src1, _src2, _src3, _src4, _dst);
	//	}

		// optimization, so that 1/8th of the workspace is needed overall
	//	i32 compressIntArray8(
	//		IntTab& src1,
	//		IntTab& src2,
	//		IntTab& src3,
	//		IntTab& src4,
	//		IntTab& src5,
	//		IntTab& src6,
	//		IntTab& src7,
	//		IntTab& src8,
	//		IntTab& dst
	//	) {
	//		const List<i32> _src1(src1);
	//		const List<i32> _src2(src2);
	//		const List<i32> _src3(src3);
	//		const List<i32> _src4(src4);
	//		const List<i32> _src5(src5);
	//		const List<i32> _src6(src6);
	//		const List<i32> _src7(src7);
	//		const List<i32> _src8(src8);
	//		List<i32> _dst(dst);
	//		return I_MiscImpl::compressIntArray8(_src1, _src2, _src3, _src4, _src5, _src6, _src7, _src8, _dst);
	//	}

		/// <summary>
		/// <para>Compresses the "src" int-array and appends the compressed data to the "dst" int-array.</para>
		/// </summary>
		/// <param name="src"> - The array to be compressed.</param>
		/// <param name="dst"> - The array to which to append compressed data.</param>
		/// <returns>The number of ints written or a negative error code.</returns>
	i32 compressIntArray(
		IntTab& src,
		IntTab& dst
	) {
		const List<i32> _src(src);
		List<i32> _dst(dst);
		return I_MiscImpl::compressIntArray(_src, _dst);
	}

	i32 compressIntArrays(
		Tab<FPValue*>& src,
		IntTab& dst
	) {
		const List<FPValue*> _src(src);
		List<i32> _dst(dst);
		return I_MiscImpl::compressIntArrays(_src, _dst);
	}

	//	i32 decompressIntArray1(
	//		IntTab& src,
	//		const i32 off,
	//		IntTab& dst
	//	) {
	//		const List<i32> _src(src);
	//		List<i32> _dst1(dst);
	//		return I_MiscImpl::decompressIntArray1(_src, off - 1, _dst1);
	//	}

		// optimization, so that 1/2th of the workspace is needed overall
	//	i32 decompressIntArray2(
	//		IntTab& src,
	//		const i32 off,
	//		IntTab& dst1,
	//		IntTab& dst2
	//	) {
	//		const List<i32> _src(src);
	//		List<i32> _dst1(dst1);
	//		List<i32> _dst2(dst2);
	//		return I_MiscImpl::decompressIntArray2(_src, off - 1, _dst1, _dst2);
	//	}

		// optimization, so that 1/4th of the workspace is needed overall
	//	i32 decompressIntArray4(
	//		IntTab& src,
	//		const i32 off,
	//		IntTab& dst1,
	//		IntTab& dst2,
	//		IntTab& dst3,
	//		IntTab& dst4
	//	) {
	//		const List<i32> _src(src);
	//		List<i32> _dst1(dst1);
	//		List<i32> _dst2(dst2);
	//		List<i32> _dst3(dst3);
	//		List<i32> _dst4(dst4);
	//		return I_MiscImpl::decompressIntArray4(_src, off - 1, _dst1, _dst2, _dst3, _dst4);
	//	}

		// optimization, so that 1/8th of the workspace is needed overall
	//	i32 decompressIntArray8(
	//		IntTab& src,
	//		const i32 off,
	//		IntTab& dst1,
	//		IntTab& dst2,
	//		IntTab& dst3,
	//		IntTab& dst4,
	//		IntTab& dst5,
	//		IntTab& dst6,
	//		IntTab& dst7,
	//		IntTab& dst8
	//	) {
	//		const List<i32> _src(src);
	//		List<i32> _dst1(dst1);
	//		List<i32> _dst2(dst2);
	//		List<i32> _dst3(dst3);
	//		List<i32> _dst4(dst4);
	//		List<i32> _dst5(dst5);
	//		List<i32> _dst6(dst6);
	//		List<i32> _dst7(dst7);
	//		List<i32> _dst8(dst8);
	//		return I_MiscImpl::decompressIntArray8(_src, off - 1, _dst1, _dst2, _dst3, _dst4, _dst5, _dst6, _dst7, _dst8);
	//	}

		/// <summary>
		/// <para>Decompresses the data in "src" starting at "off". The "dst" array will contain only the decompressed data.</para>
		/// </summary>
		/// <param name="src"> - The array to be compressed.</param>
		/// <param name="dst"> - The array in which to store compressed data.</param>
		/// <param name="off"> - The offset into "src", 1-indexed.</param>
		/// <returns>The length of the compressed data or a negative error code.</returns>
	i32 decompressIntArray(
		IntTab& src,
		const i32 off,
		IntTab& dst
	) {
		const List<i32> _src(src);
		List<i32> _dst1(dst);
		return I_MiscImpl::decompressIntArray(_src, off - 1, _dst1);
	}

	i32 decompressIntArrays(
		IntTab& src,
		const i32 off,
		Tab<FPValue*>& dst
	) {
		const List<i32> _src(src);
		List<FPValue*> _dst(dst);
		return I_MiscImpl::decompressIntArrays(_src, off - 1, _dst);
	}

	/// <summary>
	/// <para>Returns a clone of the node or null if it could not be cloned.</para>
	/// <para>This is only appropriate for simple scene objects (no cameras, lights, ...).</para>
	/// </summary>
	/// <param name="node"> - The node to clone.</param>
	/// <returns>The clone or null.</returns>
	INode* cloneNode(
		INode* node
	) {
		return Util::cloneNode(node);
	}

	// Creates a swap-sequence containing 1-indexed values that when swapped in-place sequentially 
	// will produce a sorted list from an unsorted list. 
	// So you'd swap a[1] with a[seq[1]], then a[2] with a[seq[2]], etc. This part is O(N).
	// This sequence can be used by any number of unsorted lists of the same un-order.
	// Right now, this may not be suitable for very large lists (~2^30 elements) because of integer 
	// overflow potential. Currently this is implemented with a bottom-up mergesort, meaning it is
	// stable in both directions and completes in good time.
	// If "ascending" is true, the resulting order is ascending, else descending.
	// "sequence" will contain the swap index for each element in range. Always address it by minus "from". 
	// You always swap-sort in natural (ascending) order, even with a descending sequence.
	// "trans" will keep track of where each indexed element in range will go after sorting. Always address 
	// it by minus "from". You can use it to translate indices from before to after sorting. Very useful to 
	// have in certain cases and generating it along the sequence is extremely cheap.
	// "from" is 1-indexed.
	// Note that right now, this does not consider unicode at all and is case insensitive. It's simply a 
	// naive sort. Which is fine in most cases.
	// Also note that this is naturally slower than simply sorting a single list. But the actual sorting with
	// swap sequence is O(N), so when you need to sort M lists in the exact same way, the overall performance
	// is O(N*Log2(N)+M*N), which is much better than O(N*Log2(N)*M) for any M > 1. Plus, you know, it's much
	// more flexible and cache-friendly than having to provide slow and complex swapping procedures that 
	// include all to-be-sorted lists at once.
	i32 buildSwapSequence(
		Tab<const wchar_t*>& values,
		const i32 from,
		const i32 count,
		IntTab& sequence,
		IntTab& trans,
		const bool ascending
	) {
		const List<const wchar_t*> _values(values);
		List<i32> _sequence(sequence);
		List<i32> _trans(trans);
		return I_MiscImpl::buildSwapSequence(_values, from, count, _sequence, _trans, ascending);
	}

	// Returns the message mapped to the given id. An id of 0 always returns "OK", negative 
	// ids return the mapped error string, un-mapped ids return an emtpy string.
	const wchar_t* message(
		const i32 id
	) {
		return Codes::getMessage(id);
	}

	i32 toBitset(
		BitArray& src,
		IntTab& dst
	) {
		Bitset _dst(dst);
		return I_MiscImpl::toBitset(src, _dst);
	}

	i32 toBitArray(
		IntTab& src,
		BitArray& dst
	) {
		Bitset _src(src);
		return I_MiscImpl::toBitArray(_src, dst);
	}

	// 0-indexed bulk copy method that is efficient and intuitive
	// never use a loop in MXS again. tabs must have the proper size already.
	// mode == 0
	//   ranges must already be valid
	// mode == 1
	//   dst is resized to dstOff + len if necessary
	// mode == 2
	//   dst is always resized to dstOff + len
	// else
	//   dst is always resized to dstOff + len and its storage is re-allocated accordingly
	i32 copyInts(
		IntTab& src,
		const i32 srcOff,
		IntTab& dst,
		const i32 dstOff,
		const i32 len,
		const i32 mode
	) {
		switch (mode) {
		case 0: {
			if (srcOff < 0 || dstOff < 0 || len < 0 || srcOff > src.Count() - len || dstOff > dst.Count() - len) {
				throw MaxSDK::Util::OutOfRangeException(_M("Bad bounds."));
			}
			break;
		}
		case 1: {
			if (srcOff < 0 || dstOff < 0 || len < 0 || srcOff > src.Count() - len || dstOff > dst.Count()) {
				throw MaxSDK::Util::OutOfRangeException(_M("Bad bounds."));
			}
			const i32 minLen = dstOff + len;
			if (minLen < 0) {
				throw MaxSDK::Util::RunTimeException(_M("Argument 'len' is too large"));
			}
			if (dst.Count() < minLen) {
				dst.SetCount(minLen, false);
			}
			break;
		}
		case 2: {
			if (srcOff < 0 || dstOff < 0 || len < 0 || srcOff > src.Count() - len || dstOff > dst.Count()) {
				throw MaxSDK::Util::OutOfRangeException(_M("Bad bounds."));
			}
			const i32 minLen = dstOff + len;
			if (minLen < 0) {
				throw MaxSDK::Util::RunTimeException(_M("Argument 'len' is too large"));
			}
			dst.SetCount(minLen, false);
			break;
		}
		default: {
			if (srcOff < 0 || dstOff < 0 || len < 0 || srcOff > src.Count() - len || dstOff > dst.Count()) {
				throw MaxSDK::Util::OutOfRangeException(_M("Bad bounds."));
			}
			const i32 minLen = dstOff + len;
			if (minLen < 0) {
				throw MaxSDK::Util::RunTimeException(_M("Argument 'len' is too large"));
			}
			dst.SetCount(minLen, true);
			break;
		}
		}
		if (len != 0) {
			i32* ptrA = src.Addr(0);
			i32* ptrB = dst.Addr(0);
			std::copy(ptrA + srcOff, ptrA + srcOff + len, ptrB + dstOff);
		}
		return Codes::OK;
	}
	

	enum functionID : FunctionID {
		//compressIntArray1ID,
		//compressIntArray2ID,
		//compressIntArray4ID,
		//compressIntArray8ID,
		compressIntArrayID,
		compressIntArraysID,
		//decompressIntArray1ID,
		//decompressIntArray2ID,
		//decompressIntArray4ID,
		//decompressIntArray8ID,
		decompressIntArrayID,
		decompressIntArraysID,
		cloneNodeID,
		buildSwapSequenceID,
		messageID,
		toBitsetID,
		toBitArrayID,
		copyIntsID,
	};

	I_Misc() : FPInterfaceDesc(I_MISC_ID, _M("Misc"), 0, GetMXTopoToolsDesc(), FP_STATIC_METHODS, p_end) {
		//	AppendFunction(
		//		compressIntArray1ID, _M("compressIntArray1"), 0, TYPE_INT, 0, 2
		//		, _M("src1"), 0, TYPE_INT_TAB_BR
		//		, _M("dst"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		compressIntArray2ID, _M("compressIntArray2"), 0, TYPE_INT, 0, 3
		//		, _M("src1"), 0, TYPE_INT_TAB_BR
		//		, _M("src2"), 0, TYPE_INT_TAB_BR
		//		, _M("dst"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		compressIntArray4ID, _M("compressIntArray4"), 0, TYPE_INT, 0, 5
		//		, _M("src1"), 0, TYPE_INT_TAB_BR
		//		, _M("src2"), 0, TYPE_INT_TAB_BR
		//		, _M("src3"), 0, TYPE_INT_TAB_BR
		//		, _M("src4"), 0, TYPE_INT_TAB_BR
		//		, _M("dst"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		compressIntArray8ID, _M("compressIntArray8"), 0, TYPE_INT, 0, 9
		//		, _M("src1"), 0, TYPE_INT_TAB_BR
		//		, _M("src2"), 0, TYPE_INT_TAB_BR
		//		, _M("src3"), 0, TYPE_INT_TAB_BR
		//		, _M("src4"), 0, TYPE_INT_TAB_BR
		//		, _M("src5"), 0, TYPE_INT_TAB_BR
		//		, _M("src6"), 0, TYPE_INT_TAB_BR
		//		, _M("src7"), 0, TYPE_INT_TAB_BR
		//		, _M("src8"), 0, TYPE_INT_TAB_BR
		//		, _M("dst"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		AppendFunction(
			compressIntArrayID, _M("compressIntArray"), 0, TYPE_INT, 0, 2
			, _M("src"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			compressIntArraysID, _M("compressIntArrays"), 0, TYPE_INT, 0, 2
			, _M("src"), 0, TYPE_FPVALUE_TAB_BR
			, _M("dst"), 0, TYPE_INT_TAB_BR
			, p_end);
		//	AppendFunction(
		//		decompressIntArray1ID, _M("decompressIntArray1"), 0, TYPE_INT, 0, 3
		//		, _M("src"), 0, TYPE_INT_TAB_BR
		//		, _M("off"), 0, TYPE_INT
		//		, _M("dst1"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		decompressIntArray2ID, _M("decompressIntArray2"), 0, TYPE_INT, 0, 4
		//		, _M("src"), 0, TYPE_INT_TAB_BR
		//		, _M("off"), 0, TYPE_INT
		//		, _M("dst1"), 0, TYPE_INT_TAB_BR
		//		, _M("dst2"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		decompressIntArray4ID, _M("decompressIntArray4"), 0, TYPE_INT, 0, 6
		//		, _M("src"), 0, TYPE_INT_TAB_BR
		//		, _M("off"), 0, TYPE_INT
		//		, _M("dst1"), 0, TYPE_INT_TAB_BR
		//		, _M("dst2"), 0, TYPE_INT_TAB_BR
		//		, _M("dst3"), 0, TYPE_INT_TAB_BR
		//		, _M("dst4"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		//	AppendFunction(
		//		decompressIntArray8ID, _M("decompressIntArray8"), 0, TYPE_INT, 0, 10
		//		, _M("src"), 0, TYPE_INT_TAB_BR
		//		, _M("off"), 0, TYPE_INT
		//		, _M("dst1"), 0, TYPE_INT_TAB_BR
		//		, _M("dst2"), 0, TYPE_INT_TAB_BR
		//		, _M("dst3"), 0, TYPE_INT_TAB_BR
		//		, _M("dst4"), 0, TYPE_INT_TAB_BR
		//		, _M("dst5"), 0, TYPE_INT_TAB_BR
		//		, _M("dst6"), 0, TYPE_INT_TAB_BR
		//		, _M("dst7"), 0, TYPE_INT_TAB_BR
		//		, _M("dst8"), 0, TYPE_INT_TAB_BR
		//		, p_end);
		AppendFunction(
			decompressIntArrayID, _M("decompressIntArray"), 0, TYPE_INT, 0, 3
			, _M("src"), 0, TYPE_INT_TAB_BR
			, _M("off"), 0, TYPE_INT
			, _M("dst"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			decompressIntArraysID, _M("decompressIntArrays"), 0, TYPE_INT, 0, 3
			, _M("src"), 0, TYPE_INT_TAB_BR
			, _M("off"), 0, TYPE_INT
			, _M("dst"), 0, TYPE_FPVALUE_TAB_BR
			, p_end);
		AppendFunction(
			cloneNodeID, _M("cloneNode"), 0, TYPE_INODE, 0, 1
			, _M("node"), 0, TYPE_INODE
			, p_end);
		AppendFunction(
			buildSwapSequenceID, _M("buildSwapSequence"), 0, TYPE_INT, 0, 6
			, _M("values"), 0, TYPE_STRING_TAB_BR
			, _M("from"), 0, TYPE_INT
			, _M("to"), 0, TYPE_INT
			, _M("sequence"), 0, TYPE_INT_TAB_BR
			, _M("trans"), 0, TYPE_INT_TAB_BR
			, _M("ascending"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			messageID, _M("message"), 0, TYPE_STRING, 0, 1
			, _M("id"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			toBitsetID, _M("toBitset"), 0, TYPE_INT, 0, 2
			, _M("src"), 0, TYPE_BITARRAY_BR
			, _M("dst"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			toBitArrayID, _M("toBitArray"), 0, TYPE_INT, 0, 2
			, _M("src"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_BITARRAY_BR
			, p_end);
		AppendFunction(
			copyIntsID, _M("copyInts"), 0, TYPE_INT, 0, 6
			, _M("src"), 0, TYPE_INT_TAB_BR
			, _M("srcOff"), 0, TYPE_INT
			, _M("dst"), 0, TYPE_INT_TAB_BR
			, _M("dstOff"), 0, TYPE_INT
			, _M("len"), 0, TYPE_INT
			, _M("mode"), 0, TYPE_INT
			, p_end);
	}
	BEGIN_FUNCTION_MAP
		//	FN_2(compressIntArray1ID, TYPE_INT, compressIntArray1
		//		, TYPE_INT_TAB_BR		// src1
		//		, TYPE_INT_TAB_BR		// dst
		//	)
		//	FN_3(compressIntArray2ID, TYPE_INT, compressIntArray2
		//		, TYPE_INT_TAB_BR		// src1
		//		, TYPE_INT_TAB_BR		// src2
		//		, TYPE_INT_TAB_BR		// dst
		//	)
		//	FN_5(compressIntArray4ID, TYPE_INT, compressIntArray4
		//		, TYPE_INT_TAB_BR		// src1
		//		, TYPE_INT_TAB_BR		// src2
		//		, TYPE_INT_TAB_BR		// src3
		//		, TYPE_INT_TAB_BR		// src4
		//		, TYPE_INT_TAB_BR		// dst
		//	)
		//	FN_9(compressIntArray8ID, TYPE_INT, compressIntArray8
		//		, TYPE_INT_TAB_BR		// src1
		//		, TYPE_INT_TAB_BR		// src2
		//		, TYPE_INT_TAB_BR		// src3
		//		, TYPE_INT_TAB_BR		// src4
		//		, TYPE_INT_TAB_BR		// src5
		//		, TYPE_INT_TAB_BR		// src6
		//		, TYPE_INT_TAB_BR		// src7
		//		, TYPE_INT_TAB_BR		// src8
		//		, TYPE_INT_TAB_BR		// dst
		//	)
		FN_2(compressIntArrayID, TYPE_INT, compressIntArray
			, TYPE_INT_TAB_BR		// src
			, TYPE_INT_TAB_BR		// dst
		)
		FN_2(compressIntArraysID, TYPE_INT, compressIntArrays
			, TYPE_FPVALUE_TAB_BR	// src
			, TYPE_INT_TAB_BR		// dst
		)
		//	FN_3(decompressIntArray1ID, TYPE_INT, decompressIntArray1
		//		, TYPE_INT_TAB_BR		// src
		//		, TYPE_INT				// srcOff
		//		, TYPE_INT_TAB_BR		// dst1
		//	)
		//	FN_4(decompressIntArray2ID, TYPE_INT, decompressIntArray2
		//		, TYPE_INT_TAB_BR		// src
		//		, TYPE_INT				// srcOff
		//		, TYPE_INT_TAB_BR		// dst1
		//		, TYPE_INT_TAB_BR		// dst2
		//	)
		//	FN_6(decompressIntArray4ID, TYPE_INT, decompressIntArray4
		//		, TYPE_INT_TAB_BR		// src
		//		, TYPE_INT				// srcOff
		//		, TYPE_INT_TAB_BR		// dst1
		//		, TYPE_INT_TAB_BR		// dst2
		//		, TYPE_INT_TAB_BR		// dst3
		//		, TYPE_INT_TAB_BR		// dst4
		//	)
		//	FN_10(decompressIntArray8ID, TYPE_INT, decompressIntArray8
		//		, TYPE_INT_TAB_BR		// src
		//		, TYPE_INT				// srcOff
		//		, TYPE_INT_TAB_BR		// dst1
		//		, TYPE_INT_TAB_BR		// dst2
		//		, TYPE_INT_TAB_BR		// dst3
		//		, TYPE_INT_TAB_BR		// dst4
		//		, TYPE_INT_TAB_BR		// dst5
		//		, TYPE_INT_TAB_BR		// dst6
		//		, TYPE_INT_TAB_BR		// dst7
		//		, TYPE_INT_TAB_BR		// dst8
		//	)
		FN_3(decompressIntArrayID, TYPE_INT, decompressIntArray
			, TYPE_INT_TAB_BR		// src
			, TYPE_INT				// srcOff
			, TYPE_INT_TAB_BR		// dst
		)
		FN_3(decompressIntArraysID, TYPE_INT, decompressIntArrays
			, TYPE_INT_TAB_BR		// src
			, TYPE_INT				// srcOff
			, TYPE_FPVALUE_TAB_BR	// dst
		)
		FN_1(cloneNodeID, TYPE_INODE, cloneNode
			, TYPE_INODE			// node
		)
		FN_6(buildSwapSequenceID, TYPE_INT, buildSwapSequence
			, TYPE_STRING_TAB_BR	// values
			, TYPE_INT				// from
			, TYPE_INT				// to
			, TYPE_INT_TAB_BR		// sequence
			, TYPE_INT_TAB_BR		// trans
			, TYPE_BOOL				// ascending
		)
		FN_1(messageID, TYPE_STRING, message
			, TYPE_INT				// id
		)
		FN_2(toBitsetID, TYPE_INT, toBitset
			, TYPE_BITARRAY_BR		// src
			, TYPE_INT_TAB_BR		// dst
		)
		FN_2(toBitArrayID, TYPE_INT, toBitArray
			, TYPE_INT_TAB_BR		// src
			, TYPE_BITARRAY_BR		// dst
		)
		FN_6(copyIntsID, TYPE_INT, copyInts
			, TYPE_INT_TAB_BR		// src
			, TYPE_INT				// srcOff
			, TYPE_INT_TAB_BR		// dst
			, TYPE_INT				// dstOff
			, TYPE_INT				// len
			, TYPE_INT				// mode
		)
	END_FUNCTION_MAP
};
