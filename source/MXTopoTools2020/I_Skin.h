#pragma once

#include "I_SkinImpl.h"

extern const Interface_ID I_SKIN_ID;

class I_Skin : public FPInterfaceDesc
{
public:

	// Returns the number of skin mods.
	// If "enabled" is true, enabled skin mods are counted.
	// If "disabled" is true, disabled skin mods are counted.
	// TODO test this
	i32 skinCount(
		INode* node,
		const bool enabled,
		const bool disabled
	) {
		return I_SkinImpl::skinCount(
			node, 
			enabled, 
			disabled
		);
	}

	// Returns the next 1-index of the next enabled or disabled skin mod, starting at "start" (1-indexed).
	// Returns 0 if none or < 0 on error.
	// If "enabled" is true, finds next enabled, else finds next disabled skin mod index.
	// TODO test this
	i32 skinIndex(
		INode* node,
		const i32 start,
		const bool enabled
	) {
		return I_SkinImpl::skinIndex(
			node, 
			start - 1, 
			enabled
		);
	}

	// Returns all 1-indices of the next enabled or disabled skin mods, starting at 1-indexed start in the supplied tab.
	// Returns 0 if everything is fine, < 0 on error.
	i32 skinIndices(
		INode* node,
		const i32 start,
		const bool enabled,
		const bool disabled,
		IntTab& indices
	) {
		List<i32> _indices(indices);
		return I_SkinImpl::skinIndices(
			node, 
			start - 1, 
			enabled, 
			disabled, 
			_indices
		);
	}


	// Returns the 1-indexed skin modifier's bone affect limit.
	// There is no direct advantage to using this over maxscript, except that it follows the same conventions as the other methods (error checkting, indexing, etc...).
	// TODO test this
	i32 getBoneLimit(
		INode* node,
		const i32 modIndex
	) {
		return I_SkinImpl::getBoneLimit(
			node, 
			modIndex - 1
		);
	}

	// Sets the 1-indexed skin modifier's bone affect limit.
	// There is no direct advantage to using this over maxscript, except that it follows the same conventions as the other methods (error checkting, indexing, etc...).
	// Undoable.
	// TODO test this
	i32 setBoneLimit(
		INode* node,
		const i32 modIndex,
		const i32 limit
	) {
		return I_SkinImpl::setBoneLimit(
			node, 
			modIndex - 1, 
			limit
		);
	}

	// Returns the number of bones of the 1-indexed skin mod or < 0 on error.
	// TODO test this
	i32 numBones(
		INode* node,
		const i32 modIndex,
		const bool flat		// if true, returns the flat count, else the logical count
	) {
		return I_SkinImpl::numBones(
			node, 
			modIndex - 1, 
			flat
		);
	}

	// Returns the 1-indexed skin-mod's reported num points or < 0 on error.
	// Mostly useful for debugging topo changes...
	i32 numPoints(
		INode* node,
		const i32 modIndex
	) {
		return I_SkinImpl::numPoints(
			node, 
			modIndex - 1
		);
	}

	// Collects only those bones which are listed with influence > threshold by at least one weight table in the 1-indexed skin mod.
	i32 collectUsedBones(
		INode* node,
		const i32 modIndex,
		Tab<INode*>& bones,
		const f32 threshold
	) {
		List<INode*> _bones(bones);
		return I_SkinImpl::collectUsedBones(
			node, 
			modIndex - 1, 
			_bones, 
			threshold
		);
	}

	// Removes all bones with weights <= threshold from all weight tables for the 1-indexed skin mod.
	// Undoable.
	// TODO test this
	i32 stripWeights(
		INode* node,
		const i32 modIndex,
		const f32 threshold
	) {
		return I_SkinImpl::stripWeights(
			node, 
			modIndex - 1,
			threshold
		);
	}

