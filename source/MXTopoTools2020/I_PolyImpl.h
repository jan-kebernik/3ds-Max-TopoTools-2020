#pragma once

#include "Util.h"
#include "Codes.h"
#include "Helpers.h"
#include "Restore.h"

class I_PolyImpl {

private:
	static inline void computePostRemovalWeights(
		const i32 numSkin,
		const i32 numAfter,
		const RemovalTranslation& trans,
		const Restore::WeightLossBefore& restA,
		Pointers<BitArray>& sel,
		Pointers<Restore::Snapshot>& snap
	) {
		for (i32 i = 0; i < numSkin; ++i) {
			// translate old to new selection
			const BitArray& selBefore = restA.selection(i);
			BitArray* selAfter = new BitArray(numAfter);
			for (RemovalTranslation::RedoIterator it(trans, true); it.next();) {
				if (selBefore[it.fromIndex()]) {
					selAfter->Set(it.toIndex());
				}
			}
			sel.push(selAfter);
			// translate old to new weight tables
			const Restore::Snapshot& snapBefore = restA.weights(i);
			Pointers<Tab<INode*>>* bonesAfter = new Pointers<Tab<INode*>>(numAfter, numAfter);
			Pointers<FloatTab>* weightsAfter = new Pointers<FloatTab>(numAfter, numAfter);
			List<f32>* dqsAfter = new List<f32>(numAfter, numAfter);
			for (RemovalTranslation::RedoIterator it(trans, true); it.next();) {
				const i32 from = it.fromIndex();
				const i32 to = it.toIndex();
				(*bonesAfter)[to] = Util::tabCopy(*(*snapBefore.m_bones)[from]);
				(*weightsAfter)[to] = Util::tabCopy(*(*snapBefore.m_weights)[from]);
				(*dqsAfter)[to] = (*snapBefore.m_dqs)[from];
			}
			snap.push(new Restore::Snapshot(bonesAfter, weightsAfter, dqsAfter));
		}
	}

private:
	static inline void applyAfterTopoChange(const List<Helpers::SkinMod>& skins, const i32 numAfter, const Pointers<BitArray>& sel, const Pointers<Restore::Snapshot>& snap) {
		for (i32 i = 0; i < skins.length(); ++i) {
			Helpers::SkinMod& mod = skins[i];
			BitArray& selAfter = *sel[i];
			Restore::Snapshot& snapAfter = *snap[i];
			mod.setVertexSelection(selAfter);
			for (i32 j = 0; j < numAfter; ++j) {
				mod.addWeights(j, *(*snapAfter.m_bones)[j], *(*snapAfter.m_weights)[j]);
			}
			for (i32 j = 0; j < numAfter; ++j) {
				mod.setDQBlendWeight(j, (*snapAfter.m_dqs)[j]);
			}
			mod.mod().NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
			mod.toggleMod();	// necessary to fix broken cached display mesh? maybe we can do this differently? TODO
		}
	}











public:
	static inline i32 isPoly(
		INode* node
	) {
		using ERR = Codes::I_Poly_isPoly;
		Helpers::Poly p;
		const i32 r = p.init(node);
		if (r == Codes::OK) {
			return 1;
		}
		if (r == Codes::Poly_init::wrong_class_ID) {
			return 0;	// nothing wrong with that.
		}
		return ERR::resolve(r);
	}

public:
	static inline i32 getSelection(
		INode* node,
		const i32 level,
		Bitset& out,
		const i32 mode
	) {
		using ERR = Codes::I_Poly_getSelection;
		Helpers::Poly th;
		const i32 r = th.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = th.mesh();
		switch (level) {
		case 0: {
			const i32 num = mesh.numv;
			if (out.length() < num) {
				out.setLength(num);
			}
			if (mode < 0) {
				for (i32 v = 0; v < num; ++v) {
					const MNVert& fl = mesh.v[v];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.clear(v);
					}
				}
			}
			else {
				if (mode == 0) {
					out.clearAll();
				}
				for (i32 v = 0; v < num; ++v) {
					const MNVert& fl = mesh.v[v];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.set(v);
					}
				}
			}
			break;
		}
		case 1: {
			const i32 num = mesh.nume;
			if (out.length() < num) {
				out.setLength(num);
			}
			if (mode < 0) {
				for (i32 e = 0; e < num; ++e) {
					const MNEdge& fl = mesh.e[e];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.clear(e);
					}
				}
			}
			else {
				if (mode == 0) {
					out.clearAll();
				}
				for (i32 e = 0; e < num; ++e) {
					const MNEdge& fl = mesh.e[e];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.set(e);
					}
				}
			}
			break;
		}
		default: {
			const i32 num = mesh.numf;
			if (out.length() < num) {
				out.setLength(num);
			}
			if (mode < 0) {
				for (i32 f = 0; f < num; ++f) {
					const MNFace& fl = mesh.f[f];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.clear(f);
					}
				}
			}
			else {
				if (mode == 0) {
					out.clearAll();
				}
				for (i32 f = 0; f < num; ++f) {
					const MNFace& fl = mesh.f[f];
					if (fl.GetFlag(MN_SEL) && !fl.GetFlag(MN_DEAD)) {
						out.set(f);
					}
				}
			}
			break;
		}
		}
		return Codes::OK;
	}

public:
	static inline i32 setSelection(
		INode* node,
		const i32 level,
		const Bitset& in,
		const i32 mode
	) {
		using ERR = Codes::I_Poly_setSelection;
		Helpers::Poly th;
		const i32 r = th.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		Defer<Restore::SubSelRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SubSelRestore(th, level, MN_SEL));
			rest.get()->before();
		}
		if (mode < 0) {
			th.clearElemFlag(level, in, MN_SEL);
		}
		else {
			th.setElemFlag(level, in, mode == 0, MN_SEL);
		}
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		th.epoly().LocalDataChanged(SELECT_CHANNEL);
		th.epoly().RefreshScreen();
		Util::redrawViews();	// TODO not needed here?
		return Codes::OK;
	}

public:
	static inline i32 setSelection2(
		INode* node,
		const i32 level,
		const List<i32>& in,
		const i32 mode
	) {
		using ERR = Codes::I_Poly_setSelection2;
		Helpers::Poly th;
		const i32 r = th.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		Defer<Restore::SubSelRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SubSelRestore(th, level, MN_SEL));
			rest.get()->before();
		}
		const i32 len = in.length();
		MNMesh& mesh = th.mesh();
		switch (level) {
		case 0: {
			if (!Helpers::Geom::checkBounds(in, mesh.numv)) {
				return ERR::bad_vert;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					mesh.v[in[i] - 1].ClearFlag(MN_SEL);
				}
			}
			else {
				if (mode == 0) {
					mesh.ClearVFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					mesh.v[in[i] - 1].SetFlag(MN_SEL);
				}
			}
			break;
		}
		case 1: {
			if (!Helpers::Geom::checkBounds(in, mesh.nume)) {
				return ERR::bad_edge;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					mesh.e[in[i] - 1].ClearFlag(MN_SEL);
				}
			}
			else {
				if (mode == 0) {
					mesh.ClearEFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					mesh.e[in[i] - 1].SetFlag(MN_SEL);
				}
			}
			break;
		}
		default: {
			if (!Helpers::Geom::checkBounds(in, mesh.numf)) {
				return ERR::bad_face;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					mesh.f[in[i] - 1].ClearFlag(MN_SEL);
				}
			}
			else {
				if (mode == 0) {
					mesh.ClearFFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					mesh.f[in[i] - 1].SetFlag(MN_SEL);
				}
			}
			break;
		}
		}
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		th.epoly().LocalDataChanged(SELECT_CHANNEL);
		th.epoly().RefreshScreen();
		Util::redrawViews();	// TODO not needed here?
		return Codes::OK;
	}














public:
	static inline i32 copyVerts(
		INode* src,
		const i32 srcOff,
		INode* dst,
		const i32 dstOff,
		const i32 len
	) {
		using ERR = Codes::I_Poly_copyVerts;
		i32 r;
		Helpers::Poly pa;
		r = pa.init(src);
		if (r != Codes::OK) {
			return ERR::resolvePolySrc(r);
		}
		Helpers::Poly pb;
		r = pb.init(dst);
		if (r != Codes::OK) {
			return ERR::resolvePolyDst(r);
		}
		const MNMesh& meshA = pa.mesh();
		MNMesh& meshB = pb.mesh();
		if (srcOff < 0 || dstOff < 0 || len < 0 || srcOff > meshA.numv - len || dstOff > meshB.numv - len) {
			return ERR::bad_bounds;
		}
		// actual algorithm
		Defer<Restore::PosRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::PosRestore(pb));
			rest.get()->before();
		}
		const bool same = &meshA == &meshB;	// same exact object?
		if (same && dstOff > srcOff) {
			// self backward copy
			for (i32 i = srcOff + len - 1, j = dstOff + len - 1; i >= srcOff; --i, --j) {
				const Point3 posA = meshA.P(i);
				Point3& posB = meshB.P(j);
				posB.x = posA.x;
				posB.y = posA.y;
				posB.z = posA.z;
			}
		}
		else if (dstOff < srcOff || !same) {
			// forward copy
			for (i32 i = srcOff, j = dstOff, end = srcOff + len; i < end; ++i, ++j) {
				const Point3 posA = meshA.P(i);
				Point3& posB = meshB.P(j);
				posB.x = posA.x;
				posB.y = posA.y;
				posB.z = posA.z;
			}
		}
		// else: don't bother copying self to same index
		pb.changedGeom();
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		Util::redrawViews();
		return Codes::OK;
	}

public:
	static inline i32 hasDead(
		INode* node
	) {
		using ERR = Codes::I_Poly_hasDead;
		Helpers::Poly p;
		const i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = p.mesh();
		for (i32 v = 0, n = mesh.numv; v < n; ++v) {
			if (Helpers::Geom::isVertFlagged(mesh, v, MN_DEAD)) {
				return 1;
			}
		}
		for (i32 e = 0, n = mesh.nume; e < n; ++e) {
			if (Helpers::Geom::isEdgeFlagged(mesh, e, MN_DEAD)) {
				return 1;
			}
		}
		for (i32 f = 0, n = mesh.numf; f < n; ++f) {
			if (Helpers::Geom::isFaceFlagged(mesh, f, MN_DEAD)) {
				return 1;
			}
		}
		return 0;
	}

