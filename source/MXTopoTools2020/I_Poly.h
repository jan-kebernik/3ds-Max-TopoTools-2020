#pragma once

#include "Util.h"

#include "I_PolyImpl.h"
#include "Helpers.h"
#include "Restore.h"

extern const Interface_ID I_POLY_ID;

class I_Poly : public FPInterfaceDesc
{
public:

	// Returns 1 if the node is a well-formed editable poly, 0 if not, < 0 on error.
	i32 isPoly(
		INode* node
	) {
		return I_PolyImpl::isPoly(node);
	}

	// Collects the specified verts, edges or faces.
	// level = 0 -> verts
	// level = 1 -> edges
	// level = 2 -> faces
	// Dead elements that are still marked as selected are ignored.
	// Why not just use polyop, you ask? Because it fails spuriously when certain modifiers are selected or whatever.
	// This is more reliable and no less efficient. In fact, more efficient because re-using the same objects puts less strain on the clunky GC.
	// ____ tested and working
	// mode == 0 set out to selected
	// mode > 0 add selected to out
	// mode < 0 subtract selected from out
	i32 getSelection(
		INode* node,
		const i32 level,
		IntTab& out,	// bitset
		const i32 mode
	) {
		Bitset _out(out);
		return I_PolyImpl::getSelection(
			node, 
			level, 
			_out, 
			mode
		);
	}

	// Selects the specified verts, edges or faces.
	// mode == 0 only the specified elements will be selected
	// mode > 0 specified elements are added to selection
	// mode < 0 specified elements are removed from selection
	// level = 0 -> verts
	// level = 1 -> edges
	// level = 2 -> faces
	// undoable
	// ____ tested and working
	i32 setSelection(
		INode* node,
		const i32 level,
		IntTab& in,	// bitset
		const i32 mode
	) {
		const Bitset _in(in);
		return I_PolyImpl::setSelection(
			node, 
			level, 
			_in, 
			mode
		);
	}

	// same, but takes a list of ints
	i32 setSelection2(
		INode* node,
		const i32 level,
		IntTab& in,	// bitset
		const i32 mode
	) {
		const List<i32> _in(in);
		return I_PolyImpl::setSelection2(
			node, 
			level, 
			_in, 
			mode
		);
	}

	// Copies a (0-indexed) range of vert positions from "src" to "dst". Can self copy.
	// "srcOff" is the 0-indexed vert to copy from (inclusive).
	// "dstOff" is the 0-indexed vert to copy to (inclusive).
	// "len" is the number of vert positions to copy.
	// Undoable.
	i32 copyVerts(
		INode* src,
		const i32 srcOff,
		INode* dst,
		const i32 dstOff,
		const i32 len
	) {
		return I_PolyImpl::copyVerts(
			src, 
			srcOff, 
			dst, 
			dstOff, 
			len
		);
	}

	// True if any elements are dead.
	// Returns 1 if any structs are dead, 0 if none or < 0 on error.
	// TODO actually test this
	i32 hasDead(
		INode* node
	) {
		return I_PolyImpl::hasDead(node);
	}

	// Remove dead stuff. 
	// This will preserver the weights of any skin mods applied to the node. 
	// However all other mods are kept as they are.
	// Undoable.
	// TODO actually test this
	i32 collapseDead(
		INode* node
	) {
		return I_PolyImpl::collapseDead(node);
	}

	// Deletes the specified faces from the poly. 
	// Weight data for all skin modifiers is preserved in a lossless manner. Sub-object selection is preserved.
	// Undoable.
	// ____ tested and working
	// TODO non-critical bug: rarely after, skin mods still report the old number of points. It's really hard to reproduce, though, so I can't test things properly.
	i32 deleteFaces(
		INode* node,
		IntTab& faces	// bitset
	) {
		const Bitset _faces(faces);
		return I_PolyImpl::deleteFaces(node, _faces);
	}