	// Disables all bones that are not used in any weight table for the 1-indexed skin mod.
	// First strips weigths like stripWeights() if "stripWeights" is true.
	// Note that be bones are not truly removed, only hidden. Unfortunately, this is just how skin mods work. Nothing I can do about it.
	// Undoable.
	// TODO test this
	i32 stripBones(
		INode* node,
		const i32 modIndex,
		const bool stripWeights,
		const f32 threshold
	) {
		return I_SkinImpl::stripBones(
			node, 
			modIndex - 1, 
			stripWeights, 
			threshold
		);
	}

	// Returns the 1-indexed modifier's vert handle selection.
	// This is order of magnitude more efficient that any solution possible via skinOps. (This state of affairs is downright criminal!)
	// ____ tested and working
	// TODO It is a little unclear to me if the node must have been selected at least once for the skin data to populate?
	// But it's hard to test that right now. Only once the tool is up and running can I test this.
	// mode == 0 set out to selected
	// mode > 0 add selected to out
	// mode < 0 subtract selected from out
	i32 getSelection(
		INode* node,
		const i32 modIndex,
		IntTab& selection,	// bitset
		const i32 mode
	) {
		Bitset _selection(selection);
		return I_SkinImpl::getSelection(
			node, 
			modIndex - 1,
			_selection, 
			mode
		);
	}

	// Sets the 1-indexed skin modifier's vert handle selection.
	// This is order of magnitude more efficient that any solution possible via skinOps. (This state of affairs is downright criminal!)
	// Undoable.
	// TODO test this
	i32 setSelection(
		INode* node,
		const i32 modIndex,
		IntTab& selection,	// bitset
		const i32 mode
	) {
		const Bitset _selection(selection);
		return I_SkinImpl::setSelection(
			node, 
			modIndex - 1, 
			_selection, 
			mode
		);
	}

	i32 setSelection2(
		INode* node,
		const i32 modIndex,
		IntTab& selection,
		const i32 mode
	) {
		const List<i32> _selection(selection);
		return I_SkinImpl::setSelection2(
			node, 
			modIndex - 1,
			_selection, 
			mode
		);
	}

	// Adds the specified bones to the 1-indexed skin mod.
	// This is not safe to use unless you know what is happening.
	// Doesn't check if bones are already present, etc...
	// Not undoable.
	i32 addBones(
		INode* node,
		const i32 modIndex,
		Tab<INode*>& bones
	) {
		const List<INode*> _bones(bones);
		return I_SkinImpl::addBones(
			node, 
			modIndex - 1, 
			_bones
		);
	}

	// Copies weights from src to dst, using the same bone nodes, but irrespective or bone order. 
	// Weights with influence == 0 or NaN are not copied (defaulting to 0 [letting the SDK handle it]).
	// Relevant bones must already exist. Expect: src != dest. Etc...
	// This is not safe to use unless you know what is happening.
	// Not undoable.
	i32 copyWeights(
		INode* src,
		const i32 srcModIndex,
		INode* dst,
		const i32 dstModIndex,
		const f32 threshold
	) {
		return I_SkinImpl::copyWeights(
			src, 
			srcModIndex, 
			dst, 
			dstModIndex, 
			threshold
		);
	}

	enum functionID : FunctionID {
		skinCountID,
		skinIndexID,
		skinIndicesID,
		getBoneLimitID,
		setBoneLimitID,
		numBonesID,
		numPointsID,
		collectUsedBonesID,	// TODO also add collectUnusedBones
		stripWeightsID,
		stripBonesID,
		getSelectionID,
		setSelectionID,
		setSelection2ID,
		// unsafe
		addBonesID,
		copyWeightsID,
	};

