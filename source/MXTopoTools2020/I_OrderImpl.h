#pragma once

#include "Util.h"
#include "Codes.h"
#include "Helpers.h"
#include "Restore.h"

class I_OrderImpl {

public:
	static inline i32 blendMirrorPos(
		INode* node,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const Bitset& selection,
		const i32 axis,
		const f32 weight
	) {
		using ERR = Codes::I_Order_blendMirrorPos;
		Helpers::Poly helper;
		const i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		MNMesh& mesh = helper.mesh();
		if (orderA.length() != orderB.length()) {
			return ERR::different_lengths;
		}
		if (!Helpers::Geom::checkBounds(orderM, mesh.numv)) {
			return ERR::bad_vert_m;
		}
		if (!Helpers::Geom::checkBounds(orderA, mesh.numv)) {
			return ERR::bad_vert_a;
		}
		if (!Helpers::Geom::checkBounds(orderB, mesh.numv)) {
			return ERR::bad_vert_b;
		}
		Defer<Restore::PosRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::PosRestore(helper));
			rest.get()->before();
		}
		if (weight >= 0.000002f) {
			const bool eq1 = weight >= 0.999998f;
			switch (axis) {
			case 0: {
				// X
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								posB.x = F32(-F64(NaN32(posA.x)));
								posB.y = NaN32(posA.y);
								posB.z = NaN32(posA.z);
							}
							else {
								posA.x = F32(-F64(NaN32(posB.x)));
								posA.y = NaN32(posB.y);
								posA.z = NaN32(posB.z);
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							if (dx < 0.0) {
								posA.x = F32(d);
								posB.x = F32(-d);
							}
							else {
								posA.x = F32(-d);
								posB.x = F32(d);
							}
							posA.y = posB.y = F32((ay / 2.0) + (by / 2.0));
							posA.z = posB.z = F32((az / 2.0) + (bz / 2.0));
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							mesh.P(v).x = 0.0f;
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							if (s) {
								posB.x = F32(bx * _w - ax * w);
								posB.y = F32(by * _w + ay * w);
								posB.z = F32(bz * _w + az * w);
							}
							else {
								posA.x = F32(ax * _w - bx * w);
								posA.y = F32(ay * _w + by * w);
								posA.z = F32(az * _w + bz * w);
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							f64 ax1, bx1;
							if (dx < 0.0) {
								ax1 = d;
								bx1 = -d;
							}
							else {
								ax1 = -d;
								bx1 = d;
							}
							const f64 ay1 = (ay / 2.0) + (by / 2.0);
							const f64 az1 = (az / 2.0) + (bz / 2.0);
							posA.x = F32(ax * _w + ax1 * w);
							posA.y = F32(ay * _w + ay1 * w);
							posA.z = F32(az * _w + az1 * w);
							posB.x = F32(bx * _w + bx1 * w);
							posB.y = F32(by * _w + ay1 * w);
							posB.z = F32(bz * _w + az1 * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& p = mesh.P(v);
							p.x = F32(F64(NaN32(p.x)) * _w);
						}
					}
				}
				break;
			}
			case 1: {
				// Y
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								posB.x = NaN32(posA.x);
								posB.y = F32(-F64(NaN32(posA.y)));
								posB.z = NaN32(posA.z);
							}
							else {
								posA.x = NaN32(posB.x);
								posA.y = F32(-F64(NaN32(posB.y)));
								posA.z = NaN32(posB.z);
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							posA.x = posB.x = F32((ax / 2.0) + (bx / 2.0));
							if (dy < 0.0) {
								posA.y = F32(d);
								posB.y = F32(-d);
							}
							else {
								posA.y = F32(-d);
								posB.y = F32(d);
							}
							posA.z = posB.z = F32((az / 2.0) + (bz / 2.0));
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							mesh.P(v).y = 0.0f;
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							if (s) {
								posB.x = F32(bx * _w + ax * w);
								posB.y = F32(by * _w - ay * w);
								posB.z = F32(bz * _w + az * w);
							}
							else {
								posA.x = F32(ax * _w + bx * w);
								posA.y = F32(ay * _w - by * w);
								posA.z = F32(az * _w + bz * w);
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							const f64 ax1 = (ax / 2.0) + (bx / 2.0);
							f64 ay1, by1;
							if (dy < 0.0) {
								ay1 = d;
								by1 = -d;
							}
							else {
								ay1 = -d;
								by1 = d;
							}
							const f64 az1 = (az / 2.0) + (bz / 2.0);
							posA.x = F32(ax * _w + ax1 * w);
							posA.y = F32(ay * _w + ay1 * w);
							posA.z = F32(az * _w + az1 * w);
							posB.x = F32(bx * _w + ax1 * w);
							posB.y = F32(by * _w + by1 * w);
							posB.z = F32(bz * _w + az1 * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& p = mesh.P(v);
							p.y = F32(F64(NaN32(p.y)) * _w);
						}
					}
				}
				break;
			}
			default: {
				// Z
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								posB.x = NaN32(posA.x);
								posB.y = NaN32(posA.y);
								posB.z = F32(-F64(NaN32(posA.z)));
							}
							else {
								posA.x = NaN32(posB.x);
								posA.y = NaN32(posB.y);
								posA.z = F32(-F64(NaN32(posB.z)));
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							posA.x = posB.x = F32((ax / 2.0) + (bx / 2.0));
							posA.y = posB.y = F32((ay / 2.0) + (by / 2.0));
							if (dz < 0.0) {
								posA.z = F32(d);
								posB.z = F32(-d);
							}
							else {
								posA.z = F32(-d);
								posB.z = F32(d);
							}
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							mesh.P(v).x = 0.0f;
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						Point3& posA = mesh.P(a);
						Point3& posB = mesh.P(b);
						const bool s = selection[a];
						if (s != selection[b]) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							if (s) {
								posB.x = F32(bx * _w + ax * w);
								posB.y = F32(by * _w + ay * w);
								posB.z = F32(bz * _w - az * w);
							}
							else {
								posA.x = F32(ax * _w + bx * w);
								posA.y = F32(ay * _w + by * w);
								posA.z = F32(az * _w - bz * w);
							}
						}
						else if (s) {
							const f64 ax = F64(NaN32(posA.x)), ay = F64(NaN32(posA.y)), az = F64(NaN32(posA.z));
							const f64 bx = F64(NaN32(posB.x)), by = F64(NaN32(posB.y)), bz = F64(NaN32(posB.z));
							const f64 dx = (bx - ax) / 2.0;
							const f64 dy = (by - ay) / 2.0;
							const f64 dz = (bz - az) / 2.0;
							const f64 d = sqrt((dx * dx) + (dy * dy) + (dz * dz));
							const f64 ax1 = (ax / 2.0) + (bx / 2.0);
							const f64 ay1 = (ay / 2.0) + (by / 2.0);
							f64 az1, bz1;
							if (dz < 0.0) {
								az1 = d;
								bz1 = -d;
							}
							else {
								az1 = -d;
								bz1 = d;
							}
							posA.x = F32(ax * _w + ax1 * w);
							posA.y = F32(ay * _w + ay1 * w);
							posA.z = F32(az * _w + az1 * w);
							posB.x = F32(bx * _w + ax1 * w);
							posB.y = F32(by * _w + ay1 * w);
							posB.z = F32(bz * _w + bz1 * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& p = mesh.P(v);
							p.z = F32(F64(NaN32(p.z)) * _w);
						}
					}
				}
				break;
			}
			}
		}
		helper.changedGeom();
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		helper.epoly().RefreshScreen();
		Util::redrawViews();
		return Codes::OK;
	}

public:
	static inline i32 flipMirrorPos(
		INode* node,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const Bitset& selection,
		const i32 axis,
		const f32 weight
	) {
		using ERR = Codes::I_Order_flipMirrorPos;
		Helpers::Poly helper;
		const i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		MNMesh& mesh = helper.mesh();
		if (orderA.length() != orderB.length()) {
			return ERR::different_lengths;
		}
		if (!Helpers::Geom::checkBounds(orderM, mesh.numv)) {
			return ERR::bad_vert_m;
		}
		if (!Helpers::Geom::checkBounds(orderA, mesh.numv)) {
			return ERR::bad_vert_a;
		}
		if (!Helpers::Geom::checkBounds(orderB, mesh.numv)) {
			return ERR::bad_vert_b;
		}
		Restore::PosRestore* rest = nullptr;
		const bool holding = theHold.Holding();
		if (holding) {
			rest = new Restore::PosRestore(helper);
			rest->before();
		}
		if (weight >= 0.000002f) {
			const bool eq1 = weight >= 0.999998f;
			switch (axis) {
			case 0: {
				// X
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								pos.x = F32(-F64(NaN32(pos.x)));
							}
							else {
								Point3& pos = mesh.P(b);
								pos.x = F32(-F64(NaN32(pos.x)));
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f32 t = posA.x;
							posA.x = posB.x;
							posB.x = t;
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							pos.x = F32(-F64(NaN32(pos.x)));
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								const f64 x = F64(NaN32(pos.x));
								pos.x = F32(x * _w - x * w);
							}
							else {
								Point3& pos = mesh.P(b);
								const f64 x = F64(NaN32(pos.x));
								pos.x = F32(x * _w - x * w);
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f64 ax = F64(NaN32(posA.x));
							const f64 bx = F64(NaN32(posB.x));
							posA.x = F32(ax * _w + bx * w);
							posB.x = F32(bx * _w + ax * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							const f64 x = F64(NaN32(pos.x));
							pos.x = F32(x * _w - x * w);
						}
					}
				}
				break;
			}
			case 1: {
				// Y
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								pos.y = F32(-F64(NaN32(pos.y)));
							}
							else {
								Point3& pos = mesh.P(b);
								pos.y = F32(-F64(NaN32(pos.y)));
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f32 t = posA.y;
							posA.y = posB.y;
							posB.y = t;
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							pos.y = F32(-F64(NaN32(pos.y)));
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								const f64 y = F64(NaN32(pos.y));
								pos.y = F32(y * _w - y * w);
							}
							else {
								Point3& pos = mesh.P(b);
								const f64 y = F64(NaN32(pos.y));
								pos.y = F32(y * _w - y * w);
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f64 ay = F64(NaN32(posA.y));
							const f64 by = F64(NaN32(posB.y));
							posA.y = F32(ay * _w + by * w);
							posB.y = F32(by * _w + ay * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							const f64 y = F64(NaN32(pos.y));
							pos.y = F32(y * _w - y * w);
						}
					}
				}
				break;
			}
			default: {
				// Z
				if (eq1) {
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								pos.z = F32(-F64(NaN32(pos.z)));
							}
							else {
								Point3& pos = mesh.P(b);
								pos.z = F32(-F64(NaN32(pos.z)));
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f32 t = posA.z;
							posA.z = posB.z;
							posB.z = t;
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							pos.z = F32(-F64(NaN32(pos.z)));
						}
					}
				}
				else {
					const f64 w = F64(weight);
					const f64 _w = 1.0 - w;
					for (i32 i = 0; i < orderA.length(); ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						const bool s = selection[a];
						if (s != selection[b]) {
							if (s) {
								Point3& pos = mesh.P(a);
								const f64 z = F64(NaN32(pos.z));
								pos.z = F32(z * _w - z * w);
							}
							else {
								Point3& pos = mesh.P(b);
								const f64 z = F64(NaN32(pos.z));
								pos.z = F32(z * _w - z * w);
							}
						}
						else if (s) {
							Point3& posA = mesh.P(a);
							Point3& posB = mesh.P(b);
							const f64 az = F64(NaN32(posA.z));
							const f64 bz = F64(NaN32(posB.z));
							posA.z = F32(az * _w + bz * w);
							posB.z = F32(bz * _w + az * w);
						}
					}
					for (i32 i = 0; i < orderM.length(); ++i) {
						const i32 v = orderM[i] - 1;
						if (selection[v]) {
							Point3& pos = mesh.P(v);
							const f64 z = F64(NaN32(pos.z));
							pos.z = F32(z * _w - z * w);
						}
					}
				}
				break;
			}
			}
		}
		helper.changedGeom();
		if (holding) {
			rest->after();
			theHold.Put(rest);
		}
		helper.epoly().RefreshScreen();
		Util::redrawViews();
		return Codes::OK;
	}