	// Removes the specified edges from the poly. This is not the same as deleting them.
	// If "removeEdgeVerts" is true, those verts that as a results of the edge removal end up having only two edges left, are removed as well.
	// Weight data for all skin modifiers is preserved in a lossless manner. Element selections are preserved.
	// Undoable.
	// ____ tested and working
	// TODO non-critical bug: rarely after, skin mods still report the old number of points. It's really hard to reproduce, though, so I can't test things properly.
	i32 removeEdges(
		INode* node,
		IntTab& edges,	// bitset
		const bool removeEdgeVerts	// also removes the verts that as a result of removing edges only have 2 edges left
	) {
		const Bitset _edges(edges);
		return I_PolyImpl::removeEdges(node, _edges, removeEdgeVerts);
	}

	// Finds and target-welds opposing selected border verts. Several selections are valid:
	// 1) Exactly two separate runs of selected border edges of the same length, that do not form loops. 
	//    The verts of the two runs will be welded.
	// 2) Exactly one run of selected border edges, containing exactly two borders. 
	//    The two edges are connected. The two non-middle verts will be welded.
	// 3) Exactly two selected border verts. 
	//    If they lie on the same edge, the edge itself is welded.
	// Non-border edges are ignored. The face orientation is taken into account to result in a valid operation.
	// If one of these is found, the verts will be welded according to the "direction" given, which determines 
	// which "side"'s positions and weights are retained. The side is chosen by the "direction" deterministically, 
	// but ultimately arbitrarily. So you may need to undo and flip the direction around in order to get the desired result.
	// After welding, all dead structures are collapsed. This can reduce the number of verts, edges and faces in 
	// possibly unexpected ways.
	// This can mess with modifiers that depend on topology. The function will not check for this, except for skin modifiers, 
	// whose weights are losslessly modidified.
	// Undoable, and performs many orders of magnitude faster than any possible solution implemented using PolyOp and SkinOps. 
	// E.g. it is possible to weld 100.000+ verts in a second, whereas the naive solution would take minutes or hours or just 
	// crashes because it creates a new undo object for every single vert pair.
	// ____ tested and working
	// TODO maybe also deal with normals. but it's not critical.
	i32 weldBorders(
		INode* node,
		IntTab& edges,	// bitset
		IntTab& verts,	// bitset
		const bool direction
	) {
		const Bitset _edges(edges);
		const Bitset _verts(verts);
		return I_PolyImpl::weldBorders(
			node, 
			_edges, 
			_verts, 
			direction
		);
	}

	// Adds all specified nodes to a clone of the dst node, which will be selected automatically.
	// All nodes must have the same number of skin modifiers as the dst node and must be editable polies.
	// Losslessly combines all weight data for each "row" of corresponding skin mods into one skin mod per row. 
	// Undoable.
	// ____ tested and working
	i32 attach(
		INode* dst,
		Tab<INode*>& nodes
	) {
		// TODO Note that if getting weird results, maybe try stripping mods instead of disabling them. 
		// The implementation already supports this.
		List<INode*> _nodes(nodes);
		return I_PolyImpl::attach(
			dst, 
			_nodes, 
			false
		);
	}

	// Copies the specified faces into a new node, along with all modifiers, while losslessly preserving weight data.
	// If "keep" is false, deletes the same faces from the original node. Automatically selects the new node.
	// Undoable.
	// ____ tested and working
	i32 detach(
		INode* node,
		IntTab& faces,	// bitset
		const bool keep
	) {
		Bitset _faces(faces);
		return I_PolyImpl::detach(
			node, 
			_faces, 
			keep
		);
	}

