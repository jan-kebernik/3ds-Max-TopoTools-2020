#pragma once

#include "Util.h"
#include "I_OrderImpl.h"

extern const Interface_ID I_ORDR_ID;

class I_Order : public FPInterfaceDesc
{
public:

	// Copies or blends the coordinates of the "selected" verts along the chosen mirror axis.
	// If a vert is "selected" on only one side, its coordinates are mirroed to the other side.
	// If the counterpart vert is also "selected", both verts have their coordinates equalized along the mirror axis, while preserving their total distance.
	// Note that this can have unpredictable results if (and only if) the distance along the mirror axis itself is exactly 0. 
	// For example, given a vert A with coordinates [0, 5, 10] and an opposing vert B with coordinates [0, 3, 8], 
	// the X-axis distance is exactly 0. If equalizing along X, there simply is no information that could definitively tell us on which side each vert belongs.
	// The side for each vert is then chosen deterministically, but ultimately arbitrarily. The user will have to fix such cases manually, preferably using flipMirrorPos().
	// In other cases, for axis == X, both verts get a an averaged Y and Z coordinate, while the X coordinates will be set to + and - half the total distance.
	// This is a fast and uncomplicated way to better preserve volume when equalizing compared to just averaging all coordinates.
	// However, it cannot restore lost information. Results are best for only slightly deformed geometries.
	// A more complex solution could involve preserving the area of each face, however that would require a significantly more complex algorithm.
	// "weight" determines to what degree the current positions are blended with the calculated positions. 0 means no change, 1 means fully morphed.
	// Undoable.
	// TODO test this
	i32 blendMirrorPos(
		INode* node,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		IntTab& selection,	// bitset
		const i32 axis,
		const f32 weight
	) {
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		Bitset _selection(selection);
		return I_OrderImpl::blendMirrorPos(
			node, 
			_orderM, 
			_orderA, 
			_orderB, 
			_selection, 
			axis, 
			weight
		);
	}

	// Flips or swaps the coordinates of the "selected" verts along the chosen mirror axis.
	// If a vert is "selected" on only one side (this includes middle verts), its coordinate along the mirror axis is inverted.
	// If its counterpart vert is also "selected", both verts' positions are swapped along the mirror axis. Meaning, if the X-axis is the mirror axis,
	// then their X coordinates are swapped, while Y and Z coordinates remain untouched.
	// axis == 0 -> X
	// axis == 0 -> Y
	// axis == 0 -> Z
	// "weight" determines to what degree the current positions are blended with the calculated positions. 0 means no change, 1 means fully morphed.
	// Undoable.
	// TODO test this
	i32 flipMirrorPos(
		INode* node,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		IntTab& selection,	// bitset
		const i32 axis,
		const f32 weight
	) {
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		Bitset _selection(selection);
		return I_OrderImpl::flipMirrorPos(
			node, 
			_orderM, 
			_orderA, 
			_orderB, 
			_selection, 
			axis, 
			weight
		);
	}

	// Copies the "selected" verts' positions from one poly to another. Can self copy.
	// The order matches the verts from one poly to those of another, as computed via buildConformOrder().
	// Undoable.
	// TODO test this
	i32 copyPos(
		INode* src,
		IntTab& srcOrder,
		INode* dst,
		IntTab& dstOrder,
		IntTab& selection	// bitset
	) {
		const List<i32> _srcOrder(srcOrder);
		const List<i32> _dstOrder(dstOrder);
		const Bitset _selection(selection);
		return I_OrderImpl::copyPos(
			src, 
			_srcOrder, 
			dst, 
			_dstOrder, 
			_selection
		);
	}

	// weighted swap with w (0..1)
	//		a' = (a * w + b * (1 - w))
	//		b' = (a * (1 - w) + b * w)
	// re-balance with v (0..1)
	//		a_ = (a' * (1 - v) + b' * v)
	//		b_ = (a_ * v + b' * (1 - v))

	// Blends the positions of nodeA and nodeB, according to provided arguments.
	// "orderA" and "orderB" are used to match verts.
	// "weight" determines how much of the new positions is taken from nodeA vs nodeB. At 0, 100% of the positions are taken from B, for instance.
	// "outA" determines how much of the new positions is blended into nodeA
	// "outB" determines how much of the new positions is blended into nodeB
	// verts in selectionA are used for both sides if useSelectionA is true
	// verts in selectionB are used for both sides if useSelectionB is true
	// This method is thus suitable for conforming one selection to another, or for equalizing two selections, or what-have-you.
	// Efforts were taken to make this as lossless as is feasible. This includes accounting for values very close to 0 and 1, such that exact binary
	// copies of the floating point values are made where appropriate.
	i32 blendPos(
		INode* nodeA,
		IntTab& orderA,
		INode* nodeB,
		IntTab& orderB,
		const f32 weight,
		const f32 outA,
		const f32 outB,
		IntTab& selectionA,	// bitset
		IntTab& selectionB,	// bitset
		const bool useSelectionA,
		const bool useSelectionB
	) {
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		const Bitset _selectionA(selectionA);
		const Bitset _selectionB(selectionB);
		return I_OrderImpl::blendPos(
			nodeA, 
			_orderA, 
			nodeB, 
			_orderB, 
			weight, 
			outA, 
			outB, 
			_selectionA, 
			_selectionB, 
			useSelectionA, 
			useSelectionB
		);
	}

