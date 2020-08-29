#pragma once

#include "Util.h"
#include "Codes.h"
#include "Helpers.h"
#include "Restore.h"

class I_SkinImpl {

public:
	static inline i32 skinCount(
		INode* node,
		const bool enabled,
		const bool disabled
	) {
		i32 c = 0;
		if (enabled != disabled) {
			// count enabled/disabled skins only
			for (Helpers::SkinModIterator it(*node); it.next(); ) {
				if ((it.mod()->IsEnabled() != 0) == enabled) {
					++c;
				}
			}
		}
		else if (enabled) {
			// count every skin
			for (Helpers::SkinModIterator it(*node); it.next(); ) {
				++c;
			}
		}
		// else: count nothing
		return c;
	}

public:
	static inline i32 skinIndex(
		INode* node,
		const i32 start,
		const bool enabled
	) {
		using ERR = Codes::I_Skin_skinIndex;
		if (!node) {
			return ERR::null_node;
		}
		const i32 st = Util::mathMax(0, start);
		for (Helpers::SkinModIterator it(*node); it.next(); ) {
			if (it.index() >= st && (it.mod()->IsEnabled() != 0) == enabled) {
				Helpers::SkinMod h;
				const i32 r = h.init(it);
				if (r != Codes::OK) {
					return ERR::resolve(r);
				}
				return it.index() + 1;
			}
		}
		return 0;
	}

public:
	static inline i32 skinIndices(
		INode* node,
		const i32 start,
		const bool enabled,
		const bool disabled,
		List<i32>& indices
	) {
		using ERR = Codes::I_Skin_skinIndices;
		if (!node) {
			return ERR::null_node;
		}
		indices.clearAll();
		if (enabled || disabled) {
			const i32 st = Util::mathMax(0, start);
			for (Helpers::SkinModIterator it(*node); it.next(); ) {
				if (it.index() >= st) {
					const bool b = it.mod()->IsEnabled() != 0;
					if (b == enabled || b != disabled) {
						Helpers::SkinMod h;
						const i32 r = h.init(it);
						if (r != Codes::OK) {
							return ERR::resolve(r);
						}
						indices.push(it.index() + 1);
					}
				}
			}
		}
		return Codes::OK;
	}

public:
	static inline i32 getBoneLimit(
		INode* node,
		const i32 modIndex
	) {
		using ERR = Codes::I_Skin_getBoneLimit;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		return h.getBoneLimit();
	}

public:
	static inline i32 setBoneLimit(
		INode* node,
		const i32 modIndex,
		const i32 limit
	) {
		using ERR = Codes::I_Skin_setBoneLimit;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		h.setBoneLimit(limit);
		return Codes::OK;
	}

public:
	static inline i32 numBones(
		INode* node,
		const i32 modIndex,
		const bool flat
	) {
		using ERR = Codes::I_Skin_numBones;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		return flat ? h.getNumBonesFlat() : h.getNumBones();
	}

public:
	static inline i32 numPoints(
		INode* node,
		const i32 modIndex
	) {
		using ERR = Codes::I_Skin_numPoints;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		return h.getNumPoints();
	}

public:
	static inline i32 collectUsedBones(
		INode* node,
		const i32 modIndex,
		List<INode*>& bones,
		const f32 threshold
	) {
		using ERR = Codes::I_Skin_collectUsedBones;
		Helpers::SkinMod sk;
		const i32 r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		f32 t = threshold;
		if (threshold != threshold) {
			t = 0.0f;
		}
		else if (threshold >= 0.999998f) {
			t = Util::MAX_F32;
		}
		const i32 numBones = sk.getNumBones();
		Bitset usedBoneIds(numBones);
		bones.clearAll();
		List<Slice> names(numBones);
		i32 numBonesUsed = 0;
		for (i32 v = 0, n = sk.getNumPoints(); v < n && numBonesUsed < numBones; ++v) {
			for (i32 j = 0, nb = sk.getNumAssignedBones(v); j < nb; ++j) {
				const f32 w = sk.getBoneWeight(v, j);
				if (w > t) {
					const i32 id = sk.getAssignedBone(v, j);
					if (usedBoneIds.trySet(id)) {
						++numBonesUsed;
						INode* bone = sk.getBone(id);
						Slice name(bone->GetName());
						const i32 idx = names.indexOfAscending(name);
						names.insert(idx, name);
						bones.insert(idx, bone);
					}
				}
			}
		}
		return Codes::OK;
	}

public:
	static inline i32 stripWeights(
		INode* node,
		const i32 modIndex,
		const f32 threshold
	) {
		using ERR = Codes::I_Skin_stripWeights;
		Helpers::SkinMod sk;
		const i32 r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		stripWeightsImpl(sk, threshold);
		return Codes::OK;
	}

public:
	static inline i32 stripBones(
		INode* node,
		const i32 modIndex,
		const bool stripWeights,
		const f32 threshold
	) {
		using ERR = Codes::I_Skin_stripBones;
		Helpers::SkinMod sk;
		const i32 r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		if (stripWeights) {
			stripWeightsImpl(sk, threshold);
		}
		List<i32> boneIds;
		sk.collectUnusedBoneIds(boneIds);
		if (!boneIds.isEmpty()) {
			Interface7* ip = GetCOREInterface7();
			ip->DisableSceneRedraw();
			ip->EnableShowEndResult(false);
			{
				// select node if necessary
				INodeTab sel;
				ip->GetSelNodeTab(sel);
				if (sel.Count() != 1 || sel[0] != node) {
					ip->SelectNode(node, 1);
				}
			}
			if (ip->GetCommandPanelTaskMode() != TASK_MODE_MODIFY) {
				ip->SetCommandPanelTaskMode(TASK_MODE_MODIFY);
			}
			if (ip->GetCurEditObject() != &sk.mod()) {
				ip->SetCurEditObject(&sk.mod());
			}
			CharBuffer buf(L"skinOps.removebone (GetAnimByHandle 0xFFFFFFFFFFFFFFFFP) 0xFFFFFFFF", 68);
			const AnimHandle h = Animatable::GetHandleByAnim(&sk.mod());
			buf.writeU64Hex(38, U64(h), false);	// override the hex value completely
			for (i32 j = boneIds.length() - 1; j >= 0; --j) {
				buf.writeU32Hex(59, U32(boneIds[j] + 1), false);
				FPValue v;
				ExecuteMAXScriptScript(buf.addr(), true, &v, false);
			}
			ip->EnableSceneRedraw();
			ip->EnableShowEndResult(true);
		}
		return Codes::OK;
	}