public:
	static inline i32 collapseDead(
		INode* node
	) {
		using ERR = Codes::I_Poly_collapseDead;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		List<Helpers::SkinMod> skins;
		r = Helpers::SkinMod::collect(node, skins, true);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		if (!Helpers::SkinMod::checkNumPoints(skins, p.mesh().numv)) {
			return ERR::bad_num_points;
		}
		EPoly& epoly = p.epoly();
		MNMesh& mesh = p.mesh();

		// automatically delete these unless moved manually.
		Defer<Restore::WeightLossBefore> restA(new Restore::WeightLossBefore());
		{
			Defer<Restore::WeightLossAfter> restB(new Restore::WeightLossAfter(*restA.get()));
			Defer<Restore::TopoChangeRestore> topoRest(nullptr);

			restA.get()->before(skins);
			const bool holding = theHold.Holding();
			if (holding) {
				topoRest.set(new Restore::TopoChangeRestore(&epoly, mesh));
				topoRest.get()->Before();
			}

			// first determine which verts will be removed
			Bitset dead(mesh.numv);
			p.collectFlaggedVerts(dead, MN_DEAD);

			// create a translation for those verts
			const RemovalTranslation trans(dead, mesh.numv);
			const i32 numAfter = trans.numElements() - trans.numRemoved();

			// compute after removal weights and selections, using before snapshots and translation. this must be done before topo changes.
			const i32 numSkin = restA.get()->length();
			Pointers<BitArray> sel(numSkin);
			Pointers<Restore::Snapshot> snap(numSkin);

			computePostRemovalWeights(numSkin, numAfter, trans, *restA.get(), sel, snap);

			// we can now change the topo without losing weight table information
			mesh.CollapseDeadStructs();
			epoly.LocalDataChanged(ALL_CHANNELS - SUBSEL_TYPE_CHANNEL);

			// after the topo changes have concluded, apply pre-computed skin data.
			applyAfterTopoChange(skins, numAfter, sel, snap);

			// finally, discard or commit to restore objects in proper order
			if (holding) {
				topoRest.get()->After();
				restB.get()->after(trans);
				bool x = true;
				x = Restore::put(x, restA);		// prime B for redo
				x = Restore::put(x, topoRest);		// change topo
				x = Restore::put(x, restB);		// prime A for undo
			}

			// redraw
			epoly.RefreshScreen();
			Util::redrawViews();
		}
		return Codes::OK;
	}






public:
	// Deletes the specified faces from the poly. 
	// Weight data for all skin modifiers is preserved in a lossless manner. Sub-object selection is preserved.
	// Undoable.
	// ____ tested and working
	// TODO non-critical bug: rarely after, skin mods still report the old number of points. It's really hard to reproduce, though, so I can't test things properly.
	static inline i32 deleteFaces(
		INode* node,
		const Bitset& faces
	) {
		using ERR = Codes::I_Poly_deleteFaces;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		List<Helpers::SkinMod> skins;
		r = Helpers::SkinMod::collect(node, skins, true);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		if (!Helpers::SkinMod::checkNumPoints(skins, p.mesh().numv)) {
			return ERR::bad_num_points;
		}
		if (!deleteFacesImpl(p, skins, faces)) {
			return ERR::illegal_state;
		}
		return Codes::OK;
	}

private:
	static inline bool deleteFacesImpl(
		Helpers::Poly& p,
		const List<Helpers::SkinMod>& skins,
		const Bitset& faces
	) {
		EPoly& epoly = p.epoly();
		MNMesh& mesh = p.mesh();

		const bool holding = theHold.Holding();

		// first determine which verts will be removed
		Bitset dead(mesh.numv);
		p.collectFlaggedVerts(dead, MN_DEAD);
		{
			// gather the number of connected faces for each vert
			List<i32> fcounts(mesh.numv);
			for (i32 v = 0; v < mesh.numv; ++v) {
				fcounts.push(mesh.vfac[v].Count());
			}
			// reduce the connection counts for each face to be deleted.
			// if it reaches 0 (ie. not already 0 initially), then that vert will be deleted.
			bool nothing = true;
			Bitset::Iterator it(faces);
			for (i32 f; ((f = it.next()) >= 0 && f < mesh.numf); ) {
				// face to be deleted
				const MNFace& face = mesh.f[f];
				for (i32 i = 0; i < face.deg; ++i) {
					const i32 v = face.vtx[i];
					if (--fcounts[v] <= 0) {
						// vert no longer connected to any faces. vert will die.
						dead.set(v);
					}
				}
				nothing = false;
			}
			if (nothing) {
				// no faces are actually going to be deleted. still, make it undoable.
				if (holding) {
					Restore::put(true, new Restore::FakeRestore());
				}
				return true;
			}
		}
		// create a translation for those verts
		const RemovalTranslation trans(dead, mesh.numv);
		const i32 numAfter = trans.numElements() - trans.numRemoved();

		// collect currently selected faces, select wanted faces and deselect unwanted faces.
		// we do all this so that all other flags remain untouched.
		Defer<Bitset> facesOld(new Bitset(mesh.numf));	// defer is always vetoed
		for (i32 f = 0; f < mesh.numf; ++f) {
			MNFace& face = mesh.f[f];
			if (face.GetFlag(MN_SEL)) {
				facesOld.get()->set(f);
				if (!faces[f]) {
					face.ClearFlag(MN_SEL);
				}
			}
			else if (faces[f]) {
				face.SetFlag(MN_SEL);
			}
		}
		// translate current selection to after removal
		RemovalTranslation transF(faces, mesh.numf);
		Defer<Bitset> facesNew(new Bitset(transF.numElements() - transF.numRemoved()));	// defer is always vetoed
		for (RemovalTranslation::RedoIterator it(transF, true); it.next(); ) {
			if ((*facesOld.get())[it.fromIndex()]) {
				facesNew.get()->set(it.toIndex());
			}
		}

		// init restore logic
		Defer<Restore::SubSelRestoreSimple> restSelA(new Restore::SubSelRestoreSimple(p, facesOld.move(), 2, true, MN_SEL));
		Defer<Restore::SubSelRestoreSimple> restSelB(new Restore::SubSelRestoreSimple(p, facesNew.move(), 2, false, MN_SEL));
		Defer<Restore::WeightLossBefore> restA(new Restore::WeightLossBefore());
		{
			Defer<Restore::WeightLossAfter> restB(new Restore::WeightLossAfter(*restA.get()));
			restA.get()->before(skins);

			// compute after removal weights and selections, using before snapshots and translation. this must be done before topo changes.
			const i32 numSkin = restA.get()->length();
			Pointers<BitArray> sel(numSkin);
			Pointers<Restore::Snapshot> snap(numSkin);
			computePostRemovalWeights(numSkin, numAfter, trans, *restA.get(), sel, snap);

			bool x = true;
			if (holding) {
				// this must happen before topo is changed using epoly function
				x = Restore::put(x, restSelA);
				x = Restore::put(x, restA);
			}

			// change topo. this will place its own restore objects. that's why restA must be placed first. it will also collapse dead structs.
			if (!epoly.EpfnDelete(MNM_SL_FACE, static_cast<DWORD>(MN_SEL), true)) {
				return false;	// this is really bad. I actually have no idea how to clean up the undo buffer now. TODO
			}

			// after the topo changes have concluded, apply pre-computed skin data.
			applyAfterTopoChange(skins, numAfter, sel, snap);
			restSelB.get()->select();
			if (holding) {
				restB.get()->after(trans);
				x = Restore::put(x, restB);
				x = Restore::put(x, restSelB);
			}

			epoly.RefreshScreen();
			Util::redrawViews();
		}
		return true;
	}

public:
	// this behaves just like the epoly variant, except superior because skin mods are accounted for
	// and because the selection is truly arbitrary (no flags)
	// TODO now we need a removeVerts function, too of course.
	// But for that we'd have to first intuit how that works at all.
	// My guess: Go through verts in order, if part of selection, remove its removable edges.
	// But it's not that simple, becase this can remove borders, right. Cannot remove verts of a triangle. 
	// Something like that. Whatever, we won't really need that function just yet anyway.
	static inline i32 removeEdges(
		INode* node,
		const Bitset& edges,
		const bool removeEdgeVerts
	) {
		using ERR = Codes::I_Poly_removeEdges;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		List<Helpers::SkinMod> skins;
		r = Helpers::SkinMod::collect(node, skins, true);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		if (!Helpers::SkinMod::checkNumPoints(skins, p.mesh().numv)) {
			return ERR::bad_num_points;
		}
		EPoly& epoly = p.epoly();
		MNMesh& mesh = p.mesh();

		const bool holding = theHold.Holding();

		// collect already dead verts
		Bitset deadVerts(mesh.numv);
		p.collectFlaggedVerts(deadVerts, MN_DEAD);

		// find the edge count for each vert
		List<i32> edgeCounts(mesh.numv);
		for (i32 v = 0, n = mesh.numv; v < n; ++v) {
			edgeCounts.push(mesh.vedg[v].Count());
		}

		// for each specified edge, test whether removing it is possible
		// and if so, also collect connected edges that would be left dangling
		Bitset edgesToRemove(mesh.nume);
		List<i32> work;
		{
			Bitset::Iterator it(edges);
			for (i32 e; ((e = it.next()) >= 0 && e < mesh.nume); ) {
				const MNEdge& edge = mesh.e[e];
				if (!edge.GetFlag(MN_DEAD) && edge.f2 >= 0 && edge.f1 != edge.f2 && edgesToRemove.trySet(e)) {
					// presumably, any connected edges will behave.
					work.push(e);
					do {
						const MNEdge& _edge = mesh.e[work.pop()];
						extendEdgeRemoval(mesh, _edge.v1, edgeCounts, edgesToRemove, work);
						extendEdgeRemoval(mesh, _edge.v2, edgeCounts, edgesToRemove, work);
					} while (!work.isEmpty());
				}
			}
		}
		// see which verts are removed as a result of removing edges
		if (removeEdgeVerts) {
			// specifically collect the verts that have to be removed after edges are gone
			Bitset::Iterator it(edgesToRemove);
			for (i32 e; (e = it.next()) >= 0; ) {
				const MNEdge& edge = mesh.e[e];
				if (edgeCounts[edge.v1] < 2) {
					deadVerts.set(edge.v1);
				}
				else if (edgeCounts[edge.v1] == 2 && edges[e] && deadVerts.trySet(edge.v1)) {
					work.push(edge.v1);
				}
				if (edgeCounts[edge.v2] < 2) {
					deadVerts.set(edge.v2);
				}
				else if (edgeCounts[edge.v2] == 2 && edges[e] && deadVerts.trySet(edge.v2)) {
					work.push(edge.v2);
				}
			}
		}
		else {
			// no specific removal will happen
			Bitset::Iterator it(edgesToRemove);
			for (i32 e; (e = it.next()) >= 0; ) {
				const MNEdge& edge = mesh.e[e];
				if (edgeCounts[edge.v1] < 2) {
					deadVerts.set(edge.v1);
				}
				if (edgeCounts[edge.v2] < 2) {
					deadVerts.set(edge.v2);
				}
			}
		}
		// now we can change topo with full knowledge of what is being removed beforehand
		// and all of it without touching any flags.

		RemovalTranslation transV(deadVerts, mesh.numv);
		const i32 numAfter = transV.numElements() - transV.numRemoved();

		// capture state before changing anything
		Defer<Restore::WeightLossBefore> restA(new Restore::WeightLossBefore());
		{
			Defer<Restore::WeightLossAfter> restB(new Restore::WeightLossAfter(*restA.get()));
			restA.get()->before(skins);
			Defer<Restore::TopoChangeRestore> restTopo;
			if (holding) {
				restTopo.set(new Restore::TopoChangeRestore(&epoly, mesh));
				restTopo.get()->Before();
			}
			// compute after state for all skin mods
			const i32 numSkin = restA.get()->length();
			Pointers<BitArray> sel(numSkin);
			Pointers<Restore::Snapshot> snap(numSkin);
			computePostRemovalWeights(numSkin, numAfter, transV, *restA.get(), sel, snap);

			// actually change topo
			Bitset::Iterator it(edgesToRemove);
			for (i32 e; (e = it.next()) >= 0; ) {
				if (!Helpers::Geom::isEdgeFlagged(mesh, e, MN_DEAD)) {
					mesh.RemoveEdge(e);
				}
			}
			for (i32 i = 0, n = work.length(); i < n; ++i) {
				mesh.RemoveVertex(work[i]);
			}
			mesh.CollapseDeadStructs();
			epoly.LocalDataChanged(ALL_CHANNELS - SUBSEL_TYPE_CHANNEL);

			// after the topo changes have concluded, apply pre-computed skin data.
			applyAfterTopoChange(skins, numAfter, sel, snap);

			// discard or commit to restore objects
			if (holding) {
				restTopo.get()->After();
				restB.get()->after(transV);
				bool x = true;
				x = Restore::put(x, restA);		// prime B for redo
				x = Restore::put(x, restTopo);	// change topo
				x = Restore::put(x, restB);		// prime A for undo
			}

			epoly.RefreshScreen();
			Util::redrawViews();
		}
		return Codes::OK;
	}


	// whenever an edge is removed, you have to check wether connected edges are left dangling.
	// those will be removed as well. so collect them.