public:
	static inline i32 copyPos(
		INode* src,
		const List<i32>& srcOrder,
		INode* dst,
		const List<i32>& dstOrder,
		const Bitset& selection
	) {
		using ERR = Codes::I_Order_copyPos;
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
		const MNMesh& meshB = pb.mesh();
		if (!Helpers::Geom::checkBounds(srcOrder, meshA.numv)) {
			return ERR::bad_vert_a;
		}
		if (!Helpers::Geom::checkBounds(dstOrder, meshB.numv)) {
			return ERR::bad_vert_b;
		}
		for (i32 i = srcOrder.length() - 1; i >= dstOrder.length(); --i) {
			if (selection[srcOrder[i] - 1]) {
				// vert selected in A has no addressable counterpart in B. Only possible if A.length() > B.length()
				return ERR::bad_selection;
			}
		}
		Defer<Restore::PosRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::PosRestore(pb));
			rest.get()->before();	// only those can change
		}
		if (std::addressof(meshA) == std::addressof(meshB)) {
			// self copy. need defensive copy
			List<Point3> copy(meshA.numv);
			pa.collectPositions(copy);
			for (i32 i = 0; i < srcOrder.length(); ++i) {
				const i32 a = srcOrder[i] - 1;
				if (selection[a]) {
					const i32 b = dstOrder[i] - 1;
					const Point3 posA = copy[a];
					Point3& posB = meshB.P(b);
					posB.x = posA.x;
					posB.y = posA.y;
					posB.z = posA.z;
				}
			}
		}
		else {
			for (i32 i = 0; i < srcOrder.length(); ++i) {
				const i32 a = srcOrder[i] - 1;
				if (selection[a]) {
					const i32 b = dstOrder[i] - 1;
					const Point3 posA = meshA.P(a);
					Point3& posB = meshB.P(b);
					posB.x = posA.x;
					posB.y = posA.y;
					posB.z = posA.z;
				}
			}
		}
		pb.changedGeom();
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		pb.epoly().RefreshScreen();
		Util::redrawViews();
		return Codes::OK;
	}

public:
	static inline i32 blendPos(
		INode* nodeA,
		const List<i32>& orderA,
		INode* nodeB,
		const List<i32>& orderB,
		const f32 weight,
		const f32 outA,
		const f32 outB,
		const Bitset& selectionA,
		const Bitset& selectionB,
		const bool useSelectionA,
		const bool useSelectionB
	) {
		using ERR = Codes::I_Order_blendPos;
		i32 r;
		Helpers::Poly pa;
		r = pa.init(nodeA);
		if (r != Codes::OK) {
			return ERR::resolvePolySrc(r);
		}
		Helpers::Poly pb;
		r = pb.init(nodeB);
		if (r != Codes::OK) {
			return ERR::resolvePolyDst(r);
		}
		if (orderA.length() != orderB.length()) {
			return ERR::different_lengths;
		}
		MNMesh& meshA = pa.mesh();
		if (!Helpers::Geom::checkBounds(orderA, meshA.numv)) {
			return ERR::bad_vert_a;
		}
		MNMesh& meshB = pb.mesh();
		if (!Helpers::Geom::checkBounds(orderB, meshB.numv)) {
			return ERR::bad_vert_b;
		}
		if (!useSelectionA && !useSelectionB) {
			// nothing is selected
			return Codes::OK;
		}
		const i32 len = orderA.length();
		Defer<Restore::PosRestore> restA;
		Defer<Restore::PosRestore> restB;
		const bool holding = theHold.Holding();
		if (holding) {
			restA.set(new Restore::PosRestore(pa));
			restA.get()->before();
			restB.set(new Restore::PosRestore(pb));
			restB.get()->before();
		}
		const bool gt0A = outA >= 0.000002f;
		const bool gt0B = outB >= 0.000002f;
		if (gt0A || gt0B) {
			const bool eq1A = outA >= 0.999998f;
			const bool eq1B = outB >= 0.999998f;
			const f64 outA64 = getOut(outA, gt0A, eq1A);
			const f64 outB64 = getOut(outB, gt0B, eq1B);
			const f64 _outA64 = 1.0 - outA64;
			const f64 _outB64 = 1.0 - outB64;
			if (weight <= 0.000002f) {
				// w = 0
				if (gt0A) {
					if (useSelectionA == useSelectionB) {
						for (i32 i = 0; i < len; ++i) {
							const i32 a = orderA[i] - 1;
							const i32 b = orderB[i] - 1;
							if (selectionA[a] || selectionB[b]) {
								blendPos0(meshA, a, meshB, b, eq1A, outA64, _outA64);
							}
						}
					}
					else if (useSelectionA) {
						for (i32 i = 0; i < len; ++i) {
							const i32 a = orderA[i] - 1;
							if (selectionA[a]) {
								const i32 b = orderB[i] - 1;
								blendPos0(meshA, a, meshB, b, eq1A, outA64, _outA64);
							}
						}
					}
					else {
						for (i32 i = 0; i < len; ++i) {
							const i32 b = orderB[i] - 1;
							if (selectionB[b]) {
								const i32 a = orderA[i] - 1;
								blendPos0(meshA, a, meshB, b, eq1A, outA64, _outA64);
							}
						}
					}
				}
			}
			else if (weight >= 0.999998f) {
				// w = 1
				if (gt0B) {
					if (useSelectionA == useSelectionB) {
						for (i32 i = 0; i < len; ++i) {
							const i32 a = orderA[i] - 1;
							const i32 b = orderB[i] - 1;
							if (selectionA[a] || selectionB[b]) {
								blendPos1(meshA, a, meshB, b, eq1B, outB64, _outB64);
							}
						}
					}
					else if (useSelectionA) {
						for (i32 i = 0; i < len; ++i) {
							const i32 a = orderA[i] - 1;
							if (selectionA[a]) {
								const i32 b = orderB[i] - 1;
								blendPos1(meshA, a, meshB, b, eq1B, outB64, _outB64);
							}
						}
					}
					else {
						for (i32 i = 0; i < len; ++i) {
							const i32 b = orderB[i] - 1;
							if (selectionB[b]) {
								const i32 a = orderA[i] - 1;
								blendPos1(meshA, a, meshB, b, eq1B, outB64, _outB64);
							}
						}
					}
				}
			}
			else {
				const f64 w = F64(NaN32(weight));
				const f64 _w = 1.0 - w;
				if (useSelectionA == useSelectionB) {
					for (i32 i = 0; i < len; ++i) {
						const i32 a = orderA[i] - 1;
						const i32 b = orderB[i] - 1;
						if (selectionA[a] || selectionB[b]) {
							blendPos(meshA, a, meshB, b, w, _w, gt0A, eq1A, gt0B, eq1B, outA64, _outA64, outB64, _outB64);
						}
					}
				}
				else if (useSelectionA) {
					for (i32 i = 0; i < len; ++i) {
						const i32 a = orderA[i] - 1;
						if (selectionA[a]) {
							const i32 b = orderB[i] - 1;
							blendPos(meshA, a, meshB, b, w, _w, gt0A, eq1A, gt0B, eq1B, outA64, _outA64, outB64, _outB64);
						}
					}
				}
				else {
					for (i32 i = 0; i < len; ++i) {
						const i32 b = orderB[i] - 1;
						if (selectionB[b]) {
							const i32 a = orderA[i] - 1;
							blendPos(meshA, a, meshB, b, w, _w, gt0A, eq1A, gt0B, eq1B, outA64, _outA64, outB64, _outB64);
						}
					}
				}
			}
		}
		pa.changedGeom();
		pb.changedGeom();
		if (holding) {
			restA.get()->after();
			restB.get()->after();
			bool x = true;
			x = Restore::put(x, restA);
			x = Restore::put(x, restB);
		}
		pa.epoly().RefreshScreen();
		pb.epoly().RefreshScreen();
		Util::redrawViews();
		return Codes::OK;
	}

private:
	static inline f64 getOut(const f32 out, const bool gt0, const bool eq1) {
		if (out != out || !gt0) {
			return 0.0;
		}
		if (eq1) {
			return 1.0;
		}
		return F64(out);
	}

private:
	static inline void blendPos(
		MNMesh& meshA, const i32 a,
		MNMesh& meshB, const i32 b,
		const f64 w, const f64 _w,
		const bool gt0A, const bool eq1A,
		const bool gt0B, const bool eq1B,
		const f64 outA64, const f64 _outA64,
		const f64 outB64, const f64 _outB64
	) {
		Point3& posA = meshA.P(a);
		Point3& posB = meshB.P(b);

		const f64 ax = F64(NaN32(posA.x));
		const f64 ay = F64(NaN32(posA.y));
		const f64 az = F64(NaN32(posA.z));

		const f64 bx = F64(NaN32(posB.x));
		const f64 by = F64(NaN32(posB.y));
		const f64 bz = F64(NaN32(posB.z));

		const f64 x = (ax * w + bx * _w);
		const f64 y = (ay * w + by * _w);
		const f64 z = (az * w + bz * _w);

		if (eq1A) {
			posA.x = F32(x);
			posA.y = F32(y);
			posA.z = F32(z);
		}
		else if (gt0A) {
			posA.x = F32(ax * _outA64 + x * outA64);
			posA.y = F32(ay * _outA64 + y * outA64);
			posA.z = F32(az * _outA64 + z * outA64);
		}
		if (eq1B) {
			posB.x = F32(x);
			posB.y = F32(y);
			posB.z = F32(z);
		}
		else if (gt0B) {
			posB.x = F32(bx * _outB64 + x * outB64);
			posB.y = F32(by * _outB64 + y * outB64);
			posB.z = F32(bz * _outB64 + z * outB64);
		}
	}

private:
	static inline void blendPos0(
		MNMesh& meshA, const i32 a,
		MNMesh& meshB, const i32 b,
		const bool eq1A,
		const f64 outA64, const f64 _outA64
	) {
		Point3& posA = meshA.P(a);
		const Point3& posB = meshB.P(b);

		if (eq1A) {
			posA.x = posB.x;
			posA.y = posB.y;
			posA.z = posB.z;
		}
		else {
			const f64 ax = F64(NaN32(posA.x));
			const f64 ay = F64(NaN32(posA.y));
			const f64 az = F64(NaN32(posA.z));
			const f64 bx = F64(NaN32(posB.x));
			const f64 by = F64(NaN32(posB.y));
			const f64 bz = F64(NaN32(posB.z));
			posA.x = F32(ax * _outA64 + bx * outA64);
			posA.y = F32(ay * _outA64 + by * outA64);
			posA.z = F32(az * _outA64 + bz * outA64);
		}
	}

