#pragma once

#include "Util.h"
#include "Codes.h"

// convenience classes for dealing with editable polies and (skin) modifiers.
class Helpers {
public:

	class Misc {
	public:

		static inline i32 extractIntTabs(
			const List<FPValue*>& value,
			List<Tab<i32>*>& tabs
		) {
			using ERR = Codes::errors_extractIntTabs;
			tabs.clearAll();
			tabs.ensureCapacity(value.length());
			for (i32 i = 0; i < value.length(); ++i) {
				FPValue* v = value[i];
				if (!v) {
					return ERR::null_val;
				}
				const i32 type = v->type;
				if (type != TYPE_INT_TAB) {
					return ERR::not_int_tab;
				}
				Tab<i32>* vTab = v->i_tab;
				if (!vTab) {
					return ERR::null_tab;
				}
				tabs.push(vTab);
			}
			return Codes::OK;
		}
	};

	class Geom {
	public:

		static inline bool checkBounds(const List<i32>& list, const i32 bound) {
			DbgAssert(bound >= 0);
			for (i32 i = 0; i < list.length(); ++i) {
				const i32 v = list[i] - 1;
				if (v < 0 || v >= bound) {
					return false;
				}
			}
			return true;
		}

		static inline bool isVertFlagged(const MNMesh& mesh, const i32 v, const DWORD flag) {
			return mesh.v[v].GetFlag(flag);
		}

		static inline bool isEdgeFlagged(const MNMesh& mesh, const i32 e, const DWORD flag) {
			return mesh.e[e].GetFlag(flag);
		}

		static inline bool isFaceFlagged(const MNMesh& mesh, const i32 f, const DWORD flag) {
			return mesh.f[f].GetFlag(flag);
		}


		static inline i32 otherVert(const MNEdge& edge, const i32 v) {
			return v == edge.v1 ? edge.v2 : edge.v1;
		}

		static inline i32 otherVert(const MNMesh& mesh, const i32 e, const i32 v) {
			return otherVert(mesh.e[e], v);
		}

		static inline i32 otherFace(const MNEdge& edge, const i32 f) {
			return f == edge.f1 ? edge.f2 : edge.f1;
		}


		static inline i32 otherFace(const MNMesh& mesh, const i32 e, const i32 f) {
			return otherFace(mesh.e[e], f);
		}

		// index of vert in iteration order
		static inline i32 vertIndexOrdered(const MNFace& face, const i32 edgeIdx, const i32 v) {
			i32 vIdx = edgeIdx;
			if (v != face.vtx[edgeIdx]) {
				vIdx = edgeIdx + 1;
				if (vIdx == face.deg) {
					return 0;
				}
			}
			return vIdx;
		}


		// index of vert in iteration order
		static inline i32 vertIndexOrdered(const MNMesh& mesh, const i32 f, const i32 edgeIdx, const i32 v) {
			const MNFace& face = mesh.f[f];
			return vertIndexOrdered(face, edgeIdx, v);
		}

		static inline i32 vertIndex(const MNFace& face, const i32 v) {
			const i32 n = face.deg;
			for (i32 i = 0; i < n; ++i) {
				if (v == face.vtx[i]) {
					return i;
				}
			}
			return -1;
		}

		static inline i32 vertIndex(const MNMesh& mesh, const i32 f, const i32 v) {
			return vertIndex(mesh.f[f], v);
		}

		static inline i32 edgeIndex(const MNFace& face, const i32 e) {
			for (i32 i = 0; i < face.deg; ++i) {
				if (e == face.edg[i]) {
					return i;
				}
			}
			return -1;
		}

		static inline i32 edgeIndex(const MNMesh& mesh, const i32 f, const i32 e) {
			return edgeIndex(mesh.f[f], e);
		}

		// advances indices in correct order, while maintaining offset between edge index and vert index
		static inline void nextEdgeIdx(const i32 deg, i32& edgeIdx, i32& vertIdx) {
			if (edgeIdx == vertIdx) {
				if (vertIdx == 0) {
					vertIdx = deg;
				}
				--vertIdx;
				edgeIdx = vertIdx;
			}
			else {
				edgeIdx = vertIdx;
				++vertIdx;
				if (vertIdx == deg) {
					vertIdx = 0;
				}
			}
		}

		// collects edges and verts of face in proper order
		static inline void collectFace(
			const MNMesh& mesh,
			List<i32>& edges, List<i32>& verts, Bitset& facesDone,
			const i32 f, const i32 e, const i32 v
		) {
			const MNFace& face = mesh.f[f];
			i32 eIdx = edgeIndex(face, e);
			i32 vIdx = vertIndexOrdered(face, eIdx, v);
			edges.push(e);
			verts.push(v);
			for (i32 i = 1; i < face.deg; ++i) {
				nextEdgeIdx(face.deg, eIdx, vIdx);
				edges.push(face.edg[eIdx]);
				verts.push(face.vtx[vIdx]);
			}
			facesDone.set(f + 1);
		}