private:
	static inline void extendEdgeRemoval(const MNMesh& mesh, const i32 v, List<i32>& edgeCounts, Bitset& edgesToRemove, List<i32>& work) {
		if (--edgeCounts[v] == 1) {
			const IntTab& vedg = mesh.vedg[v];
			const i32* ptr = vedg.Addr(0);
			for (i32 i = 0, n = vedg.Count(); i < n; ++i) {
				const i32 ne = ptr[i];
				if (edgesToRemove.trySet(ne)) {
					work.push(ne);
					break;
				}
			}
		}
	}

public:
	static inline i32 weldBorders(
		INode* node,
		const Bitset& edges,	// the virtual edge selection
		const Bitset& verts,	// the virtual vert selection
		const bool direction
	) {
		using ERR = Codes::I_Poly_weldBorders;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		List<Helpers::SkinMod> skins;
		r = Helpers::SkinMod::collect(node, skins, true);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		if (!Helpers::SkinMod::checkNumPoints(skins, p.mesh().numv)) {
			return ERR::bad_num_points;
		}
		Deque<i32> vertsA(2);
		Deque<i32> edgesA(2);
		Deque<i32> vertsB(2);
		Deque<i32> edgesB(2);
		r = findOpposingBorderVerts(p.mesh(), vertsA, edgesA, vertsB, edgesB, edges, verts);
		if (r < 0) {
			return r;
		}
		return direction
			? weldBordersImpl(p, skins, vertsA, edgesA, vertsB, edgesB)
			: weldBordersImpl(p, skins, vertsB, edgesB, vertsA, edgesA);
	}


private:
	// geez, without Defer, this was so much worse.
	static inline i32 weldBordersImpl(
		Helpers::Poly& p,
		const List<Helpers::SkinMod>& skins,
		const Deque<i32>& vertsA,
		const Deque<i32>& edgesA,
		const Deque<i32>& vertsB,
		const Deque<i32>& edgesB
	) {
		using ERR = Codes::I_Poly_weldBorders;
		MNMesh& mesh = p.mesh();
		Defer<Restore::WeightLossBefore> restA(new Restore::WeightLossBefore());
		{
			Defer<Restore::WeightLossAfter> restB(new Restore::WeightLossAfter(*restA.get()));
			restA.get()->before(skins);
			Defer<Restore::TopoChangeRestore> topoRest;
			// collect current vert selection
			Defer<Bitset> vertsBefore(new Bitset(mesh.numv));
			for (i32 v = 0, n = mesh.numv; v < n; ++v) {
				if (Helpers::Geom::isVertFlagged(mesh, v, MN_SEL)) {
					vertsBefore.get()->set(v);
				}
			}
			// collect current edge selection
			Defer<Bitset> edgesBefore(new Bitset(mesh.nume));
			for (i32 e = 0, n = mesh.nume; e < n; ++e) {
				if (Helpers::Geom::isEdgeFlagged(mesh, e, MN_SEL)) {
					edgesBefore.get()->set(e);
				}
			}
			// if either edge is selected before, make sure both are selected. this way, whatever the welding 
			// does to the topo, the welded edge will be selected if either of the pre-weld edges were selected
			for (i32 i = 0; i < edgesA.length(); ++i) {
				const i32 edgeA = edgesA[i];
				const i32 edgeB = edgesB[i];
				if ((*edgesBefore.get())[edgeA] || (*edgesBefore.get())[edgeB]) {
					mesh.e[edgeA].SetFlag(MN_SEL);
					mesh.e[edgeB].SetFlag(MN_SEL);
				}
			}

			const bool holding = theHold.Holding();
			if (holding) {
				topoRest.set(new Restore::TopoChangeRestore(&p.epoly(), mesh));
				topoRest.get()->Before();
			}

			// first determine which verts will be removed

			Bitset dead(mesh.numv);
			p.collectFlaggedVerts(dead, MN_DEAD);
			// but also the runB verts, unless it's the special case
			i32 weldingEdge = 0;
			i32 sharedEdge = -1;
			if (vertsA.length() == 1 && edgesA.isEmpty()) {
				const i32 a = vertsA[0];
				const i32 b = vertsB[0];
				// check if same edge
				sharedEdge = Helpers::Geom::sharedVertEdge(mesh, a, b);
				if (sharedEdge >= 0) {
					// a and b are on the same edge
					// edge.v2 is always the vert that is going to be removed
					if (Helpers::Geom::vert2(mesh, sharedEdge) == b) {
						dead.set(b);
						weldingEdge = 1;
					}
					else {
						dead.set(a);
						weldingEdge = 2;
					}
				}
			}
			if (weldingEdge == 0) {
				for (i32 i = 0, n = vertsB.length(); i < n; ++i) {
					dead.set(vertsB[i]);
				}
			}

			// create translation for dead verts
			RemovalTranslation trans(dead, mesh.numv);
			i32 numAfter = trans.numElements() - trans.numRemoved();

			Defer<Bitset> vertsAfter(new Bitset(numAfter));
			{
				Bitset cpy(*vertsBefore.get());
				
				if (weldingEdge == 2) {
					const i32 a = vertsA[0];
					const i32 b = vertsB[0];
					if (cpy[a]) {
						cpy.set(b);
					}
				}
				else {
					for (i32 j = 0, n = vertsB.length(); j < n; ++j) {
						if (cpy[vertsB[j]]) {
							cpy.set(vertsA[j]);
						}
					}
				}
				for (RemovalTranslation::RedoIterator it(trans, true); it.next(); ) {
					if (cpy[it.fromIndex()]) {
						vertsAfter.get()->set(it.toIndex());
					}
				}
			}

			// compute after state
			const i32 numSkin = restA.get()->length();
			Pointers<BitArray> sel(numSkin);
			Pointers<Restore::Snapshot> snap(numSkin);
			for (i32 idx = 0; idx < skins.length(); ++idx) {
				const Restore::Snapshot& snapBefore = restA.get()->weights(idx);
				Defer<Pointers<Tab<INode*>>> bonesAfter(new Pointers<Tab<INode*>>(numAfter));
				bonesAfter.get()->setLength(numAfter);

				Defer<Pointers<FloatTab>> weightsAfter(new Pointers<FloatTab>(numAfter));
				weightsAfter.get()->setLength(numAfter);

				Defer<List<f32>> dqsAfter(new List<f32>(numAfter, true));
				BitArray cpy(restA.get()->selection(idx));
				if (weldingEdge == 2) {
					// a dies, b stays. a's weights must be applied to b.
					const i32 a = vertsA[0];
					const i32 b = vertsB[0];
					if (cpy[a]) {
						cpy.Set(b);
					}
					// translate snapshot data, but copy a's weights instead of b's
					Helpers::SkinMod& mod = skins[idx];
					const i32 nb = mod.getNumAssignedBones(a);
					List<INode*> aBones(nb);
					List<f32> aWeights(nb);
					for (i32 i = 0; i < nb; ++i) {
						aBones.push(mod.getBone(mod.getAssignedBone(a, i)));
						aWeights.push(mod.getBoneWeight(a, i));
					}
					const f32 aDq = mod.getDQBlendWeight(a);
					for (RemovalTranslation::RedoIterator it(trans, true); it.next(); ) {
						const i32 from = it.fromIndex();
						const i32 to = it.toIndex();
						if (from == b) {
							// b stays. just give it a's weights, instead of b's.
							(*bonesAfter.get())[to] = aBones.move();
							(*weightsAfter.get())[to] = aWeights.move();
							(*dqsAfter.get())[to] = aDq;
						}
						else {
							(*bonesAfter.get())[to] = Util::tabCopy(*(*snapBefore.m_bones)[from]);
							(*weightsAfter.get())[to] = Util::tabCopy(*(*snapBefore.m_weights)[from]);
							(*dqsAfter.get())[to] = (*snapBefore.m_dqs)[from];
						}
					}
				}
				else {
					// combine selections
					for (i32 j = 0, n = vertsB.length(); j < n; ++j) {
						if (cpy[vertsB[j]]) {
							cpy.Set(vertsA[j]);
						}
					}
					// tranlate snapshot data
					for (RemovalTranslation::RedoIterator it(trans, true); it.next(); ) {
						const i32 from = it.fromIndex();
						const i32 to = it.toIndex();
						(*bonesAfter.get())[to] = Util::tabCopy(*(*snapBefore.m_bones)[from]);
						(*weightsAfter.get())[to] = Util::tabCopy(*(*snapBefore.m_weights)[from]);
						(*dqsAfter.get())[to] = (*snapBefore.m_dqs)[from];
					}
				}
				Defer<BitArray> out(new BitArray(numAfter));
				for (RemovalTranslation::RedoIterator it(trans, true); it.next(); ) {
					if (cpy[it.fromIndex()]) {
						out.get()->Set(it.toIndex());
					}
				}
				snap.push(new Restore::Snapshot(bonesAfter.move(), weightsAfter.move(), dqsAfter.move()));
				sel.push(out.move());
			}

			// actually change topo
			if (weldingEdge == 0) {
				for (i32 i = 0, n = vertsA.length(); i < n; ++i) {
					const i32 a = vertsA[i];
					const i32 b = vertsB[i];
					Point3 p = mesh.v[a].p;
					// always only weld border verts.
					if (!mesh.WeldBorderVerts(a, b, &p)) {
						// this is really bad, actually... but there is also nothing to be done.
						return ERR::weld_failed_internally;
					}
				}
			}
			else {
				const i32 a = vertsA[0];
				const i32 b = vertsB[0];
				Point3 p = mesh.P(a);
				if (!mesh.WeldEdge(sharedEdge)) {
					// this is really bad, actually... but there is also nothing to be done.
					return ERR::weld_failed_internally;
				}
				if (weldingEdge == 1) {
					mesh.v[a].p = p;
				}
				else {
					mesh.v[b].p = p;
				}
			}

			mesh.CollapseDeadStructs();
			p.setVertFlag(*vertsAfter.get(), true, MN_SEL);
			p.epoly().LocalDataChanged(ALL_CHANNELS - SUBSEL_TYPE_CHANNEL);

			// after the topo changes have concluded, apply pre-computed skin data.
			applyAfterTopoChange(skins, numAfter, sel, snap);

			// finally, discard or commit to restore objects in proper order
			if (holding) {
				topoRest.get()->After();
				restB.get()->after(trans);
				// re-collect edge selection after everything has changed.
				Defer<Bitset> edgesAfter(new Bitset(mesh.nume));
				for (i32 e = 0; e < mesh.nume; ++e) {
					if (Helpers::Geom::isEdgeFlagged(mesh, e, MN_SEL)) {
						edgesAfter.get()->set(e);
					}
				}
				bool x = true;
				x = Restore::put(x, restA);		// prime B for redo
				x = Restore::put(x, new Restore::SubSelRestoreSimple(p, vertsBefore.move(), 0, true, MN_SEL));	// restore old selection
				x = Restore::put(x, new Restore::SubSelRestoreSimple(p, edgesBefore.move(), 1, true, MN_SEL));	// restore old selection
				x = Restore::put(x, topoRest);	// change topo
				x = Restore::put(x, new Restore::SubSelRestoreSimple(p, edgesAfter.move(), 1, false, MN_SEL));	// apply new selection
				x = Restore::put(x, new Restore::SubSelRestoreSimple(p, vertsAfter.move(), 0, false, MN_SEL));	// apply new selection
				x = Restore::put(x, restB);		// prime A for undo
			}
			p.epoly().LocalDataChanged(SELECT_CHANNEL);
			p.epoly().RefreshScreen();
			Util::redrawViews();
		}
		return Codes::OK;
	}