	// Extracts a starting set of elements from the specified poly and its virtual selection for use by buildSymmetryOrder().
	//
	// Terminology:
	// Selected: A sub-element is considered "selected" if it is contained in the relevant bitarray.
	// Patch: A "patch" is a set of "selected" faces connected by shared edges. 
	//        This means that each patch is separated from one another. They may share corner verts, but cannot share edges. 
	//        The order is built for one or two patches, not the entire mesh. The patches are specified by the "faces" array. 
	//        Only faces contained in it are considered present.
	// Border: An edge is considered a "border" if it is connected to only one patch-face.
	// Open/Closed Patch: A patch is considered open when it contains border edges and closed when it does not.
	//
	// The algorithm will attempt two find 2 faces, 2 edges and 2 verts that serve as the starting points for building a symmetry order.
	// Valid patterns are:
	// 1) One open/closed patch containing exactly one "selected" non-border edge that has no verts "selected". 
	//    This edge will be considered to lie along the symmetry axis (the middle).
	// 2) One open/closed patch containing exactly one "selected" border or non-border edge that has both verts "selected".
	//    This edge will be considered to be perpendicular to the symmetry axis. Its verts therefore land on side A and side B, respectively.
	//    This is necessary to specify an unambiguous starting point for patches that have no middle edges (and have middle faces instead).
	// 3) One open patch containing exactly two "selected" border edges that each have exactly one vert "selected". 
	//    These two edge-vert pairs must point toward each other (run in opposing directions). They may even share the same vert.
	// 4) Two open patches, each containing exactly one "selected" border edge that has exactly one vert "selected".
	// 5) Two open/closed patches, each containing exactly one face with exactly two edges "selected", of which exactly one has exactly one vert "selected". 
	//    This is necessary to specify an unambiguous starting point for two separate closed patches. 
	//    The edge with the "selected" vert serves as the starting point, the other edge identifies which face connected to the starting edge to start with.
	// 6) One open/closed patch containing exactly two faces with exactly two edges "selected", of which exactly one has exactly one vert "selected". 
	//    See 5) for an explanation. It is always simpler to use 1), but this is still valid.
	// Sub elements that don't satisfy these parameters are ignored. 
	// If too many valid starting points are available, there is ambiguity and the algorithm will terminate early.
	//
	// "axis" determines X/Y/Z.
	//
	// The extracted selection can be directly passed to buildSymmetryOrder().
	// TODO test this
	i32 extractSymmetrySelection(
		INode* node,	
		IntTab& faces,	// bitset
		IntTab& edges,	// bitset
		IntTab& verts,	// bitset
		i32& faceA,
		i32& edgeA,
		i32& vertA,
		i32& faceB,
		i32& edgeB,
		i32& vertB,
		const i32 axis
	) {
		const Bitset _faces(faces);
		const Bitset _edges(edges);
		const Bitset _verts(verts);
		return I_OrderImpl::extractSymmetrySelection(
			node, 
			_faces, 
			_edges, 
			_verts, 
			faceA, 
			edgeA, 
			vertA, 
			faceB, 
			edgeB, 
			vertB, 
			axis
		);
	}