		// returns the only eligibly face connected to the edge, or -1 if no such face exists
		static inline i32 nextEdgeFace(const MNMesh& mesh, const Bitset& facesDone, const i32 e, const Bitset& bits) {
			const MNEdge& edge = mesh.e[e];
			const i32 f1 = edge.f1;
			if (!facesDone[f1 + 1]) {
				return bits[f1] ? f1 : -1;
			}
			const i32 f2 = edge.f2;
			if (!facesDone[f2 + 1]) {
				// never -1
				return bits[f2] ? f2 : -1;
			}
			return -1;
		}


		static inline i32 reverse(const MNMesh& mesh, const MNFace& face, i32 edgeIdx, i32 vertIdx) {
			return vertIndexOrdered(face, edgeIdx, otherVert(mesh.e[face.edg[edgeIdx]], face.vtx[vertIdx]));
		}



		static inline i32 sharedFaceEdge(const MNMesh& mesh, const i32 faceA, const i32 faceB) {
			const MNFace& fa = mesh.f[faceA];
			const MNFace& fb = mesh.f[faceB];
			if (fa.deg < fb.deg) {
				for (i32 i = 0; i < fa.deg; ++i) {
					const i32 e = fa.edg[i];
					const MNEdge& edge = mesh.e[e];
					if (edge.f1 == faceB || edge.f2 == faceB) {
						return e;
					}
				}
			}
			else {
				for (i32 i = 0; i < fb.deg; ++i) {
					const i32 e = fb.edg[i];
					const MNEdge& edge = mesh.e[e];
					if (edge.f1 == faceA || edge.f2 == faceA) {
						return e;
					}
				}
			}
			return -1;
		}

		static inline i32 sharedVertEdge(const MNMesh& mesh, const i32 vertA, const i32 vertB) {
			const List<i32> vedg(mesh.vedg[vertA]);
			for (i32 i = 0; i < vedg.length(); ++i) {
				const i32 e = vedg[i];
				const MNEdge& edge = mesh.e[e];
				if (edge.v1 == vertB || edge.v2 == vertB) {
					return e;
				}
			}
			return -1;
		}



		static inline i32 vert1(const MNMesh& mesh, const i32 e) {
			const MNEdge& edge = mesh.e[e];
			return edge.v1;
		}
		static inline i32 vert2(const MNMesh& mesh, const i32 e) {
			const MNEdge& edge = mesh.e[e];
			return edge.v2;
		}
		static inline i32 face1(const MNMesh& mesh, const i32 e) {
			const MNEdge& edge = mesh.e[e];
			return edge.f1;
		}
		static inline i32 face2(const MNMesh& mesh, const i32 e) {
			const MNEdge& edge = mesh.e[e];
			return edge.f2;
		}

		static inline i32 numFaceEdges(const MNMesh& mesh, const i32 f) {
			const MNFace& face = mesh.f[f];
			return face.deg;
		}


		static inline i32 numVertEdges(const MNMesh& mesh, const i32 v) {
			const IntTab& tab = mesh.vedg[v];
			return tab.Count();
		}

		static inline i32 numVertFaces(const MNMesh& mesh, const i32 v) {
			const IntTab& tab = mesh.vfac[v];
			return tab.Count();
		}

		static inline bool isBorder(const MNMesh& mesh, const i32 e) {
			return mesh.e[e].f2 < 0;
		}







	};


	class ModIterator {
	protected:
		INode* m_node;
		i32 m_numMods;
		i32 m_idx;
		i32 m_nextIdx;
		Modifier* m_nextMod;
		ModContext* m_nextCtx;
		Object* m_object;
		IDerivedObject* m_derived;
	public:
		explicit ModIterator(INode& node) {
			reset(node);
		}
		~ModIterator() {}

		inline void reset(INode& node) {
			m_node = &node;
			m_numMods = 0;
			m_idx = 0;
			m_nextIdx = -1;
			m_nextMod = nullptr;
			m_nextCtx = nullptr;
			m_derived = nullptr;
			m_object = node.GetObjectRef();
			if (m_object->SuperClassID() == GEN_DERIVOB_CLASS_ID) {
				m_derived = dynamic_cast<IDerivedObject*>(m_object);
				m_numMods = m_derived->NumModifiers();
			}
		}

		// moves to the next mod. false if no next mod exist.
		inline bool next() {
			do {
				// only one condition on hot path? nice.
				if (m_idx < m_numMods) {
					m_nextMod = m_derived->GetModifier(m_idx);
					m_nextCtx = m_derived->GetModContext(m_idx);
					++m_nextIdx;
					++m_idx;
					return true;
				}
				if (!m_derived) {
					return false;
				}
				m_object = m_derived->GetObjRef();
				if (m_object->SuperClassID() != GEN_DERIVOB_CLASS_ID) {
					m_derived = nullptr;
					return false;
				}
				m_derived = dynamic_cast<IDerivedObject*>(m_object);
				m_numMods = m_derived->NumModifiers();
				m_idx = 0;
			} while (true);
		}