private:
	static inline i32 findOpposingBorderVerts(
		const MNMesh& mesh,
		Deque<i32>& vertsA,
		Deque<i32>& edgesA,
		Deque<i32>& vertsB,
		Deque<i32>& edgesB,
		const Bitset& edgeSel,
		const Bitset& vertSel
	) {
		using ERR = Codes::I_Poly_weldBorders;
		const i32 numE = mesh.nume;
		Bitset done(numE);
		for (i32 e = 0; e < numE; ++e) {
			if (!done[e] && Helpers::Geom::isBorder(mesh, e) && edgeSel[e]) {
				// start of run
				i32 r = collectRun(mesh, vertsA, edgesA, done, e, edgeSel);
				if (r < 0) {
					return r;
				}
				for (++e; e < numE; ++e) {
					if (!done[e] && Helpers::Geom::isBorder(mesh, e) && edgeSel[e]) {
						// start of run
						r = collectRun(mesh, vertsB, edgesB, done, e, edgeSel);
						if (r < 0) {
							return r;
						}
						for (++e; e < numE; ++e) {
							if (!done[e] && Helpers::Geom::isBorder(mesh, e) && edgeSel[e]) {
								// start of run. cannot match three runs or more without guess-work
								return ERR::too_many_runs;
							}
						}
						// dual run
						const u64 len = vertsA.length();
						if (len != vertsB.length()) {
							return ERR::runs_mismatch;
						}
						// runs are never empty
						if (runContainsDeadVerts(mesh, vertsA) || runContainsDeadVerts(mesh, vertsB)) {
							// can't weld dead stuff. inform user
							return ERR::dead_verts;
						}
						// determine if in matching order.
						const bool dir = runDirection(r, mesh, edgesA.getLo(), vertsA.getLo()) == runDirection(r, mesh, edgesB.getLo(), vertsB.getLo());
						if (r < 0) {
							return r;
						}
						if (dir) {
							// runs go in opposing directions. reverse one of them.
							vertsB.reverse();
							edgesB.reverse();
						}
						return Codes::OK;
					}
				}
				// single run
				if (vertsA.length() != 3) {
					// must have exactly two vert-connected borders (three verts)
					return ERR::bad_single_run;
				}
				if (runContainsDeadVerts(mesh, vertsA)) {
					// can't weld dead stuff
					return ERR::dead_verts;
				}
				if (Helpers::Geom::numVertEdges(mesh, vertsA[1]) <= 2) {
					// shared vert must have more than two connected edges, or you'd get illegal geometry
					return ERR::bad_single_run;
				}
				// distribute one end to b
				vertsB.pushHi(vertsA.popHi());
				vertsA.popHi();	// get rid of middle vert
				edgesB.pushHi(edgesA.popHi());
				// now both have one vert and one edge in it
				return Codes::OK;
			}
		}
		// nothing. no "selected" borders.
		// well then, check if maybe exactly two border verts are selected.
		i32 v1 = 0, v2 = 0, found = 0;
		const i32 numV = mesh.numv;
		for (i32 v = 0; v < numV; ++v) {
			if (vertSel[v]) {
				// found a "selected" vert
				const List<i32> edges(mesh.vedg[v]);
				for (i32 i = 0, m = edges.length(); i < m; ++i) {
					// check each edge containing this vert
					if (Helpers::Geom::isBorder(mesh, edges[i])) {
						// edge is a physical border
						if (Helpers::Geom::isVertFlagged(mesh, v, MN_DEAD)) {
							return ERR::dead_verts;
						}
						if (found == 0) {
							v1 = v;
							found = 1;
						}
						else if (found == 1) {
							v2 = v;
							found = 2;
						}
						else {
							// more than 2
							return ERR::pair_mismatch;
						}
						break;	// skip all the other edges
					}
				}
			}
		}
		if (found < 2) {
			// no borders were found is still the error, technically.
			return ERR::no_borders;
		}
		// else exactly 2 verts selected that lie on a border
		vertsA.clearAll();	// kind of pointless
		vertsB.clearAll();	// 
		vertsA.pushHi(v1);
		vertsB.pushHi(v2);
		// now, they may or may not lie on the same edge. detect this case by checking if edgesA/edgesB are empty
		return Codes::OK;
	}


private:
	static inline i32 collectRun(
		const MNMesh& mesh,
		Deque<i32>& run,
		Deque<i32>& runEdges,
		Bitset& done,
		const i32 start,
		const Bitset& edgeSel
	) {
		using ERR = Codes::I_Poly_weldBorders;
		done.set(start);
		const MNEdge& ed = mesh.e[start];
		run.pushHi(ed.v1);
		run.pushLo(ed.v2);
		runEdges.pushHi(start);
		for (i32 v = ed.v1; ;) {
			const List<i32> vedg(mesh.vedg[v]);
			const i32 n = vedg.length();
			i32 c = -1;
			for (i32 i = 0; i < n; ++i) {
				const i32 e = vedg[i];
				if (!done[e] && Helpers::Geom::isBorder(mesh, e) && edgeSel[e]) {
					if (c >= 0) {
						// ambiguous. as in, i don't know how to safely weld when this is the case
						return ERR::forked_border;
					}
					c = e;
					v = Helpers::Geom::otherVert(mesh, e, v);
				}
			}
			if (c < 0) {
				break;
			}
			done.set(c);
			run.pushHi(v);
			runEdges.pushHi(c);
		}
		for (i32 v = ed.v2; ;) {
			const List<i32> vedg(mesh.vedg[v]);
			const i32 n = vedg.length();
			i32 c = -1;
			for (i32 i = 0; i < n; ++i) {
				const i32 e = vedg[i];
				if (!done[e] && Helpers::Geom::isBorder(mesh, e) && edgeSel[e]) {
					if (c >= 0) {
						// ambiguous. as in, i don't know how to safely weld when this is the case
						return ERR::forked_border;
					}
					c = e;
					v = Helpers::Geom::otherVert(mesh, e, v);
				}
			}
			if (c < 0) {
				break;
			}
			done.set(c);
			run.pushLo(v);
			runEdges.pushLo(c);
		}
		if (run.getLo() == run.getHi()) {
			// loops in on itself. there is no clear starting point.
			return ERR::run_forms_loop;
		}
		return Codes::OK;
	}

private:
	static inline bool runDirection(i32& r, const MNMesh& mesh, const i32 e, const i32 v) {
		using ERR = Codes::I_Poly_weldBorders;
		const MNEdge& edge = mesh.e[e];
		const MNFace& face = mesh.f[edge.f1];
		const i32 idx = Helpers::Geom::edgeIndex(face, e);
		if (idx < 0) {
			// edge not found in its own face
			r = ERR::broken_mesh;
			return false;
		}
		return v == face.vtx[idx];	// check if vert that starts the edge is expected vert
	}