	// For each patch*, tries to find the edges and verts making up its subdivision.
	// If a patch is fully closed (no physical or selection borders) or has no identifiable corners, then there must be exactly one vert selected for that patch that 
	// determines the "phase" of the subdivision. This would be a vert that stays around after unsubdividing.
	// Does not actually modify the scene, just collects the edges and verts that can be removed.
	// *A patch is simply a set of selected faces connected by shared edges.
	// Returns 0 on success, < 0 on error or > 0 on partial success, where 0 minus the returned value is the error code. 
	// Partial success means that at least one patch was processed successfully, but at least one error has occured.
	// ____ tested and working
	i32 unsubdivide(
		INode* node,
		IntTab& selVerts,	// bitset
		IntTab& selFaces,	// bitset
		IntTab& edges,		// bitset
		IntTab& verts		// bitset
	) {
		const Bitset _selVerts(selVerts);
		const Bitset _selFaces(selFaces);
		Bitset _edges(edges);
		Bitset _verts(verts);
		return I_PolyImpl::unsubdivide(
			node, 
			_selVerts, 
			_selFaces, 
			_edges, 
			_verts
		);
	}

	// For each patch*, tries to find the edges that can be removed in order to form quads unambiguously. Because there is a lot of ambiguity generally, only a little 
	// bit of progress can be made per call, if any. The user has to iteratively remove edges or refine the selection to fully detriangulate the desired faces.
	// If "grid" is true, the algorithm becomes less strict but also only works for well-formed grids of "quads" (no contractions or forks (t-poles)). 
	// Does not actually modify the scene, just collects the edges that can be removed.
	// *A patch is simply a set of selected faces connected by shared edges.
	// It helps to already have unambiguous corner triangles or quads in the selection. Non-grid mode works best with "strips" of faces selected.
	// Returns 0 on success, < 0 on error or > 0 on partial success, where 0 minus the returned value is the error code. 
	// Partial success means that at least one patch was processed successfully, but at least one error has occured.
	// ____ tested and working
	i32 detriangulate(
		INode* node,
		IntTab& selFaces,	// bitset
		IntTab& edges,		// bitset
		const bool grid
	) {
		const Bitset _selFaces(selFaces);
		Bitset _edges(edges);
		return I_PolyImpl::detriangulate(
			node, 
			_selFaces, 
			_edges, 
			grid
		);
	}

	enum functionID : FunctionID {
		isPolyID,
		getSelectionID,
		setSelectionID,
		setSelection2ID,
		copyVertsID,
		hasDeadID,
		collapseDeadID,
		deleteFacesID,
		removeEdgesID,
		weldBordersID,
		attachID,
		detachID,
		unsubdivideID,
		detriangulateID,
	};