		inline INode* node() const {
			return m_node;
		}

		// returns the index of the current mod, or the total mod count after iteration has concluded.
		// only valid after calling next() at least once.
		inline i32 index() const {
			return m_nextIdx;
		}
		// returns the current mod. 
		// only valid after calling next() at least once.
		inline Modifier* mod() const {
			return m_nextMod;
		}
		// returns the current mod context. 
		// only valid after calling next() at least once.
		inline ModContext* context() const {
			return m_nextCtx;
		}
		// returns the current object.
		// only valid after calling next() at least once.
		inline Object* object() const {
			return m_object;
		}
		// return the base object, i.e the end of the modstack pipeline.
		// this means it will always fully iterate to the end before returning.
		inline Object* baseObject() {
			while (next()) {}
			return m_object;
		}
	};

	// skips over non-skin mods.
	class SkinModIterator final : public ModIterator {
	public:
		explicit SkinModIterator(INode& node) : ModIterator::ModIterator(node) {}
		~SkinModIterator() {}
		inline bool next() {
			while (ModIterator::next()) {
				if (m_nextMod->ClassID() == SKIN_CLASSID) {
					return true;
				}
			}
			return false;
		}
	};

	class Poly {
	protected:
		INode* m_node;
		EPoly* m_poly;
		MNMesh* m_mesh;
	private:
		inline i32 initImpl(
			INode* node,
			Object* baseObject
		) {
			using ERR = Codes::Poly_init;
			if (!baseObject) {
				return ERR::null_base_object;
			}
			if (baseObject->ClassID() != EPOLYOBJ_CLASS_ID) {
				return ERR::wrong_class_ID;
			}
			EPoly* poly = (EPoly*)(baseObject->GetInterface(EPOLY_INTERFACE));
			if (!poly) {
				return ERR::failed_cast;
			}
			MNMesh* mesh = poly->GetMeshPtr();
			if (!mesh) {
				return ERR::null_mesh;
			}
			if (!(mesh->GetFlag(MN_MESH_FILLED_IN))) {
				return ERR::mesh_not_filled_in;
			}
			m_node = node;
			m_poly = poly;
			m_mesh = mesh;
			return Codes::OK;
		}

	public:
		Poly() :
			m_node{ nullptr },
			m_poly{ nullptr },
			m_mesh{ nullptr }
		{}
		Poly(const Poly& other) noexcept :
			m_node{ other.m_node },
			m_poly{ other.m_poly },
			m_mesh{ other.m_mesh }
		{}
		Poly(Poly&& other) :
			m_node{ other.m_node },
			m_poly{ other.m_poly },
			m_mesh{ other.m_mesh }
		{
			other.m_node = nullptr;
			other.m_poly = nullptr;
			other.m_mesh = nullptr;
		}
		Poly& operator= (const Poly& other) {
			if (&other != this) {
				m_node = other.m_node;
				m_poly = other.m_poly;
				m_mesh = other.m_mesh;
			}
			return *this;
		}
		Poly& operator= (Poly&& other) {
			if (&other != this) {
				m_node = other.m_node;
				m_poly = other.m_poly;
				m_mesh = other.m_mesh;
				other.m_node = nullptr;
				other.m_poly = nullptr;
				other.m_mesh = nullptr;
			}
			return *this;
		}
		~Poly() {}

		inline i32 init(
			INode* node
		) {
			using ERR = Codes::Poly_init;
			if (!node) {
				return ERR::null_node;
			}

			ModIterator it(*node);

			return initImpl(node, it.baseObject());
		}

		inline i32 init(
			INode* node,
			Object* baseObject
		) {
			using ERR = Codes::Poly_init;
			if (!node) {
				return ERR::null_node;
			}
			return initImpl(node, baseObject);
		}

		inline INode* node() const {
			return m_node;
		}
		inline EPoly& epoly() const {
			return *m_poly;
		}
		inline MNMesh& mesh() const {
			return *m_mesh;
		}

		inline void changedGeom() {
			m_mesh->InvalidateGeomCache();
			m_node->NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
			// TODO possibly redraw views?
			m_poly->LocalDataChanged(GEOM_CHANNEL);
			m_poly->RefreshScreen();
		}