private:
	static inline void blendPos1(
		const MNMesh& meshA, const i32 a,
		MNMesh& meshB, const i32 b,
		const bool eq1B,
		const f64 outB64, const f64 _outB64
	) {
		const Point3& posA = meshA.P(a);
		Point3& posB = meshB.P(b);

		if (eq1B) {
			posB.x = posA.x;
			posB.y = posA.y;
			posB.z = posA.z;
		}
		else {
			const f64 ax = F64(NaN32(posA.x));
			const f64 ay = F64(NaN32(posA.y));
			const f64 az = F64(NaN32(posA.z));
			const f64 bx = F64(NaN32(posB.x));
			const f64 by = F64(NaN32(posB.y));
			const f64 bz = F64(NaN32(posB.z));
			posB.x = F32(bx * _outB64 + ax * outB64);
			posB.y = F32(by * _outB64 + ay * outB64);
			posB.z = F32(bz * _outB64 + az * outB64);
		}
	}

public:
	static inline i32 extractSymmetrySelection(
		INode* node,
		const Bitset& faces,
		const Bitset& edges,
		const Bitset& verts,
		i32& faceA,
		i32& edgeA,
		i32& vertA,
		i32& faceB,
		i32& edgeB,
		i32& vertB,
		const i32 axis
	) {
		using ERR = Codes::I_Order_extractSymmetrySelection;
		Helpers::Poly helper;
		i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = helper.mesh();
		const i32 numF = mesh.numf;
		Bitset patchFaces(numF);
		List<i32> patch(Util::mathMax(Util::mathMin(256, numF), numF >> 3));
		i32 _faceA = 0;
		i32 _eIdxA = 0;
		i32 _vIdxA = 0;
		i32 _faceB = 0;
		i32 _eIdxB = 0;
		i32 _vIdxB = 0;
		r = extractOrderSelection(
			mesh, faces, edges, verts, patchFaces, patch,
			_faceA, _eIdxA, _vIdxA, _faceB, _eIdxB, _vIdxB, true
		);
		if (r < 0) {
			return ERR::resolveExtra(r);
		}
		// make a rough guess as to which side is probably left/right.
		const MNFace& a = mesh.f[_faceA];
		const MNFace& b = mesh.f[_faceB];
		bool xyz;
		switch (axis) {
		case 0: {
			xyz = helper.avgPosX(_faceA) > helper.avgPosX(_faceB);
			break;
		}
		case 1: {
			xyz = helper.avgPosY(_faceA) > helper.avgPosY(_faceB);
			break;
		}
		default: {
			xyz = helper.avgPosZ(_faceA) > helper.avgPosZ(_faceB);
			break;
		}
		}
		if (xyz) {
			// a is right side
			faceA = _faceB + 1;
			edgeA = b.edg[_eIdxB] + 1;
			vertA = b.vtx[_vIdxB] + 1;
			faceB = _faceA + 1;
			edgeB = a.edg[_eIdxA] + 1;
			vertB = a.vtx[_vIdxA] + 1;
		}
		else {
			// b is right side
			faceA = _faceA + 1;
			edgeA = a.edg[_eIdxA] + 1;
			vertA = a.vtx[_vIdxA] + 1;
			faceB = _faceB + 1;
			edgeB = b.edg[_eIdxB] + 1;
			vertB = b.vtx[_vIdxB] + 1;
		}
		return Codes::OK;
	}

public:
	static inline i32 extractTraversalSelection(
		INode* node,
		const Bitset& faces,
		const Bitset& edges,
		const Bitset& verts,
		i32& face,
		i32& edge,
		i32& vert
	) {
		using ERR = Codes::I_Order_extractTraversalSelection;
		Helpers::Poly helper;
		i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = helper.mesh();
		const i32 numF = mesh.numf;
		Bitset patchFaces(numF);
		List<i32> patch(Util::mathMax(Util::mathMin(256, numF), numF >> 3));
		i32 _face = 0, _eIdx = 0, _vIdx = 0;
		r = extractOrderSelection(
			mesh, faces, edges, verts, patchFaces, patch,
			_face, _eIdx, _vIdx, _face, _eIdx, _vIdx, false
		);
		if (r < 0) {
			return ERR::resolveExtra(r);
		}
		const MNFace& f = mesh.f[_face];
		face = _face + 1;
		edge = f.edg[_eIdx] + 1;
		vert = f.vtx[_vIdx] + 1;
		return Codes::OK;
	}

private:
	static inline i32 extractOrderSelection(
		const MNMesh& mesh,
		const Bitset& faces, const Bitset& edges, const Bitset& verts,
		Bitset& patchFaces, List<i32>& patch,
		i32& faceA, i32& edgeAIdx, i32& vertAIdx,
		i32& faceB, i32& edgeBIdx, i32& vertBIdx,
		const bool symmetrySelection
	) {
		patchFaces.clearAll();
		const i32 numF = mesh.numf;
		const i32 numE = mesh.nume;
		const i32 numV = mesh.numv;

		i32 edge1Count = 0;

		i32 edge0Count = 0;
		i32 edge0 = -1;

		i32 edge2Count = 0;
		i32 edge2 = -1;

		i32 border2Count = 0;
		i32 border2 = -1;

		i32 patchIdA = -1;
		i32 patchIdB = -1;

		for (i32 p = 0; p < numF; ++p) {
			if (!patchFaces[p] && faces[p] && !Helpers::Geom::isFaceFlagged(mesh, p, MN_DEAD)) {
				patchFaces.set(p);
				patch.push(p);
				// start of a new patch
				do {
					i32 fEdge0Count = 0;
					i32 fBorder0Count = 0;
					i32 fEdge2Count = 0;
					i32 fBorder2Count = 0;
					i32 fEdge1Count = 0;
					i32 fEdge1EIdxA = -1;
					i32 fEdge1EIdxB = -1;
					i32 fEdge1VIdxA = -1;
					i32 fEdge1VIdxB = -1;
					bool fEdge1BorderA = false;
					const i32 f = patch.pop();
					const MNFace& face = mesh.f[f];
					const i32 deg = face.deg;
					if (deg < 3) {
						// face has too few edges
						return Codes::errors_extractOrderSelection::broken_mesh;
					}
					for (i32 i = 0; i < deg; ++i) {
						const i32 e = face.edg[i];
						if (e < 0 || e >= numE) {
							return Codes::errors_extractOrderSelection::broken_mesh;
						}
						const MNEdge& edge = mesh.e[e];
						if (edge.v1 < 0 || edge.v1 >= numV || edge.v2 < 0 || edge.v2 >= numV) {
							return Codes::errors_extractOrderSelection::broken_mesh;
						}
						if (edge.GetFlag(MN_DEAD)
							|| Helpers::Geom::isVertFlagged(mesh, edge.v1, MN_DEAD)
							|| Helpers::Geom::isVertFlagged(mesh, edge.v2, MN_DEAD)) {
							return Codes::errors_extractOrderSelection::zombie_mesh;
						}
						const i32 f2 = Helpers::Geom::otherFace(edge, f);
						if (f2 >= 0 && faces[f2]) {
							// has opposing face, not a border
							if (!patchFaces[f2]) {
								if (Helpers::Geom::isFaceFlagged(mesh, f2, MN_DEAD)) {
									return Codes::errors_extractOrderSelection::zombie_mesh;
								}
								patchFaces.set(f2);
								patch.push(f2);
							}
							if (edges[e]) {
								// selected non-border edge
								const bool s1 = verts[edge.v1];
								const bool s2 = verts[edge.v2];
								if (s1 == s2) {
									if (s1) {
										// both edge verts selected
										edge2 = e;
										++fEdge2Count;
									}
									else {
										// neither edge vert selected
										edge0 = e;
										++fEdge0Count;
									}
								}
								else {
									// edge has one vert selected
									const i32 v = s1 ? edge.v1 : edge.v2;
									if (fEdge1Count == 0) {
										fEdge1BorderA = false;	// edge is not a border
										fEdge1EIdxA = i;
										fEdge1VIdxA = Helpers::Geom::vertIndexOrdered(face, i, v);
									}
									else {
										fEdge1EIdxB = i;
										fEdge1VIdxB = Helpers::Geom::vertIndexOrdered(face, i, v);
									}
									++fEdge1Count;
								}
							}
						}
						else if (edges[e]) {
							// selected border edge
							const bool s1 = verts[edge.v1];
							const bool s2 = verts[edge.v2];
							if (s1 == s2) {
								if (s1) {
									// both border verts selected
									border2 = e;
									++fBorder2Count;
								}
								else {
									// neither border vert selected
									++fBorder0Count;
								}
							}
							else {
								// one border vert selected
								const i32 v = s1 ? edge.v1 : edge.v2;
								if (fEdge1Count == 0) {
									fEdge1BorderA = true;	// edge is a border
									fEdge1EIdxA = i;
									fEdge1VIdxA = Helpers::Geom::vertIndexOrdered(face, i, v);
								}
								else {
									fEdge1EIdxB = i;
									fEdge1VIdxB = Helpers::Geom::vertIndexOrdered(face, i, v);
								}
								++fEdge1Count;
							}
						}
					}
					// integrate face patterns into patch patterns
					if (fEdge1Count == 1 && (fEdge1BorderA || fEdge0Count != 0 || fBorder0Count != 0 || fEdge2Count != 0 || fBorder2Count != 0)) {
						// face has one selected edge with 1 selected vert
						// either we don't need or we do have additional edges specifying this face
						if (edge1Count == 0) {
							faceA = f;
							edgeAIdx = fEdge1EIdxA;
							vertAIdx = fEdge1VIdxA;
							patchIdA = p;
						}
						else {
							faceB = f;
							edgeBIdx = fEdge1EIdxA;
							vertBIdx = fEdge1VIdxA;
							patchIdB = p;
						}
						++edge1Count;
					}
					else if (symmetrySelection && fEdge1Count == 2 && (fEdge1EIdxA == fEdge1VIdxA) != (fEdge1EIdxB == fEdge1VIdxB)) {
						// face has two selected edges with 1 selected vert each, not needing/considering a specifying edge at all
						// both edge/vert-pairs go in opposite directions.
						if (edge1Count == 0) {
							faceA = f;
							edgeAIdx = fEdge1EIdxA;
							vertAIdx = fEdge1VIdxA;
							faceB = f;
							edgeBIdx = fEdge1EIdxB;
							vertBIdx = fEdge1VIdxB;
						}
						edge1Count += 2;
					}
					// update shortcut patterns
					edge0Count += fEdge0Count;
					edge2Count += fEdge2Count;
					border2Count += fBorder2Count;
					// process next face
				} while (!patch.isEmpty());
				// patch fully processed
			}
		}
		// all patches done. Evaluate collected patterns.
		if (symmetrySelection) {
			if (edge1Count != 2) {
				// check for shortcuts
				if (edge0Count == 2) {
					if (edge2Count != 0 || border2Count != 0) {
						return Codes::errors_extractOrderSelection::ambivalent_selection;
					}
					// symmetry axis shortcut
					// the same non-border edge has been seen once by each if its faces
					const MNEdge& _edge = mesh.e[edge0];
					const MNFace& _faceA = mesh.f[_edge.f1];
					const MNFace& _faceB = mesh.f[_edge.f2];
					const i32 v = _edge.v1;
					faceA = _edge.f1;
					faceB = _edge.f2;
					edgeAIdx = Helpers::Geom::edgeIndex(_faceA, edge0);
					edgeBIdx = Helpers::Geom::edgeIndex(_faceB, edge0);
					vertAIdx = Helpers::Geom::vertIndexOrdered(_faceA, edgeAIdx, v);
					vertBIdx = Helpers::Geom::vertIndexOrdered(_faceB, edgeBIdx, v);
					return Codes::OK;
				}
				if (edge2Count == 2) {
					if (edge0Count != 0 || border2Count != 0) {
						return Codes::errors_extractOrderSelection::ambivalent_selection;
					}
					// edge perpendicular to symmetry axis shortcut
					const MNEdge& _edge = mesh.e[edge2];
					const i32 f = _edge.f1;
					const MNFace& _face = mesh.f[f];
					const i32 eIdx = Helpers::Geom::edgeIndex(_face, edge2);
					faceA = f;
					faceB = f;
					edgeAIdx = eIdx;
					edgeBIdx = eIdx;
					vertAIdx = Helpers::Geom::vertIndexOrdered(_face, eIdx, _edge.v1);
					vertBIdx = Helpers::Geom::vertIndexOrdered(_face, eIdx, _edge.v2);
					return Codes::OK;
				}
				if (border2Count == 1) {
					if (edge0Count != 0 || edge2Count != 0) {
						return Codes::errors_extractOrderSelection::ambivalent_selection;
					}
					// border perpendicular to symmetry axis shortcut
					const MNEdge& _edge = mesh.e[border2];
					const i32 f = faces[_edge.f1] ? _edge.f1 : _edge.f2;
					const MNFace& _face = mesh.f[f];
					const i32 eIdx = Helpers::Geom::edgeIndex(_face, border2);
					faceA = f;
					faceB = f;
					edgeAIdx = eIdx;
					edgeBIdx = eIdx;
					vertAIdx = Helpers::Geom::vertIndexOrdered(_face, eIdx, _edge.v1);
					vertBIdx = Helpers::Geom::vertIndexOrdered(_face, eIdx, _edge.v2);
					return Codes::OK;
				}
				return edge1Count < 2
					? Codes::errors_extractOrderSelection::insufficient_selection
					: Codes::errors_extractOrderSelection::ambivalent_selection;
			}
			else if (patchIdA == patchIdB && (edgeAIdx == vertAIdx) == (edgeBIdx == vertBIdx)) {
				// faces are part of the same patch. but the selection is not turning in the same direction
				return Codes::errors_extractOrderSelection::ambivalent_selection;
			}
		}
		else {
			if (edge1Count < 1) {
				return Codes::errors_extractOrderSelection::insufficient_selection;
			}
			if (edge1Count > 1) {
				return Codes::errors_extractOrderSelection::ambivalent_selection;
			}
		}
		return Codes::OK;
	}