	// Extracts a starting set of elements from the specified poly and the virtual selection for use by buildConformOrder().
	//
	// Terminology:
	// Selected: A sub-element is considered "selected" if it is contained in the relevant bitarray.
	// Patch: A "patch" is a set of "selected" faces connected by shared edges. This means that each patch is separated from one another. They may share corner
	// verts, but cannot share edges. The order is built for exactly one such patch, not the entire mesh. The patches are specified by the "faces" array. Only faces 
	// contained in it are considered present.
	// Border: An edge is considered a "border" if it is connected to only one patch-face.
	// Open/Closed Patch: A patch is considered open when it contains border edges and closed when it does not.
	//
	// The algorithm will attempt to find 1 face, 1 edge and 1 vert that serve as the starting points for building a traversal order.
	// Valid selections are:
	// 1) One open patch containing exactly one "selected" border edge that has exactly one vert "selected".
	// 2) One open/closed patch containing exactly one face with exactly two edges "selected", of which exactly one has exactly one vert "selected".
	//    This is necessary to specify an unambiguous starting point for a closed patch.
	//    The edge with the "selected" vert serves as the starting point, the other edge identifies which face connected to the starting edge to start with.
	// TODO maybe expand this to a third case like 2) that instead has 1 edge, but 2 verts. where ony 1 vert has 1 edge selected. the other vert would determine the face
	// Sub elements that don't satisfy these parameters are ignored. 
	// If too many valid starting points are available, there is ambiguity and the algorithm will terminate early.
	//
	// The extracted selection can be directly passed to buildTraversalOrder().
	// TODO test this
	i32 extractTraversalSelection(
		INode* node,
		IntTab& faces,	// bitset
		IntTab& edges,	// bitset
		IntTab& verts,	// bitset
		i32& face,
		i32& edge,
		i32& vert
	) {
		const Bitset _faces(faces);
		const Bitset _edges(edges);
		const Bitset _verts(verts);
		return I_OrderImpl::extractTraversalSelection(
			node, 
			_faces, 
			_edges, 
			_verts, 
			face, 
			edge, 
			vert
		);
	}

	// Given two sets of starting face, edge and vert (1-valued), construct three symmetry-related iteration orders (1-valued) over the 
	// verts contained in the specified "faces".
	// For each set: The vert must be part of the edge, which must be part of the face.
	// "OrderM" will contain middle verts, while "orderA" and "orderB" correspond to each other.
	// It is recommended to only use starting inputs derived from extractSymmetrySelection().
	// TODO test this
	i32 buildSymmetryOrderV(
		INode* node,
		IntTab& faces,	// bitset
		const i32 faceA,
		const i32 edgeA,
		const i32 vertA,
		const i32 faceB,
		const i32 edgeB,
		const i32 vertB,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB
	) {
		const Bitset _faces(faces);
		List<i32> _orderM(orderM);
		List<i32> _orderA(orderA);
		List<i32> _orderB(orderB);
		return I_OrderImpl::buildSymmetryOrder(
			node, 
			_faces, 
			faceA - 1,
			edgeA - 1,
			vertA - 1,
			faceB - 1,
			edgeB - 1, 
			vertB - 1,
			_orderM, 
			_orderA, 
			_orderB, 
			0
		);
	}

	i32 buildSymmetryOrderE(
		INode* node,
		IntTab& faces,	// bitset
		const i32 faceA,
		const i32 edgeA,
		const i32 vertA,
		const i32 faceB,
		const i32 edgeB,
		const i32 vertB,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB
	) {
		const Bitset _faces(faces);
		List<i32> _orderM(orderM);
		List<i32> _orderA(orderA);
		List<i32> _orderB(orderB);
		return I_OrderImpl::buildSymmetryOrder(
			node, 
			_faces, 
			faceA - 1,
			edgeA - 1,
			vertA - 1,
			faceB - 1,
			edgeB - 1,
			vertB - 1,
			_orderM, 
			_orderA, 
			_orderB, 
			1
		);
	}

	i32 buildSymmetryOrderF(
		INode* node,
		IntTab& faces,	// bitset
		const i32 faceA,
		const i32 edgeA,
		const i32 vertA,
		const i32 faceB,
		const i32 edgeB,
		const i32 vertB,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB
	) {
		const Bitset _faces(faces);
		List<i32> _orderM(orderM);
		List<i32> _orderA(orderA);
		List<i32> _orderB(orderB);
		return I_OrderImpl::buildSymmetryOrder(
			node, 
			_faces, 
			faceA - 1, 
			edgeA - 1, 
			vertA - 1, 
			faceB - 1, 
			edgeB - 1, 
			vertB - 1,
			_orderM, 
			_orderA, 
			_orderB, 
			2
		);
	}

	// Given a starting face, edge and vert (1-valued), constructs an iteration order (1-valued) over 
	// the verts contained in the specfied "faces".
	// The starting vert must be part of the starting edge, which must be part of the starting face.
	// It is recommended to only use starting inputs derived from extractConformSelection().
	// I guess traversal order might be a better name.
	// The point is that traversal order is a function of the starting selection and relational topology.
	// To drive the point home: If you have two meshes with completely different actual vertex orders, 
	// if you select the "same" stuff initially, the traversal order for each will be the same, RELATIVELY.
	// Meaning two traversal orders act as a mapping between two meshes. Or three meshes. Or four, etc...
	// This is extremely useful for solving a whole slew of problems. 
	i32 buildTraversalOrder(
		INode* node,
		IntTab& faces,	// bitset
		const i32 face,
		const i32 edge,
		const i32 vert,
		IntTab& order,
		const i32 level
	) {
		const Bitset _faces(faces);
		List<i32> _order(order);
		return I_OrderImpl::buildTraversalOrder(
			node, 
			_faces, 
			face - 1,
			edge - 1,
			vert - 1,
			_order, 
			level
		);
	}