	I_Poly() : FPInterfaceDesc(I_POLY_ID, _M("Poly"), 0, GetMXTopoToolsDesc(), FP_STATIC_METHODS, p_end) {
		AppendFunction(
			isPolyID, _M("isPoly"), 0, TYPE_INT, 0, 1
			, _M("node"), 0, TYPE_INODE
			, p_end);
		AppendFunction(
			getSelectionID, _M("getSelection"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("level"), 0, TYPE_INT
			, _M("out"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			setSelectionID, _M("setSelection"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("level"), 0, TYPE_INT
			, _M("in"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			setSelection2ID, _M("setSelection2"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("level"), 0, TYPE_INT
			, _M("in"), 0, TYPE_INT_TAB_BR
			, _M("mode"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			copyVertsID, _M("copyVerts"), 0, TYPE_INT, 0, 5
			, _M("src"), 0, TYPE_INODE
			, _M("srcOff"), 0, TYPE_INT
			, _M("dst"), 0, TYPE_INODE
			, _M("dstOff"), 0, TYPE_INT
			, _M("len"), 0, TYPE_INT
			, p_end);
		AppendFunction(
			hasDeadID, _M("hasDead"), 0, TYPE_INT, 0, 1
			, _M("node"), 0, TYPE_INODE
			, p_end);
		AppendFunction(
			collapseDeadID, _M("collapseDead"), 0, TYPE_INT, 0, 1
			, _M("node"), 0, TYPE_INODE
			, p_end);
		AppendFunction(
			deleteFacesID, _M("deleteFaces"), 0, TYPE_INT, 0, 2
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			removeEdgesID, _M("removeEdges"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("removeEdgeVerts"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			weldBordersID, _M("weldBorders"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("verts"), 0, TYPE_INT_TAB_BR
			, _M("direction"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			attachID, _M("attach"), 0, TYPE_INT, 0, 2
			, _M("dst"), 0, TYPE_INODE
			, _M("nodes"), 0, TYPE_INODE_TAB_BR
			, p_end);
		AppendFunction(
			detachID, _M("detach"), 0, TYPE_INT, 0, 3
			, _M("node"), 0, TYPE_INODE
			, _M("faces"), 0, TYPE_INT_TAB_BR
			, _M("keep"), 0, TYPE_BOOL
			, p_end);
		AppendFunction(
			unsubdivideID, _M("unsubdivide"), 0, TYPE_INT, 0, 5
			, _M("node"), 0, TYPE_INODE
			, _M("selVerts"), 0, TYPE_INT_TAB_BR
			, _M("selFaces"), 0, TYPE_INT_TAB_BR
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("verts"), 0, TYPE_INT_TAB_BR
			, p_end);
		AppendFunction(
			detriangulateID, _M("detriangulate"), 0, TYPE_INT, 0, 4
			, _M("node"), 0, TYPE_INODE
			, _M("selFaces"), 0, TYPE_INT_TAB_BR
			, _M("edges"), 0, TYPE_INT_TAB_BR
			, _M("grid"), 0, TYPE_BOOL
			, p_end);
	}
	BEGIN_FUNCTION_MAP
		FN_1(isPolyID, TYPE_INT, isPoly
			, TYPE_INODE		// node
		)
		FN_4(getSelectionID, TYPE_INT, getSelection
			, TYPE_INODE		// node
			, TYPE_INT			// level
			, TYPE_INT_TAB_BR	// out
			, TYPE_INT			// mode
		)
		FN_4(setSelectionID, TYPE_INT, setSelection
			, TYPE_INODE		// node
			, TYPE_INT			// level
			, TYPE_INT_TAB_BR	// in
			, TYPE_INT			// mode
		)
		FN_4(setSelection2ID, TYPE_INT, setSelection2
			, TYPE_INODE		// node
			, TYPE_INT			// level
			, TYPE_INT_TAB_BR	// in
			, TYPE_INT			// mode
		)
		FN_5(copyVertsID, TYPE_INT, copyVerts
			, TYPE_INODE		// src
			, TYPE_INT			// srcOff
			, TYPE_INODE		// dst
			, TYPE_INT			// dstOff
			, TYPE_INT			// len
		)
		FN_1(hasDeadID, TYPE_INT, hasDead
			, TYPE_INODE		// node
		)
		FN_1(collapseDeadID, TYPE_INT, collapseDead
			, TYPE_INODE		// node
		)
		FN_2(deleteFacesID, TYPE_INT, deleteFaces
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
		)
		FN_3(removeEdgesID, TYPE_INT, removeEdges
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// edges
			, TYPE_BOOL			// removeEdgeVerts
		)
		FN_4(weldBordersID, TYPE_INT, weldBorders
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// edges
			, TYPE_INT_TAB_BR	// verts
			, TYPE_BOOL			// direction
		)
		FN_2(attachID, TYPE_INT, attach
			, TYPE_INODE		// dst
			, TYPE_INODE_TAB_BR	// nodes
		)
		FN_3(detachID, TYPE_INT, detach
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// faces
			, TYPE_BOOL			// keep
		)
		FN_5(unsubdivideID, TYPE_INT, unsubdivide
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// selVerts
			, TYPE_INT_TAB_BR	// selFaces
			, TYPE_INT_TAB_BR	// edges
			, TYPE_INT_TAB_BR	// verts
		)
		FN_4(detriangulateID, TYPE_INT, detriangulate
			, TYPE_INODE		// node
			, TYPE_INT_TAB_BR	// selFaces
			, TYPE_INT_TAB_BR	// edges
			, TYPE_BOOL			// grid
		)
	END_FUNCTION_MAP

};