	// removes from all specified weight-tables all bones with an influence <= threshold.
	// if threshold is >= 1.0, will strip all bones
	// undoable.
private:
	static inline bool stripWeightsImpl(
		Helpers::SkinMod& mod,
		f32 threshold
	) {
		if (threshold != threshold) {
			threshold = 0.0f;
		}
		else if (threshold >= 0.999998) {
			threshold = Util::MAX_F32;
		}

		Interface7* ip = GetCOREInterface7();
		ip->DisableSceneRedraw();
		{
			// select node if necessary
			INodeTab sel;
			ip->GetSelNodeTab(sel);
			if (sel.Count() != 1 || sel[0] != mod.node()) {
				ip->SelectNode(mod.node(), 1);
			}
		}
		if (ip->GetCommandPanelTaskMode() != TASK_MODE_MODIFY) {
			ip->SetCommandPanelTaskMode(TASK_MODE_MODIFY);
		}
		if (ip->GetCurEditObject() != &mod.mod()) {
			ip->SetCurEditObject(&mod.mod());
		}

		const f32 t = threshold;
		const bool holding = theHold.Holding();
		Defer<Restore::WeightChangeRestore> rest;
		if (holding) {
			rest.set(new Restore::WeightChangeRestore(mod));
			rest.get()->before();
		}
		bool change = false;
		List<INode*> bones;
		List<f32> weights;
		const i32 numPoints = mod.getNumPoints();
		for (i32 i = 0; i < numPoints; ++i) {
			bones.clearAll();
			weights.clearAll();
			const i32 nb = mod.getNumAssignedBones(i);
			for (i32 j = 0; j < nb; ++j) {
				const f32 w = mod.getBoneWeight(i, j);
				if (w == w && w > t) {
					bones.push(mod.getBone(mod.getAssignedBone(i, j)));
					weights.push(w);
				}
			}
			if (bones.length() != nb) {
				mod.addWeights(i, *(bones.tab()), *(weights.tab()));
				change = true;
			}
		}
		if (change) {
			if (holding) {
				rest.get()->after();
				Restore::put(true, rest);
			}
			mod.mod().NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		}

		ip->EnableSceneRedraw();
		return change;
	}

public:
	static inline i32 getSelection(
		INode* node,
		const i32 modIndex,
		Bitset& selection,
		const i32 mode
	) {
		using ERR = Codes::I_Skin_getSelection;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		BitArray verts(h.getNumPoints());
		if (mode == 0) {
			h.getVertexSelection(verts);
		}
		else {
			BitArray b(verts.GetSize());
			h.getVertexSelection(b);
			if (mode < 0) {
				BitArray c = ~b;
				if (verts.GetSize() != c.GetSize()) {
					verts.SetSize(c.GetSize(), 1);
				}
				verts &= c;
			}
			else {
				if (verts.GetSize() != b.GetSize()) {
					verts.SetSize(b.GetSize(), 1);
				}
				verts |= b;
			}
		}
		selection.readBits(verts);
		return Codes::OK;
	}

public:
	static inline i32 setSelection(
		INode* node,
		const i32 modIndex,
		const Bitset& selection,
		const i32 mode
	) {
		using ERR = Codes::I_Skin_setSelection;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		Defer<Restore::SkinSelectionRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SkinSelectionRestore());
			rest.get()->before(h);
		}
		BitArray sel(selection.length());
		selection.writeBits(sel);

		const i32 numPoints = h.getNumPoints();
		if (sel.GetSize() != numPoints) {
			sel.SetSize(numPoints, 1);
		}
		if (mode != 0) {
			BitArray tmp(numPoints);
			h.getVertexSelection(tmp);
			if (tmp.GetSize() != numPoints) {
				tmp.SetSize(numPoints, 1);
			}
			if (mode < 0) {
				tmp &= ~sel;	// TODO verify that this works
			}
			else {
				tmp |= sel;
			}
			h.setVertexSelection(tmp);
		}
		else {
			h.setVertexSelection(sel);
		}
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		return Codes::OK;
	}