public:
	static inline i32 buildSymmetryOrder(
		INode* node,
		const Bitset& faces,
		i32 faceA,
		i32 edgeA,
		i32 vertA,
		i32 faceB,
		i32 edgeB,
		i32 vertB,
		List<i32>& orderM,
		List<i32>& orderA,
		List<i32>& orderB,
		const i32 level
	) {
		using ERR = Codes::I_Order_buildSymmetryOrder;
		Helpers::Poly helper;
		i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = helper.mesh();

		orderM.clearAll();
		orderA.clearAll();
		orderB.clearAll();

		if (!checkSelection(mesh, faces, faceA, edgeA, vertA) ||
			!checkSelection(mesh, faces, faceB, edgeB, vertB)) {
			return ERR::bad_selection;
		}
		List<i32> edgesA(64);
		List<i32> vertsA(64);
		List<i32> edgesB(64);
		List<i32> vertsB(64);
		Bitset patchA(mesh.numf);
		Bitset patchB(mesh.numf);
		Bitset facesDone(mesh.numf + 1);
		facesDone.set(0);	// include -1
		if (collectPatch(mesh, faces, faceA, patchA, edgesA) != collectPatch(mesh, faces, faceB, patchB, edgesA)) {
			// collect all faces for each side, used later to abort early if a bad middle transition occurs
			return ERR::order_mismatch;
		}
		// If A and B never meet, we can use a faster algorithm.
		if (!patchA.intersects(patchB)) {
			// A side will never traverse into B side
			// some verts might still meet in the middle, though.
			Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
			Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
			if (edgesA.length() != edgesB.length()) {
				return ERR::order_mismatch;
			}
			switch (level) {
			case 0: {
				// vert order
				orderM.ensureCapacity(mesh.numv >> 10);	// should be very rare
				orderA.ensureCapacity(mesh.numv >> 3);
				orderB.ensureCapacity(mesh.numv >> 3);
				Bitset done(mesh.numv);
				do {
					edgeA = edgesA.pop();
					vertA = vertsA.pop();
					edgeB = edgesB.pop();
					vertB = vertsB.pop();
					if (done.trySet(vertA)) {
						if (vertA == vertB) {
							// verts met in the middle
							orderM.push(vertA + 1);
						}
						else {
							if (!done.trySet(vertB)) {
								return ERR::order_mismatch;
							}
							orderA.push(vertA + 1);
							orderB.push(vertB + 1);
						}
					}
					else if (!done[vertB]) {
						// a is done, but b is not done
						return ERR::order_mismatch;
					}
					faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
					faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
					const bool a = faceA >= 0;
					const bool b = faceB >= 0;
					if (a != b) {
						return ERR::order_mismatch;
					}
					if (a) {
						Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
						Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
					}
					if (edgesA.length() != edgesB.length()) {
						return ERR::order_mismatch;
					}
				} while (!edgesA.isEmpty());
				break;
			}
			case 1: {
				// edge order
				orderA.ensureCapacity(mesh.nume >> 3);
				orderB.ensureCapacity(mesh.nume >> 3);
				Bitset done(mesh.nume);
				do {
					edgeA = edgesA.pop();
					vertA = vertsA.pop();
					edgeB = edgesB.pop();
					vertB = vertsB.pop();
					if (done.trySet(edgeA)) {
						if (!done.trySet(edgeB)) {
							return ERR::order_mismatch;
						}
						orderA.push(edgeA + 1);
						orderB.push(edgeB + 1);
					}
					else if (!done[vertB]) {
						// a is done, but b is not done
						return ERR::order_mismatch;
					}
					faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
					faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
					const bool a = faceA >= 0;
					const bool b = faceB >= 0;
					if (a != b) {
						return ERR::order_mismatch;
					}
					if (a) {
						Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
						Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
					}
					if (edgesA.length() != edgesB.length()) {
						return ERR::order_mismatch;
					}
				} while (!edgesA.isEmpty());
				break;
			}
			default: {
				// face order
				orderA.ensureCapacity(mesh.numf >> 3);
				orderA.push(faceA + 1);
				orderB.ensureCapacity(mesh.numf >> 3);
				orderB.push(faceB + 1);
				do {
					edgeA = edgesA.pop();
					vertA = vertsA.pop();
					edgeB = edgesB.pop();
					vertB = vertsB.pop();
					faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
					faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
					const bool a = faceA >= 0;
					const bool b = faceB >= 0;
					if (a != b) {
						return ERR::order_mismatch;
					}
					if (a) {
						orderA.push(faceA + 1);
						orderB.push(faceB + 1);
						Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
						Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
					}
					if (edgesA.length() != edgesB.length()) {
						return ERR::order_mismatch;
					}
				} while (!edgesA.isEmpty());
				break;
			}
			}
			return Codes::OK;
		}
		// A and B will meet at some point and must be prevented from traversing into one another.
		// patchA and patchB can be used to determine more asymmetries
		switch (level) {
		case 0: {
			// vert order
			orderM.ensureCapacity(mesh.numv >> 8);
			orderA.ensureCapacity(mesh.numv >> 3);
			orderB.ensureCapacity(mesh.numv >> 3);
			Bitset done(mesh.numv);
			while (true) {
				if (faceA == faceB) {
					// handle traversal order issues with shared face. 
					// e.g. with ladder-shaped meshes, you must make sure that traversal always goes 
					// outward from the middle in order to prevent sides bleeding into one another.
					if (faceA >= 0 && !symSharedFaceV(mesh, facesDone, edgesA, edgesB, vertsA, vertsB, faceA, edgeA, vertA, edgeB, vertB, done, orderM)) {
						return ERR::order_mismatch;
					}
					// else: both are -1. processed already
				}
				else {
					if (faceA < 0 || faceB < 0) {
						return ERR::order_mismatch;
					}
					if (patchA[faceB] != patchB[faceA]) {
						// faceA bled into side B, but faceB didn't bleed into side A. or vice versa
						return ERR::order_mismatch;
					}
					Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
					Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
				}
				if (edgesA.length() != edgesB.length()) {
					return ERR::order_mismatch;
				}
				if (edgesA.isEmpty()) {
					return Codes::OK;
				}
				edgeA = edgesA.pop();
				edgeB = edgesB.pop();
				vertA = vertsA.pop();
				vertB = vertsB.pop();
				if (done.trySet(vertA)) {
					if (vertA == vertB) {
						orderM.push(vertA + 1);
					}
					else {
						if (!done.trySet(vertB)) {
							return ERR::order_mismatch;
						}
						orderA.push(vertA + 1);
						orderB.push(vertB + 1);
					}
				}
				faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
				faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
			}
			break;
		}
		case 1: {
			// edge order
			orderM.ensureCapacity(mesh.nume >> 8);
			orderA.ensureCapacity(mesh.nume >> 3);
			orderB.ensureCapacity(mesh.nume >> 3);
			Bitset done(mesh.nume);
			while (true) {
				if (faceA == faceB) {
					if (faceA >= 0 && !symSharedFaceFE(mesh, facesDone, edgesA, edgesB, vertsA, vertsB, faceA, edgeA, vertA, edgeB, vertB)) {
						return ERR::order_mismatch;
					}
				}
				else {
					if (faceA < 0 || faceB < 0) {
						return ERR::order_mismatch;
					}
					if (patchA[faceB] != patchB[faceA]) {
						// faceA bled into side B, but faceB didn't bleed into side A. or vice versa
						return ERR::order_mismatch;
					}
					Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
					Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
				}
				if (edgesA.length() != edgesB.length()) {
					return ERR::order_mismatch;
				}
				if (edgesA.isEmpty()) {
					return Codes::OK;
				}
				edgeA = edgesA.pop();
				edgeB = edgesB.pop();
				vertA = vertsA.pop();
				vertB = vertsB.pop();
				if (done.trySet(edgeA)) {
					if (edgeA == edgeB) {
						orderM.push(edgeA + 1);
					}
					else {
						if (!done.trySet(edgeB)) {
							return ERR::order_mismatch;
						}
						orderA.push(edgeA + 1);
						orderB.push(edgeB + 1);
					}
				}
				faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
				faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
			}
			break;
		}
		default: {
			orderM.ensureCapacity(mesh.numf >> 8);
			orderA.ensureCapacity(mesh.numf >> 3);
			orderB.ensureCapacity(mesh.numf >> 3);
			while (true) {
				if (faceA == faceB) {
					if (faceA >= 0) {
						orderM.push(faceA + 1);
						if (!symSharedFaceFE(mesh, facesDone, edgesA, edgesB, vertsA, vertsB, faceA, edgeA, vertA, edgeB, vertB)) {
							return ERR::order_mismatch;
						}
					}
				}
				else {
					if (faceA < 0 || faceB < 0) {
						return ERR::order_mismatch;
					}
					if (patchA[faceB] != patchB[faceA]) {
						// faceA bled into side B, but faceB didn't bleed into side A. or vice versa
						return ERR::order_mismatch;
					}
					orderA.push(faceA + 1);
					orderB.push(faceB + 1);
					Helpers::Geom::collectFace(mesh, edgesA, vertsA, facesDone, faceA, edgeA, vertA);
					Helpers::Geom::collectFace(mesh, edgesB, vertsB, facesDone, faceB, edgeB, vertB);
				}
				if (edgesA.length() != edgesB.length()) {
					return ERR::order_mismatch;
				}
				if (edgesA.isEmpty()) {
					return Codes::OK;
				}
				edgeA = edgesA.pop();
				edgeB = edgesB.pop();
				vertA = vertsA.pop();
				vertB = vertsB.pop();
				faceA = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeA, faces);
				faceB = Helpers::Geom::nextEdgeFace(mesh, facesDone, edgeB, faces);
			}
			break;
		}
		}
		return Codes::OK;
	}