private:
	static inline bool runContainsDeadVerts(const MNMesh& mesh, const Deque<i32>& run) {
		const i32 n = run.length();
		for (i32 i = 0; i < n; ++i) {
			if (Helpers::Geom::isVertFlagged(mesh, run[i], MN_DEAD)) {
				return true;
			}
		}
		return false;
	}


public:
	static inline i32 attach(
		INode* dst,
		List<INode*>& nodes,
		const bool stripModsOrDisable
	) {
		using ERR = Codes::I_Poly_attach;
		// gather topos and skins in corresponding rows
		// then pass that along to impl
		if (!dst) {
			return ERR::dst_null_node;
		}
		if (nodes.length() < 1) {
			return ERR::nothing_to_attach;
		}
		List<Helpers::Poly> topo(nodes.length() + 1);
		Pointers<List<Helpers::SkinMod>> rows;
		Helpers::Poly dstPoly;
		{
			Helpers::SkinModIterator it(*dst);
			while (it.next()) {
				Helpers::SkinMod mod;
				const i32 r = mod.init(it);
				if (r != Codes::OK) {
					return ERR::resolveSkinDst(r);
				}
				Defer<List<Helpers::SkinMod>> tt(new List<Helpers::SkinMod>(nodes.length() + 1));
				tt.get()->push(mod);
				rows.push(tt.move());
			}
			const i32 r = dstPoly.init(dst, it.baseObject());
			if (r != Codes::OK) {
				return ERR::resolvePolyDst(r);
			}
			topo.push(dstPoly);
		}
		for (i32 i = 0; i < nodes.length(); ++i) {
			INode* n = nodes[i];
			if (!n) {
				return ERR::src_null_node;
			}
			Helpers::SkinModIterator it(*n);
			i32 j = 0;
			for (; it.next(); ++j) {
				Helpers::SkinMod mod;
				const i32 r = mod.init(it);
				if (r != Codes::OK) {
					return ERR::resolveSkinSrc(r);
				}
				if (j >= rows.length()) {
					// node has more skin mods than dest
					return ERR::different_num_skins;
				}
				rows[j]->push(mod);
			}
			if (j != rows.length()) {
				// node has fewer skin mods than dest
				return ERR::different_num_skins;
			}
			Helpers::Poly nPoly;
			const i32 r = nPoly.init(n, it.baseObject());
			if (r != Codes::OK) {
				return ERR::resolvePolySrc(r);
			}
			topo.push(nPoly);
		}
		// check for correct num points. this is to simplify everything else by having certain guarantees.
		const i32 numRows = rows.length();
		for (i32 i = 0; i < topo.length(); ++i) {
			const i32 numV = topo[i].mesh().numv;
			for (i32 j = 0; j < numRows; ++j) {
				const i32 p = (*rows[j])[i].getNumPoints();
				if (p > 0 && p != numV) {
					return ERR::bad_num_points;
				}
			}
		}
		// topo contains all polies in order
		// rows contains matching skins in order, where each row has the same length as topo
		// the idea then, is to combine all topos
		// and combine each skin row
		// the first element is always the target, the others must be added to the target in order
		// OR rather to a clone of the target
		INode* clone = Util::cloneNode(topo[0].node());
		if (!clone) {
			return ERR::clone_error;
		}
		Helpers::Poly cloneHelper;
		const i32 r = cloneHelper.init(clone);
		if (r != Codes::OK) {
			return ERR::clone_error;	// this is some brundle-fly situation...
		}
		{
			INodeTab clonesTab;	// only using this because EPoly expects it. and it stupidly expects this exact sub-class for no good reason.
			List<INode*> clones(clonesTab);
			Interface7& ip = *GetCOREInterface7();
			const TimeValue tv = ip.GetTime();
			{
				if (stripModsOrDisable) {
					// clone attach nodes with mods stripped
					for (i32 i = 1; i < topo.length(); ++i) {
						INode* cl = Util::cloneNode(topo[i].node());
						if (!cl || !stripMods(ip, cl)) {
							// delete created clones
							ip.DeleteNode(clone, false, false);
							for (i32 k = 0; k < clones.length(); ++k) {
								ip.DeleteNode(clones[k], false, false);
							}
							return ERR::clone_error;
						}
						clones.push(cl);
					}
				}
				else {
					// clone attach nodes with mods disabled
					for (i32 i = 1; i < topo.length(); ++i) {
						INode* cl = Util::cloneNode(topo[i].node());
						if (!cl) {
							// delete created clones
							ip.DeleteNode(clone, false, false);
							for (i32 k = 0; k < clones.length(); ++k) {
								ip.DeleteNode(clones[k], false, false);
							}
							return ERR::clone_error;
						}
						disableMods(cl);	// disabling mods seems like it's enough. no need to strip them.
						clones.push(cl);
					}
				}
			}
			// now, attach the clones to the dest clone
			cloneHelper.epoly().EpfnMultiAttach(clonesTab, cloneHelper.node(), tv);
			ip.SelectNode(clone, 1);	// necessary to force an update for the skin mods
		}
		// clone has the full geometry.
		// now, for each skin in row, attach each to to dest skin.
		// note, the first element in row is the original uncloned skin mod.
		// TODO could check if they're the same as the cloned ones.
		// anyway, first we should collect the max bone limit and add all missing bones.
		// then we can simply copy weights.
		// and we do this per row.

		Helpers::SkinModIterator it(*clone);
		for (i32 i = 0; i < rows.length(); ++i) {
			if (!it.next()) {
				// we'd expect the clone to have the same number of modifiers
				return ERR::clone_error;
			}
			Helpers::SkinMod h;
			const i32 r = h.init(it);
			if (r != Codes::OK) {
				// we'd expect all modifiers to be just as valid for the clone
				return ERR::clone_error;
			}
			const List<Helpers::SkinMod>& row = *(rows[i]);
			const i32 limitOld = h.getBoneLimit();
			i32 limit = limitOld;	// the original bone limit
			for (i32 j = 1; j < row.length(); ++j) {
				Helpers::SkinMod& g = row[j];
				limit = Util::mathMax(limit, g.getBoneLimit());
				const i32 numBones = g.getNumBones();
				for (i32 k = 0; k < numBones; ++k) {
					INode* bone = g.getBone(k);
					if (h.indexOfBone(bone) < 0) {
						h.addBoneEx(bone);
					}
				}
			}
			// first increase limit if needed
			if (limit != limitOld) {
				h.setBoneLimit(limit);
			}
			// now that the clone dst mod has the appropriate limits and bones, we can just copy weights to it
			// ok, but there is an issue:
			// it just may happen that the reported numPoints are lower than the actual number of verts, for example, when 
			// the skin has no bones (and never had bones).
			// well, then let's copy the minimum, and always advance the expected amount.
			// well... this is where the check earlier comes in, then.

			List<INode*> bones;
			List<f32> weights;
			i32 off = topo[0].mesh().numv;
			for (u64 j = 1ui64; j < row.length(); ++j) {
				const Helpers::Poly& th = topo[j];
				Helpers::SkinMod& g = row[j];
				const i32 numC = Util::mathMin(th.mesh().numv, g.getNumPoints());
				for (i32 k = 0; k < numC; ++k) {
					g.collectBones(k, bones);
					g.collectWeights(k, weights);
					h.addWeights(k + off, *bones.tab(), *weights.tab());
					h.setDQBlendWeight(k + off, g.getDQBlendWeight(k));
				}
				off += th.mesh().numv;
			}
			// is that it?
		}
		return Codes::OK;
	}

	// removes all mods from a node
private:
	static inline bool stripMods(Interface7& ip, INode* node) {
		List<Modifier*> mods;
		for (Helpers::ModIterator it(*node); it.next();) {
			mods.push(it.mod());
		}
		// doing this by ref, rather than by index seems more reliable.
		for (i32 i = 0; i < mods.length(); ++i) {
			if (ip.DeleteModifier(*node, *mods[i]) != Interface7::ResCode::kRES_SUCCESS) {
				return false;
			}
		}
		return true;
	}

private:
	static inline void disableMods(INode* node) {
		for (Helpers::ModIterator it(*node); it.next();) {
			it.mod()->DisableMod();
		}
	}

public:
	static inline i32 detach(
		INode* node,
		Bitset& faces,
		const bool keep
	) {
		using ERR = Codes::I_Poly_detach;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		List<Helpers::SkinMod> skins;
		r = Helpers::SkinMod::collect(node, skins, true);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		if (!Helpers::SkinMod::checkNumPoints(skins, p.mesh().numv)) {
			return ERR::bad_num_points;
		}
		const i32 numV = p.mesh().numv;
		const i32 numF = p.mesh().numf;

		Bitset::Iterator it(faces);
		const i32 n = it.next();
		if (n >= 0 && n < numF) {
			// at least one face will be deleted
			Interface7& ip = *GetCOREInterface7();
			INode* clone = Util::cloneNode(node);
			Helpers::Poly p2;
			i32 r = p2.init(clone);
			if (r != Codes::OK) {
				return ERR::clone_error;
			}
			ip.SelectNode(clone, 1);	// may be necessary to update skin mod internals
			List<Helpers::SkinMod> skins2;
			r = Helpers::SkinMod::collect(node, skins2, true);
			if (r != Codes::OK || !Helpers::SkinMod::checkNumPoints(skins2, numV) || skins2.length() != skins.length()) {
				return ERR::clone_error;
			}
			{
				// copy skin mod vert selections to clone, because clone doesn't
				// TODO should Util::cloneNode maybe just do this by itself?
				BitArray bits(numV);
				for (i32 i = 0; i < skins.length(); ++i) {
					skins[i].getVertexSelection(bits);
					skins2[i].setVertexSelection(bits);
				}
			}

			Bitset faces2(numF);
			{
				// make faces2 a flipped copy of faces
				Bitset cpy(faces);	// don't touch faces
				
				cpy.setLength(numF);
				faces2.setAll();
				faces2 ^= cpy;
			}
			deleteFacesImpl(p2, skins2, faces2);
			if (!keep) {
				deleteFacesImpl(p, skins, faces);
			}
		}
		return Codes::OK;
	}