		// sets the spefied flag for all verts in bitset
		// if clear is true, clears the flag from all other verts
		inline void setVertFlag(const Bitset& sel, const bool clear, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			const i32 num = mesh.numv;
			if (clear) {
				mesh.ClearVFlags(flag);
				//for (i32 i = 0; i < num; ++i) {
				//	mesh.v[i].ClearFlag(flag);
				//}
			}
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < num);) {
				mesh.v[i].SetFlag(flag);
			}
		}

		// sets the spefied flag for all edges in bitset
		// if clear is true, clears the flag from all other edges
		inline void setEdgeFlag(const Bitset& sel, const bool clear, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			const i32 num = mesh.nume;
			if (clear) {
				mesh.ClearEFlags(flag);
				//for (i32 i = 0; i < num; ++i) {
				//	mesh.e[i].ClearFlag(flag);
				//}
			}
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < num);) {
				mesh.e[i].SetFlag(flag);
			}
		}

		// sets the spefied flag for all faces in bitset
		// if clear is true, clears the flag from all other faces
		inline void setFaceFlag(const Bitset& sel, const bool clear, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			const i32 num = mesh.numf;
			if (clear) {
				mesh.ClearFFlags(flag);
				//for (i32 i = 0; i < num; ++i) {
				//	mesh.f[i].ClearFlag(flag);
				//}
			}
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < num);) {
				mesh.f[i].SetFlag(flag);
			}
		}

		inline void setElemFlag(const i32 level, const Bitset& sel, const bool clear, const DWORD flag) {
			switch (level) {
			case 0: {
				setVertFlag(sel, clear, flag);
				break;
			}
			case 1: {
				setEdgeFlag(sel, clear, flag);
				break;
			}
			default: {
				setFaceFlag(sel, clear, flag);
				break;
			}
			}
		}












		// sets the spefied flag for all faces in bitset
		// if clear is true, clears the flag from all other faces
		inline void clearVertFlag(const Bitset& sel, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < mesh.numv);) {
				mesh.v[i].ClearFlag(flag);
			}
		}

		// sets the spefied flag for all faces in bitset
		// if clear is true, clears the flag from all other faces
		inline void clearEdgeFlag(const Bitset& sel, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < mesh.nume);) {
				mesh.e[i].ClearFlag(flag);
			}
		}

		// sets the spefied flag for all faces in bitset
		// if clear is true, clears the flag from all other faces
		inline void clearFaceFlag(const Bitset& sel, const DWORD flag) {
			MNMesh& mesh = *m_mesh;
			Bitset::Iterator it(sel);
			for (i32 i; ((i = it.next()) >= 0 && i < mesh.numf);) {
				mesh.f[i].ClearFlag(flag);
			}
		}


		inline void clearElemFlag(const i32 level, const Bitset& sel, const DWORD flag) {
			switch (level) {
			case 0: {
				clearVertFlag(sel, flag);
				break;
			}
			case 1: {
				clearEdgeFlag(sel, flag);
				break;
			}
			default: {
				clearFaceFlag(sel, flag);
				break;
			}
			}
		}

		// adds verts with flag to bits. does not sets bitset length. careful
		inline void collectFlaggedVerts(Bitset& bits, const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			for (i32 i = 0; i < mesh.numv; ++i) {
				if (mesh.v[i].GetFlag(flag)) {
					bits.set(i);
				}
			}
		}

		// adds edges with flag to bits. does not sets bitset length. careful
		inline void collectFlaggedEdges(Bitset& bits, const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			for (i32 i = 0; i < mesh.nume; ++i) {
				if (mesh.e[i].GetFlag(flag)) {
					bits.set(i);
				}
			}
		}

		// adds faces with flag to bits. does not sets bitset length. careful
		inline void collectFlaggedFaces(Bitset& bits, const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			for (i32 i = 0; i < mesh.numf; ++i) {
				if (mesh.f[i].GetFlag(flag)) {
					bits.set(i);
				}
			}
		}



		inline void collectFlaggedElem(const i32 level, Bitset& bits, const DWORD flag) const {
			switch (level) {
			case 0: {
				collectFlaggedVerts(bits, flag);
				break;
			}
			case 1: {
				collectFlaggedEdges(bits, flag);
				break;
			}
			default: {
				collectFlaggedFaces(bits, flag);
				break;
			}
			}
		}



		inline Bitset* collectFlaggedVerts(const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			Bitset* bs = new Bitset(mesh.numv);
			collectFlaggedVerts(*bs, flag);
			return bs;
		}

		inline Bitset* collectFlaggedEdges(const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			Bitset* bs = new Bitset(mesh.nume);
			collectFlaggedEdges(*bs, flag);
			return bs;
		}

		inline Bitset* collectFlaggedFaces(const DWORD flag) const {
			const MNMesh& mesh = *m_mesh;
			Bitset* bs = new Bitset(mesh.numf);
			collectFlaggedFaces(*bs, flag);
			return bs;
		}

		inline Bitset* collectFlaggedElem(const i32 level, const DWORD flag) const {
			switch (level) {
			case 0: {
				return collectFlaggedVerts(flag);
			}
			case 1: {
				return collectFlaggedEdges(flag);
			}
			default: {
				return collectFlaggedFaces(flag);
			}
			}
		}

		inline void collectPositions(List<Point3>& positions) const {
			const MNMesh& mesh = *m_mesh;
			positions.clearAll();
			for (i32 i = 0; i < mesh.numv; ++i) {
				const Point3 pos = mesh.P(i);
				positions.push(pos);
			}
		}

		// get average x pos of face f's verts. checks for NaNs and ignores them in the calculation
		inline f64 avgPosX(const i32 f) const {
			const MNMesh& mesh = *m_mesh;
			f64 x = 0.0;
			const MNFace& face = mesh.f[f];
			const i32 deg = face.deg;
			i32 valid = 0;
			for (i32 i = 0; i < deg; ++i) {
				const Point3& pos = mesh.P(face.vtx[i]);
				if (pos.x == pos.x) {
					++valid;	// not NaN
				}
			}
			if (valid > 0) {
				f64 factor = 1.0 / valid;
				for (i32 i = 0; i < deg; ++i) {
					const Point3& pos = mesh.P(face.vtx[i]);
					if (pos.x == pos.x) {
						x += (factor * pos.x);
					}
				}
			}
			return x;
		}

		// get average y pos of face f's verts. checks for NaNs and ignores them in the calculation
		inline f64 avgPosY(const i32 f) const {
			const MNMesh& mesh = *m_mesh;
			f64 y = 0.0;
			const MNFace& face = mesh.f[f];
			const i32 deg = face.deg;
			i32 valid = 0;
			for (i32 i = 0; i < deg; ++i) {
				const Point3& pos = mesh.P(face.vtx[i]);
				if (pos.y == pos.y) {
					++valid;	// not NaN
				}
			}
			if (valid > 0) {
				f64 factor = 1.0 / valid;
				for (i32 i = 0; i < deg; ++i) {
					const Point3& pos = mesh.P(face.vtx[i]);
					if (pos.y == pos.y) {
						y += (factor * pos.y);
					}
				}
			}
			return y;
		}

		// get average z pos of face f's verts. checks for NaNs and ignores them in the calculation
		inline f64 avgPosZ(const i32 f) const {
			const MNMesh& mesh = *m_mesh;
			f64 z = 0.0;
			const MNFace& face = mesh.f[f];
			const i32 deg = face.deg;
			i32 valid = 0;
			for (i32 i = 0; i < deg; ++i) {
				const Point3& pos = mesh.P(face.vtx[i]);
				if (pos.z == pos.z) {
					++valid;	// not NaN
				}
			}
			if (valid > 0) {
				f64 factor = 1.0 / valid;
				for (i32 i = 0; i < deg; ++i) {
					const Point3& pos = mesh.P(face.vtx[i]);
					if (pos.z == pos.z) {
						z += (factor * pos.z);
					}
				}
			}
			return z;
		}



	};



	// Skin modifier helper.
	class SkinMod {
	protected:
		INode* m_node;
		i32 m_idx;
		Modifier* m_mod;
		ModContext* m_modCtx;
		ISkin* m_skin;
		ISkin2* m_skin2;
		ISkinContextData* m_ctx;
		ISkinImportData* m_imp;
	private:

		inline i32 initImpl(
			INode* node,
			const ModIterator& it
		) {
			using ERR = Codes::SkinMod_init;
			Modifier* mod = it.mod();
			ModContext* modCtx = it.context();
			if (!modCtx) {
				return ERR::null_mod_context;
			}
			ISkin* skin = (ISkin*)mod->GetInterface(I_SKIN);
			if (!skin) {
				return ERR::failed_cast;
			}
			ISkin2* skin2 = (ISkin2*)mod->GetInterface(I_SKIN2);
			if (!skin2) {
				return ERR::failed_cast_2;
			}
			ISkinContextData* ctx = skin->GetContextInterface(node);
			if (!ctx) {
				return ERR::null_context_data;
			}
			ISkinImportData* imp = (ISkinImportData*)mod->GetInterface(I_SKINIMPORTDATA);
			if (!imp) {
				return ERR::null_import_data;
			}
			m_node = node;
			m_idx = it.index();
			m_mod = mod;
			m_modCtx = modCtx;
			m_skin = skin;
			m_skin2 = skin2;
			m_ctx = ctx;
			m_imp = imp;
			return Codes::OK;
		}

	public:
		SkinMod() :
			m_node{ nullptr },
			m_idx{ 0 },
			m_mod{ nullptr },
			m_modCtx{ nullptr },
			m_skin{ nullptr },
			m_skin2{ nullptr },
			m_ctx{ nullptr },
			m_imp{ nullptr }
		{}
		SkinMod(const SkinMod& other) noexcept :
			m_node{ other.m_node },
			m_idx{ other.m_idx },
			m_mod{ other.m_mod },
			m_modCtx{ other.m_modCtx },
			m_skin{ other.m_skin },
			m_skin2{ other.m_skin2 },
			m_ctx{ other.m_ctx },
			m_imp{ other.m_imp }
		{}
		SkinMod(SkinMod&& other) :
			m_node{ other.m_node },
			m_idx{ other.m_idx },
			m_mod{ other.m_mod },
			m_modCtx{ other.m_modCtx },
			m_skin{ other.m_skin },
			m_skin2{ other.m_skin2 },
			m_ctx{ other.m_ctx },
			m_imp{ other.m_imp }
		{
			other.m_node = nullptr;
			other.m_idx = 0;
			other.m_mod = nullptr;
			other.m_modCtx = nullptr;
			other.m_skin = nullptr;
			other.m_skin2 = nullptr;
			other.m_ctx = nullptr;
			other.m_imp = nullptr;
		}
		SkinMod& operator= (const SkinMod& other) {
			if (&other != this) {
				m_node = other.m_node;
				m_idx = other.m_idx;
				m_mod = other.m_mod;
				m_modCtx = other.m_modCtx;
				m_skin = other.m_skin;
				m_skin2 = other.m_skin2;
				m_ctx = other.m_ctx;
				m_imp = other.m_imp;
			}
			return *this;
		}
		SkinMod& operator= (SkinMod&& other) {
			if (&other != this) {
				m_node = other.m_node;
				m_idx = other.m_idx;
				m_mod = other.m_mod;
				m_modCtx = other.m_modCtx;
				m_skin = other.m_skin;
				m_skin2 = other.m_skin2;
				m_ctx = other.m_ctx;
				m_imp = other.m_imp;
				other.m_node = nullptr;
				other.m_idx = 0;
				other.m_mod = nullptr;
				other.m_modCtx = nullptr;
				other.m_skin = nullptr;
				other.m_skin2 = nullptr;
				other.m_ctx = nullptr;
				other.m_imp = nullptr;
			}
			return *this;
		}
		~SkinMod() {}

		inline i32 init(
			INode* node,
			const i32 modIndex
		) {
			using ERR = Codes::SkinMod_init;
			if (!node) {
				return ERR::null_node;
			}
			if (modIndex < 0) {
				return ERR::bad_index;
			}
			for (ModIterator it(*node); it.next();) {
				if (it.index() == modIndex) {
					if (it.mod()->ClassID() != SKIN_CLASSID) {
						return ERR::wrong_class_ID;
					}
					return initImpl(node, it);
				}
			}
			return ERR::bad_index;
		}

		inline i32 init(
			const SkinModIterator& it
		) {
			return initImpl(it.node(), it);
		}

		// collects node's skin mods in a list
		// ignoreZeroPointSkins - if true, any skin mod with <= 0 numPoints will not be collected 
		static inline i32 collect(INode* node, List<SkinMod>& list, const bool ignoreZeroPointSkins) {
			using ERR = Codes::SkinMod_init;
			if (!node) {
				return ERR::null_node;
			}
			list.clearAll();
			for (SkinModIterator it(*node); it.next(); ) {
				SkinMod m;
				const i32 r = m.initImpl(node, it);
				if (r != Codes::OK) {
					return r;
				}
				if (!ignoreZeroPointSkins || m.getNumPoints() > 0) {
					list.push(m);
				}
			}
			return Codes::OK;
		}

		// false if any skin mod in the list has numPoints != expectedNumPoints
		static inline bool checkNumPoints(const List<SkinMod>& list, const i32 expectedNumPoints) {
			for (i32 i = 0; i < list.length(); ++i) {
				if (list[i].getNumPoints() != expectedNumPoints) {
					return false;
				}
			}
			return true;
		}

		inline void toggleMod() {
			Modifier& mod = *m_mod;
			if (mod.IsEnabled()) {
				mod.DisableMod();
				mod.EnableMod();
			}
			else {
				mod.EnableMod();
				mod.DisableMod();
			}
		}

		// the modifier's node
		inline INode* node() const {
			return m_node;
		}
		// the index in the modstack
		inline i32 index() const {
			return m_idx;
		}
		// the actual modifier
		inline Modifier& mod() const {
			return *m_mod;
		}
		// the actual modifier context
		inline ModContext& context() const {
			return *m_modCtx;
		}


		// ISkin

		inline i32 getBoneInitTM(INode* pNode, Matrix3& InitTM, const bool bObjOffset = false) {
			return m_skin->GetBoneInitTM(pNode, InitTM, bObjOffset);
		}
		inline i32 getSkinInitTM(INode* pNode, Matrix3& InitTM, const bool bObjOffset = false) {
			return m_skin->GetSkinInitTM(pNode, InitTM, bObjOffset);
		}
		inline i32 getNumBones() {
			return m_skin->GetNumBones();
		}
		inline INode* getBone(const i32 idx) {
			return m_skin->GetBone(idx);
		}
		inline DWORD getBoneProperty(const i32 idx) {
			return m_skin->GetBoneProperty(idx);
		}
		inline const wchar_t* getBoneName(const i32 index) {
			return m_skin->GetBoneName(index);
		}
		inline i32 getSelectedBone() {
			return m_skin->GetSelectedBone();
		}
		inline Matrix3 getBoneTm(const i32 id) {
			return m_skin->GetBoneTm(id);
		}
		inline INode* getBoneFlat(const i32 idx) {
			return m_skin->GetBoneFlat(idx);
		}
		inline i32 getNumBonesFlat() {
			return m_skin->GetNumBonesFlat();
		}
		inline i32 getRefFrame() {
			return m_skin->GetRefFrame();
		}
		inline bool getDQBlending() {
			return static_cast<bool>(m_skin->GetDQBlending());
		}
		inline void setDQBlending(const bool blending) {
			m_skin->SetDQBlending(static_cast<i32>(blending));
		}

		// ISkinContextData

		inline i32 getNumPoints() {
			return m_ctx->GetNumPoints();
		}
		inline i32 getNumAssignedBones(const i32 vertexIdx) {
			return m_ctx->GetNumAssignedBones(vertexIdx);
		}
		inline i32 getAssignedBone(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetAssignedBone(vertexIdx, boneIdx);
		}
		inline f32 getBoneWeight(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetBoneWeight(vertexIdx, boneIdx);
		}
		inline i32 getSubCurveIndex(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetSubCurveIndex(vertexIdx, boneIdx);
		}
		inline i32 getSubSegmentIndex(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetSubSegmentIndex(vertexIdx, boneIdx);
		}
		inline f32 getSubSegmentDistance(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetSubSegmentDistance(vertexIdx, boneIdx);
		}
		inline Point3 getTangent(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetTangent(vertexIdx, boneIdx);
		}
		inline Point3 getOPoint(const i32 vertexIdx, const i32 boneIdx) {
			return m_ctx->GetOPoint(vertexIdx, boneIdx);
		}
		inline void setDQBlendWeight(const i32 index, const f32 value) {
			m_ctx->SetDQBlendWeight(index, value);
		}
		inline f32 getDQBlendWeight(const i32 index) {
			return m_ctx->GetDQBlendWeight(index);
		}

		// ISkinImportData

		inline bool addBoneEx(INode* boneNode, const bool update = false) {
			return static_cast<bool>(m_imp->AddBoneEx(boneNode, static_cast<i32>(update)));
		}
		inline bool setSkinTm(Matrix3 objectTm, Matrix3 nodeTm) {
			return static_cast<bool>(m_imp->SetSkinTm(m_node, objectTm, nodeTm));
		}
		inline bool setBoneTm(INode* boneNode, Matrix3 objectTm, Matrix3 nodeTm) {
			return static_cast<bool>(m_imp->SetBoneTm(boneNode, objectTm, nodeTm));
		}
		inline bool addWeights(const i32 vertexID, Tab<INode*>& boneNodeList, Tab<float>& weights) {
			return static_cast<bool>(m_imp->AddWeights(m_node, vertexID, boneNodeList, weights));
		}

		// ISkin2

		inline bool setBoneStretchTm(INode* boneNode, Matrix3 stretchTm) {
			return static_cast<bool>(m_skin2->SetBoneStretchTm(boneNode, stretchTm));
		}
		inline Matrix3 getBoneStretchTm(INode* boneNode) {
			return m_skin2->GetBoneStretchTm(boneNode);
		}
		inline void getVertexSelection(BitArray& sel) {
			m_skin2->GetVertexSelection(m_node, sel);
		}


		// actually sets the vert selection. may require that "sel" have a size <= getNumPoints(). not sure.
		inline void setVertexSelection(const BitArray& sel) {
			const i32 numPoints = m_ctx->GetNumPoints();
			BitArray tmp(numPoints);
			m_skin2->GetVertexSelection(m_node, tmp);	// this somehow "primes" SetVertexSelection for "tmp". ohtherwise nothing works.
			tmp.ClearAll();	// keeps it intact
			tmp |= sel;	// keeps it intact because verts <= tmp
			m_skin2->SetVertexSelection(m_node, tmp);	// requires that GetVertexSelection was called with the same array right before. Stupid, but still better than any other impl available.
			m_mod->NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);	// update UI
		}

		// returns the bone affect limit of the skin mod
		// very slow and shitty. don't run in a tight loop.
		inline i32 getBoneLimit() {
			// generate and run a tiny scrap of MaxScript.
			FPValue v;
			CharBuffer buf(L"(GetAnimByHandle 0xFFFFFFFFFFFFFFFFP).bone_limit", 49);
			buf.writeU64Hex(19, U64(Animatable::GetHandleByAnim(m_mod)), false);
			ExecuteMAXScriptScript(buf.addr(), true, &v, false);
			return v.i;
		}

		// sets the bone affect limit of the skin mod
		// undoable. calling with undo off works as expected.
		// very slow and shitty. don't run in a tight loop.
		inline void setBoneLimit(const i32 val) {
			FPValue v;
			CharBuffer buf(L"(GetAnimByHandle 0xFFFFFFFFFFFFFFFFP).bone_limit=0xFFFFFFFF", 60);
			buf.writeU64Hex(19, U64(Animatable::GetHandleByAnim(m_mod)), false);
			buf.writeU32Hex(51, U32(val), false);
			ExecuteMAXScriptScript(buf.addr(), true, &v, false);
		}

		// true if both share stuff inconsistently
		inline bool isMangled(const Helpers::SkinMod& other) const noexcept {
			return m_mod == other.m_mod && (m_ctx != other.m_ctx || m_imp != other.m_imp);
		}

		inline i32 indexOfBone(INode* bone) {
			const i32 numBones = m_skin->GetNumBones();
			for (i32 i = 0; i < numBones; ++i) {
				if (m_skin->GetBone(i) == bone) {
					return i;
				}
			}
			return -1;
		}
















		inline f32 averageDQ(const i32 a, const i32 b) {
			return F32((F64(NaN32(getDQBlendWeight(a))) + F64(NaN32(getDQBlendWeight(b)))) / 2.0);	// average DQ blend weight
		}


		// collects all bone names in ascending order, with ids collected in the same order.
		// false if duplicate trimmed names are present (terminates early)
		inline bool collectBoneNames(List<Slice>& boneNames, List<i32>& boneIds) {
			boneIds.clearAll();
			boneNames.clearAll();
			const i32 numBones = getNumBones();
			const CompSlice comp;
			for (i32 i = 0; i < numBones; ++i) {
				INode* bone = getBone(i);
				Slice name(bone->GetName());
				name.trim();
				const i32 idx = boneNames.indexOfAscending(name, comp);
				if (boneNames.detect(idx, name, comp)) {
					return false;
				}
				boneNames.insert(idx, name);
				boneIds.insert(idx, i);
			}
			return true;
		}

		inline void collectBones(const i32 vert, List<INode*>& bones) {
			bones.clearAll();
			const i32 nb = getNumAssignedBones(vert);
			for (i32 i = 0; i < nb; ++i) {
				bones.push(getBone(getAssignedBone(vert, i)));
			}
		}

		inline void collectWeights(const i32 vert, List<f32>& weights) {
			weights.clearAll();
			const i32 nb = getNumAssignedBones(vert);
			for (i32 i = 0; i < nb; ++i) {
				weights.push(getBoneWeight(vert, i));
			}
		}

		// bonesIds must have correct capacity!
		inline void collectUsedBoneIds(Bitset& boneIds, const List<i32>& verts, const Bitset& selection) {
			boneIds.clearAll();
			for (i32 i = 0; i < verts.length(); ++i) {
				const i32 v = verts[i] - 1;
				if (selection[v]) {
					const i32 nb = getNumAssignedBones(v);
					for (i32 j = 0; j < nb; ++j) {
						boneIds.set(getAssignedBone(v, j));
					}
				}
			}
		}

		inline void collectUnusedBoneIds(List<i32>& boneIds) {
			const i32 numBones = getNumBones();
			const i32 numPoints = getNumPoints();
			i32 numUnused = numBones;
			Bitset used(numBones);
			for (i32 i = 0; i < numPoints; ++i) {
				const i32 nb = getNumAssignedBones(i);
				for (i32 j = 0; j < nb; ++j) {
					const i32 id = getAssignedBone(i, j);
					if (used.trySet(id) && --numUnused == 0) {
						// all bones are used. collect nothing, terminate early.
						boneIds.clearAll();
						return;
					}
				}
			}
			boneIds.ensureCapacity(numUnused);
			boneIds.clearAll();
			for (i32 i = 0; i < numBones; ++i) {
				if (!used[i]) {
					boneIds.push(i);
				}
			}
		}

		inline void collectAll(Pointers<IntTab>& ids, Pointers<FloatTab>& wgt, List<f32>& dqs, const i32 v) {
			const i32 nb = getNumAssignedBones(v);
			List<i32> b(nb);
			List<f32> w(nb);
			for (i32 j = 0; j < nb; ++j) {
				b.push(getAssignedBone(v, j));
			}
			for (i32 j = 0; j < nb; ++j) {
				w.push(getBoneWeight(v, j));
			}
			ids.push(b.move());
			wgt.push(w.move());
			dqs.push(getDQBlendWeight(v));
		}

		// collects ids, weights and dqs for all selected verts in order.
		inline void collectAll(Pointers<IntTab>& ids, Pointers<FloatTab>& wgt, List<f32>& dqs, const List<i32>& verts, const Bitset& selection) {
			for (i32 i = 0; i < verts.length(); ++i) {
				const i32 v = verts[i] - 1;
				if (selection[v]) {
					collectAll(ids, wgt, dqs, v);
				}
			}
		}



















	};





};