public:
	static inline i32 buildTraversalOrder(
		INode* node,
		const Bitset& faces,
		const i32 face,
		const i32 edge,
		const i32 vert,
		List<i32>& order,
		const i32 level
	) {
		using ERR = Codes::I_Order_buildTraversalOrder;
		Helpers::Poly helper;
		i32 r = helper.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		const MNMesh& mesh = helper.mesh();
		order.clearAll();
		if (!checkSelection(mesh, faces, face, edge, vert)) {
			return ERR::bad_selection;
		}
		Bitset facesDone(mesh.numf + 1);
		facesDone.set(0);	// include -1
		List<i32> edges(64);
		List<i32> verts(64);
		Helpers::Geom::collectFace(mesh, edges, verts, facesDone, face, edge, vert);
		switch (level) {
		case 0: {
			// vert order
			order.ensureCapacity(mesh.numv >> 3);
			Bitset done(mesh.numv);
			do {
				const i32 edge = edges.pop();
				const i32 vert = verts.pop();
				if (done.trySet(vert)) {
					order.push(vert + 1);
				}
				const i32 next = Helpers::Geom::nextEdgeFace(mesh, facesDone, edge, faces);
				if (next >= 0) {
					Helpers::Geom::collectFace(mesh, edges, verts, facesDone, next, edge, vert);
				}
			} while (!edges.isEmpty());
			break;
		}
		case 1: {
			// edge order
			order.ensureCapacity(mesh.nume >> 3);
			Bitset done(mesh.nume);
			do {
				const i32 edge = edges.pop();
				const i32 vert = verts.pop();
				if (done.trySet(edge)) {
					order.push(edge + 1);
				}
				const i32 next = Helpers::Geom::nextEdgeFace(mesh, facesDone, edge, faces);
				if (next >= 0) {
					Helpers::Geom::collectFace(mesh, edges, verts, facesDone, next, edge, vert);
				}
			} while (!edges.isEmpty());
			break;
		}
		default: {
			// face order
			order.ensureCapacity(mesh.numf >> 3);
			order.push(face + 1);
			do {
				const i32 edge = edges.pop();
				const i32 vert = verts.pop();
				const i32 next = Helpers::Geom::nextEdgeFace(mesh, facesDone, edge, faces);
				if (next >= 0) {
					order.push(next + 1);
					Helpers::Geom::collectFace(mesh, edges, verts, facesDone, next, edge, vert);
				}
			} while (!edges.isEmpty());
			break;
		}
		}
		return Codes::OK;
	}

	// grows the face by edge-neighbor, collects all faces in "patch"
private:
	static inline i32 collectPatch(
		const MNMesh& mesh,
		const Bitset& bits,
		const i32 start,
		Bitset& patch,
		List<i32>& temp
	) {
		temp.clearAll();
		i32 c = 0;
		patch.set(start);
		temp.push(start);
		do {
			++c;
			const i32 f = temp.pop();
			const MNFace& face = mesh.f[f];
			for (i32 i = 0; i < face.deg; ++i) {
				const i32 f2 = Helpers::Geom::otherFace(mesh, face.edg[i], f);
				if (f2 >= 0 && !patch[f2] && bits[f2]) {
					temp.push(f2);
					patch.set(f2);
				}
			}
		} while (!temp.isEmpty());
		return c;
	}

	// handle shared face for vert order
private:
	static inline bool symSharedFaceV(
		const MNMesh& mesh,
		Bitset& facesDone,
		List<i32>& edgesA,
		List<i32>& edgesB,
		List<i32>& vertsA,
		List<i32>& vertsB,
		const i32 f,
		i32 edgeA,
		const i32 vertA,
		const i32 edgeB,
		const i32 vertB,
		Bitset& verts,
		List<i32>& orderM
	) {
		const MNFace& face = mesh.f[f];
		const i32 deg = face.deg;
		i32 eIdxA = Helpers::Geom::edgeIndex(face, edgeA);
		i32 eIdxB = Helpers::Geom::edgeIndex(face, edgeB);
		i32 vIdxA = Helpers::Geom::vertIndexOrdered(face, eIdxA, vertA);
		i32 vIdxB = Helpers::Geom::vertIndexOrdered(face, eIdxB, vertB);
		if (edgeA != edgeB) {
			// If the edges are immediately shared, don't reverse direction.
			// This case can only be arrived at from another shared face. Let that transition reverse direction instead.
			// Otherwise, we'd need additional context, here.
			bool match = false;
			for (i32 i = 0; i < deg; ++i) {
				if (eIdxA == eIdxB || vIdxA == vIdxB) {
					if (vIdxA == vIdxB) {
						// have to do this, or this middle vert could be skipped if it lies on two "borders"
						const i32 v = face.vtx[vIdxA];
						if (verts.trySet(v)) {
							orderM.push(v + 1);
						}
					}
					// reverse direction. this prevents opposing sides from bleeding into one another
					vIdxA = Helpers::Geom::reverse(mesh, face, eIdxA, vIdxA);
					vIdxB = Helpers::Geom::reverse(mesh, face, eIdxB, vIdxB);
					match = true;
					break;
				}
				Helpers::Geom::nextEdgeIdx(deg, eIdxA, vIdxA);
				Helpers::Geom::nextEdgeIdx(deg, eIdxB, vIdxB);
			}
			if (!match) {
				return false;
			}
		}
		edgesA.push(face.edg[eIdxA]);
		edgesB.push(face.edg[eIdxB]);
		vertsA.push(face.vtx[vIdxA]);
		vertsB.push(face.vtx[vIdxB]);
		bool match = false;
		// TODO maybe it should not be deg. deg describes ALL of it, not half of it.
		// however we'd have to check if they are "out of sync" or they might never meet. 
		for (i32 i = 0; i < deg; ++i) {
			Helpers::Geom::nextEdgeIdx(deg, eIdxA, vIdxA);
			Helpers::Geom::nextEdgeIdx(deg, eIdxB, vIdxB);
			edgesA.push(face.edg[eIdxA]);
			edgesB.push(face.edg[eIdxB]);
			if (eIdxA == eIdxB) {
				// Push closing shared edge in reverse direction.
				// Ensures that "ladder"-like meshes can be traversed fully and correctly, by always traversing outward.
				vertsA.push(face.vtx[Helpers::Geom::reverse(mesh, face, eIdxA, vIdxA)]);
				vertsB.push(face.vtx[Helpers::Geom::reverse(mesh, face, eIdxB, vIdxB)]);
				match = true;
				break;
			}
			vertsA.push(face.vtx[vIdxA]);
			vertsB.push(face.vtx[vIdxB]);
			if (vIdxA == vIdxB) {
				match = true;
				break;
			}
		}
		if (!match) {
			// the two pairs are out of sync
			return false;
		}
		facesDone.set(f + 1);
		return true;
	}

	// handle shared face for face or edge order
private:
	static inline bool symSharedFaceFE(
		const MNMesh& mesh,
		Bitset& facesDone,
		List<i32>& edgesA,
		List<i32>& edgesB,
		List<i32>& vertsA,
		List<i32>& vertsB,
		const i32 f,
		i32 edgeA,
		const i32 vertA,
		const i32 edgeB,
		const i32 vertB
	) {
		const MNFace& face = mesh.f[f];
		const i32 deg = face.deg;
		i32 eIdxA = Helpers::Geom::edgeIndex(face, edgeA);
		i32 eIdxB = Helpers::Geom::edgeIndex(face, edgeB);
		i32 vIdxA = Helpers::Geom::vertIndexOrdered(face, eIdxA, vertA);
		i32 vIdxB = Helpers::Geom::vertIndexOrdered(face, eIdxB, vertB);
		if (edgeA != edgeB) {
			bool match = false;
			for (i32 i = 0; i < deg; ++i) {
				if (eIdxA == eIdxB || vIdxA == vIdxB) {
					vIdxA = Helpers::Geom::reverse(mesh, face, eIdxA, vIdxA);
					vIdxB = Helpers::Geom::reverse(mesh, face, eIdxB, vIdxB);
					match = true;
					break;
				}
				Helpers::Geom::nextEdgeIdx(deg, eIdxA, vIdxA);
				Helpers::Geom::nextEdgeIdx(deg, eIdxB, vIdxB);
			}
			if (!match) {
				return false;
			}
		}
		edgesA.push(face.edg[eIdxA]);
		edgesB.push(face.edg[eIdxB]);
		vertsA.push(face.vtx[vIdxA]);
		vertsB.push(face.vtx[vIdxB]);
		bool match = false;
		for (i32 i = 0; i < deg; ++i) {
			Helpers::Geom::nextEdgeIdx(deg, eIdxA, vIdxA);
			Helpers::Geom::nextEdgeIdx(deg, eIdxB, vIdxB);
			edgesA.push(face.edg[eIdxA]);
			edgesB.push(face.edg[eIdxB]);
			if (eIdxA == eIdxB) {
				vertsA.push(face.vtx[Helpers::Geom::reverse(mesh, face, eIdxA, vIdxA)]);
				vertsB.push(face.vtx[Helpers::Geom::reverse(mesh, face, eIdxB, vIdxB)]);
				match = true;
				break;
			}
			vertsA.push(face.vtx[vIdxA]);
			vertsB.push(face.vtx[vIdxB]);
			if (vIdxA == vIdxB) {
				match = true;
				break;
			}
		}
		if (!match) {
			return false;
		}
		facesDone.set(f + 1);
		return true;
	}