public:
	static inline i32 unsubdivide(
		INode* node,
		const Bitset& selVerts,
		const Bitset& selFaces,
		Bitset& edges,
		Bitset& verts)
	{
		using ERR = Codes::I_Poly_unsubdivide;
		Helpers::Poly th;
		i32 r = th.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = th.mesh();
		edges.setLength(mesh.nume);
		edges.clearAll();
		verts.setLength(mesh.numv);
		verts.clearAll();
		const i32 numF = mesh.numf;
		Bitset faces(numF + 1);
		Bitset patchFaces(numF + 1);
		// face 0 (-1+1) remains unset, makes it possible to not have to check for < 0 all the time.

		List<i32> patch(32);

		Bitset tempEdges(edges.length());
		Bitset tempVerts(verts.length());

		r = Codes::OK;
		bool partial = false;
		for (i32 f1 = 0; f1 < numF; ++f1) {
			if (!faces[f1 + 1] && selFaces[f1]) {
				// face not part of an already completed patch / start of a new patch
				bool bad = false;
				i32 sel = -1;
				patch.push(f1);
				patchFaces.set(f1 + 1);
				for (i32 i = 0; i < patch.length(); ++i) {
					const i32 f = patch[i];
					const MNFace& face = mesh.f[f];
					if (face.deg != 4) {
						bad = true;	// not a quad
					}
					i32 numBorders = 0;
					i32 borderIdx = -1;
					for (i32 j = 0; j < face.deg; ++j) {
						const i32 e = face.edg[j];
						const MNEdge& edge = mesh.e[e];
						const i32 f2 = Helpers::Geom::otherFace(edge, f);
						if (f2 < 0 || !selFaces[f2]) {
							// no f2, so e is border
							if (face.deg == 4) {
								// edge is a quad border
								if (numBorders == 0) {
									// first border
									borderIdx = j;
									numBorders = 1;
								}
								else if (numBorders == 1) {
									// second border
									if (j == 3 && borderIdx == 0) {
										sel = face.vtx[0];
									}
									else if (j - 1 == borderIdx) {
										sel = face.vtx[j];
									}
									else {
										bad = true;	// not consecutive. not subdivided
									}
									numBorders = 2;
								}
								else {
									// third or fourth border. not subdivided
									bad = true;
								}
							}
						}
						else if (patchFaces.trySet(f2 + 1)) {
							// f2 is part of patch and hasn't been processed yet
							patch.push(f2);
						}
					}
				}
				// patch fully collected
				if (bad || (U32(patch.length()) & 3) != 0) {
					// not a proper subdivided patch
					r = ERR::not_unsubdivisible;
				}
				else {
					// no non-quads, have a multiple of 4 quads
					if (sel < 0) {
						// patch is closed or has no corners. try to find exactly one selected vert for this patch
						for (i32 i = 0; i < patch.length(); ++i) {
							const i32 f = patch[i];
							const MNFace& face = mesh.f[f];
							for (i32 j = 0; j < face.deg; ++j) {
								const i32 v = face.vtx[j];
								if (v != sel && selVerts[v]) {
									if (sel >= 0) {
										// too many verts selected. can't decide easily
										sel = -2;
										break;
									}
									sel = v;
								}
							}
							if (sel == -2) {
								break;
							}
						}
						// fall-through
					}
					if (sel >= 0) {
						const i32 r2 = unsubdividePatch(mesh, sel, patch, patchFaces, tempEdges, tempVerts);
						if (r2 == Codes::OK) {
							edges |= tempEdges;
							verts |= tempVerts;
							partial = true;
						}
						else {
							// don't stop, but do report an issue
							r = r2;
						}
						tempEdges.clearAll();
						tempVerts.clearAll();
					}
					else {
						//  no corner could be determined or we had a bad selection
						r = ERR::bad_selection;
					}
				}
				patch.clearAll();
				faces |= patchFaces;
				patchFaces.clearAll();
			}
		}
		return ERR::qualify(r, partial);
	}

private:
	static inline i32 unsubdividePatch(
		const MNMesh& mesh, 
		i32 sel, 
		List<i32>& work, 
		const Bitset& faces, 
		Bitset& edges, 
		Bitset& verts
	) {
		using ERR = Codes::I_Poly_unsubdivide;
		// traverse from a corner to a middle vert (simply advance by two edges on any connected accepted face)
		work.clearAll();
		{
			i32 ff = -1;
			const List<i32> vfac(mesh.vfac[sel]);
			for (i32 i = 0, n = vfac.length(); i < n; ++i) {
				const i32 f = vfac[i];
				if (faces[f + 1]) {
					ff = f;
					break;
				}
			}
			if (ff < 0) {
				return ERR::illegal_state_1;
			}
			// ff is corner quad now
			const MNFace& face = mesh.f[ff];
			i32 idx = Helpers::Geom::vertIndex(face, sel) + 2;
			if (idx >= 4) {
				idx -= 4;
			}
			const i32 v = face.vtx[idx];
			work.push(v);
			verts.set(v);
		}
		// starting with the any vert in the middle of N quads, traverse in all edge directions twice to the next such vert
		// while collecting the edges. These verts and edges, when removed, result in a subdivion level being removed.
		do {
			const i32 v1 = work.pop();
			i32 numFacesInPatch = 0;
			const List<i32> vfac(mesh.vfac[v1]);
			for (i32 i = 0, num = vfac.length(); i < num; ++i) {
				if (faces[vfac[i] + 1]) {
					++numFacesInPatch;
				}
			}
			if (numFacesInPatch < 3) {
				// cannot be right. a proper subdivision cannot have this.
				// but maybe it's only the selection.
				work.clearAll();
				return ERR::bad_selection;
			}
			const List<i32> t(mesh.vedg[v1]);
			for (i32 i = 0, num = t.length(); i < num; ++i) {
				const i32 e1 = t[i];
				edges.set(e1);
				const MNEdge& edge1 = mesh.e[e1];
				if (!faces[edge1.f1 + 1] || !faces[edge1.f2 + 1]) {
					// this is bad
					return ERR::illegal_state_2;
				}
				const i32 v2 = Helpers::Geom::otherVert(edge1, v1);
				if (!verts[v2]) {
					verts.set(v2);
					// find the other edge containing v2 that isn't e1
					const i32 f1e = findPerpendicularEdge(mesh, edge1.f1, e1, v2);
					const i32 f2e = findPerpendicularEdge(mesh, edge1.f2, e1, v2);
					if (f1e < 0 || f2e < 0) {
						// this is bad
						return ERR::illegal_state_3;
					}
					// advance to the neighboring faces, if any
					const i32 f3 = Helpers::Geom::otherFace(mesh, f1e, edge1.f1);
					const i32 f4 = Helpers::Geom::otherFace(mesh, f2e, edge1.f2);
					const bool b = faces[f3 + 1];
					if (b != faces[f4 + 1]) {
						return ERR::not_unsubdivisible;
					}
					if (b) {
						const i32 e2 = Helpers::Geom::sharedFaceEdge(mesh, f3, f4);
						if (e2 < 0) {
							return ERR::not_unsubdivisible;
						}
						const i32 v3 = Helpers::Geom::otherVert(mesh, e2, v2);
						if (!verts[v3]) {
							verts.set(v3);
							work.push(v3);
						}
					}
				}
			}
		} while (!work.isEmpty());
		return Codes::OK;
	}

private:
	static i32 findPerpendicularEdge(const MNMesh& mesh, const i32 f, const i32 e, const i32 v) {
		const MNFace& face = mesh.f[f];
		for (i32 i = 0; i < face.deg; ++i) {
			const i32 ee = face.edg[i];
			if (ee != e) {
				const MNEdge& edge = mesh.e[ee];
				if (edge.v1 == v || edge.v2 == v) {
					return ee;
				}
			}
		}
		return -1;
	}

public:
	static inline i32 detriangulate(
		INode* node,
		const Bitset& selFaces,
		Bitset& edges,
		const bool grid
	) {
		using ERR = Codes::I_Poly_detriangulate;
		Helpers::Poly th;
		i32 r = th.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = th.mesh();
		r = Codes::OK;
		const bool g = grid;
		const i32 numF = mesh.numf;
		const i32 numE = mesh.nume;
		const i32 numV = mesh.numv;
		edges.setLength(numE);
		edges.clearAll();
		Bitset tempEdges(edges.length());
		List<i32> patch(32);
		List<i32> work(32);
		Bitset faces(numF + 1);	// contains all the faces of a patch + 1 for face -1
		Bitset facesAll(numF + 1);	// contains all patches, used only to identify new patches
		Bitset borders(g ? numE : Util::mathMax(numE, numF + 1));
		Bitset work2(g ? (numF + 1) : 0);
		bool dirty = false;
		bool partial = false;
		for (i32 f = 0; f < numF; ++f) {
			if (!facesAll[f + 1] && selFaces[f]) {
				// start of a new patch
				if (dirty) {
					tempEdges.clearAll();	// don't bother for the first patch
				}
				patch.clearAll();
				work.clearAll();
				borders.clearAll();
				work2.clearAll();
				i32 numBorders = 0;
				i32 border;
				u32 numTri = 0;
				const bool notGrid = growPatchFindAnyBorder(mesh, selFaces, patch, faces, border, numBorders, numTri, f, g);
				i32 r2 = Codes::OK;
				if (!notGrid && numBorders > 0) {
					findBorderLoop(mesh, faces, borders, work, border);
				}
				if (notGrid || work.length() != numBorders) {
					// we have several border loops or patch is not grid-shaped when it needs to be
					r2 = ERR::mismatch;
				}
				else {
					r2 = g
						? handleGridPatch(mesh, patch, work, faces, borders, work2, numTri, tempEdges)
						: handleNonGridPatch(mesh, patch, work, faces, borders, tempEdges);
				}
				if (r2 != Codes::OK) {
					r = r2;
				}
				else {
					edges |= tempEdges;
					partial = true;
				}
				facesAll |= faces;
				faces.clearAll();
				dirty = true;
			}
		}
		return ERR::qualify(r, partial);
	}


	// starting with "start" face, grow the path along neighbor faces
	// and store any one edge you find. return that edge, if any, after the patch is fully grown.
	// faces are stored in "faces" and "patch" (in no particular order). also counts the total number of borders the patch has in "numBorders".