	// Copies or blends the weights of the "selected" verts for the 1-indexed skin modifier along the "logical mirror axis".
	// The order tabs contains the "left"-, "middle"- and "right"-side verts (1-valued) in corresponding order.
	// The bones are matched against each other by name according to the provided affixes.
	// If a middle vert is "selected" it's weights are blended such that they are equally distributed along both sides.
	// For all other verts:
	// If a vert is "selected" on only one side, the counterpart vert receives a mirrored copy of its weights.
	// If the counterpart vert is also "selected", both verts are blended such that each side receives a copy of the average weights between the two, mirrored for its side.
	// Note that during blending an influence-sorted list of weights is constructed. The lowest influence weights (or pairs, in case of middle verts) is dropped
	// from this list until the list has a length <= the bone affect limit.
	// The "receive" option specifies how copying works when only one side is "selected": 
	// If "receive" is true, the "selected" vert gets a mirrored copy of the opposing vert's weights.
	// If "receive" is false, the opposing vert gets a mirrored copy of the "selected" vert's weights.
	// TODO test this
	i32 blendMirrorWeights(
		INode* node,
		const i32 modIndex,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		IntTab& selection,	// bitset
		const wchar_t* affixA,
		const bool modeA,
		const wchar_t* affixB,
		const bool modeB,
		const bool receive
	) {
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		const Bitset _selection(selection);
		Slice _affixA(affixA);
		Slice _affixB(affixB);
		return I_OrderImpl::blendMirrorWeights(
			node, 
			modIndex - 1,
			_orderM, 
			_orderA, 
			_orderB, 
			_selection, 
			_affixA, 
			modeA, 
			_affixB, 
			modeB, 
			receive
		);
	}

	// Flips or swaps the weights of the "selected" verts for the 1-indexed skin modifier along the "logical mirror axis".
	// The order tabs contains the "left"-, "middle"- and "right"-side verts (1-valued) in corresponding order.
	// The bones are matched against each other by name according to the provided affixes.
	// If a vert is "selected" on only one side, its bones are replaced with the counterpart vert's bones, but the weights and DQ blend blend weights stay intact.
	// This case always applies for "selected" middle verts, because they have no counterpart vert.
	// If a "selected" vert's counterpart vert is also "selected", then the weights are completely swapped for both, including the DQ blend weight.
	// Undoable.
	// TODO test this
	i32 flipMirrorWeights(
		INode* node,
		const i32 modIndex,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		IntTab& selection,	// bitset
		const wchar_t* affixA,
		const bool modeA,
		const wchar_t* affixB,
		const bool modeB
	) {
		// TODO, just for the hell of it:
		//   Bitsets should maybe have a limit.
		//   if stuff is ABOVE the limit, always return 0
		// Wait. that'S length.
		// Yup. I want a SECOND limit that applies to CONST shit only, though?
		// OR apply length every time.
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		const Bitset _selection(selection);
		Slice _affixA(affixA);
		Slice _affixB(affixB);
		return I_OrderImpl::flipMirrorWeights(
			node, 
			modIndex - 1,
			_orderM, 
			_orderA, 
			_orderB, 
			_selection, 
			_affixA, 
			modeA, 
			_affixB, 
			modeB
		);
	}

	// For any element selected in "src", the corresponding element is selected in "dst".
	// Level 0 -> verts
	// Level 0 -> edges
	// Level 0 -> faces
	// If "clear" is true, all other elements of that level are unselected.
	// Undoable.
	i32 translateSelection(
		INode* src,
		IntTab& srcOrder,
		INode* dst,
		IntTab& dstOrder,
		IntTab& srcSelection,	// bitset
		const i32 level,
		const i32 mode
	) {
		const List<i32> _srcOrder(srcOrder);
		const List<i32> _dstOrder(dstOrder);
		const Bitset _srcSelection(srcSelection);
		return I_OrderImpl::translateSelection(
			src, 
			_srcOrder, 
			dst, 
			_dstOrder, 
			_srcSelection, 
			level, 
			mode
		);
	}

	// For any vert handle selected in indexed "src" skin mod, the corresponding element is selected in "dst" skin mod.
	// Undoable.
	i32 translateHandles(
		INode* src,
		const i32 srcModIndex,
		IntTab& srcOrder,
		INode* dst,
		const i32 dstModIndex,
		IntTab& dstOrder,
		IntTab& srcSelection,	// bitset
		const i32 mode
	) {
		const List<i32> _srcOrder(srcOrder);
		const List<i32> _dstOrder(dstOrder);
		const Bitset _srcSelection(srcSelection);
		return I_OrderImpl::translateHandles(
			src, 
			srcModIndex - 1, 
			_srcOrder, 
			dst, 
			dstModIndex- 1, 
			_dstOrder, 
			_srcSelection, 
			mode
		);
	}