private:
	static inline bool checkSelection(
		const MNMesh& mesh,
		const Bitset& faces,
		const i32 f,
		const i32 e,
		const i32 v
	) {
		if (f >= 0 && f < mesh.numf && faces[f]) {
			if (Helpers::Geom::edgeIndex(mesh, f, e) >= 0) {
				const MNEdge& edge = mesh.e[e];
				if (edge.v1 == v || edge.v2 == v) {
					return true;
				}
			}
		}
		return false;
	}

public:
	static inline i32 blendMirrorWeights(
		INode* node,
		const i32 modIndex,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const Bitset& selection,
		Slice& affixA,
		const bool modeA,
		Slice& affixB,
		const bool modeB,
		const bool receive
	) {
		using ERR = Codes::I_Order_blendMirrorWeights;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		Helpers::SkinMod sk;
		r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		affixA.trim();	// get rid of surrounding white-space
		affixB.trim();	// get rid of surrounding white-space
		if (affixA.length() == 0) {
			// aka: all white-space. meaningless.
			return ERR::a_affix_empty;
		}
		if (affixB.length() == 0) {
			// aka: all white-space. meaningless.
			return ERR::b_affix_empty;
		}
		{
			const i32 numPoints = sk.getNumPoints();
			if (!Helpers::Geom::checkBounds(orderM, numPoints)) {
				return ERR::bad_handle_m;
			}
			if (!Helpers::Geom::checkBounds(orderA, numPoints)) {
				return ERR::bad_handle_a;
			}
			if (!Helpers::Geom::checkBounds(orderB, numPoints)) {
				return ERR::bad_handle_b;
			}
		}
		const i32 numBones = sk.getNumBones();
		List<i32> table(numBones);
		if (!buildMatchTable(sk, affixA, modeA, affixB, modeB, table)) {
			return ERR::duplicate_bone_name;
		}
		Defer<Restore::WeightChangeRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::WeightChangeRestore(sk));
			rest.get()->before();
		}
		// start changes
		const i32 boneLimitBefore = sk.getBoneLimit();
		i32 boneLimit = boneLimitBefore;
		if (boneLimit < 2) {
			boneLimit = 2;
			sk.setBoneLimit(2);
		}
		const i32 limit = boneLimit;
		List<INode*> bonesA;
		List<INode*> bonesB;
		List<f32> weights;
		List<i32> ids;
		List<i32> idsA;
		List<i32> idsB;
		List<f64> wgt;
		List<f64> wgtA;
		List<f64> wgtB;
		Bitset bits(numBones);
		for (i32 i = 0; i < orderA.length(); ++i) {
			const i32 a = orderA[i] - 1;
			const i32 b = orderB[i] - 1;
			const bool sa = selection[a];
			const bool sb = selection[b];
			if (sa != sb) {
				// only one side is selected. 
				if (sa ^ receive) {
					copyMirroredWeights(sk, a, b, table, bonesA, weights);
				}
				else {
					copyMirroredWeights(sk, b, a, table, bonesA, weights);
				}
			}
			else if (sa) {
				// both verts are selected. they both get equalized along their side (same weight values, opposing bones).
				bits.clearAll();
				ids.clearAll();
				wgt.clearAll();
				// gather valid weights in bone id order
				gatherWeights(sk, a, idsA, wgtA);
				gatherWeights(sk, b, idsB, wgtB);
				// find A <-> B matches, sum their weights
				for (i32 j = 0; j < idsA.length(); ++j) {
					const i32 id = idsA[j];
					const i32 matchId = Util::selectNegative(table[id], id);	// select matchId if valid, else id
					f64 w = wgtA[j];	// get A weight
					{
						// look for match/self in B
						const i32 idx = idsB.indexOfAscending(matchId);
						if (idsB.detect(idx, matchId)) {
							// match/self is present in opposing weight table
							bits.set(idx);	// skip over this in B
							w += wgtB[idx];	// add opposing value
						}
					}
					// add summed weight in weight order
					const i32 idx = wgt.indexOfDescending(w);
					wgt.insert(idx, w);
					ids.insert(idx, id);
				}
				// account for remaining unmatched entries in B
				for (i32 j = 0; j < idsB.length(); ++j) {
					if (bits.trySet(j)) {
						const i32 id = idsB[j];
						const f64 w = wgtB[j];
						const i32 idx = wgt.indexOfDescending(w);
						wgt.insert(idx, w);
						ids.insert(idx, id);
					}
				}
				// finally, apply the new weights
				const f32 dq = sk.averageDQ(a, b);
				applyMirrorWeights(sk, a, b, table, ids, wgt, limit, bonesA, bonesB, weights);
				sk.setDQBlendWeight(a, dq);
				sk.setDQBlendWeight(b, dq);
			}
		}
		// every selected middle vert has its weights equalized along both sides.
		for (i32 i = 0; i < orderM.length(); ++i) {
			const i32 v = orderM[i] - 1;
			if (selection[v]) {
				// buid paired weight table, in descending weight order, while pairing up matched weights
				bits.clearAll();
				ids.clearAll();
				wgt.clearAll();
				gatherWeights(sk, v, idsA, wgtA);
				// idsA and wgtA now contain matching bone ids and weights in ascending id order
				for (i32 j = 0, mmm = idsA.length(); j < mmm; ++j) {
					if (bits.trySet(j)) {
						// unseen index
						f64 w = wgtA[j];
						const i32 id = idsA[j];
						const i32 matchId = table[id];
						i32 idx;
						if (matchId >= 0) {
							// paired
							const i32 _idx = idsA.indexOfAscending(matchId);
							if (idsA.detect(_idx, matchId)) {
								// found match
								bits.set(_idx);	// skip index during next iteration
								w += wgtA[_idx];
							}
							// else: add 0
							idx = wgt.indexOfDescending(w);
							// complement pair
							wgt.insert(idx, w);
							ids.insert(idx, matchId);
							// pairs are inserted with twice the sum of their weights, really. this is to prevent them from being truncated too early.
							// by inserting them both with the exact same weight prevents stable binary search from splitting them arbitrarily.
						}
						else {
							// unpaired
							idx = wgt.indexOfDescending(w);
						}
						wgt.insert(idx, w);
						ids.insert(idx, id);
					}
				}
				// truncate table to below limit, without splitting pairs (this is why limit must be >= 2)
				while (wgt.length() > limit) {
					wgt.pop();
					const i32 id = ids.pop();
					const i32 matchId = table[id];
					if (matchId >= 0) {
						// remove paired entries as a unit
						ids.pop();
						wgt.pop();
					}
				}
				// divide paired weights by two again
				for (i32 j = 0; j < wgt.length(); ++j) {
					const i32 id = ids[j];
					const i32 matchId = table[id];
					if (matchId >= 0) {
						const f64 w = wgt[j] / 2.0;
						wgt[j] = w;
						wgt[++j] = w;
					}
				}
				// finally, normalize and apply the weights. This is getting somewhat expensive, but whatever...
				bonesA.clearAll();
				weights.clearAll();
				normalize(wgt, wgt.length());
				for (i32 j = 0; j < wgt.length(); ++j) {
					bonesA.push(sk.getBone(ids[j]));
					weights.push(F32(wgt[j]));
				}
				sk.addWeights(v, *bonesA.tab(), *weights.tab());
				// DQs stays intact
			}
		}
		sk.mod().NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		// end changes
		if (holding) {
			rest.get()->after(boneLimitBefore, boneLimit);
			Restore::put(true, rest);
		}
		p.epoly().RefreshScreen();	// TODO pointless?
		Util::redrawViews();
		return Codes::OK;
	}

	// collects the weights for a, and copies them to b, with all bones replaced by their counterparts. Also copies the DQ.
private:
	static inline void copyMirroredWeights(
		Helpers::SkinMod& sk,
		const i32 a,
		const i32 b,
		const List<i32>& T,
		List<INode*>& bones,
		List<f32>& weights
	) {
		collectBonesMirrored(sk, a, T, bones);
		sk.collectWeights(a, weights);
		const f32 dq = sk.getDQBlendWeight(a);
		sk.addWeights(b, *bones.tab(), *weights.tab());
		sk.setDQBlendWeight(b, dq);
	}


	// given ids, wgt and match table T, apply the same weights to a and b, but where b gets matched bone ids.
	// also normalizes the weights first.
private:
	static inline void applyMirrorWeights(
		Helpers::SkinMod& sk,
		const i32 a,
		const i32 b,
		const List<i32>& T,
		List<i32>& ids,
		List<f64>& wgt,
		const i32 limit,
		List<INode*>& bonesA,
		List<INode*>& bonesB,
		List<f32>& weights
	) {
		bonesA.clearAll();
		bonesB.clearAll();
		weights.clearAll();
		const i32 n = Util::mathMin(wgt.length(), limit);
		normalize(wgt, n);
		for (i32 j = 0; j < n; ++j) {
			const i32 id = ids[j];
			const i32 matchId = T[id];
			INode* boneA = sk.getBone(id);
			INode* boneB = matchId < 0 ? boneA : sk.getBone(matchId);
			bonesA.push(boneA);
			bonesB.push(boneB);
			weights.push(F32(wgt[j]));
		}
		sk.addWeights(a, *bonesA.tab(), *weights.tab());
		sk.addWeights(b, *bonesB.tab(), *weights.tab());
	}

	// builds a table matching affixed A bones to affixed B bones and vice versa for the same skin
private:
	static inline bool buildMatchTable(
		Helpers::SkinMod& sk,
		const Slice& affixA,
		const bool modeA,
		const Slice& affixB,
		const bool modeB,
		List<i32>& table
	) {
		const i32 numBones = sk.getNumBones();
		for (i32 i = 0; i < numBones; ++i) {
			table.push(-1);
		}
		List<Slice> names(numBones);
		List<i32> ids(numBones);
		if (!sk.collectBoneNames(names, ids)) {
			// duplicate bone name
			return false;
		}
		CharBuffer buf(64);
		for (i32 i = 0; i < numBones; ++i) {
			const i32 id = ids[i];
			if (table[id] < 0) {
				const Slice& name = names[i];
				if (replaceAffix(name, buf, affixA, modeA, affixB, modeB)) {
					Slice match = buf.slice();
					const i32 idx = names.indexOfAscending(match);
					if (names.detect(idx, match)) {
						const i32 matchId = ids[idx];
						table[id] = matchId;
						table[matchId] = id;
					}
				}
			}
		}
		return true;
	}

private:
	static inline bool replaceAffix(
		const Slice& name,
		CharBuffer& buf,
		const Slice& affixA,
		const bool modeA,
		const Slice& affixB,
		const bool modeB
	) {
		if (modeA ? name.matchPrefix(affixA) : name.matchSuffix(affixA)) {
			buf.clearAll();
			if (modeB) {
				buf.push(affixB);
			}
			if (modeA) {
				buf.push(name.chars() + affixA.length(), name.length() - affixA.length());
			}
			else {
				buf.push(name.chars(), name.length() - affixA.length());
			}
			if (!modeB) {
				buf.push(affixB);
			}
			return true;
		}
		return false;
	}

	// fills ids and wgt with valid weights in ascending bone id order. 
	// if a bone is listed multiple times, its weights are summed.
	// NaNs and 0-valued weights are skipped.