	I_Skin() : FPInterfaceDesc(I_SKIN_ID, _M("Skin"), 0, GetMXTopoToolsDesc(), FP_STATIC_METHODS, p_end) {
		AppendFunction(
			skinCountID, _M("skinCount"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("enabled"), 0, TYPE_BOOL
			, _M("disabled"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			skinIndexID, _M("skinIndex"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("start"), 0, TYPE_INT
			, _M("enabled"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			skinIndicesID, _M("skinIndices"), 0, TYPE_INT, 0, 5
			, _M("node"), 0, TYPE_INODE
			, _M("start"), 0, TYPE_INT
			, _M("enabled"), 0, TYPE_BOOL
			, _M("disabled"), 0, TYPE_BOOL
			, _M("indices"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			getBoneLimitID, _M("getBoneLimit"), 0, TYPE_INT, 0, 2
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			setBoneLimitID, _M("setBoneLimit"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("limit"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			numBonesID, _M("numBones"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("flat"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			numPointsID, _M("numPoints"), 0, TYPE_INT, 0, 2
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			collectUsedBonesID, _M("collectUsedBones"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("bones"), 0, TYPE_INODE_TAB_BR
			, _M("threshold"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			stripWeightsID, _M("stripWeights"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("threshold"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			stripBonesID, _M("stripBones"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("stripWeights"), 0, TYPE_BOOL
			, _M("threshold"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			addBonesID, _M("addBones"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("bones"), 0, TYPE_INODE_TAB_BR
			, p_end);
		AppendFunction(
			copyWeightsID, _M("copyWeights"), 0, TYPE_INT, 0, 5
			, _M("src"), 0, TYPE_INODE
			, _M("srcModIndex"), 0, TYPE_INT
			, _M("dest"), 0, TYPE_INODE
			, _M("destModIndex"), 0, TYPE_INT
			, _M("threshold"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			getSelectionID, _M("getSelection"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			setSelectionID, _M("setSelection"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			setSelection2ID, _M("setSelection2"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
	}
	BEGIN_FUNCTION_MAP
		FN_3(skinCountID, TYPE_INT, skinCount
			, TYPE_INODE		// node
			, TYPE_BOOL			// enabled
			, TYPE_BOOL			// disabled
		)
		FN_3(skinIndexID, TYPE_INT, skinIndex
			, TYPE_INODE		// node
			, TYPE_INT			// start
			, TYPE_BOOL			// enabled
		)
		FN_5(skinIndicesID, TYPE_INT, skinIndices
			, TYPE_INODE		// node
			, TYPE_INT			// start
			, TYPE_BOOL			// enabled
			, TYPE_BOOL			// disabled
			, TYPE_INT_TAB_BR	// indices
		)
		FN_2(getBoneLimitID, TYPE_INT, getBoneLimit
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
		)
		FN_3(setBoneLimitID, TYPE_INT, setBoneLimit
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT			// limit
		)
		FN_3(numBonesID, TYPE_INT, numBones
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_BOOL			// flat
		)
		FN_2(numPointsID, TYPE_INT, numPoints
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
		)
		FN_4(collectUsedBonesID, TYPE_INT, collectUsedBones
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INODE_TAB_BR	// bones
			, TYPE_FLOAT		// threshold
		)
		FN_3(stripWeightsID, TYPE_INT, stripWeights
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_FLOAT		// threshold
		)
		FN_4(stripBonesID, TYPE_INT, stripBones
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_BOOL			// stripWeights
			, TYPE_FLOAT		// threshold
		)
		FN_3(addBonesID, TYPE_INT, addBones
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INODE_TAB_BR	// bones
		)
		FN_5(copyWeightsID, TYPE_INT, copyWeights
			, TYPE_INODE		// src
			, TYPE_INT			// srcModIndex
			, TYPE_INODE		// dest
			, TYPE_INT			// destModIndex
			, TYPE_FLOAT		// threshold
		)
		FN_4(getSelectionID, TYPE_INT, getSelection
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// selection
			, TYPE_INT			// mode
		)
		FN_4(setSelectionID, TYPE_INT, setSelection
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// selection
			, TYPE_INT			// mode
		)
		FN_4(setSelection2ID, TYPE_INT, setSelection2
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// selection
			, TYPE_INT			// mode
		)
	END_FUNCTION_MAP

};