public:
	static inline i32 setSelection2(
		INode* node,
		const i32 modIndex,
		const List<i32>& selection,
		const i32 mode
	) {
		using ERR = Codes::I_Skin_setSelection2;
		Helpers::SkinMod h;
		const i32 r = h.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const i32 numPoints = h.getNumPoints();
		if (!Helpers::Geom::checkBounds(selection, numPoints)) {
			return ERR::bad_handle;
		}
		Defer<Restore::SkinSelectionRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SkinSelectionRestore());
			rest.get()->before(h);
		}
		BitArray sel(numPoints);
		if (mode != 0) {
			h.getVertexSelection(sel);
			if (sel.GetSize() != numPoints) {
				sel.SetSize(numPoints, 1);
			}
		}
		if (mode < 0) {
			for (i32 i = 0; i < selection.length(); ++i) {
				sel.Clear(selection[i] - 1);
			}
		}
		else {
			for (i32 i = 0; i < selection.length(); ++i) {
				sel.Set(selection[i] - 1);
			}
		}
		h.setVertexSelection(sel);
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		return Codes::OK;
	}

public:
	static inline i32 addBones(
		INode* node,
		const i32 modIndex,
		const List<INode*>& bones
	) {
		using ERR = Codes::I_Skin_addBones;
		Helpers::SkinMod sk;
		const i32 r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const i32 n = bones.length() - 1;
		if (n >= 0) {
			for (i32 i = 0; i < n; ++i) {
				sk.addBoneEx(bones[i], false);
			}
			sk.addBoneEx(bones[n], true);
		}
		return Codes::OK;
	}

public:
	static inline i32 copyWeights(
		INode* src,
		const i32 srcModIndex,
		INode* dst,
		const i32 dstModIndex,
		const f32 threshold
	) {
		using ERR = Codes::I_Skin_copyWeights;
		i32 r;
		Helpers::SkinMod skA;
		r = skA.init(src, srcModIndex);
		if (r != Codes::OK) {
			return ERR::resolveSkinSrc(r);
		}
		Helpers::SkinMod skB;
		r = skB.init(dst, dstModIndex);
		if (r != Codes::OK) {
			return ERR::resolveSkinDst(r);
		}
		if (src == dst && srcModIndex == dstModIndex) {
			return Codes::OK;
		}
		const i32 numPointsA = skA.getNumPoints();
		const i32 numPointsB = skB.getNumPoints();
		if (numPointsA != numPointsB) {
			return ERR::different_num_points;
		}
		if (numPointsA > 0) {
			const i32 limitA = skA.getBoneLimit();
			const i32 limitB = skB.getBoneLimit();
			if (limitB < limitA) {
				skB.setBoneLimit(limitA);
			}
			const i32 numBonesA = skA.getNumBones();
			const i32 numBonesB = skB.getNumBones();
			List<i32> trans(numBonesA);
			for (i32 i = 0; i < numBonesA; ++i) {
				INode* bone = skA.getBone(i);
				i32 k = -1;
				for (i32 j = 0; j < numBonesB; ++j) {
					if (skB.getBone(j) == bone) {
						k = j;
						break;
					}
				}
				trans.push(k);
			}
			f32 t = threshold;
			if (threshold != threshold) {
				t = 0.0f;
			}
			else if (threshold >= 0.999998f) {
				t = Util::MAX_F32;
			}
			List<INode*> bones;
			List<f32> weights;
			for (i32 v = 0; v < numPointsA; ++v) {
				bones.clearAll();
				weights.clearAll();
				const i32 nb = skA.getNumAssignedBones(v);
				for (i32 i = 0; i < nb; ++i) {
					const f32 w = skA.getBoneWeight(v, i);
					if (w > t) {
						const i32 id = trans[skA.getAssignedBone(v, i)];
						if (id < 0) {
							return ERR::bone_unmatched;
						}
						bones.push(skB.getBone(id));
						weights.push(w);
					}
				}
				skB.addWeights(v, *bones.tab(), *weights.tab());
				skB.setDQBlendWeight(v, skA.getDQBlendWeight(v));
			}
		}
		return Codes::OK;
	}

};