private:
	static inline void gatherWeights(
		Helpers::SkinMod& sk,
		const i32 vert,
		List<i32>& ids,
		List<f64>& wgt
	) {
		wgt.clearAll();
		ids.clearAll();
		const i32 nb = sk.getNumAssignedBones(vert);
		for (i32 i = 0; i < nb; ++i) {
			const f32 w = sk.getBoneWeight(vert, i);
			if (w == w && w > 0.0f) {
				const i32 id = sk.getAssignedBone(vert, i);
				const i32 idx = ids.indexOfAscending(id);
				if (ids.detect(idx, id)) {
					// if bone is listed multiple times, sum its listed weights
					wgt[idx] += F64(w);
				}
				else {
					ids.insert(idx, id);
					wgt.insert(idx, F64(w));
				}
			}
		}
	}

private:
	static inline void normalize(
		List<f64>& wgt,
		const i32 count
	) {
		f64 sum = 0.0;
		for (i32 i = 0; i < count; ++i) {
			sum += wgt[i];
		}
		// sum cannot be 0, or if it is, count is 0, and no division takes place
		for (i32 i = 0; i < count; ++i) {
			wgt[i] /= sum;
		}
	}

private:
	static inline void collectBonesMirrored(
		Helpers::SkinMod& sk,
		const i32 vert,
		const List<i32>& T,
		List<INode*>& bones
	) {
		bones.clearAll();
		const i32 nb = sk.getNumAssignedBones(vert);
		for (i32 i = 0; i < nb; ++i) {
			const i32 id = sk.getAssignedBone(vert, i);
			bones.push(sk.getBone(Util::selectNegative(T[id], id)));
		}
	}

public:
	static inline i32 flipMirrorWeights(
		INode* node,
		const i32 modIndex,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const Bitset& selection,
		Slice& affixA,
		const bool modeA,
		Slice& affixB,
		const bool modeB
	) {
		using ERR = Codes::I_Order_flipMirrorWeights;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolvePoly(r);
		}
		Helpers::SkinMod sk;
		r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolveSkin(r);
		}
		affixA.trim();	// get rid of surrounding white-space
		affixB.trim();	// get rid of surrounding white-space
		if (affixA.length() == 0) {
			// aka: all white-space. meaningless.
			return ERR::a_affix_empty;
		}
		if (affixB.length() == 0) {
			// aka: all white-space. meaningless.
			return ERR::b_affix_empty;
		}
		{
			const i32 numPoints = sk.getNumPoints();
			if (!Helpers::Geom::checkBounds(orderM, numPoints)) {
				return ERR::bad_vert_m;
			}
			if (!Helpers::Geom::checkBounds(orderA, numPoints)) {
				return ERR::bad_vert_a;
			}
			if (!Helpers::Geom::checkBounds(orderB, numPoints)) {
				return ERR::bad_vert_b;
			}
		}
		const i32 numBones = sk.getNumBones();
		List<i32> table(numBones);
		if (!buildMatchTable(sk, affixA, modeA, affixB, modeB, table)) {
			return ERR::duplicate_bone_name;
		}
		Defer<Restore::WeightChangeRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::WeightChangeRestore(sk));
			rest.get()->before();
		}
		// start changes
		List<INode*> bonesA;
		List<INode*> bonesB;
		List<f32> weightsA;
		List<f32> weightsB;
		for (i32 i = 0; i < orderA.length(); ++i) {
			const i32 a = orderA[i] - 1;
			const i32 b = orderB[i] - 1;
			const bool sa = selection[a];
			const bool sb = selection[b];
			if (sa != sb) {
				// only one side is selected. replace bones with their counterparts.
				if (sa) {
					flipBones(sk, a, table, bonesA, weightsA);
				}
				else {
					flipBones(sk, b, table, bonesA, weightsA);
				}
			}
			else if (sa) {
				// both sides selected. swap bones, weights ands DQs
				sk.collectBones(a, bonesA);
				sk.collectWeights(a, weightsA);
				sk.collectBones(b, bonesB);
				sk.collectWeights(b, weightsB);
				const i32 dqA = sk.getDQBlendWeight(a);
				const i32 dqB = sk.getDQBlendWeight(b);
				sk.addWeights(a, *bonesB.tab(), *weightsB.tab());
				sk.addWeights(b, *bonesA.tab(), *weightsA.tab());
				sk.setDQBlendWeight(a, dqB);
				sk.setDQBlendWeight(b, dqA);
			}
		}
		for (i32 i = 0; i < orderM.length(); ++i) {
			const i32 v = orderM[i] - 1;
			if (selection[v]) {
				// middle vert selected. replace bones with their counterparts
				flipBones(sk, v, table, bonesA, weightsA);
			}
		}
		sk.mod().NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		// end changes
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		p.epoly().RefreshScreen();	// TODO pointless?
		Util::redrawViews();
		return Codes::OK;
	}

private:
	static inline void flipBones(
		Helpers::SkinMod& sk,
		const i32 vert,
		const List<i32>& T,
		List<INode*>& bones,
		List<f32>& weights
	) {
		collectBonesMirrored(sk, vert, T, bones);
		sk.collectWeights(vert, weights);
		sk.addWeights(vert, *bones.tab(), *weights.tab());
	}


public:
	static inline i32 translateSelection(
		INode* src,
		const List<i32>& srcOrder,
		INode* dst,
		const List<i32>& dstOrder,
		const Bitset& srcSelection,
		const i32 level,
		const i32 mode
	) {
		using ERR = Codes::I_Order_translateSelection;
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
		Defer<Restore::SubSelRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SubSelRestore(pb, level, MN_SEL));
			rest.get()->before();
		}

		const i32 len = srcOrder.length();
		switch (level) {
		case 0: {
			// verts
			if (!Helpers::Geom::checkBounds(srcOrder, meshA.numv)) {
				r = ERR::a_bad_vert;
				break;
			}
			if (!Helpers::Geom::checkBounds(dstOrder, meshB.numv)) {
				r = ERR::b_bad_vert;
				break;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.v[dstOrder[i] - 1].ClearFlag(MN_SEL);
					}
				}
			}
			else {
				if (mode == 0) {
					meshB.ClearVFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.v[dstOrder[i] - 1].SetFlag(MN_SEL);
					}
				}
			}
			break;
		}
		case 1: {
			// edges
			if (!Helpers::Geom::checkBounds(srcOrder, meshA.nume)) {
				r = ERR::a_bad_edge;
				break;
			}
			if (!Helpers::Geom::checkBounds(dstOrder, meshB.nume)) {
				r = ERR::b_bad_edge;
				break;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.e[dstOrder[i] - 1].ClearFlag(MN_SEL);
					}
				}
			}
			else {
				if (mode == 0) {
					meshB.ClearEFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.e[dstOrder[i] - 1].SetFlag(MN_SEL);
					}
				}
			}
			break;
		}
		default: {
			// faces
			if (!Helpers::Geom::checkBounds(srcOrder, meshA.numf)) {
				r = ERR::a_bad_face;
				break;
			}
			if (!Helpers::Geom::checkBounds(dstOrder, meshB.numf)) {
				r = ERR::b_bad_face;
				break;
			}
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.f[dstOrder[i] - 1].ClearFlag(MN_SEL);
					}
				}
			}
			else {
				if (mode == 0) {
					meshB.ClearFFlags(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						meshB.f[dstOrder[i] - 1].SetFlag(MN_SEL);
					}
				}
			}
			break;
		}
		}
		if (r == Codes::OK) {
			pb.epoly().LocalDataChanged(SELECT_CHANNEL);
			if (holding) {
				rest.get()->after();
				Restore::put(true, rest);
			}
			pb.epoly().RefreshScreen();
		}
		return r;
	}

public:
	static inline i32 translateHandles(
		INode* src,
		const i32 srcModIndex,
		const List<i32>& srcOrder,	// src verts in order
		INode* dst,
		const i32 dstModIndex,
		const List<i32>& dstOrder,
		const Bitset& srcSelection,
		const i32 mode
	) {
		using ERR = Codes::I_Order_translateHandles;
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
		if (srcOrder.length() != dstOrder.length()) {
			return ERR::different_lengths;
		}
		const i32 numPointsA = skA.getNumPoints();
		if (!Helpers::Geom::checkBounds(srcOrder, numPointsA)) {
			return ERR::a_bad_handle;
		}
		const i32 numPointsB = skB.getNumPoints();
		if (!Helpers::Geom::checkBounds(dstOrder, numPointsB)) {
			return ERR::b_bad_handle;
		}
		Defer<Restore::SkinSelectionRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SkinSelectionRestore());
			rest.get()->before(skB);
		}
		{
			// TODO translate more efficiently between bitset and bitarray
			BitArray selectionB(numPointsB);
			if (mode != 0) {
				skB.getVertexSelection(selectionB);
				if (selectionB.GetSize() != numPointsB) {
					selectionB.SetSize(numPointsB, 1);
				}
			}
			const i32 len = srcOrder.length();
			if (mode < 0) {
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						selectionB.Clear(dstOrder[i] - 1);
					}
				}
			}
			else {
				for (i32 i = 0; i < len; ++i) {
					if (srcSelection[srcOrder[i] - 1]) {
						selectionB.Set(dstOrder[i] - 1);
					}
				}
			}
			skB.setVertexSelection(selectionB);
		}
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		return Codes::OK;
	}