private:
	static inline bool growPatchFindAnyBorder(
		const MNMesh& mesh, 
		const Bitset& selFaces, 
		List<i32>& patch, 
		Bitset& faces, 
		i32& border, 
		i32& numBorders, 
		u32& numTri, 
		const i32 start, 
		const bool grid
	) {
		border = -1;
		bool notGrid = false;
		patch.push(start);
		faces.set(start + 1);
		for (i32 i = 0; i < patch.length(); ++i) {
			const i32 f = patch[i];
			const MNFace& face = mesh.f[f];
			if (grid && face.deg == 3) {
				++numTri;
			}
			if (grid && face.deg > 4) {
				notGrid = true;	// grids can't have faces with more than 4 edges
			}
			for (i32 j = 0; j < face.deg; ++j) {
				const i32 e = face.edg[j];
				const MNEdge& edge = mesh.e[e];
				const i32 f2 = Helpers::Geom::otherFace(edge, f);
				if (f2 < 0 || !selFaces[f2]) {
					border = e;
					++numBorders;
				}
				else if (faces.trySet(f2 + 1)) {
					patch.push(f2);
				}
			}
		}
		return notGrid;
	}

	// starting with "start" edge, creep along connected edge verts until you loop back to "start" (but in a slightly more efficient manner).
	// collect all border edges in "borders". "faces" contains all patch faces.
private:
	static inline void findBorderLoop(
		const MNMesh& mesh, 
		const Bitset& faces, 
		Bitset& borders, 
		List<i32>& work, 
		const i32 start
	) {
		const MNEdge& edge = mesh.e[start];
		i32 f = faces[edge.f1 + 1] 
			? edge.f1 
			: edge.f2;	// get the face
		i32 e = start;
		i32 v = Helpers::Geom::vert1(mesh, start);
		i32 eIdx = Helpers::Geom::edgeIndex(mesh, f, start);
		i32 vIdx = Helpers::Geom::vertIndexOrdered(mesh, f, eIdx, v);
		do {
			const MNFace& face = mesh.f[f];
			Helpers::Geom::nextEdgeIdx(face.deg, eIdx, vIdx);
			i32 ne = face.edg[eIdx];
			i32 nv = face.vtx[vIdx];
			i32 f2 = Helpers::Geom::otherFace(mesh, ne, f);
			// neighbor creep around v until you find another border
			while (faces[f2 + 1]) {
				f = f2;
				const MNFace& face2 = mesh.f[f2];
				eIdx = Helpers::Geom::edgeIndex(face2, ne);
				vIdx = Helpers::Geom::vertIndexOrdered(face2, eIdx, v);
				Helpers::Geom::nextEdgeIdx(face2.deg, eIdx, vIdx);
				ne = face2.edg[eIdx];
				nv = face2.vtx[vIdx];
				f2 = Helpers::Geom::otherFace(mesh, ne, f2);
			}
			borders.set(ne);
			work.push(ne);
			e = ne;
			v = nv;
		} while (e != start);
	}

	// "corners" is empty
	// "patch" contains all patch faces. but it's cleared immediately
	// "faces" contains all patch faces
	// "borders" contains the outer border edge loop
	// "work" contains the outer border edge loop
	// "work2" is empty
	// only safe if patch contains only quads and triangles
private:
	static inline i32 handleGridPatch(
		const MNMesh& mesh,
		List<i32>& patch, 
		List<i32>& work,
		const Bitset& faces, 
		Bitset& borders, 
		Bitset& work2, 
		u32 numTri, 
		Bitset& edges
	) {
		using ERR = Codes::I_Poly_detriangulate;
		if (work.isEmpty()) {
			// no borders. closed patch is never a grid.
			return ERR::not_a_grid;
		}
		if ((numTri & 1) != 0) {
			// must have an even number of triangles
			return ERR::odd_num_triangles;
		}
		patch.clearAll();
		work2.set(0);
		// collect only border quads and unambiguous border tri pairs (and shared edge) in "patch"
		while (!work.isEmpty()) {
			const i32 e = work.pop();
			const MNEdge& edge = mesh.e[e];
			const i32 f = faces[edge.f1 + 1] ? edge.f1 : edge.f2;
			const MNFace& face = mesh.f[f];
			if (face.deg == 4) {
				// quad. no shared edge
				if (work2.trySet(f + 1)) {
					// push the same corner quad only once
					patch.push(f);
				}
			}
			else {
				// triangle. collect shared edge
				const i32 e0 = face.edg[0];
				const i32 e1 = face.edg[1];
				const i32 e2 = face.edg[2];
				const bool b0 = borders[e0];
				const bool b1 = borders[e1];
				const bool b2 = borders[e2];
				const i32 num = b0 + b1 + b2;
				if (num == 2) {
					// two borders: unambiguous corner triangle
					const i32 e = !b0 ? e0 : (!b1 ? e1 : e2);	// shared edge
					const i32 f2 = Helpers::Geom::otherFace(mesh, e, f);
					const MNFace& face2 = mesh.f[f2];
					if (face2.deg == 3) {
						// tri pair
						if (!edges[e]) {
							// check ensures corner is pushed only once
							edges.set(e);	// shared edge
							patch.push(f);
							patch.push(e);	// push so we don't have to find it again
							patch.push(f2);
							work2.set(f + 1);
							work2.set(f2 + 1);
						}
					}
					else {
						// not a grid
						return ERR::not_a_grid;
					}
				}
				else if (num == 3) {
					// just one lonely triangle?
					return ERR::not_a_grid;
				}
			}
		}
		// work is empty
		// patch contains starting quads (border quads or border triangle pairs+shared edges)
		// work2 now contains -1 and starting faces
		do {
			// creep along outer border, add new quads to patch
			for (i32 i = 0; i < patch.length();) {
				// get a quad along outer border
				const i32 f1 = patch[i++];
				const MNFace& face1 = mesh.f[f1];
				// extract verts and edges in a consistent topological order
				i32 f2, v0, v1, v2, v3, e0, e1, e2, e3;
				if (face1.deg == 3) {
					// tri pair
					const i32 shared = patch[i++];	// shared edge. order matters
					f2 = patch[i++];
					const MNFace& face2 = mesh.f[f2];
					// find non shared edges for each tri
					i32 ee0, ee1, ee2, ee3;
					otherEdges(face1, shared, ee0, ee1);
					otherEdges(face2, shared, ee2, ee3);
					// select verts
					const MNEdge& sharedEdge = mesh.e[shared];
					v0 = sharedEdge.v1;
					v2 = sharedEdge.v2;
					// order stuff 
					chooseOrdered(mesh, ee0, ee1, v0, v2, e0, e1, v1);
					chooseOrdered(mesh, ee2, ee3, v2, v0, e2, e3, v3);
					// edges and verts are now ordered like a quad
					// this let's us apply the same logic
					numTri -= 2;
				}
				else {
					// quad
					v0 = face1.vtx[0];
					v1 = face1.vtx[1];
					v2 = face1.vtx[2];
					v3 = face1.vtx[3];
					e0 = face1.edg[0];
					e1 = face1.edg[1];
					e2 = face1.edg[2];
					e3 = face1.edg[3];
					f2 = f1;	// for simplicity's sake
					// this stuff is already ordered
				}
				// handle each border in both directions
				// since borders are only updated between big iterations, this results in creeping along the outer border.
				const i32 x0 = Helpers::Geom::otherFace(mesh, e0, f1);
				const i32 x1 = Helpers::Geom::otherFace(mesh, e1, f1);
				const i32 x2 = Helpers::Geom::otherFace(mesh, e2, f2);
				const i32 x3 = Helpers::Geom::otherFace(mesh, e3, f2);
				i32 r = Codes::OK;
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e0, e1, x1, v1, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e0, e3, x3, v0, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e1, e2, x2, v2, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e1, e0, x0, v1, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e2, e3, x3, v3, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e2, e1, x1, v2, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e3, e0, x0, v0, r);
				r = handleQuad(mesh, patch, work2, faces, borders, edges, e3, e2, x2, v3, r);
				if (r != Codes::OK) {
					return r;
				}
				// determine if quad is corner, and if so, push corner vert for next iteration
				const bool b0 = borders[e0];
				const bool b1 = borders[e1];
				const bool b2 = borders[e2];
				const bool b3 = borders[e3];
				if (b0 + b1 + b2 + b3 == 2) {
					// quad is corner if exacxtly 2 edges are borders and consecutive
					// push corner vert
					if (b0 && b1) {
						work.push(v3);
					}
					else if (b1 && b2) {
						work.push(v0);
					}
					else if (b2 && b3) {
						work.push(v1);
					}
					else if (b3 && b0) {
						work.push(v2);
					}
					if (work.length() > 4) {
						// more than 4 corners. no point in going further
						return ERR::not_a_grid;
					}
				}
			}
			// work now contains corner verts (0 or 4 hopefully)
			// update the borders inbetween iterations (during would break everything)
			while (!patch.isEmpty()) {
				// for each "quad" along the outer border, add all face-edges as borders
				const MNFace& face1 = mesh.f[patch.pop()];
				if (face1.deg == 3) {
					patch.pop();	// skip over edge
					const MNFace& face2 = mesh.f[patch.pop()];
					borders.set(face2.edg[0]);
					borders.set(face2.edg[1]);
					borders.set(face2.edg[2]);
				}
				borders.set(face1.edg[0]);
				borders.set(face1.edg[1]);
				borders.set(face1.edg[2]);
			}
			if (!work.isEmpty() && work.length() != 4) {
				// must have either 0 or 4 corners for the next iteration
				return ERR::not_a_grid;
			}
			// expand corner verts to corner quads for next iteration
			if (!initNextQuads(mesh, patch, work, work2, faces, borders, edges)) {
				return ERR::mismatch;
			}
		} while (!patch.isEmpty());
		if (numTri != 0) {
			return ERR::leftover_triangles;
		}
		return Codes::OK;
	}

private:
	static inline void otherEdges(const MNFace& face, const i32 e, i32& edgeA, i32& edgeB) {
		const i32 e0 = face.edg[0];
		const i32 e1 = face.edg[1];
		edgeA = e0;
		edgeB = e1;
		if (e0 == e) {
			edgeA = face.edg[2];
		}
		else if (e1 == e) {
			edgeB = face.edg[2];
		}
	}

	// decide edge/vert order based on input verts.
	//                                             poly,           ee2,           ee3,           v2,           v0,      e2,      e3,      v3
private:
	static inline void chooseOrdered(const MNMesh& mesh, const i32 ee0, const i32 ee1, const i32 v0, const i32 v2, i32& e0, i32& e1, i32& v1) {
		const MNEdge& edge = mesh.e[ee0];
		if (edge.v1 == v0) {
			e0 = ee0;
			e1 = ee1;
			v1 = edge.v2;
		}
		else if (edge.v2 == v0) {
			e0 = ee0;
			e1 = ee1;
			v1 = edge.v1;
		}
		else if (edge.v1 == v2) {
			e0 = ee1;
			e1 = ee0;
			v1 = edge.v2;
		}
		else {
			e0 = ee1;
			e1 = ee0;
			v1 = edge.v1;
		}
	}


	// work2 contains processed faces