	// Swaps selected state for corresponding elements.
	// Mode < 0: Remove specified elements from selection
	// Mode == 0: Set exactly to specified elements.
	// Mode > 0: Add specified to selection.
	// Other elements are unaffected.
	// Undoable.
	i32 mirrorSelection(
		INode* node,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		const i32 level,
		const i32 mode
	) {
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);
		return I_OrderImpl::mirrorSelection(
			node, 
			_orderM, 
			_orderA, 
			_orderB, 
			level, 
			mode
		);
	}

	// Swaps selected state for corresponding elements.
	// If clear is false, no element will be unselected.
	// Other elements are unaffected.
	// Undoable.
	i32 mirrorHandles(
		INode* node,
		const i32 modIndex,
		IntTab& orderM,
		IntTab& orderA,
		IntTab& orderB,
		const i32 mode
	) {
		const List<i32> _orderM(orderM);
		const List<i32> _orderA(orderA);
		const List<i32> _orderB(orderB);

		return I_OrderImpl::mirrorHandles(
			node, 
			modIndex - 1, 
			_orderM, 
			_orderA, 
			_orderB,
			mode
		);
	}

	// Transfers weights losslessly from one skin modifier to another. The bones are re-mapped from the source to dst mod 
	// by name, according to the provided affixes. No bones are added to the destination modifier, the mapped bones must already be listed.
	// The "srcOrder" and "dstOrder" contain the corresponding verts (1-valued) of "src" and "dst".
	// "selection" contains the verts in "src" whose weights to transfer.
	// Undoable.
	// TODO test this
	i32 transferWeights(
		INode* src,
		const i32 srcModIndex,
		IntTab& srcOrder,
		INode* dst,
		const i32 dstModIndex,
		IntTab& dstOrder,
		IntTab& selection,	// bitset
		const wchar_t* srcAffix,
		const bool srcMode,
		const wchar_t* dstAffix,
		const bool dstMode
	) {
		const List<i32> _srcOrder(srcOrder);
		const List<i32> _dstOrder(dstOrder);
		const Bitset _selection(selection);
		Slice _srcAffix(srcAffix);
		Slice _dstAffix(dstAffix);
		return I_OrderImpl::transferWeights(
			src, 
			srcModIndex - 1,
			_srcOrder, 
			dst, 
			dstModIndex - 1,
			_dstOrder, 
			_selection, 
			_srcAffix, 
			srcMode,
			_dstAffix, 
			dstMode
		);
	}

	// Returns the next selected element(type 0: vert, 1: edge, 2: face) >= start or 0 if no such element exists, or < 0 on error.
	// For example, nextSelected $ 0 5 returns 5 if vert 5 is selected
	// For example, nextSelected $ 0 5 returns 10 if vert 5..9 are unselected, but 10 is selected.
	// This is cheaper than getting a full selection array, when all you want is one or two elements.
	// TODO test this
	//i32 nextSelected(
	//	INode* node,
	//	const i32 level,
	//	const i32 start
	//) {
	//	return 0;
	//}

	// Any bits set in selectionA for orderA are added to selectionB for corresponding orderB.
	// E.g. with corresponding orders 
	// A = [1, 5, 3, 10, ...] and
	// B = [4, 8, 10, 2, ...]
	// if bit 5 is set for A, bit 8 will be set for B
	// If "clear" is true, selectionB will be cleared first.
	// Orders contain 1-indexed elements and must have the same length.
	// TODO test this
	//i32 transferBits(
	//	IntTab& orderA,
	//	IntTab& selectionA,
	//	IntTab& orderB,
	//	IntTab& selectionB,
	//	const bool clear				// if != 0, clears selectionB first
	//) {
	//	return 0;
	//}

	enum functionID : FunctionID {
		// order
		extractSymmetrySelectionID,
		extractTraversalSelectionID,
		// had to split there because of 11 argument limit... TODO could use arrays instead?
		buildSymmetryOrderVID,
		buildSymmetryOrderEID,
		buildSymmetryOrderFID,
		buildTraversalOrderID,
		blendMirrorPosID,
		flipMirrorPosID,
		copyPosID,
		blendPosID,
		blendMirrorWeightsID,
		flipMirrorWeightsID,
		translateSelectionID,
		translateHandlesID,
		mirrorSelectionID,
		mirrorHandlesID,
		transferWeightsID,
		// remove?
		//nextSelectedID,
		//transferBitsID,
	};

	I_Order() : FPInterfaceDesc(I_ORDR_ID, _M("Order"), 0, GetMXTopoToolsDesc(), FP_STATIC_METHODS, p_end) {
		AppendFunction(
			blendMirrorPosID, _M("blendMirrorPos"), 0, TYPE_INT, 0, 7
			, _M("node"), 0, TYPE_INODE
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("axis"), 0, TYPE_INT
			, _M("weight"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			flipMirrorPosID, _M("flipMirrorPos"), 0, TYPE_INT, 0, 7
			, _M("node"), 0, TYPE_INODE
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("axis"), 0, TYPE_INT
			, _M("weight"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			copyPosID, _M("copyPos"), 0, TYPE_INT, 0, 5
			, _M("src"), 0, TYPE_INODE
			, _M("srcOrder"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_INODE
			, _M("dstOrder"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			blendPosID, _M("blendPos"), 0, TYPE_INT, 0, 11
			, _M("nodeA"), 0, TYPE_INODE
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("nodeB"), 0, TYPE_INODE
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("weight"), 0, TYPE_FLOAT
			, _M("outA"), 0, TYPE_FLOAT
			, _M("outB"), 0, TYPE_FLOAT
			, _M("selectionA"), 0, TYPE_INT_TAB_BR
			, _M("selectionB"), 0, TYPE_INT_TAB_BR
			, _M("useSelectionA"), 0, TYPE_BOOL
			, _M("useSelectionB"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			extractSymmetrySelectionID, _M("extractSymmetrySelection"), 0, TYPE_INT, 0, 11
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("verts"), 0, TYPE_INT_TAB_BR
			, _M("faceA"), 0, TYPE_INT_BR
			, _M("edgeA"), 0, TYPE_INT_BR
			, _M("vertA"), 0, TYPE_INT_BR
			, _M("faceB"), 0, TYPE_INT_BR
			, _M("edgeB"), 0, TYPE_INT_BR
			, _M("vertB"), 0, TYPE_INT_BR
			, _M("axis"), 0, TYPE_FLOAT
			, p_end);
		AppendFunction(
			extractTraversalSelectionID, _M("extractTraversalSelection"), 0, TYPE_INT, 0, 7
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("verts"), 0, TYPE_INT_TAB_BR
			, _M("face"), 0, TYPE_INT_BR
			, _M("edge"), 0, TYPE_INT_BR
			, _M("vert"), 0, TYPE_INT_BR
			, p_end);
		AppendFunction(
			buildSymmetryOrderVID, _M("buildSymmetryOrderV"), 0, TYPE_INT, 0, 11
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("faceA"), 0, TYPE_INT
			, _M("edgeA"), 0, TYPE_INT
			, _M("vertA"), 0, TYPE_INT
			, _M("faceB"), 0, TYPE_INT
			, _M("edgeB"), 0, TYPE_INT
			, _M("vertB"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			buildSymmetryOrderEID, _M("buildSymmetryOrderE"), 0, TYPE_INT, 0, 11
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("faceA"), 0, TYPE_INT
			, _M("edgeA"), 0, TYPE_INT
			, _M("vertA"), 0, TYPE_INT
			, _M("faceB"), 0, TYPE_INT
			, _M("edgeB"), 0, TYPE_INT
			, _M("vertB"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			buildSymmetryOrderFID, _M("buildSymmetryOrderF"), 0, TYPE_INT, 0, 11
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("faceA"), 0, TYPE_INT
			, _M("edgeA"), 0, TYPE_INT
			, _M("vertA"), 0, TYPE_INT
			, _M("faceB"), 0, TYPE_INT
			, _M("edgeB"), 0, TYPE_INT
			, _M("vertB"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			buildTraversalOrderID, _M("buildTraversalOrder"), 0, TYPE_INT, 0, 7
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("face"), 0, TYPE_INT
			, _M("edge"), 0, TYPE_INT
			, _M("vert"), 0, TYPE_INT
			, _M("order"), 0, TYPE_INT_TAB_BR
			, _M("level"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			blendMirrorWeightsID, _M("blendMirrorWeights"), 0, TYPE_INT, 0, 11
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("affixA"), 0, TYPE_STRING
			, _M("modeA"), 0, TYPE_BOOL
			, _M("affixB"), 0, TYPE_STRING
			, _M("modeB"), 0, TYPE_BOOL
			, _M("receive"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			flipMirrorWeightsID, _M("flipMirrorWeights"), 0, TYPE_INT, 0, 10
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("affixA"), 0, TYPE_STRING
			, _M("modeA"), 0, TYPE_BOOL
			, _M("affixB"), 0, TYPE_STRING
			, _M("modeB"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			translateSelectionID, _M("translateSelection"), 0, TYPE_INT, 0, 7
			, _M("src"), 0, TYPE_INODE
			, _M("srcOrder"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_INODE
			, _M("dstOrder"), 0, TYPE_INT_TAB_BR
			, _M("srcSelection"), 0, TYPE_INT_TAB_BR
			, _M("level"), 0, TYPE_INT
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			translateHandlesID, _M("translateHandles"), 0, TYPE_INT, 0, 8
			, _M("src"), 0, TYPE_INODE
			, _M("srcModIndex"), 0, TYPE_INT
			, _M("srcOrder"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_INODE
			, _M("dstModIndex"), 0, TYPE_INT
			, _M("dstOrder"), 0, TYPE_INT_TAB_BR
			, _M("srcSelection"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			mirrorSelectionID, _M("mirrorSelection"), 0, TYPE_INT, 0, 6
			, _M("node"), 0, TYPE_INODE
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("level"), 0, TYPE_INT
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			mirrorHandlesID, _M("mirrorHandles"), 0, TYPE_INT, 0, 6
			, _M("node"), 0, TYPE_INODE
			, _M("modIndex"), 0, TYPE_INT
			, _M("orderM"), 0, TYPE_INT_TAB_BR
			, _M("orderA"), 0, TYPE_INT_TAB_BR
			, _M("orderB"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			transferWeightsID, _M("transferWeights"), 0, TYPE_INT, 0, 11
			, _M("src"), 0, TYPE_INODE
			, _M("srcModIndex"), 0, TYPE_INT
			, _M("srcOrder"), 0, TYPE_INT_TAB_BR
			, _M("dst"), 0, TYPE_INODE
			, _M("dstModIndex"), 0, TYPE_INT
			, _M("dstOrder"), 0, TYPE_INT_TAB_BR
			, _M("selection"), 0, TYPE_INT_TAB_BR
			, _M("srcAffix"), 0, TYPE_STRING
			, _M("srcMode"), 0, TYPE_BOOL
			, _M("dstAffix"), 0, TYPE_STRING
			, _M("dstMode"), 0, TYPE_BOOL
			, p_end);
		//	AppendFunction(
		//		nextSelectedID, _M("nextSelected"), 0, TYPE_INT, 0, 3
		//		, _M("node"), 0, TYPE_INODE
		//		, _M("level"), 0, TYPE_INT
		//		, _M("start"), 0, TYPE_INT
		//		, p_end);
		//AppendFunction(
		//	transferBitsID, _M("transferBits"), 0, TYPE_INT, 0, 5
		//	, _M("orderA"), 0, TYPE_INT_TAB_BR
		//	, _M("selectionA"), 0, TYPE_INT_TAB_BR
		//	, _M("orderB"), 0, TYPE_INT_TAB_BR
		//	, _M("selectionB"), 0, TYPE_INT_TAB_BR
		//	, _M("clear"), 0, TYPE_BOOL
		//	, p_end);
	}
	BEGIN_FUNCTION_MAP
		FN_7(blendMirrorPosID, TYPE_INT, blendMirrorPos
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT_TAB_BR	// selection
			, TYPE_INT			// axis
			, TYPE_FLOAT		// weight
		)
		FN_7(flipMirrorPosID, TYPE_INT, flipMirrorPos
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT_TAB_BR	// selection
			, TYPE_INT			// axis
			, TYPE_FLOAT		// weight
		)
		FN_5(copyPosID, TYPE_INT, copyPos
			, TYPE_INODE		// src
			, TYPE_INT_TAB_BR	// srcOrder
			, TYPE_INODE		// dst
			, TYPE_INT_TAB_BR	// dstOrder
			, TYPE_INT_TAB_BR	// selection
		)
		FN_11(blendPosID, TYPE_INT, blendPos
			, TYPE_INODE		// nodeA
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INODE		// nodeB
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_FLOAT		// weight
			, TYPE_FLOAT		// outA
			, TYPE_FLOAT		// outB
			, TYPE_INT_TAB_BR	// selectionA
			, TYPE_INT_TAB_BR	// selectionB
			, TYPE_BOOL			// useSelectionA
			, TYPE_BOOL			// useSelectionB
		)
		FN_11(extractSymmetrySelectionID, TYPE_INT, extractSymmetrySelection
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT_TAB_BR	// edges
			, TYPE_INT_TAB_BR	// verts
			, TYPE_INT_BR		// faceA
			, TYPE_INT_BR		// edgeA
			, TYPE_INT_BR		// vertA
			, TYPE_INT_BR		// faceB
			, TYPE_INT_BR		// edgeB
			, TYPE_INT_BR		// vertB
			, TYPE_FLOAT		// axis
		)
		FN_7(extractTraversalSelectionID, TYPE_INT, extractTraversalSelection
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT_TAB_BR	// edges
			, TYPE_INT_TAB_BR	// verts
			, TYPE_INT_BR		// face
			, TYPE_INT_BR		// edge
			, TYPE_INT_BR		// vert
		)
		FN_11(buildSymmetryOrderVID, TYPE_INT, buildSymmetryOrderV
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT			// faceA
			, TYPE_INT			// edgeA
			, TYPE_INT			// vertA
			, TYPE_INT			// faceB
			, TYPE_INT			// edgeB
			, TYPE_INT			// vertB
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
		)
		FN_11(buildSymmetryOrderEID, TYPE_INT, buildSymmetryOrderE
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT			// faceA
			, TYPE_INT			// edgeA
			, TYPE_INT			// vertA
			, TYPE_INT			// faceB
			, TYPE_INT			// edgeB
			, TYPE_INT			// vertB
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
		)
		FN_11(buildSymmetryOrderFID, TYPE_INT, buildSymmetryOrderF
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT			// faceA
			, TYPE_INT			// edgeA
			, TYPE_INT			// vertA
			, TYPE_INT			// faceB
			, TYPE_INT			// edgeB
			, TYPE_INT			// vertB
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
		)
		FN_7(buildTraversalOrderID, TYPE_INT, buildTraversalOrder
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_INT			// face
			, TYPE_INT			// edge
			, TYPE_INT			// vert
			, TYPE_INT_TAB_BR	// order
			, TYPE_INT			// level
		)
		FN_11(blendMirrorWeightsID, TYPE_INT, blendMirrorWeights
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT_TAB_BR	// selection
			, TYPE_STRING		// affixA
			, TYPE_BOOL			// modeA
			, TYPE_STRING		// affixB
			, TYPE_BOOL			// modeB
			, TYPE_BOOL			// receive
		)
		FN_10(flipMirrorWeightsID, TYPE_INT, flipMirrorWeights
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT_TAB_BR	// selection
			, TYPE_STRING		// affixA
			, TYPE_BOOL			// modeA
			, TYPE_STRING		// affixB
			, TYPE_BOOL			// modeB
		)
		FN_7(translateSelectionID, TYPE_INT, translateSelection
			, TYPE_INODE		// src
			, TYPE_INT_TAB_BR	// srcOrder
			, TYPE_INODE		// dst
			, TYPE_INT_TAB_BR	// dstOrder
			, TYPE_INT_TAB_BR	// srcSelection
			, TYPE_INT			// level
			, TYPE_INT			// mode
		)
		FN_8(translateHandlesID, TYPE_INT, translateHandles
			, TYPE_INODE		// src
			, TYPE_INT			// srcModIndex
			, TYPE_INT_TAB_BR	// srcOrder
			, TYPE_INODE		// dst
			, TYPE_INT			// dstModIndex
			, TYPE_INT_TAB_BR	// dstOrder
			, TYPE_INT_TAB_BR	// srcSelection
			, TYPE_INT			// mode
		)
		FN_6(mirrorSelectionID, TYPE_INT, mirrorSelection
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT			// level
			, TYPE_INT			// mode
		)
		FN_6(mirrorHandlesID, TYPE_INT, mirrorHandles
			, TYPE_INODE		// node
			, TYPE_INT			// modIndex
			, TYPE_INT_TAB_BR	// orderM
			, TYPE_INT_TAB_BR	// orderA
			, TYPE_INT_TAB_BR	// orderB
			, TYPE_INT			// mode
		)
		FN_11(transferWeightsID, TYPE_INT, transferWeights
			, TYPE_INODE		// src
			, TYPE_INT			// srcModIndex
			, TYPE_INT_TAB_BR	// srcOrder
			, TYPE_INODE		// dst
			, TYPE_INT			// dstModIndex
			, TYPE_INT_TAB_BR	// dstOrder
			, TYPE_INT_TAB_BR	// selection
			, TYPE_STRING		// srcAffix
			, TYPE_BOOL			// srcMode
			, TYPE_STRING		// dstAffix
			, TYPE_BOOL			// dstMode
		)
		//	FN_3(nextSelectedID, TYPE_INT, nextSelected
		//		, TYPE_INODE		// node
		//		, TYPE_INT			// level
		//		, TYPE_INT			// start
		//	)
		//FN_5(transferBitsID, TYPE_INT, transferBits
		//	, TYPE_INT_TAB_BR	// orderA
		//	, TYPE_INT_TAB_BR	// selectionA
		//	, TYPE_INT_TAB_BR	// orderB
		//	, TYPE_INT_TAB_BR	// selectionB
		//	, TYPE_BOOL			// clear
		//)
	END_FUNCTION_MAP
};