public:
	static inline i32 mirrorSelection(
		INode* node,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const i32 level,
		const i32 mode
	) {
		using ERR = Codes::I_Order_mirrorSelection;
		Helpers::Poly p;
		i32 r = p.init(node);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		if (orderA.length() != orderB.length()) {
			return ERR::different_lengths;
		}
		Defer<Restore::SubSelRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SubSelRestore(p, level, MN_SEL));
			rest.get()->before();
		}
		
		const i32 len = orderA.length();
		MNMesh& mesh = p.mesh();
		switch (level) {
		case 0: {
			if (!Helpers::Geom::checkBounds(orderM, mesh.numv)) {
				return ERR::bad_vert_m;
			}
			if (!Helpers::Geom::checkBounds(orderA, mesh.numv)) {
				return ERR::bad_vert_a;
			}
			if (!Helpers::Geom::checkBounds(orderB, mesh.numv)) {
				return ERR::bad_vert_b;
			}
			if (mode < 0) {
				for (i32 i = 0; i < orderM.length(); ++i) {
					mesh.v[orderM[i] - 1].ClearFlag(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					MNVert& a = mesh.v[orderA[i] - 1];
					MNVert& b = mesh.v[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					const bool sb = b.GetFlag(MN_SEL);
					if (sa) {
						b.ClearFlag(MN_SEL);
					}
					if (sb) {
						a.ClearFlag(MN_SEL);
					}
				}
			}
			else if (mode > 0) {
				for (i32 i = 0; i < len; ++i) {
					MNVert& a = mesh.v[orderA[i] - 1];
					MNVert& b = mesh.v[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							b.SetFlag(MN_SEL);
						}
						else {
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			else {
				for (i32 i = 0; i < len; ++i) {
					MNVert& a = mesh.v[orderA[i] - 1];
					MNVert& b = mesh.v[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							a.ClearFlag(MN_SEL);
							b.SetFlag(MN_SEL);
						}
						else {
							b.ClearFlag(MN_SEL);
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			break;
		}
		case 1: {
			if (!Helpers::Geom::checkBounds(orderM, mesh.nume)) {
				return ERR::bad_edge_m;
			}
			if (!Helpers::Geom::checkBounds(orderA, mesh.nume)) {
				return ERR::bad_edge_a;
			}
			if (!Helpers::Geom::checkBounds(orderB, mesh.nume)) {
				return ERR::bad_edge_b;
			}
			if (mode < 0) {
				for (i32 i = 0; i < orderM.length(); ++i) {
					mesh.e[orderM[i] - 1].ClearFlag(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					MNEdge& a = mesh.e[orderA[i] - 1];
					MNEdge& b = mesh.e[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					const bool sb = b.GetFlag(MN_SEL);
					if (sa) {
						b.ClearFlag(MN_SEL);
					}
					if (sb) {
						a.ClearFlag(MN_SEL);
					}
				}
			}
			else if (mode > 0) {
				for (i32 i = 0; i < len; ++i) {
					MNEdge& a = mesh.e[orderA[i] - 1];
					MNEdge& b = mesh.e[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							b.SetFlag(MN_SEL);
						}
						else {
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			else {
				for (i32 i = 0; i < len; ++i) {
					MNEdge& a = mesh.e[orderA[i] - 1];
					MNEdge& b = mesh.e[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							a.ClearFlag(MN_SEL);
							b.SetFlag(MN_SEL);
						}
						else {
							b.ClearFlag(MN_SEL);
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			break;
		}
		default: {
			if (!Helpers::Geom::checkBounds(orderM, mesh.numf)) {
				return ERR::bad_face_m;
			}
			if (!Helpers::Geom::checkBounds(orderA, mesh.numf)) {
				return ERR::bad_face_a;
			}
			if (!Helpers::Geom::checkBounds(orderB, mesh.numf)) {
				return ERR::bad_face_b;
			}
			if (mode < 0) {
				for (i32 i = 0; i < orderM.length(); ++i) {
					mesh.f[orderM[i] - 1].ClearFlag(MN_SEL);
				}
				for (i32 i = 0; i < len; ++i) {
					MNFace& a = mesh.f[orderA[i] - 1];
					MNFace& b = mesh.f[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					const bool sb = b.GetFlag(MN_SEL);
					if (sa) {
						b.ClearFlag(MN_SEL);
					}
					if (sb) {
						a.ClearFlag(MN_SEL);
					}
				}
			}
			else if (mode > 0) {
				for (i32 i = 0; i < len; ++i) {
					MNFace& a = mesh.f[orderA[i] - 1];
					MNFace& b = mesh.f[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							b.SetFlag(MN_SEL);
						}
						else {
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			else {
				for (i32 i = 0; i < len; ++i) {
					MNFace& a = mesh.f[orderA[i] - 1];
					MNFace& b = mesh.f[orderB[i] - 1];
					const bool sa = a.GetFlag(MN_SEL);
					if (sa != b.GetFlag(MN_SEL)) {
						if (sa) {
							a.ClearFlag(MN_SEL);
							b.SetFlag(MN_SEL);
						}
						else {
							b.ClearFlag(MN_SEL);
							a.SetFlag(MN_SEL);
						}
					}
				}
			}
			break;
		}
		}

		p.epoly().LocalDataChanged(SELECT_CHANNEL);
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		p.epoly().RefreshScreen();
		Util::redrawViews();	// TODO pointless?
		return Codes::OK;
	}

public:
	static inline i32 mirrorHandles(
		INode* node,
		const i32 modIndex,
		const List<i32>& orderM,
		const List<i32>& orderA,
		const List<i32>& orderB,
		const i32 mode
	) {
		using ERR = Codes::I_Order_mirrorHandles;
		Helpers::SkinMod sk;
		i32 r = sk.init(node, modIndex);
		if (r != Codes::OK) {
			return ERR::resolve(r);
		}
		if (orderA.length() != orderB.length()) {
			return ERR::different_lengths;
		}
		const i32 numPoints = sk.getNumPoints();
		if (!Helpers::Geom::checkBounds(orderM, numPoints)) {
			return ERR::bad_handle_m;
		}
		if (!Helpers::Geom::checkBounds(orderA, numPoints)) {
			return ERR::bad_handle_a;
		}
		if (!Helpers::Geom::checkBounds(orderB, numPoints)) {
			return ERR::bad_handle_b;
		}
		Defer<Restore::SkinSelectionRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::SkinSelectionRestore());
			rest.get()->before(sk);
		}
		

		BitArray selection(numPoints);
		sk.getVertexSelection(selection);
		if (selection.GetSize() != numPoints) {
			selection.SetSize(numPoints, 1);
		}
		const i32 len = orderA.length();
		if (mode < 0) {
			for (i32 i = 0; i < orderM.length(); ++i) {
				selection.Clear(orderM[i] - 1);
			}
			for (i32 i = 0; i < len; ++i) {
				const i32 a = orderA[i] - 1;
				const i32 b = orderB[i] - 1;
				const bool sa = selection[a];
				const bool sb = selection[b];
				if (sa) {
					selection.Clear(b);
				}
				if (sb) {
					selection.Clear(a);
				}
			}
		}
		else if (mode > 0) {
			for (i32 i = 0; i < len; ++i) {
				const i32 a = orderA[i] - 1;
				const i32 b = orderB[i] - 1;
				const bool sa = selection[a];
				const bool sb = selection[b];
				if (sa != sb) {
					if (sa) {
						selection.Set(b);
					}
					else {
						selection.Set(a);
					}
				}
			}
		}
		else {
			for (i32 i = 0; i < len; ++i) {
				const i32 a = orderA[i] - 1;
				const i32 b = orderB[i] - 1;
				const bool sa = selection[a];
				const bool sb = selection[b];
				if (sa != sb) {
					if (sa) {
						selection.Clear(a);
						selection.Set(b);
					}
					else {
						selection.Clear(b);
						selection.Set(a);
					}
				}
			}
		}
		sk.setVertexSelection(selection);


		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		return Codes::OK;
	}

public:
	static inline i32 transferWeights(
		INode* src,
		const i32 srcModIndex,
		const List<i32>& srcOrder,	// src verts in order
		INode* dst,
		const i32 dstModIndex,
		const List<i32>& dstOrder,
		const Bitset& selection,
		Slice& srcAffix,
		const bool srcMode,
		Slice& dstAffix,
		const bool dstMode
	) {
		using ERR = Codes::I_Order_transferWeights;
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
		if (skA.isMangled(skB)) {
			return ERR::skin_mangled;
		}
		if (srcAffix.length() == 0) {
		//	return ERR::src_affix_empty;
		}
		if (dstAffix.length() == 0) {
		//	return ERR::dst_affix_empty;
		}
		if (!Helpers::Geom::checkBounds(srcOrder, skA.getNumPoints())) {
			return ERR::src_bad_handle;
		}
		if (!Helpers::Geom::checkBounds(dstOrder, skB.getNumPoints())) {
			return ERR::dst_bad_handle;
		}
		const i32 numBonesA = skA.getNumBones();
		List<i32> table(numBonesA);
		r = buildMatchTable2(skA, skB, srcAffix, srcMode, dstAffix, dstMode, table);
		if (r != Codes::OK) {
			return r;
		}
		{
			Bitset usedBonesA(numBonesA);
			skA.collectUsedBoneIds(usedBonesA, srcOrder, selection);
			Bitset::Iterator it(usedBonesA);
			for (i32 id; (id = it.next()) >= 0; ) {
				const i32 matchId = table[id];
				if (matchId < 0) {
					return ERR::unmatched_bone;
				}
			}
		}
		// all used src ids have a matching dest id
		// can now actually do the tranfer

		Defer<Restore::WeightChangeRestore> rest;
		const bool holding = theHold.Holding();
		if (holding) {
			rest.set(new Restore::WeightChangeRestore(skB));
			rest.get()->before();	// at most all of orderB is affected
		}
		if (&skA.mod() == &skB.mod()) {
			// same mod. need a defensive copy.
			List<f32> dqs(srcOrder.length());
			Pointers<FloatTab> weights(srcOrder.length());	// TODO should maybe directly hold lists instead
			Pointers<IntTab> ids(srcOrder.length());
			skA.collectAll(ids, weights, dqs, srcOrder, selection);
			List<INode*> bones;
			for (i32 i = 0, k = 0; i < srcOrder.length(); ++i) {
				if (selection[srcOrder[i] - 1]) {
					bones.clearAll();
					List<i32> _ids(*(ids[k]));
					List<f32> _weights(*(weights[k]));
					for (i32 j = 0; j < _ids.length(); ++j) {
						bones.push(skB.getBone(table[_ids[j]]));
					}
					const i32 b = dstOrder[i] - 1;
					skB.addWeights(b, *bones.tab(), *_weights.tab());
					skB.setDQBlendWeight(b, dqs[k]);
					++k;
				}
			}
		}
		else {
			// different mods. no defensive copy needed.
			List<INode*> bones;
			List<f32> weights;
			for (i32 i = 0; i < srcOrder.length(); ++i) {
				const i32 a = srcOrder[i] - 1;
				if (selection[a]) {
					const i32 b = dstOrder[i] - 1;
					bones.clearAll();
					weights.clearAll();
					const i32 nb = skA.getNumAssignedBones(a);
					for (i32 j = 0; j < nb; ++j) {
						weights.push(skA.getBoneWeight(a, j));
						bones.push(skB.getBone(table[skA.getAssignedBone(a, j)]));
					}
					const f32 dq = skA.getDQBlendWeight(a);
					skB.addWeights(b, *bones.tab(), *weights.tab());
					skB.setDQBlendWeight(b, dq);
				}
			}
		}
		skB.mod().NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		if (holding) {
			rest.get()->after();
			Restore::put(true, rest);
		}
		return Codes::OK;
	}

	// builds a table matching affixed A bones to affixed B bones for skin A and skin B.
private:
	static inline i32 buildMatchTable2(
		Helpers::SkinMod& skA,
		Helpers::SkinMod& skB,
		const Slice& affixA,
		const bool modeA,
		const Slice& affixB,
		const bool modeB,
		List<i32>& table
	) {
		using ERR = Codes::I_Order_transferWeights;
		const i32 numBonesA = skA.getNumBones();
		const i32 numBonesB = skB.getNumBones();
		for (i32 i = 0; i < numBonesA; ++i) {
			table.push(-1);
		}
		List<Slice> namesA(numBonesA);
		List<i32> idsA(numBonesA);
		if (!skA.collectBoneNames(namesA, idsA)) {
			return ERR::src_duplicate_bone_name;
		}
		List<Slice> namesB(numBonesB);
		List<i32> idsB(numBonesB);
		if (!skB.collectBoneNames(namesB, idsB)) {
			return ERR::dst_duplicate_bone_name;
		}
		CharBuffer buf(64);
		for (i32 i = 0; i < numBonesA; ++i) {
			const Slice& name = namesA[i];
			if (replaceAffix(name, buf, affixA, modeA, affixB, modeB)) {
				Slice match = buf.slice();
				const u64 idx = namesB.indexOfAscending(match);
				if (namesB.detect(idx, match)) {
					table[idsA[i]] = idsB[idx];
				}
			}
		}
		return Codes::OK;
	}


};