private:
	static i32 handleQuad(
		const MNMesh& mesh, 
		List<i32>& patch,
		Bitset& work2, 
		const Bitset& faces, 
		const Bitset& borders, 
		Bitset& edges,
		const i32 border, 
		const i32 helper, 
		const i32 ext, 
		const i32 corner, 
		const i32 r
	) {
		using ERR = Codes::I_Poly_detriangulate;
		if (r >= 0 && borders[border] && !work2[ext + 1] && faces[ext + 1]) {
			// extension exists, is new.
			const MNFace& extension = mesh.f[ext];
			if (extension.deg == 3) {
				// pair
				i32 edgeA, edgeB;
				otherEdges(extension, helper, edgeA, edgeB);
				const bool bA = borders[edgeA];
				const bool bB = borders[edgeB];
				i32 cpt;	// counterpart / match
				i32 shared;
				if (bA != bB) {
					// exactly one border for the extension. the non-border edge is shared with the counterpart
					// however, we must check that the border contains the corner
					shared = bA ? edgeB : edgeA;
					cpt = Helpers::Geom::otherFace(mesh, shared, ext);
					// since shared is not a border, we know the face exists
					if (!isMatchCandidate(mesh, work2, faces, cpt)) {
						// no match is an error here
						return ERR::mismatch;
					}
				}
				else if (!bA) {
					// no border edges means we have two candidates for counterparts.
					// check if either is quad
					// check if either is old
					// if ambiguous always go with the edge that shares the corner
					const i32 fA = Helpers::Geom::otherFace(mesh, edgeA, ext);
					const i32 fB = Helpers::Geom::otherFace(mesh, edgeB, ext);
					const bool bbA = isMatchCandidate(mesh, work2, faces, fA);
					const bool bbB = isMatchCandidate(mesh, work2, faces, fB);
					if (bbA != bbB) {
						// only one qualifies
						if (bbA) {
							shared = edgeA;
							cpt = fA;
						}
						else {
							shared = edgeB;
							cpt = fB;
						}
					}
					else if (bbA) {
						// both qualify so far
						// take the one containing the corner
						const MNEdge& edge = mesh.e[edgeA];
						if (edge.v1 == corner || edge.v2 == corner) {
							shared = edgeA;
							cpt = fA;
						}
						else {
							shared = edgeB;
							cpt = fB;
						}
					}
					else {
						// neither qualifies, error in this case.
						return ERR::mismatch;
					}
				}
				else {
					return ERR::mismatch;	// no counterpart
				}
				// push pair
				patch.push(ext);	// extension
				patch.push(shared);	// shared edge
				patch.push(cpt);	// counterpart
				work2.set(ext + 1);
				work2.set(cpt + 1);
				edges.set(shared);
			}
			else if (extension.deg == 4) {
				// push quad
				patch.push(ext);
				work2.set(ext + 1);
			}
			else {
				return ERR::mismatch;
			}
			// fall-through
		}
		return r;
	}

	// extrapolate next corner quads from current corner verts
private:
	static bool initNextQuads(
		const MNMesh& mesh, 
		List<i32>& patch, 
		List<i32>& work,
		Bitset& work2, 
		const Bitset& faces, 
		const Bitset& borders, 
		Bitset& edges
	) {
		while (!work.isEmpty()) {
			const List<i32> vfac(mesh.vfac[work.pop()]);
			i32 f1 = -1;
			i32 f2 = -1;
			for (i32 i = 0, n = vfac.length(); i < n; ++i) {
				const i32 f = vfac[i];
				if (!work2[f + 1] && faces[f + 1]) {
					if (f2 >= 0) {
						// too many
						return false;
					}
					if (f1 >= 0) {
						f2 = f;
					}
					else {
						f1 = f;
					}
				}
			}
			if (f2 >= 0) {
				const i32 shared = Helpers::Geom::sharedFaceEdge(mesh, f1, f2);
				if (shared < 0 || Helpers::Geom::numFaceEdges(mesh, f1) != 3 || Helpers::Geom::numFaceEdges(mesh, f2) != 3) {
					// not a quad
					return false;
				}
				patch.push(f1);
				patch.push(shared);
				patch.push(f2);
				work2.set(f1 + 1);
				work2.set(f2 + 1);
				edges.set(shared);
			}
			else if (f1 >= 0) {
				const MNFace& face1 = mesh.f[f1];
				if (face1.deg == 3) {
					// find counterpart
					// the other edges are part of borders already
					// so simply find the non-border edge
					// if there is none, then error
					i32 shared;
					if (!borders[face1.edg[0]]) {
						shared = face1.edg[0];
					}
					else if (!borders[face1.edg[1]]) {
						shared = face1.edg[1];
					}
					else if (!borders[face1.edg[2]]) {
						shared = face1.edg[2];
					}
					else {
						// proper mismatch
						return false;
					}
					// not a border, means it leads to a patch face
					f2 = Helpers::Geom::otherFace(mesh, shared, f1);
					const MNFace& face2 = mesh.f[f2];
					if (face2.deg != 3 || work2[f2 + 1]) {
						// well, crap
						return false;
					}
					patch.push(f1);
					patch.push(shared);
					patch.push(f2);
					work2.set(f1 + 1);
					work2.set(f2 + 1);
					edges.set(shared);
				}
				else {
					// quad
					patch.push(f1);
					work2.set(f1 + 1);
				}
			}
		}
		return true;
	}


	// only collects unambiguous edges
	// "work" initially contains borders as well
private:
	static inline i32 handleNonGridPatch(
		const MNMesh& mesh,
		List<i32>& patch, 
		List<i32>& work,
		const Bitset& faces, 
		Bitset& borders, 
		Bitset& edges
	) {
		using ERR = Codes::I_Poly_detriangulate;
		// drain patch to quads, only keeping n-gons and corner tri pairs
		work.clearAll();
		while (!patch.isEmpty()) {
			const i32 f = patch.pop();
			const MNFace& face = mesh.f[f];
			if (face.deg > 3) {
				work.push(f);
			}
			else {
				const i32 e0 = face.edg[0];
				const i32 e1 = face.edg[1];
				const i32 e2 = face.edg[2];
				const bool b0 = borders[e0];
				const bool b1 = borders[e1];
				const bool b2 = borders[e2];
				if (b0 + b1 + b2 == 2) {
					// two borders: corner triangle
					const i32 e = !b0 ? e0 : (!b1 ? e1 : e2);	// shared edge
					const i32 f2 = Helpers::Geom::otherFace(mesh, e, f);
					const MNFace& face2 = mesh.f[f2];
					if (face2.deg == 3) {
						// tri pair
						if (!edges[e]) {
							// check ensures corner is pushed only once
							edges.set(e);	// shared edge
							work.push(f);
							work.push(f2);
						}
					}
				}
			}
		}
		// patch now empty, quads contains n-gons and corner tri pairs (quads)
		// add quad edges to borders
		if (work.isEmpty()) {
			return ERR::mismatch;
		}
		borders.clearAll();	// re-use for faces
		borders.set(0);		// include -1 as processed
		// add initial faces to borders
		for (u64 i = 0ui64; i < work.length(); ++i) {
			borders.set(work[i] + 1);
		}
		do {
			const i32 f = work.pop();
			const MNFace& face = mesh.f[f];
			for (i32 i = 0; i < face.deg; ++i) {
				const i32 e = face.edg[i];
				const i32 f2 = Helpers::Geom::otherFace(mesh, e, f);
				if (!borders[f2 + 1] && faces[f2 + 1]) {
					// f2 is part of the patch, haven't processed it yet
					const MNFace& face2 = mesh.f[f2];
					if (face2.deg > 3) {
						// n-gon
						work.push(f2);
						borders.set(f2 + 1);
					}
					else {
						// triangle. try to find an unamgibuous match for it.
						// first, get the other two edges (the ones that are not e)
						i32 eA, eB;
						otherEdges(face2, e, eA, eB);
						const i32 fA = Helpers::Geom::otherFace(mesh, eA, f2);
						const i32 fB = Helpers::Geom::otherFace(mesh, eB, f2);
						const bool bA = isMatchCandidate(mesh, borders, faces, fA);
						const bool bB = isMatchCandidate(mesh, borders, faces, fB);
						if (bA != bB) {
							// exactly one match, no ambiguity
							if (bA) {
								pushMatch(work, borders, edges, f2, fA, eA);
							}
							else {
								pushMatch(work, borders, edges, f2, fB, eB);
							}
						}
						else if (bA) {
							// two candidates. check how many borders each has
							const i32 nA = countBorders(mesh, borders, faces, fA, eA);
							const i32 nB = countBorders(mesh, borders, faces, fB, eB);
							if (nA != nB) {
								// 2 wins against 1 or 0, it closes a gap neatly
								if (nA == 2) {
									pushMatch(work, borders, edges, f2, fA, eA);
								}
								else if (nB == 2) {
									pushMatch(work, borders, edges, f2, fB, eB);
								}
								// else: 1 against 0, cannot disambiguate
							}
							// else if nA == 1: 1 against 1, cannot disambiguate
							// else: both have 0 or 2 borders, cannot disambiguate
						}
						// else, neither matches (yet. this face might get matched later still)
					}
				}
			}
		} while (!work.isEmpty());
		return Codes::OK;
	}

private:
	static inline bool isMatchCandidate(
		const MNMesh& mesh, 
		const Bitset& work2, 
		const Bitset& faces, 
		const i32 f
	) {
		return !work2[f + 1] && faces[f + 1] && Helpers::Geom::numFaceEdges(mesh, f) == 3;
	}

private:
	static inline i32 countBorders(
		const MNMesh& mesh, 
		const Bitset& borders, 
		const Bitset& faces, 
		const i32 f, 
		const i32 e
	) {
		const MNFace& face = mesh.f[f];
		i32 eA, eB;
		otherEdges(face, e, eA, eB);
		const i32 fA = Helpers::Geom::otherFace(mesh, eA, f);
		const i32 fB = Helpers::Geom::otherFace(mesh, eB, f);
		const bool bA = borders[fA + 1] || !faces[fA + 1];
		const bool bB = borders[fB + 1] || !faces[fB + 1];
		return bA + bB;
	}

private:
	static inline void pushMatch(
		List<i32>& quads, 
		Bitset& borders, 
		Bitset& edges, 
		const i32 f1, 
		const i32 f2, 
		const i32 edge
	) {
		edges.set(edge);
		quads.push(f1);
		quads.push(f2);
		borders.set(f1 + 1);
		borders.set(f2 + 1);
	}

};
