#pragma once

#include "Util.h"
#include "DiffPFOR.h"

class Codes {
private:
	static constexpr i32 COUNTER_BASE = __COUNTER__;
public:
	// free to re-order everything except OK. that needs to be 0
	static constexpr const i32 OK = 0;

	class Poly_init {
	public:
		enum {
			null_node = -1,
			null_base_object = -2,
			null_mesh = -3,
			wrong_class_ID = -4,
			failed_cast = -5,
			mesh_not_filled_in = -6,
		};
	};

	class SkinMod_init {
	public:
		enum {
			null_node = -1,
			null_mod_context = -2,
			null_context_data = -3,
			null_import_data = -4,
			failed_cast = -5,
			failed_cast_2 = -6,
			bad_index = -7,
			wrong_class_ID = -8,
		};
	};

	class errors_extractOrderSelection {
	public:
		enum {
			broken_mesh = -1,
			zombie_mesh = -2,
			insufficient_selection = -3,
			ambivalent_selection = -4,
		};
	};

	class errors_extractIntTabs {
	public:
		enum {
			null_val = -1,
			not_int_tab = -2,
			null_tab = -3,
		};
	};

	class I_Poly_isPoly {
	public:
		enum {
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_getSelection {
	public:
		enum {
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_setSelection {
	public:
		enum {
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_setSelection2 {
	public:
		enum {
			bad_vert = COUNTER_BASE - __COUNTER__,
			bad_edge = COUNTER_BASE - __COUNTER__,
			bad_face = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_copyVerts {
	public:
		enum {
			bad_bounds = COUNTER_BASE - __COUNTER__,
			// src poly
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			src_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			src_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			src_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			src_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// dst poly
			dst_null_node = COUNTER_BASE - __COUNTER__,
			dst_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			dst_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			dst_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			dst_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePolySrc(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return src_null_node;
			case Poly_init::null_base_object:
				return src_poly_null_base_object;
			case Poly_init::null_mesh:
				return src_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return src_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return src_poly_failed_cast;
			default:
				return src_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolvePolyDst(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return dst_null_node;
			case Poly_init::null_base_object:
				return dst_poly_null_base_object;
			case Poly_init::null_mesh:
				return dst_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return dst_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return dst_poly_failed_cast;
			default:
				return dst_poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_hasDead {
	public:
		enum {
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Poly_collapseDead {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			bad_num_points = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_deleteFaces {
	public:
		enum {
			// happens epfn fails
			null_node = COUNTER_BASE - __COUNTER__,
			bad_num_points = COUNTER_BASE - __COUNTER__,
			illegal_state = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_removeEdges {
	public:
		enum {
			bad_num_points = COUNTER_BASE - __COUNTER__,
			null_node = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_weldBorders {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			bad_num_points = COUNTER_BASE - __COUNTER__,
			too_many_runs = COUNTER_BASE - __COUNTER__,
			runs_mismatch = COUNTER_BASE - __COUNTER__,
			pair_mismatch = COUNTER_BASE - __COUNTER__,
			bad_single_run = COUNTER_BASE - __COUNTER__,
			dead_verts = COUNTER_BASE - __COUNTER__,
			forked_border = COUNTER_BASE - __COUNTER__,
			run_forms_loop = COUNTER_BASE - __COUNTER__,
			broken_mesh = COUNTER_BASE - __COUNTER__,
			no_borders = COUNTER_BASE - __COUNTER__,
			weld_failed_internally = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_attach {
	public:
		enum {
			dst_null_node = COUNTER_BASE - __COUNTER__,
			src_null_node = COUNTER_BASE - __COUNTER__,
			nothing_to_attach = COUNTER_BASE - __COUNTER__,
			different_num_skins = COUNTER_BASE - __COUNTER__,
			bad_num_points = COUNTER_BASE - __COUNTER__,
			clone_error = COUNTER_BASE - __COUNTER__,
			// src poly
			src_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			src_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			src_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			src_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			src_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// dst poly
			dst_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			dst_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			dst_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			dst_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// src skin
			src_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			src_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			src_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			src_skin_bad_index = COUNTER_BASE - __COUNTER__,
			src_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			// dst skin
			dst_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			dst_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			dst_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			dst_skin_bad_index = COUNTER_BASE - __COUNTER__,
			dst_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePolySrc(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return src_null_node;
			case Poly_init::null_base_object:
				return src_poly_null_base_object;
			case Poly_init::null_mesh:
				return src_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return src_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return src_poly_failed_cast;
			default:
				return src_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolvePolyDst(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return dst_null_node;
			case Poly_init::null_base_object:
				return dst_poly_null_base_object;
			case Poly_init::null_mesh:
				return dst_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return dst_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return dst_poly_failed_cast;
			default:
				return dst_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolveSkinSrc(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return src_null_node;
			case SkinMod_init::null_mod_context:
				return src_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return src_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return src_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return src_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return src_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return src_skin_bad_index;
			default:
				return src_skin_wrong_class_ID;
			}
		}

		static inline constexpr i32 resolveSkinDst(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return dst_null_node;
			case SkinMod_init::null_mod_context:
				return dst_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return dst_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return dst_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return dst_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return dst_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return dst_skin_bad_index;
			default:
				return dst_skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_detach {
	public:
		enum {
			clone_error = COUNTER_BASE - __COUNTER__,
			bad_num_points = COUNTER_BASE - __COUNTER__,
			null_node = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Poly_unsubdivide {
	public:
		enum {
			bad_selection = COUNTER_BASE - __COUNTER__,
			not_unsubdivisible = COUNTER_BASE - __COUNTER__,
			illegal_state_1 = COUNTER_BASE - __COUNTER__,
			illegal_state_2 = COUNTER_BASE - __COUNTER__,
			illegal_state_3 = COUNTER_BASE - __COUNTER__,
			illegal_state_4 = COUNTER_BASE - __COUNTER__,

			q_bad_selection = COUNTER_BASE - __COUNTER__,
			q_not_unsubdivisible = COUNTER_BASE - __COUNTER__,
			q_illegal_state_1 = COUNTER_BASE - __COUNTER__,
			q_illegal_state_2 = COUNTER_BASE - __COUNTER__,
			q_illegal_state_3 = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 qualify(const i32 err, const bool partial) {
			if (err < 0 && partial) {
				switch (err) {
				case bad_selection:
					return q_bad_selection;
				case not_unsubdivisible:
					return q_not_unsubdivisible;
				case illegal_state_1:
					return q_illegal_state_1;
				case illegal_state_2:
					return q_illegal_state_2;
				case illegal_state_3:
					return q_illegal_state_3;
				default:
					return illegal_state_4;
				}
			}
			return err;
		}

	};

	class I_Poly_detriangulate {
	public:
		enum {
			// mismatch => no counterpart, asymmetries, etc...
			mismatch = COUNTER_BASE - __COUNTER__,
			not_a_grid = COUNTER_BASE - __COUNTER__,
			odd_num_triangles = COUNTER_BASE - __COUNTER__,
			leftover_triangles = COUNTER_BASE - __COUNTER__,

			q_mismatch = COUNTER_BASE - __COUNTER__,
			q_not_a_grid = COUNTER_BASE - __COUNTER__,
			q_odd_num_triangles = COUNTER_BASE - __COUNTER__,
			q_leftover_triangles = COUNTER_BASE - __COUNTER__,

			illegal_state = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 qualify(const i32 err, const bool partial) {
			if (err < 0 && partial) {
				switch (err) {
				case mismatch:
					return q_mismatch;
				case not_a_grid:
					return q_not_a_grid;
				case odd_num_triangles:
					return q_odd_num_triangles;
				case leftover_triangles:
					return q_leftover_triangles;
				default:
					return illegal_state;
				}
			}
			return err;
		}
	};















	class I_Skin_skinIndex {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};



	class I_Skin_skinIndices {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_getBoneLimit {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_setBoneLimit {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_numBones {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_numPoints {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_collectUsedBones {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_stripWeights {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_stripBones {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_getSelection {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};


	class I_Skin_setSelection {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_setSelection2 {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			bad_handle = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_addBones {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Skin_copyWeights {
	public:
		enum {
			dst_null_node = COUNTER_BASE - __COUNTER__,
			src_null_node = COUNTER_BASE - __COUNTER__,
			different_num_points = COUNTER_BASE - __COUNTER__,
			bone_unmatched = COUNTER_BASE - __COUNTER__,

			// src skin
			src_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			src_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			src_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			src_skin_bad_index = COUNTER_BASE - __COUNTER__,
			src_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			// dst skin
			dst_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			dst_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			dst_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			dst_skin_bad_index = COUNTER_BASE - __COUNTER__,
			dst_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolveSkinSrc(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return src_null_node;
			case SkinMod_init::null_mod_context:
				return src_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return src_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return src_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return src_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return src_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return src_skin_bad_index;
			default:
				return src_skin_wrong_class_ID;
			}
		}

		static inline constexpr i32 resolveSkinDst(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return dst_null_node;
			case SkinMod_init::null_mod_context:
				return dst_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return dst_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return dst_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return dst_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return dst_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return dst_skin_bad_index;
			default:
				return dst_skin_wrong_class_ID;
			}
		}
	};

	class I_Order_blendMirrorPos {
	public:
		enum {
			different_lengths = COUNTER_BASE - __COUNTER__,
			bad_vert_m = COUNTER_BASE - __COUNTER__,
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_flipMirrorPos {
	public:
		enum {
			different_lengths = COUNTER_BASE - __COUNTER__,
			bad_vert_m = COUNTER_BASE - __COUNTER__,
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_copyPos {
	public:
		enum {
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			bad_selection = COUNTER_BASE - __COUNTER__,
			// src poly
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			src_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			src_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			src_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			src_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// dst poly
			dst_null_node = COUNTER_BASE - __COUNTER__,
			dst_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			dst_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			dst_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			dst_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePolySrc(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return src_null_node;
			case Poly_init::null_base_object:
				return src_poly_null_base_object;
			case Poly_init::null_mesh:
				return src_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return src_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return src_poly_failed_cast;
			default:
				return src_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolvePolyDst(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return dst_null_node;
			case Poly_init::null_base_object:
				return dst_poly_null_base_object;
			case Poly_init::null_mesh:
				return dst_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return dst_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return dst_poly_failed_cast;
			default:
				return dst_poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_blendPos {
	public:
		enum {
			different_lengths = COUNTER_BASE - __COUNTER__,
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			// src poly
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			src_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			src_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			src_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			src_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// dst poly
			dst_null_node = COUNTER_BASE - __COUNTER__,
			dst_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			dst_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			dst_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			dst_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePolySrc(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return src_null_node;
			case Poly_init::null_base_object:
				return src_poly_null_base_object;
			case Poly_init::null_mesh:
				return src_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return src_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return src_poly_failed_cast;
			default:
				return src_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolvePolyDst(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return dst_null_node;
			case Poly_init::null_base_object:
				return dst_poly_null_base_object;
			case Poly_init::null_mesh:
				return dst_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return dst_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return dst_poly_failed_cast;
			default:
				return dst_poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_extractSymmetrySelection {
	public:
		enum {
			broken_mesh = COUNTER_BASE - __COUNTER__,
			zombie_mesh = COUNTER_BASE - __COUNTER__,
			insufficient_selection = COUNTER_BASE - __COUNTER__,
			ambivalent_selection = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolveExtra(const i32 err) {
			switch (err) {
			case errors_extractOrderSelection::broken_mesh:
				return broken_mesh;
			case errors_extractOrderSelection::zombie_mesh:
				return zombie_mesh;
			case errors_extractOrderSelection::insufficient_selection:
				return insufficient_selection;
			default:
				return ambivalent_selection;
			}
		}
	};

	class I_Order_extractTraversalSelection {
	public:
		enum {
			broken_mesh = COUNTER_BASE - __COUNTER__,
			zombie_mesh = COUNTER_BASE - __COUNTER__,
			insufficient_selection = COUNTER_BASE - __COUNTER__,
			ambivalent_selection = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolveExtra(const i32 err) {
			switch (err) {
			case errors_extractOrderSelection::broken_mesh:
				return broken_mesh;
			case errors_extractOrderSelection::zombie_mesh:
				return zombie_mesh;
			case errors_extractOrderSelection::insufficient_selection:
				return insufficient_selection;
			default:
				return ambivalent_selection;
			}
		}
	};

	class I_Order_buildSymmetryOrder {
	public:
		enum {
			bad_selection = COUNTER_BASE - __COUNTER__,
			order_mismatch = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_buildTraversalOrder {
	public:
		enum {
			bad_selection = COUNTER_BASE - __COUNTER__,
			// poly
			null_node = COUNTER_BASE - __COUNTER__,
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_blendMirrorWeights {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			a_affix_empty = COUNTER_BASE - __COUNTER__,
			b_affix_empty = COUNTER_BASE - __COUNTER__,
			duplicate_bone_name = COUNTER_BASE - __COUNTER__,
			bad_handle_m = COUNTER_BASE - __COUNTER__,
			bad_handle_a = COUNTER_BASE - __COUNTER__,
			bad_handle_b = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Order_flipMirrorWeights {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			a_affix_empty = COUNTER_BASE - __COUNTER__,
			b_affix_empty = COUNTER_BASE - __COUNTER__,
			bad_vert_m = COUNTER_BASE - __COUNTER__,
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			duplicate_bone_name = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePoly(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
		static inline constexpr i32 resolveSkin(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Order_translateSelection {
	public:
		enum {
			a_bad_vert = COUNTER_BASE - __COUNTER__,
			a_bad_edge = COUNTER_BASE - __COUNTER__,
			a_bad_face = COUNTER_BASE - __COUNTER__,
			b_bad_vert = COUNTER_BASE - __COUNTER__,
			b_bad_edge = COUNTER_BASE - __COUNTER__,
			b_bad_face = COUNTER_BASE - __COUNTER__,
			// src poly
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			src_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			src_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			src_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			src_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
			// dst poly
			dst_null_node = COUNTER_BASE - __COUNTER__,
			dst_poly_null_base_object = COUNTER_BASE - __COUNTER__,
			dst_poly_null_mesh = COUNTER_BASE - __COUNTER__,
			dst_poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			dst_poly_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolvePolySrc(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return src_null_node;
			case Poly_init::null_base_object:
				return src_poly_null_base_object;
			case Poly_init::null_mesh:
				return src_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return src_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return src_poly_failed_cast;
			default:
				return src_poly_mesh_not_filled_in;
			}
		}

		static inline constexpr i32 resolvePolyDst(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return dst_null_node;
			case Poly_init::null_base_object:
				return dst_poly_null_base_object;
			case Poly_init::null_mesh:
				return dst_poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return dst_poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return dst_poly_failed_cast;
			default:
				return dst_poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_translateHandles {
	public:
		enum {
			different_lengths = COUNTER_BASE - __COUNTER__,
			a_bad_handle = COUNTER_BASE - __COUNTER__,
			b_bad_handle = COUNTER_BASE - __COUNTER__,
			// src skin
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			src_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			src_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			src_skin_bad_index = COUNTER_BASE - __COUNTER__,
			src_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			// dst skin
			dst_null_node = COUNTER_BASE - __COUNTER__,
			dst_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			dst_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			dst_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			dst_skin_bad_index = COUNTER_BASE - __COUNTER__,
			dst_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolveSkinSrc(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return src_null_node;
			case SkinMod_init::null_mod_context:
				return src_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return src_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return src_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return src_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return src_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return src_skin_bad_index;
			default:
				return src_skin_wrong_class_ID;
			}
		}

		static inline constexpr i32 resolveSkinDst(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return dst_null_node;
			case SkinMod_init::null_mod_context:
				return dst_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return dst_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return dst_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return dst_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return dst_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return dst_skin_bad_index;
			default:
				return dst_skin_wrong_class_ID;
			}
		}
	};

	class I_Order_mirrorSelection {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			different_lengths = COUNTER_BASE - __COUNTER__,
			bad_vert_m = COUNTER_BASE - __COUNTER__,
			bad_vert_a = COUNTER_BASE - __COUNTER__,
			bad_vert_b = COUNTER_BASE - __COUNTER__,
			bad_edge_m = COUNTER_BASE - __COUNTER__,
			bad_edge_a = COUNTER_BASE - __COUNTER__,
			bad_edge_b = COUNTER_BASE - __COUNTER__,
			bad_face_m = COUNTER_BASE - __COUNTER__,
			bad_face_a = COUNTER_BASE - __COUNTER__,
			bad_face_b = COUNTER_BASE - __COUNTER__,
			// poly
			poly_null_base_object = COUNTER_BASE - __COUNTER__,
			poly_null_mesh = COUNTER_BASE - __COUNTER__,
			poly_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			poly_failed_cast = COUNTER_BASE - __COUNTER__,
			poly_mesh_not_filled_in = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case Poly_init::null_node:
				return null_node;
			case Poly_init::null_base_object:
				return poly_null_base_object;
			case Poly_init::null_mesh:
				return poly_null_mesh;
			case Poly_init::wrong_class_ID:
				return poly_wrong_class_ID;
			case Poly_init::failed_cast:
				return poly_failed_cast;
			default:
				return poly_mesh_not_filled_in;
			}
		}
	};

	class I_Order_mirrorHandles {
	public:
		enum {
			null_node = COUNTER_BASE - __COUNTER__,
			different_lengths = COUNTER_BASE - __COUNTER__,
			bad_handle_m = COUNTER_BASE - __COUNTER__,
			bad_handle_a = COUNTER_BASE - __COUNTER__,
			bad_handle_b = COUNTER_BASE - __COUNTER__,
			// skin
			skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			skin_null_context_data = COUNTER_BASE - __COUNTER__,
			skin_null_import_data = COUNTER_BASE - __COUNTER__,
			skin_failed_cast = COUNTER_BASE - __COUNTER__,
			skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			skin_bad_index = COUNTER_BASE - __COUNTER__,
			skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return null_node;
			case SkinMod_init::null_mod_context:
				return skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return skin_null_context_data;
			case SkinMod_init::null_import_data:
				return skin_null_import_data;
			case SkinMod_init::failed_cast:
				return skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return skin_bad_index;
			default:
				return skin_wrong_class_ID;
			}
		}
	};

	class I_Order_transferWeights {
	public:
		enum {
			dst_null_node = COUNTER_BASE - __COUNTER__,
			src_null_node = COUNTER_BASE - __COUNTER__,
			src_affix_empty = COUNTER_BASE - __COUNTER__,
			dst_affix_empty = COUNTER_BASE - __COUNTER__,
			skin_mangled = COUNTER_BASE - __COUNTER__,
			src_bad_handle = COUNTER_BASE - __COUNTER__,
			dst_bad_handle = COUNTER_BASE - __COUNTER__,
			src_duplicate_bone_name = COUNTER_BASE - __COUNTER__,
			dst_duplicate_bone_name = COUNTER_BASE - __COUNTER__,
			unmatched_bone = COUNTER_BASE - __COUNTER__,
			// src skin
			src_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			src_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			src_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			src_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			src_skin_bad_index = COUNTER_BASE - __COUNTER__,
			src_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
			// dst skin
			dst_skin_null_mod_context = COUNTER_BASE - __COUNTER__,
			dst_skin_null_context_data = COUNTER_BASE - __COUNTER__,
			dst_skin_null_import_data = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast = COUNTER_BASE - __COUNTER__,
			dst_skin_failed_cast_2 = COUNTER_BASE - __COUNTER__,
			dst_skin_bad_index = COUNTER_BASE - __COUNTER__,
			dst_skin_wrong_class_ID = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolveSkinSrc(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return src_null_node;
			case SkinMod_init::null_mod_context:
				return src_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return src_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return src_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return src_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return src_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return src_skin_bad_index;
			default:
				return src_skin_wrong_class_ID;
			}
		}

		static inline constexpr i32 resolveSkinDst(const i32 err) {
			switch (err) {
			case SkinMod_init::null_node:
				return dst_null_node;
			case SkinMod_init::null_mod_context:
				return dst_skin_null_mod_context;
			case SkinMod_init::null_context_data:
				return dst_skin_null_context_data;
			case SkinMod_init::null_import_data:
				return dst_skin_null_import_data;
			case SkinMod_init::failed_cast:
				return dst_skin_failed_cast;
			case SkinMod_init::failed_cast_2:
				return dst_skin_failed_cast_2;
			case SkinMod_init::bad_index:
				return dst_skin_bad_index;
			default:
				return dst_skin_wrong_class_ID;
			}
		}
	};






	//class I_Misc_compressIntArray1 {
	//public:
	//	enum {
	//		dst_overfull = COUNTER_BASE - __COUNTER__,
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return dst_overfull;
	//		}
	//	}
	//};

	//class I_Misc_compressIntArray2 {
	//public:
	//	enum {
	//		dst_overfull = COUNTER_BASE - __COUNTER__,
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return dst_overfull;
	//		}
	//	}
	//};

	//class I_Misc_compressIntArray4 {
	//public:
	//	enum {
	//		dst_overfull = COUNTER_BASE - __COUNTER__,
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return dst_overfull;
	//		}
	//	}
	//};

	//class I_Misc_compressIntArray8 {
	//public:
	//	enum {
	//		dst_overfull = COUNTER_BASE - __COUNTER__,
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return dst_overfull;
	//		}
	//	}
	//};


	class I_Misc_compressIntArray {
	public:
		enum {
			dst_overfull = COUNTER_BASE - __COUNTER__,
			// DiffPFOR
			srcLen_negative = COUNTER_BASE - __COUNTER__,
			srcLen_too_large = COUNTER_BASE - __COUNTER__,
			srcLen_too_small = COUNTER_BASE - __COUNTER__,
			srcOff_negative = COUNTER_BASE - __COUNTER__,
			srcOff_too_large = COUNTER_BASE - __COUNTER__,
			dstOff_negative = COUNTER_BASE - __COUNTER__,
			dstOff_too_large = COUNTER_BASE - __COUNTER__,
			dstLen_negative = COUNTER_BASE - __COUNTER__,
			dstLen_too_small = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case DiffPFOR::Errors::srcLen_negative:
				return srcLen_negative;
			case DiffPFOR::Errors::srcLen_too_large:
				return srcLen_too_large;
			case DiffPFOR::Errors::srcLen_too_small:
				return srcLen_too_small;
			case DiffPFOR::Errors::srcOff_negative:
				return srcOff_negative;
			case DiffPFOR::Errors::srcOff_too_large:
				return srcOff_too_large;
			case DiffPFOR::Errors::dstOff_negative:
				return dstOff_negative;
			case DiffPFOR::Errors::dstOff_too_large:
				return dstOff_too_large;
			case DiffPFOR::Errors::dstLen_negative:
				return dstLen_negative;
			case DiffPFOR::Errors::dstLen_too_small:
				return dstLen_too_small;
			default:
				return dst_overfull;
			}
		}
	};

	class I_Misc_compressIntArrays {
	public:
		enum {
			dst_overfull = COUNTER_BASE - __COUNTER__,
			// extract tabs from value
			null_val = COUNTER_BASE - __COUNTER__,
			not_int_tab = COUNTER_BASE - __COUNTER__,
			null_tab = COUNTER_BASE - __COUNTER__,
			// DiffPFOR
			srcLen_negative = COUNTER_BASE - __COUNTER__,
			srcLen_too_large = COUNTER_BASE - __COUNTER__,
			srcLen_too_small = COUNTER_BASE - __COUNTER__,
			srcOff_negative = COUNTER_BASE - __COUNTER__,
			srcOff_too_large = COUNTER_BASE - __COUNTER__,
			dstOff_negative = COUNTER_BASE - __COUNTER__,
			dstOff_too_large = COUNTER_BASE - __COUNTER__,
			dstLen_negative = COUNTER_BASE - __COUNTER__,
			dstLen_too_small = COUNTER_BASE - __COUNTER__,
		};

		static inline constexpr i32 resolveTabs(const i32 err) {
			switch (err) {
			case errors_extractIntTabs::null_val:
				return null_val;
			case errors_extractIntTabs::not_int_tab:
				return not_int_tab;
			default:
				return null_tab;
			}
		}
		static inline constexpr i32 resolveComp(const i32 err) {
			switch (err) {
			case DiffPFOR::Errors::srcLen_negative:
				return srcLen_negative;
			case DiffPFOR::Errors::srcLen_too_large:
				return srcLen_too_large;
			case DiffPFOR::Errors::srcLen_too_small:
				return srcLen_too_small;
			case DiffPFOR::Errors::srcOff_negative:
				return srcOff_negative;
			case DiffPFOR::Errors::srcOff_too_large:
				return srcOff_too_large;
			case DiffPFOR::Errors::dstOff_negative:
				return dstOff_negative;
			case DiffPFOR::Errors::dstOff_too_large:
				return dstOff_too_large;
			case DiffPFOR::Errors::dstLen_negative:
				return dstLen_negative;
			case DiffPFOR::Errors::dstLen_too_small:
				return dstLen_too_small;
			default:
				return dst_overfull;
			}
		}
	};


	//class I_Misc_decompressIntArray1 {
	//public:
	//	enum {
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//		stream_corrupt = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return stream_corrupt;
	//		}
	//	}
	//};

	//class I_Misc_decompressIntArray2 {
	//public:
	//	enum {
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//		stream_corrupt = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return stream_corrupt;
	//		}
	//	}
	//};

	//class I_Misc_decompressIntArray4 {
	//public:
	//	enum {
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//		stream_corrupt = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return stream_corrupt;
	//		}
	//	}
	//};

	//class I_Misc_decompressIntArray8 {
	//public:
	//	enum {
	//		// DiffPFOR
	//		srcLen_negative = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_large = COUNTER_BASE - __COUNTER__,
	//		srcLen_too_small = COUNTER_BASE - __COUNTER__,
	//		srcOff_negative = COUNTER_BASE - __COUNTER__,
	//		srcOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstOff_negative = COUNTER_BASE - __COUNTER__,
	//		dstOff_too_large = COUNTER_BASE - __COUNTER__,
	//		dstLen_negative = COUNTER_BASE - __COUNTER__,
	//		dstLen_too_small = COUNTER_BASE - __COUNTER__,
	//		stream_corrupt = COUNTER_BASE - __COUNTER__,
	//	};
	//	static inline constexpr i32 resolve(const i32 err) {
	//		switch (err) {
	//		case DiffPFOR::Errors::srcLen_negative:
	//			return srcLen_negative;
	//		case DiffPFOR::Errors::srcLen_too_large:
	//			return srcLen_too_large;
	//		case DiffPFOR::Errors::srcLen_too_small:
	//			return srcLen_too_small;
	//		case DiffPFOR::Errors::srcOff_negative:
	//			return srcOff_negative;
	//		case DiffPFOR::Errors::srcOff_too_large:
	//			return srcOff_too_large;
	//		case DiffPFOR::Errors::dstOff_negative:
	//			return dstOff_negative;
	//		case DiffPFOR::Errors::dstOff_too_large:
	//			return dstOff_too_large;
	//		case DiffPFOR::Errors::dstLen_negative:
	//			return dstLen_negative;
	//		case DiffPFOR::Errors::dstLen_too_small:
	//			return dstLen_too_small;
	//		default:
	//			return stream_corrupt;
	//		}
	//	}
	//};

	class I_Misc_decompressIntArray {
	public:
		enum {
			// DiffPFOR
			srcLen_negative = COUNTER_BASE - __COUNTER__,
			srcLen_too_large = COUNTER_BASE - __COUNTER__,
			srcLen_too_small = COUNTER_BASE - __COUNTER__,
			srcOff_negative = COUNTER_BASE - __COUNTER__,
			srcOff_too_large = COUNTER_BASE - __COUNTER__,
			dstOff_negative = COUNTER_BASE - __COUNTER__,
			dstOff_too_large = COUNTER_BASE - __COUNTER__,
			dstLen_negative = COUNTER_BASE - __COUNTER__,
			dstLen_too_small = COUNTER_BASE - __COUNTER__,
			stream_corrupt = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolve(const i32 err) {
			switch (err) {
			case DiffPFOR::Errors::srcLen_negative:
				return srcLen_negative;
			case DiffPFOR::Errors::srcLen_too_large:
				return srcLen_too_large;
			case DiffPFOR::Errors::srcLen_too_small:
				return srcLen_too_small;
			case DiffPFOR::Errors::srcOff_negative:
				return srcOff_negative;
			case DiffPFOR::Errors::srcOff_too_large:
				return srcOff_too_large;
			case DiffPFOR::Errors::dstOff_negative:
				return dstOff_negative;
			case DiffPFOR::Errors::dstOff_too_large:
				return dstOff_too_large;
			case DiffPFOR::Errors::dstLen_negative:
				return dstLen_negative;
			case DiffPFOR::Errors::dstLen_too_small:
				return dstLen_too_small;
			default:
				return stream_corrupt;
			}
		}
	};

	class I_Misc_decompressIntArrays {
	public:
		enum {
			// extract tabs from value
			null_val = COUNTER_BASE - __COUNTER__,
			not_int_tab = COUNTER_BASE - __COUNTER__,
			null_tab = COUNTER_BASE - __COUNTER__,
			// DiffPFOR
			srcLen_negative = COUNTER_BASE - __COUNTER__,
			srcLen_too_large = COUNTER_BASE - __COUNTER__,
			srcLen_too_small = COUNTER_BASE - __COUNTER__,
			srcOff_negative = COUNTER_BASE - __COUNTER__,
			srcOff_too_large = COUNTER_BASE - __COUNTER__,
			dstOff_negative = COUNTER_BASE - __COUNTER__,
			dstOff_too_large = COUNTER_BASE - __COUNTER__,
			dstLen_negative = COUNTER_BASE - __COUNTER__,
			dstLen_too_small = COUNTER_BASE - __COUNTER__,
			stream_corrupt = COUNTER_BASE - __COUNTER__,
		};
		static inline constexpr i32 resolveTabs(const i32 err) {
			switch (err) {
			case errors_extractIntTabs::null_val:
				return null_val;
			case errors_extractIntTabs::not_int_tab:
				return not_int_tab;
			default:
				return null_tab;
			}
		}
		static inline constexpr i32 resolveComp(const i32 err) {
			switch (err) {
			case DiffPFOR::Errors::srcLen_negative:
				return srcLen_negative;
			case DiffPFOR::Errors::srcLen_too_large:
				return srcLen_too_large;
			case DiffPFOR::Errors::srcLen_too_small:
				return srcLen_too_small;
			case DiffPFOR::Errors::srcOff_negative:
				return srcOff_negative;
			case DiffPFOR::Errors::srcOff_too_large:
				return srcOff_too_large;
			case DiffPFOR::Errors::dstOff_negative:
				return dstOff_negative;
			case DiffPFOR::Errors::dstOff_too_large:
				return dstOff_too_large;
			case DiffPFOR::Errors::dstLen_negative:
				return dstLen_negative;
			case DiffPFOR::Errors::dstLen_too_small:
				return dstLen_too_small;
			default:
				return stream_corrupt;
			}
		}
	};


	class I_Misc_buildSwapSequence {
	public:
		enum {
			bad_bounds = COUNTER_BASE - __COUNTER__,
			too_large = COUNTER_BASE - __COUNTER__,
		};
	};

	static inline constexpr const wchar_t* getMessage(const i32 id) {
		switch (id) {
		case OK:
			return L"Ok";
			{
				using ERR = I_Poly_isPoly;
		case ERR::null_node:
			return L"Error {Poly.isPoly()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.isPoly()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.isPoly()}:\n::[no mesh]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.isPoly()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.isPoly()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_getSelection;
		case ERR::null_node:
			return L"Error {Poly.getSelection()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.getSelection()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.getSelection()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.getSelection()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.getSelection()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.getSelection()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_setSelection;
		case ERR::null_node:
			return L"Error {Poly.setSelection()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.setSelection()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.setSelection()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.setSelection()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.setSelection()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.setSelection()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_setSelection2;
		case ERR::bad_vert:
			return L"Error {Poly.setSelection2()}:\n::[out-of-bounds vert]";
		case ERR::bad_edge:
			return L"Error {Poly.setSelection2()}:\n::[out-of-bounds edge]";
		case ERR::bad_face:
			return L"Error {Poly.setSelection2()}:\n::[out-of-bounds face]";
		case ERR::null_node:
			return L"Error {Poly.setSelection2()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.setSelection2()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.setSelection2()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.setSelection2()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.setSelection2()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.setSelection2()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_copyVerts;
		case ERR::bad_bounds:
			return L"Error {Poly.copyVerts()}:\n::[input bounds invalid]";
		case ERR::src_null_node:
			return L"Error {Poly.copyVerts()}:\n::[src node is null]";
		case ERR::src_poly_null_base_object:
			return L"Error {Poly.copyVerts()}:\n::[src has no base object]";
		case ERR::src_poly_null_mesh:
			return L"Error {Poly.copyVerts()}:\n::[src has no mesh]";
		case ERR::src_poly_wrong_class_ID:
			return L"Error {Poly.copyVerts()}:\n::[src is not poly]";
		case ERR::src_poly_failed_cast:
			return L"Error {Poly.copyVerts()}:\n::[src poly-cast failed]";
		case ERR::src_poly_mesh_not_filled_in:
			return L"Error {Poly.copyVerts()}:\n::[src mesh not filled in]";
		case ERR::dst_null_node:
			return L"Error {Poly.copyVerts()}:\n::[dst node is null]";
		case ERR::dst_poly_null_base_object:
			return L"Error {Poly.copyVerts()}:\n::[dst has no base object]";
		case ERR::dst_poly_null_mesh:
			return L"Error {Poly.copyVerts()}:\n::[dst has no mesh]";
		case ERR::dst_poly_wrong_class_ID:
			return L"Error {Poly.copyVerts()}:\n::[dst is not poly]";
		case ERR::dst_poly_failed_cast:
			return L"Error {Poly.copyVerts()}:\n::[dst poly-cast failed]";
		case ERR::dst_poly_mesh_not_filled_in:
			return L"Error {Poly.copyVerts()}:\n::[dst mesh not filled in]";
			}
			{
				using ERR = I_Poly_hasDead;
		case ERR::null_node:
			return L"Error {Poly.hasDead()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.hasDead()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.hasDead()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.hasDead()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.hasDead()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.hasDead()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_collapseDead;
		case ERR::null_node:
			return L"Error {Poly.collapseDead()}:\n::[node is null]";
		case ERR::bad_num_points:
			return L"Error {Poly.collapseDead()}:\n::[skin has wrong point count]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.collapseDead()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.collapseDead()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.collapseDead()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.collapseDead()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.collapseDead()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Poly.collapseDead()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Poly.collapseDead()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Poly.collapseDead()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Poly.collapseDead()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Poly.collapseDead()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Poly.collapseDead()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Poly.collapseDead()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Poly_deleteFaces;
		case ERR::null_node:
			return L"Error {Poly.deleteFaces()}:\n::[node is null]";
		case ERR::bad_num_points:
			return L"Error {Poly.deleteFaces()}:\n::[skin has wrong point count]";
		case ERR::illegal_state:
			return L"Error {Poly.deleteFaces()}:\n::[illegal state 1. contact author.]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.deleteFaces()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.deleteFaces()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.deleteFaces()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.deleteFaces()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.deleteFaces()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Poly.deleteFaces()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Poly.deleteFaces()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Poly.deleteFaces()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Poly.deleteFaces()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Poly.deleteFaces()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Poly.deleteFaces()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Poly.deleteFaces()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Poly_removeEdges;
		case ERR::bad_num_points:
			return L"Error {Poly.removeEdges()}:\n::[skin has wrong point count]";
		case ERR::null_node:
			return L"Error {Poly.removeEdges()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.removeEdges()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.removeEdges()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.removeEdges()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.removeEdges()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.removeEdges()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Poly.removeEdges()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Poly.removeEdges()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Poly.removeEdges()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Poly.removeEdges()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Poly.removeEdges()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Poly.removeEdges()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Poly.removeEdges()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Poly_weldBorders;
		case ERR::null_node:
			return L"Error {Poly.weldBorders()}:\n::[node is null]";
		case ERR::bad_num_points:
			return L"Error {Poly.weldBorders()}:\n::[skin has wrong point count]";
		case ERR::too_many_runs:
			return L"Error {Poly.weldBorders()}:\n::[too many selected runs]";
		case ERR::runs_mismatch:
			return L"Error {Poly.weldBorders()}:\n::[runs differ in length]";
		case ERR::pair_mismatch:
			return L"Error {Poly.weldBorders()}:\n::[boder vert selection malformed]";
		case ERR::bad_single_run:
			return L"Error {Poly.weldBorders()}:\n::[single run selection malformed]";
		case ERR::dead_verts:
			return L"Error {Poly.weldBorders()}:\n::[run contains dead verts]";
		case ERR::forked_border:
			return L"Error {Poly.weldBorders()}:\n::[border is forked]";
		case ERR::run_forms_loop:
			return L"Error {Poly.weldBorders()}:\n::[run forms loop]";
		case ERR::broken_mesh:
			return L"Error {Poly.weldBorders()}:\n::[violated mesh invariants]";
		case ERR::no_borders:
			return L"Error {Poly.weldBorders()}:\n::[no borders found]";
		case ERR::weld_failed_internally:
			return L"Error {Poly.weldBorders()}:\n::[welding faild internally.]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.weldBorders()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.weldBorders()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.weldBorders()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.weldBorders()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.weldBorders()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Poly.weldBorders()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Poly.weldBorders()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Poly.weldBorders()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Poly.weldBorders()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Poly.weldBorders()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Poly.weldBorders()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Poly.weldBorders()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Poly_attach;
		case ERR::dst_null_node:
			return L"Error {Poly.attach()}:\n::[dst node is null]";
		case ERR::src_null_node:
			return L"Error {Poly.attach()}:\n::[src node is null]";
		case ERR::nothing_to_attach:
			return L"Error {Poly.attach()}:\n::[nothing to attach]";
		case ERR::different_num_skins:
			return L"Error {Poly.attach()}:\n::[nodes need same # of skin mods]";
		case ERR::bad_num_points:
			return L"Error {Poly.attach()}:\n::[skin has wrong point count]";
		case ERR::clone_error:
			return L"Error {Poly.attach()}:\n::[cloning error]";
		case ERR::src_poly_null_base_object:
			return L"Error {Poly.attach()}:\n::[src has no base object]";
		case ERR::src_poly_null_mesh:
			return L"Error {Poly.attach()}:\n::[src has no mesh]";
		case ERR::src_poly_wrong_class_ID:
			return L"Error {Poly.attach()}:\n::[src is not poly]";
		case ERR::src_poly_failed_cast:
			return L"Error {Poly.attach()}:\n::[src poly-cast failed]";
		case ERR::src_poly_mesh_not_filled_in:
			return L"Error {Poly.attach()}:\n::[src mesh not filled in]";
		case ERR::dst_poly_null_base_object:
			return L"Error {Poly.attach()}:\n::[dst has no base object]";
		case ERR::dst_poly_null_mesh:
			return L"Error {Poly.attach()}:\n::[dst has no mesh]";
		case ERR::dst_poly_wrong_class_ID:
			return L"Error {Poly.attach()}:\n::[dst is not poly]";
		case ERR::dst_poly_failed_cast:
			return L"Error {Poly.attach()}:\n::[dst poly-cast failed]";
		case ERR::dst_poly_mesh_not_filled_in:
			return L"Error {Poly.attach()}:\n::[dst mesh not filled in]";
		case ERR::src_skin_null_mod_context:
			return L"Error {Poly.attach()}:\n::[src mod has no context]";
		case ERR::src_skin_null_context_data:
			return L"Error {Poly.attach()}:\n::[src skin has no context data]";
		case ERR::src_skin_null_import_data:
			return L"Error {Poly.attach()}:\n::[src skin has no import data]";
		case ERR::src_skin_failed_cast:
			return L"Error {Poly.attach()}:\n::[src skin-cast failed]";
		case ERR::src_skin_failed_cast_2:
			return L"Error {Poly.attach()}:\n::[src skin2-cast failed]";
		case ERR::src_skin_bad_index:
			return L"Error {Poly.attach()}:\n::[src has no skin at index]";
		case ERR::src_skin_wrong_class_ID:
			return L"Error {Poly.attach()}:\n::[src mod is not skin]";
		case ERR::dst_skin_null_mod_context:
			return L"Error {Poly.attach()}:\n::[dst mod has no context]";
		case ERR::dst_skin_null_context_data:
			return L"Error {Poly.attach()}:\n::[dst skin has no context data]";
		case ERR::dst_skin_null_import_data:
			return L"Error {Poly.attach()}:\n::[dst skin has no import data]";
		case ERR::dst_skin_failed_cast:
			return L"Error {Poly.attach()}:\n::[dst skin-cast failed]";
		case ERR::dst_skin_failed_cast_2:
			return L"Error {Poly.attach()}:\n::[dst skin2-cast failed]";
		case ERR::dst_skin_bad_index:
			return L"Error {Poly.attach()}:\n::[dst has no skin at index]";
		case ERR::dst_skin_wrong_class_ID:
			return L"Error {Poly.attach()}:\n::[dst mod is not skin]";
			}
			{
				using ERR = I_Poly_detach;
		case ERR::clone_error:
			return L"Error {Poly.detach()}:\n::[cloning error]";
		case ERR::bad_num_points:
			return L"Error {Poly.detach()}:\n::[skin has wrong point count]";
		case ERR::null_node:
			return L"Error {Poly.detach()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.detach()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.detach()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.detach()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.detach()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.detach()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Poly.detach()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Poly.detach()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Poly.detach()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Poly.detach()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Poly.detach()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Poly.detach()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Poly.detach()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Poly_unsubdivide;
		case ERR::bad_selection:
			return L"Error {Poly.unsubdivide()}:\n::[bad selection]";
		case ERR::not_unsubdivisible:
			return L"Error {Poly.unsubdivide()}:\n::[patch not unsubdivisible]";
		case ERR::illegal_state_1:
			return L"Error {Poly.unsubdivide()}:\n::[illegal state 1. contact author.]";
		case ERR::illegal_state_2:
			return L"Error {Poly.unsubdivide()}:\n::[illegal state 2. contact author.]";
		case ERR::illegal_state_3:
			return L"Error {Poly.unsubdivide()}:\n::[illegal state 3. contact author.]";
		case ERR::illegal_state_4:
			return L"Error {Poly.unsubdivide()}:\n::[illegal state 4. contact author.]";
		case ERR::q_bad_selection:
			return L"Error {Poly.unsubdivide()}:\n::[partial success]\n::[bad selection]";
		case ERR::q_not_unsubdivisible:
			return L"Error {Poly.unsubdivide()}:\n::[partial success]\n::[patch not unsubdivisible]";
		case ERR::q_illegal_state_1:
			return L"Error {Poly.unsubdivide()}:\n::[partial success]\n::[illegal state 1. contact author.]";
		case ERR::q_illegal_state_2:
			return L"Error {Poly.unsubdivide()}:\n::[partial success]\n::[illegal state 2. contact author.]";
		case ERR::q_illegal_state_3:
			return L"Error {Poly.unsubdivide()}:\n::[partial success]\n::[illegal state 3. contact author.]";
		case ERR::null_node:
			return L"Error {Poly.unsubdivide()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.unsubdivide()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.unsubdivide()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.unsubdivide()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.unsubdivide()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.unsubdivide()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Poly_detriangulate;
		case ERR::mismatch:
			// mismatch => no counterpart, asymmetries, etc...
			return L"Error {Poly.detriangulate()}:\n::[detriangulation impossible]";
		case ERR::not_a_grid:
			return L"Error {Poly.detriangulate()}:\n::[selection not grid-shaped]";
		case ERR::odd_num_triangles:
			return L"Error {Poly.detriangulate()}:\n::[odd triangle count]";
		case ERR::leftover_triangles:
			return L"Error {Poly.detriangulate()}:\n::[triangles left over]";
		case ERR::q_mismatch:
			return L"Error {Poly.detriangulate()}:\n::[partial success]\n::[detriangulation impossible]";
		case ERR::q_not_a_grid:
			return L"Error {Poly.detriangulate()}:\n::[partial success]\n::[selection not grid-shaped]";
		case ERR::q_odd_num_triangles:
			return L"Error {Poly.detriangulate()}:\n::[partial success]\n::[odd triangle count]";
		case ERR::q_leftover_triangles:
			return L"Error {Poly.detriangulate()}:\n::[partial success]\n::[triangles left over]";
		case ERR::illegal_state:
			return L"Error {Poly.detriangulate()}:\n::[illegal state 1. contact author.]";
		case ERR::null_node:
			return L"Error {Poly.detriangulate()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Poly.detriangulate()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Poly.detriangulate()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Poly.detriangulate()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Poly.detriangulate()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Poly.detriangulate()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Skin_skinIndex;
		case ERR::null_node:
			return L"Error {Skin.skinIndex()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.skinIndex()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.skinIndex()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.skinIndex()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.skinIndex()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.skinIndex()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.skinIndex()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.skinIndex()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_skinIndices;
		case ERR::null_node:
			return L"Error {Skin.skinIndices()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.skinIndices()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.skinIndices()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.skinIndices()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.skinIndices()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.skinIndices()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.skinIndices()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.skinIndices()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_getBoneLimit;
		case ERR::null_node:
			return L"Error {Skin.getBoneLimit()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.getBoneLimit()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.getBoneLimit()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.getBoneLimit()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.getBoneLimit()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.getBoneLimit()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.getBoneLimit()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.getBoneLimit()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_setBoneLimit;
		case ERR::null_node:
			return L"Error {Skin.setBoneLimit()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.setBoneLimit()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.setBoneLimit()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.setBoneLimit()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.setBoneLimit()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.setBoneLimit()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.setBoneLimit()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.setBoneLimit()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_numBones;
		case ERR::null_node:
			return L"Error {Skin.numBones()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.numBones()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.numBones()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.numBones()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.numBones()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.numBones()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.numBones()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.numBones()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_numPoints;
		case ERR::null_node:
			return L"Error {Skin.numPoints()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.numPoints()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.numPoints()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.numPoints()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.numPoints()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.numPoints()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.numPoints()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.numPoints()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_collectUsedBones;
		case ERR::null_node:
			return L"Error {Skin.collectUsedBones()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.collectUsedBones()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.collectUsedBones()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.collectUsedBones()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.collectUsedBones()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.collectUsedBones()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.collectUsedBones()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.collectUsedBones()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_stripWeights;
		case ERR::null_node:
			return L"Error {Skin.stripWeights()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.stripWeights()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.stripWeights()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.stripWeights()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.stripWeights()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.stripWeights()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.stripWeights()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.stripWeights()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_stripBones;
		case ERR::null_node:
			return L"Error {Skin.stripBones()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.stripBones()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.stripBones()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.stripBones()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.stripBones()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.stripBones()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.stripBones()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.stripBones()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_getSelection;
		case ERR::null_node:
			return L"Error {Skin.getSelection()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.getSelection()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.getSelection()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.getSelection()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.getSelection()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.getSelection()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.getSelection()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.getSelection()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_setSelection;
		case ERR::null_node:
			return L"Error {Skin.setSelection()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.setSelection()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.setSelection()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.setSelection()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.setSelection()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.setSelection()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.setSelection()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.setSelection()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_setSelection2;
		case ERR::null_node:
			return L"Error {Skin.setSelection2()}:\n::[node is null]";
		case ERR::bad_handle:
			return L"Error {Skin.setSelection2()}:\n::[out-of-bounds handle]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.setSelection2()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.setSelection2()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.setSelection2()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.setSelection2()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.setSelection2()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.setSelection2()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.setSelection2()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_addBones;
		case ERR::null_node:
			return L"Error {Skin.addBones()}:\n::[node is null]";
		case ERR::skin_null_mod_context:
			return L"Error {Skin.addBones()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Skin.addBones()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Skin.addBones()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Skin.addBones()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Skin.addBones()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Skin.addBones()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Skin.addBones()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Skin_copyWeights;
		case ERR::dst_null_node:
			return L"Error {Skin.copyWeights()}:\n::[dst node is null]";
		case ERR::src_null_node:
			return L"Error {Skin.copyWeights()}:\n::[src node is null]";
		case ERR::different_num_points:
			return L"Error {Skin.copyWeights()}:\n::[skins have differing point counts]";
		case ERR::bone_unmatched:
			return L"Error {Skin.copyWeights()}:\n::[a bone was left unmatched]";
		case ERR::src_skin_null_mod_context:
			return L"Error {Skin.copyWeights()}:\n::[src mod has no context]";
		case ERR::src_skin_null_context_data:
			return L"Error {Skin.copyWeights()}:\n::[src skin has no context data]";
		case ERR::src_skin_null_import_data:
			return L"Error {Skin.copyWeights()}:\n::[src skin has no import data]";
		case ERR::src_skin_failed_cast:
			return L"Error {Skin.copyWeights()}:\n::[src skin-cast failed]";
		case ERR::src_skin_failed_cast_2:
			return L"Error {Skin.copyWeights()}:\n::[src skin2-cast failed]";
		case ERR::src_skin_bad_index:
			return L"Error {Skin.copyWeights()}:\n::[src has no skin at index]";
		case ERR::src_skin_wrong_class_ID:
			return L"Error {Skin.copyWeights()}:\n::[src mod is not skin]";
		case ERR::dst_skin_null_mod_context:
			return L"Error {Skin.copyWeights()}:\n::[dst mod has no context]";
		case ERR::dst_skin_null_context_data:
			return L"Error {Skin.copyWeights()}:\n::[dst skin has no context data]";
		case ERR::dst_skin_null_import_data:
			return L"Error {Skin.copyWeights()}:\n::[dst skin has no import data]";
		case ERR::dst_skin_failed_cast:
			return L"Error {Skin.copyWeights()}:\n::[dst skin-cast failed]";
		case ERR::dst_skin_failed_cast_2:
			return L"Error {Skin.copyWeights()}:\n::[dst skin2-cast failed]";
		case ERR::dst_skin_bad_index:
			return L"Error {Skin.copyWeights()}:\n::[dst has no skin at index]";
		case ERR::dst_skin_wrong_class_ID:
			return L"Error {Skin.copyWeights()}:\n::[dst mod is not skin]";
			}
			{
				using ERR = I_Order_blendMirrorPos;
		case ERR::different_lengths:
			return L"Error {Order.blendMirrorPos()}:\n::[orders differently sized]";
		case ERR::bad_vert_m:
			return L"Error {Order.blendMirrorPos()}:\n::[M-order: out-of-bounds vert]";
		case ERR::bad_vert_a:
			return L"Error {Order.blendMirrorPos()}:\n::[A-order: out-of-bounds vert]";
		case ERR::bad_vert_b:
			return L"Error {Order.blendMirrorPos()}:\n::[B-order: out-of-bounds vert]";
		case ERR::null_node:
			return L"Error {Order.blendMirrorPos()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.blendMirrorPos()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.blendMirrorPos()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.blendMirrorPos()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.blendMirrorPos()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.blendMirrorPos()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_flipMirrorPos;
		case ERR::different_lengths:
			return L"Error {Order.flipMirrorPos()}:\n::[orders differently sized]";
		case ERR::bad_vert_m:
			return L"Error {Order.flipMirrorPos()}:\n::[M-order: out-of-bounds vert]";
		case ERR::bad_vert_a:
			return L"Error {Order.flipMirrorPos()}:\n::[A-order: out-of-bounds vert]";
		case ERR::bad_vert_b:
			return L"Error {Order.flipMirrorPos()}:\n::[B-order: out-of-bounds vert]";
		case ERR::null_node:
			return L"Error {Order.flipMirrorPos()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.flipMirrorPos()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.flipMirrorPos()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.flipMirrorPos()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.flipMirrorPos()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.flipMirrorPos()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_copyPos;
		case ERR::bad_vert_a:
			return L"Error {Order.copyPos()}:\n::[A-order: out-of-bounds vert]";
		case ERR::bad_vert_b:
			return L"Error {Order.copyPos()}:\n::[B-order: out-of-bounds vert]";
		case ERR::bad_selection:
			return L"Error {Order.copyPos()}:\n::[bad selection]";
		case ERR::src_null_node:
			return L"Error {Order.copyPos()}:\n::[src node is null]";
		case ERR::src_poly_null_base_object:
			return L"Error {Order.copyPos()}:\n::[src has no base object]";
		case ERR::src_poly_null_mesh:
			return L"Error {Order.copyPos()}:\n::[src has no mesh]";
		case ERR::src_poly_wrong_class_ID:
			return L"Error {Order.copyPos()}:\n::[src is not poly]";
		case ERR::src_poly_failed_cast:
			return L"Error {Order.copyPos()}:\n::[src poly-cast failed]";
		case ERR::src_poly_mesh_not_filled_in:
			return L"Error {Order.copyPos()}:\n::[src mesh not filled in]";
		case ERR::dst_null_node:
			return L"Error {Order.copyPos()}:\n::[dst node is null]";
		case ERR::dst_poly_null_base_object:
			return L"Error {Order.copyPos()}:\n::[dst has no base object]";
		case ERR::dst_poly_null_mesh:
			return L"Error {Order.copyPos()}:\n::[dst has no mesh]";
		case ERR::dst_poly_wrong_class_ID:
			return L"Error {Order.copyPos()}:\n::[dst is not poly]";
		case ERR::dst_poly_failed_cast:
			return L"Error {Order.copyPos()}:\n::[dst poly-cast failed]";
		case ERR::dst_poly_mesh_not_filled_in:
			return L"Error {Order.copyPos()}:\n::[dst mesh not filled in]";
			}
			{
				using ERR = I_Order_blendPos;
		case ERR::different_lengths:
			return L"Error {Order.blendPos()}:\n::[orders differently sized]";
		case ERR::bad_vert_a:
			return L"Error {Order.blendPos()}:\n::[A-order: out-of-bounds vert]";
		case ERR::bad_vert_b:
			return L"Error {Order.blendPos()}:\n::[B-order: out-of-bounds vert]";
		case ERR::src_null_node:
			return L"Error {Order.blendPos()}:\n::[src node is null]";
		case ERR::src_poly_null_base_object:
			return L"Error {Order.blendPos()}:\n::[src has no base object]";
		case ERR::src_poly_null_mesh:
			return L"Error {Order.blendPos()}:\n::[src has no mesh]";
		case ERR::src_poly_wrong_class_ID:
			return L"Error {Order.blendPos()}:\n::[src is not poly]";
		case ERR::src_poly_failed_cast:
			return L"Error {Order.blendPos()}:\n::[src poly-cast failed]";
		case ERR::src_poly_mesh_not_filled_in:
			return L"Error {Order.blendPos()}:\n::[src mesh not filled in]";
		case ERR::dst_null_node:
			return L"Error {Order.blendPos()}:\n::[dst node is null]";
		case ERR::dst_poly_null_base_object:
			return L"Error {Order.blendPos()}:\n::[dst has no base object]";
		case ERR::dst_poly_null_mesh:
			return L"Error {Order.blendPos()}:\n::[dst has no mesh]";
		case ERR::dst_poly_wrong_class_ID:
			return L"Error {Order.blendPos()}:\n::[dst is not poly]";
		case ERR::dst_poly_failed_cast:
			return L"Error {Order.blendPos()}:\n::[dst poly-cast failed]";
		case ERR::dst_poly_mesh_not_filled_in:
			return L"Error {Order.blendPos()}:\n::[dst mesh not filled in]";
			}
			{
				using ERR = I_Order_extractSymmetrySelection;
		case ERR::broken_mesh:
			return L"Error {Order.extractSymmetrySelection()}:\n::[violated mesh invariants]";
		case ERR::zombie_mesh:
			return L"Error {Order.extractSymmetrySelection()}:\n::[dead mesh walking]";
		case ERR::insufficient_selection:
			return L"Error {Order.extractSymmetrySelection()}:\n::[insufficient selection]";
		case ERR::ambivalent_selection:
			return L"Error {Order.extractSymmetrySelection()}:\n::[ambivalent selection]";
		case ERR::null_node:
			return L"Error {Order.extractSymmetrySelection()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.extractSymmetrySelection()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.extractSymmetrySelection()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.extractSymmetrySelection()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.extractSymmetrySelection()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.extractSymmetrySelection()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_extractTraversalSelection;
		case ERR::broken_mesh:
			return L"Error {Order.extractTraversalSelection()}:\n::[violated mesh invariants]";
		case ERR::zombie_mesh:
			return L"Error {Order.extractTraversalSelection()}:\n::[dead mesh walking]";
		case ERR::insufficient_selection:
			return L"Error {Order.extractTraversalSelection()}:\n::[insufficient selection]";
		case ERR::ambivalent_selection:
			return L"Error {Order.extractTraversalSelection()}:\n::[ambivalent selection]";
		case ERR::null_node:
			return L"Error {Order.extractTraversalSelection()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.extractTraversalSelection()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.extractTraversalSelection()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.extractTraversalSelection()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.extractTraversalSelection()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.extractTraversalSelection()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_buildSymmetryOrder;
		case ERR::bad_selection:
			return L"Error {Order.buildSymmetryOrder()}:\n::[bad selection]";
		case ERR::order_mismatch:
			return L"Error {Order.buildSymmetryOrder()}:\n::[asymmetry detected]";
		case ERR::null_node:
			return L"Error {Order.buildSymmetryOrder()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.buildSymmetryOrder()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.buildSymmetryOrder()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.buildSymmetryOrder()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.buildSymmetryOrder()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.buildSymmetryOrder()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_buildTraversalOrder;
		case ERR::bad_selection:
			return L"Error {Order.buildTraversalOrder()}:\n::[bad selection]";
		case ERR::null_node:
			return L"Error {Order.buildTraversalOrder()}:\n::[node is null]";
		case ERR::poly_null_base_object:
			return L"Error {Order.buildTraversalOrder()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.buildTraversalOrder()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.buildTraversalOrder()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.buildTraversalOrder()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.buildTraversalOrder()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_blendMirrorWeights;
		case ERR::null_node:
			return L"Error {Order.blendMirrorWeights()}:\n::[node is null]";
		case ERR::a_affix_empty:
			return L"Error {Order.blendMirrorWeights()}:\n::[A-affix contains only white-space]";
		case ERR::b_affix_empty:
			return L"Error {Order.blendMirrorWeights()}:\n::[B-affix contains only white-space]";
		case ERR::duplicate_bone_name:
			return L"Error {Order.blendMirrorWeights()}:\n::[skin has duplicate bone names]";
		case ERR::bad_handle_m:
			return L"Error {Order.blendMirrorWeights()}:\n::[M-order: out-of-bounds handle]";
		case ERR::bad_handle_a:
			return L"Error {Order.blendMirrorWeights()}:\n::[A-order: out-of-bounds handle]";
		case ERR::bad_handle_b:
			return L"Error {Order.blendMirrorWeights()}:\n::[B-order: out-of-bounds handle]";
		case ERR::poly_null_base_object:
			return L"Error {Order.blendMirrorWeights()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.blendMirrorWeights()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.blendMirrorWeights()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.blendMirrorWeights()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.blendMirrorWeights()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Order.blendMirrorWeights()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Order.blendMirrorWeights()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Order.blendMirrorWeights()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Order.blendMirrorWeights()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Order.blendMirrorWeights()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Order.blendMirrorWeights()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Order.blendMirrorWeights()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Order_flipMirrorWeights;
		case ERR::null_node:
			return L"Error {Order.flipMirrorWeights()}:\n::[node is null]";
		case ERR::a_affix_empty:
			return L"Error {Order.flipMirrorWeights()}:\n::[A-affix contains only white-space]";
		case ERR::b_affix_empty:
			return L"Error {Order.flipMirrorWeights()}:\n::[B-affix contains only white-space]";
		case ERR::bad_vert_m:
			return L"Error {Order.flipMirrorWeights()}:\n::[M-order: out-of-bounds handle]";
		case ERR::bad_vert_a:
			return L"Error {Order.flipMirrorWeights()}:\n::[A-order: out-of-bounds handle]";
		case ERR::bad_vert_b:
			return L"Error {Order.flipMirrorWeights()}:\n::[B-order: out-of-bounds handle]";
		case ERR::duplicate_bone_name:
			return L"Error {Order.flipMirrorWeights()}:\n::[skin has duplicate bone names]";
		case ERR::poly_null_base_object:
			return L"Error {Order.flipMirrorWeights()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.flipMirrorWeights()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.flipMirrorWeights()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.flipMirrorWeights()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.flipMirrorWeights()}:\n::[mesh not filled in]";
		case ERR::skin_null_mod_context:
			return L"Error {Order.flipMirrorWeights()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Order.flipMirrorWeights()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Order.flipMirrorWeights()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Order.flipMirrorWeights()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Order.flipMirrorWeights()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Order.flipMirrorWeights()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Order.flipMirrorWeights()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Order_translateSelection;
		case ERR::a_bad_vert:
			return L"Error {Order.translateSelection()}:\n::[A-order: out-of-bounds vert]";
		case ERR::a_bad_edge:
			return L"Error {Order.translateSelection()}:\n::[A-order: out-of-bounds edge]";
		case ERR::a_bad_face:
			return L"Error {Order.translateSelection()}:\n::[A-order: out-of-bounds face]";
		case ERR::b_bad_vert:
			return L"Error {Order.translateSelection()}:\n::[B-order: out-of-bounds vert]";
		case ERR::b_bad_edge:
			return L"Error {Order.translateSelection()}:\n::[B-order: out-of-bounds edge]";
		case ERR::b_bad_face:
			return L"Error {Order.translateSelection()}:\n::[B-order: out-of-bounds face]";
		case ERR::src_null_node:
			return L"Error {Order.translateSelection()}:\n::[src node is null]";
		case ERR::src_poly_null_base_object:
			return L"Error {Order.translateSelection()}:\n::[src has no base object]";
		case ERR::src_poly_null_mesh:
			return L"Error {Order.translateSelection()}:\n::[src has no mesh]";
		case ERR::src_poly_wrong_class_ID:
			return L"Error {Order.translateSelection()}:\n::[src is not poly]";
		case ERR::src_poly_failed_cast:
			return L"Error {Order.translateSelection()}:\n::[src poly-cast failed]";
		case ERR::src_poly_mesh_not_filled_in:
			return L"Error {Order.translateSelection()}:\n::[src mesh not filled in]";
		case ERR::dst_null_node:
			return L"Error {Order.translateSelection()}:\n::[dst node is null]";
		case ERR::dst_poly_null_base_object:
			return L"Error {Order.translateSelection()}:\n::[dst has no base object]";
		case ERR::dst_poly_null_mesh:
			return L"Error {Order.translateSelection()}:\n::[dst has no mesh]";
		case ERR::dst_poly_wrong_class_ID:
			return L"Error {Order.translateSelection()}:\n::[dst is not poly]";
		case ERR::dst_poly_failed_cast:
			return L"Error {Order.translateSelection()}:\n::[dst poly-cast failed]";
		case ERR::dst_poly_mesh_not_filled_in:
			return L"Error {Order.translateSelection()}:\n::[dst mesh not filled in]";
			}
			{
				using ERR = I_Order_translateHandles;
		case ERR::different_lengths:
			return L"Error {Order.translateHandles()}:\n::[orders differently sized]";
		case ERR::a_bad_handle:
			return L"Error {Order.translateHandles()}:\n::[A-order: out-of-bounds handle]";
		case ERR::b_bad_handle:
			return L"Error {Order.translateHandles()}:\n::[B-order: out-of-bounds handle]";
		case ERR::src_null_node:
			return L"Error {Order.translateHandles()}:\n::[src node is null]";
		case ERR::src_skin_null_mod_context:
			return L"Error {Order.translateHandles()}:\n::[src mod has no context]";
		case ERR::src_skin_null_context_data:
			return L"Error {Order.translateHandles()}:\n::[src skin has no context data]";
		case ERR::src_skin_null_import_data:
			return L"Error {Order.translateHandles()}:\n::[src skin has no import data]";
		case ERR::src_skin_failed_cast:
			return L"Error {Order.translateHandles()}:\n::[src skin-cast failed]";
		case ERR::src_skin_failed_cast_2:
			return L"Error {Order.translateHandles()}:\n::[src skin2-cast failed]";
		case ERR::src_skin_bad_index:
			return L"Error {Order.translateHandles()}:\n::[src has no skin at index]";
		case ERR::src_skin_wrong_class_ID:
			return L"Error {Order.translateHandles()}:\n::[src mod is not skin]";
		case ERR::dst_null_node:
			return L"Error {Order.translateHandles()}:\n::[dst node is null]";
		case ERR::dst_skin_null_mod_context:
			return L"Error {Order.translateHandles()}:\n::[dst mod has no context]";
		case ERR::dst_skin_null_context_data:
			return L"Error {Order.translateHandles()}:\n::[dst skin has no context data]";
		case ERR::dst_skin_null_import_data:
			return L"Error {Order.translateHandles()}:\n::[dst skin has no import data]";
		case ERR::dst_skin_failed_cast:
			return L"Error {Order.translateHandles()}:\n::[dst skin-cast failed]";
		case ERR::dst_skin_failed_cast_2:
			return L"Error {Order.translateHandles()}:\n::[dst skin2-cast failed]";
		case ERR::dst_skin_bad_index:
			return L"Error {Order.translateHandles()}:\n::[dst has no skin at index]";
		case ERR::dst_skin_wrong_class_ID:
			return L"Error {Order.translateHandles()}:\n::[dst mod is not skin]";
			}
			{
				using ERR = I_Order_mirrorSelection;
		case ERR::null_node:
			return L"Error {Order.mirrorSelection()}:\n::[node is null]";
		case ERR::different_lengths:
			return L"Error {Order.mirrorSelection()}:\n::[orders differently sized]";
		case ERR::bad_vert_m:
			return L"Error {Order.mirrorSelection()}:\n::[M-order: out-of-bounds vert]";
		case ERR::bad_vert_a:
			return L"Error {Order.mirrorSelection()}:\n::[A-order: out-of-bounds vert]";
		case ERR::bad_vert_b:
			return L"Error {Order.mirrorSelection()}:\n::[B-order: out-of-bounds vert]";
		case ERR::bad_edge_m:
			return L"Error {Order.mirrorSelection()}:\n::[M-order: out-of-bounds edge]";
		case ERR::bad_edge_a:
			return L"Error {Order.mirrorSelection()}:\n::[A-order: out-of-bounds edge]";
		case ERR::bad_edge_b:
			return L"Error {Order.mirrorSelection()}:\n::[B-order: out-of-bounds edge]";
		case ERR::bad_face_m:
			return L"Error {Order.mirrorSelection()}:\n::[M-order: out-of-bounds face]";
		case ERR::bad_face_a:
			return L"Error {Order.mirrorSelection()}:\n::[A-order: out-of-bounds face]";
		case ERR::bad_face_b:
			return L"Error {Order.mirrorSelection()}:\n::[B-order: out-of-bounds face]";
		case ERR::poly_null_base_object:
			return L"Error {Order.mirrorSelection()}:\n::[no base object]";
		case ERR::poly_null_mesh:
			return L"Error {Order.mirrorSelection()}:\n::[no mesh]";
		case ERR::poly_wrong_class_ID:
			return L"Error {Order.mirrorSelection()}:\n::[not poly]";
		case ERR::poly_failed_cast:
			return L"Error {Order.mirrorSelection()}:\n::[poly-cast failed]";
		case ERR::poly_mesh_not_filled_in:
			return L"Error {Order.mirrorSelection()}:\n::[mesh not filled in]";
			}
			{
				using ERR = I_Order_mirrorHandles;
		case ERR::null_node:
			return L"Error {Order.mirrorHandles()}:\n::[node is null]";
		case ERR::different_lengths:
			return L"Error {Order.mirrorHandles()}:\n::[orders differently sized]";
		case ERR::bad_handle_m:
			return L"Error {Order.mirrorHandles()}:\n::[M-order: out-of-bounds handle]";
		case ERR::bad_handle_a:
			return L"Error {Order.mirrorHandles()}:\n::[A-order: out-of-bounds handle]";
		case ERR::bad_handle_b:
			return L"Error {Order.mirrorHandles()}:\n::[B-order: out-of-bounds handle]";
		case ERR::skin_null_mod_context:
			return L"Error {Order.mirrorHandles()}:\n::[mod has no context]";
		case ERR::skin_null_context_data:
			return L"Error {Order.mirrorHandles()}:\n::[skin has no context data]";
		case ERR::skin_null_import_data:
			return L"Error {Order.mirrorHandles()}:\n::[skin has no import data]";
		case ERR::skin_failed_cast:
			return L"Error {Order.mirrorHandles()}:\n::[skin-cast failed]";
		case ERR::skin_failed_cast_2:
			return L"Error {Order.mirrorHandles()}:\n::[skin2-cast failed]";
		case ERR::skin_bad_index:
			return L"Error {Order.mirrorHandles()}:\n::[no skin at index]";
		case ERR::skin_wrong_class_ID:
			return L"Error {Order.mirrorHandles()}:\n::[mod is not skin]";
			}
			{
				using ERR = I_Order_transferWeights;
		case ERR::dst_null_node:
			return L"Error {Order.transferWeights()}:\n::[dst node is null]";
		case ERR::src_null_node:
			return L"Error {Order.transferWeights()}:\n::[src node is null]";
		case ERR::src_affix_empty:
			return L"Error {Order.transferWeights()}:\n::[src affix contains only white-space]";
		case ERR::dst_affix_empty:
			return L"Error {Order.transferWeights()}:\n::[dst affix contains only white-space]";
		case ERR::skin_mangled:
			return L"Error {Order.transferWeights()}:\n::[skins are a mangled mess]";
		case ERR::src_bad_handle:
			return L"Error {Order.transferWeights()}:\n::[out-of-bounds src handle]";
		case ERR::dst_bad_handle:
			return L"Error {Order.transferWeights()}:\n::[out-of-bounds dst handle]";
		case ERR::src_duplicate_bone_name:
			return L"Error {Order.transferWeights()}:\n::[src skin has duplicate bone names]";
		case ERR::dst_duplicate_bone_name:
			return L"Error {Order.transferWeights()}:\n::[dst skin has duplicate bone names]";
		case ERR::unmatched_bone:
			return L"Error {Order.transferWeights()}:\n::[a bone was left unmatched]";
		case ERR::src_skin_null_mod_context:
			return L"Error {Order.transferWeights()}:\n::[src mod has no context]";
		case ERR::src_skin_null_context_data:
			return L"Error {Order.transferWeights()}:\n::[src skin has no context data]";
		case ERR::src_skin_null_import_data:
			return L"Error {Order.transferWeights()}:\n::[src skin has no import data]";
		case ERR::src_skin_failed_cast:
			return L"Error {Order.transferWeights()}:\n::[src skin-cast failed]";
		case ERR::src_skin_failed_cast_2:
			return L"Error {Order.transferWeights()}:\n::[src skin2-cast failed]";
		case ERR::src_skin_bad_index:
			return L"Error {Order.transferWeights()}:\n::[src has no skin at index]";
		case ERR::src_skin_wrong_class_ID:
			return L"Error {Order.transferWeights()}:\n::[src mod is not skin]";
		case ERR::dst_skin_null_mod_context:
			return L"Error {Order.transferWeights()}:\n::[dst mod has no context]";
		case ERR::dst_skin_null_context_data:
			return L"Error {Order.transferWeights()}:\n::[dst skin has no context data]";
		case ERR::dst_skin_null_import_data:
			return L"Error {Order.transferWeights()}:\n::[dst skin has no import data]";
		case ERR::dst_skin_failed_cast:
			return L"Error {Order.transferWeights()}:\n::[dst skin-cast failed]";
		case ERR::dst_skin_failed_cast_2:
			return L"Error {Order.transferWeights()}:\n::[dst skin2-cast failed]";
		case ERR::dst_skin_bad_index:
			return L"Error {Order.transferWeights()}:\n::[dst has no skin at index]";
		case ERR::dst_skin_wrong_class_ID:
			return L"Error {Order.transferWeights()}:\n::[dst mod is not skin]";
			}
	//		{
	//			using ERR = I_Misc_compressIntArray1;
	//	case ERR::dst_overfull:
	//		return L"Error {Misc.compressIntArray1()}:\n::[dst array overfull]";
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.compressIntArray1()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.compressIntArray1()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.compressIntArray1()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.compressIntArray1()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.compressIntArray1()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.compressIntArray1()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.compressIntArray1()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.compressIntArray1()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.compressIntArray1()}:\n::[dstLen too small]";
	//		}
	//		{
	//			using ERR = I_Misc_compressIntArray2;
	//	case ERR::dst_overfull:
	//		return L"Error {Misc.compressIntArray2()}:\n::[dst array overfull]";
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.compressIntArray2()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.compressIntArray2()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.compressIntArray2()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.compressIntArray2()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.compressIntArray2()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.compressIntArray2()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.compressIntArray2()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.compressIntArray2()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.compressIntArray2()}:\n::[dstLen too small]";
	//		}
	//		{
	//			using ERR = I_Misc_compressIntArray4;
	//	case ERR::dst_overfull:
	//		return L"Error {Misc.compressIntArray4()}:\n::[dst array overfull]";
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.compressIntArray4()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.compressIntArray4()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.compressIntArray4()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.compressIntArray4()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.compressIntArray4()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.compressIntArray4()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.compressIntArray4()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.compressIntArray4()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.compressIntArray4()}:\n::[dstLen too small]";
	//		}
	//		{
	//			using ERR = I_Misc_compressIntArray8;
	//	case ERR::dst_overfull:
	//		return L"Error {Misc.compressIntArray8()}:\n::[dst array overfull]";
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.compressIntArray8()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.compressIntArray8()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.compressIntArray8()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.compressIntArray8()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.compressIntArray8()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.compressIntArray8()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.compressIntArray8()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.compressIntArray8()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.compressIntArray8()}:\n::[dstLen too small]";
	//		}
			{
				using ERR = I_Misc_compressIntArray;
		case ERR::dst_overfull:
			return L"Error {Misc.compressIntArray()}:\n::[dst array overfull]";
		case ERR::srcLen_negative:
			return L"Error {Misc.compressIntArray()}:\n::[srcLen < 0]";
		case ERR::srcLen_too_large:
			return L"Error {Misc.compressIntArray()}:\n::[srcLen too large]";
		case ERR::srcLen_too_small:
			return L"Error {Misc.compressIntArray()}:\n::[srcLen too small]";
		case ERR::srcOff_negative:
			return L"Error {Misc.compressIntArray()}:\n::[srcOff < 0]";
		case ERR::srcOff_too_large:
			return L"Error {Misc.compressIntArray()}:\n::[srcOff too large]";
		case ERR::dstOff_negative:
			return L"Error {Misc.compressIntArray()}:\n::[dstOff < 0]";
		case ERR::dstOff_too_large:
			return L"Error {Misc.compressIntArray()}:\n::[dstOff too large]";
		case ERR::dstLen_negative:
			return L"Error {Misc.compressIntArray()}:\n::[dstLen < 0]";
		case ERR::dstLen_too_small:
			return L"Error {Misc.compressIntArray()}:\n::[dstLen too small]";
			}
			{
				using ERR = I_Misc_compressIntArrays;
		case ERR::dst_overfull:
			return L"Error {Misc.compressIntArrays()}:\n::[dst array overfull]";
		case ERR::null_val:
			return L"Error {Misc.compressIntArrays()}:\n::[array contains undefined]";
		case ERR::not_int_tab:
			return L"Error {Misc.compressIntArrays()}:\n::[sub array not int array]";
		case ERR::null_tab:
			return L"Error {Misc.compressIntArrays()}:\n::[sub array invalid]";
		case ERR::srcLen_negative:
			return L"Error {Misc.compressIntArrays()}:\n::[srcLen < 0]";
		case ERR::srcLen_too_large:
			return L"Error {Misc.compressIntArrays()}:\n::[srcLen too large]";
		case ERR::srcLen_too_small:
			return L"Error {Misc.compressIntArrays()}:\n::[srcLen too small]";
		case ERR::srcOff_negative:
			return L"Error {Misc.compressIntArrays()}:\n::[srcOff < 0]";
		case ERR::srcOff_too_large:
			return L"Error {Misc.compressIntArrays()}:\n::[srcOff too large]";
		case ERR::dstOff_negative:
			return L"Error {Misc.compressIntArrays()}:\n::[dstOff < 0]";
		case ERR::dstOff_too_large:
			return L"Error {Misc.compressIntArrays()}:\n::[dstOff too large]";
		case ERR::dstLen_negative:
			return L"Error {Misc.compressIntArrays()}:\n::[dstLen < 0]";
		case ERR::dstLen_too_small:
			return L"Error {Misc.compressIntArrays()}:\n::[dstLen too small]";
			}
	//		{
	//			using ERR = I_Misc_decompressIntArray1;
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[dstLen too small]";
	//	case ERR::stream_corrupt:
	//		return L"Error {Misc.decompressIntArray1()}:\n::[corrupt stream]";
	//		}
	//		{
	//			using ERR = I_Misc_decompressIntArray2;
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[dstLen too small]";
	//	case ERR::stream_corrupt:
	//		return L"Error {Misc.decompressIntArray2()}:\n::[corrupt stream]";
	//		}
	//		{
	//			using ERR = I_Misc_decompressIntArray4;
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[dstLen too small]";
	//	case ERR::stream_corrupt:
	//		return L"Error {Misc.decompressIntArray4()}:\n::[corrupt stream]";
	//		}
	//		{
	//			using ERR = I_Misc_decompressIntArray8;
	//	case ERR::srcLen_negative:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[srcLen < 0]";
	//	case ERR::srcLen_too_large:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[srcLen too large]";
	//	case ERR::srcLen_too_small:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[srcLen too small]";
	//	case ERR::srcOff_negative:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[srcOff < 0]";
	//	case ERR::srcOff_too_large:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[srcOff too large]";
	//	case ERR::dstOff_negative:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[dstOff < 0]";
	//	case ERR::dstOff_too_large:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[dstOff too large]";
	//	case ERR::dstLen_negative:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[dstLen < 0]";
	//	case ERR::dstLen_too_small:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[dstLen too small]";
	//	case ERR::stream_corrupt:
	//		return L"Error {Misc.decompressIntArray8()}:\n::[corrupt stream]";
	//		}
			{
				using ERR = I_Misc_decompressIntArray;
		case ERR::srcLen_negative:
			return L"Error {Misc.decompressIntArray()}:\n::[srcLen < 0]";
		case ERR::srcLen_too_large:
			return L"Error {Misc.decompressIntArray()}:\n::[srcLen too large]";
		case ERR::srcLen_too_small:
			return L"Error {Misc.decompressIntArray()}:\n::[srcLen too small]";
		case ERR::srcOff_negative:
			return L"Error {Misc.decompressIntArray()}:\n::[srcOff < 0]";
		case ERR::srcOff_too_large:
			return L"Error {Misc.decompressIntArray()}:\n::[srcOff too large]";
		case ERR::dstOff_negative:
			return L"Error {Misc.decompressIntArray()}:\n::[dstOff < 0]";
		case ERR::dstOff_too_large:
			return L"Error {Misc.decompressIntArray()}:\n::[dstOff too large]";
		case ERR::dstLen_negative:
			return L"Error {Misc.decompressIntArray()}:\n::[dstLen < 0]";
		case ERR::dstLen_too_small:
			return L"Error {Misc.decompressIntArray()}:\n::[dstLen too small]";
		case ERR::stream_corrupt:
			return L"Error {Misc.decompressIntArray()}:\n::[corrupt stream]";
			}
			{
				using ERR = I_Misc_decompressIntArrays;
		case ERR::srcLen_negative:
			return L"Error {Misc.decompressIntArrays()}:\n::[srcLen < 0]";
		case ERR::null_val:
			return L"Error {Misc.decompressIntArrays()}:\n::[array contains undefined]";
		case ERR::not_int_tab:
			return L"Error {Misc.decompressIntArrays()}:\n::[sub array not int array]";
		case ERR::null_tab:
			return L"Error {Misc.decompressIntArrays()}:\n::[sub array invalid]";
		case ERR::srcLen_too_large:
			return L"Error {Misc.decompressIntArrays()}:\n::[srcLen too large]";
		case ERR::srcLen_too_small:
			return L"Error {Misc.decompressIntArrays()}:\n::[srcLen too small]";
		case ERR::srcOff_negative:
			return L"Error {Misc.decompressIntArrays()}:\n::[srcOff < 0]";
		case ERR::srcOff_too_large:
			return L"Error {Misc.decompressIntArrays()}:\n::[srcOff too large]";
		case ERR::dstOff_negative:
			return L"Error {Misc.decompressIntArrays()}:\n::[dstOff < 0]";
		case ERR::dstOff_too_large:
			return L"Error {Misc.decompressIntArrays()}:\n::[dstOff too large]";
		case ERR::dstLen_negative:
			return L"Error {Misc.decompressIntArrays()}:\n::[dstLen < 0]";
		case ERR::dstLen_too_small:
			return L"Error {Misc.decompressIntArrays()}:\n::[dstLen too small]";
		case ERR::stream_corrupt:
			return L"Error {Misc.decompressIntArrays()}:\n::[corrupt stream]";
			}
			{
				using ERR = I_Misc_buildSwapSequence;
		case ERR::bad_bounds:
			return L"Error {Misc.buildSwapSequence()}:\n::[bad bounds]";
		case ERR::too_large:
			return L"Error {Misc.buildSwapSequence()}:\n::[input range too large]";
			}
		default:
			// nothing
			return L"";
		}
	}
};

