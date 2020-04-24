/*************************************************************************/
/*  convex_polygon_shape_3d.cpp                                          */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "convex_polygon_shape_3d.h"
#include "core/math/quick_hull.h"
#include "servers/physics_server_3d.h"

std::vector<Vector3> ConvexPolygonShape3D::get_debug_mesh_lines() {

	std::vector<Vector3> points = get_points();

	if (points.size() > 3) {

		std::vector<Vector3> varr = Variant(points);
		Geometry::MeshData md;
		Error err = QuickHull::build(varr, md);
		if (err == OK) {
			std::vector<Vector3> lines;
			lines.resize(md.edges.size() * 2);
			for (int i = 0; i < md.edges.size(); i++) {
				lines[i * 2 + 0] = md.vertices[md.edges[i].a];
				lines[i * 2 + 1] = md.vertices[md.edges[i].b];
			}
			return lines;
		}
	}

	return std::vector<Vector3>();
}

real_t ConvexPolygonShape3D::get_enclosing_radius() const {
	std::vector<Vector3> data = get_points();
	const Vector3 *read = data.data();
	real_t r = 0;
	for (int i(0); i < data.size(); i++) {
		r = MAX(read[i].length_squared(), r);
	}
	return Math::sqrt(r);
}

void ConvexPolygonShape3D::_update_shape() {

	PhysicsServer3D::get_singleton()->shape_set_data(get_shape(), points);
	Shape3D::_update_shape();
}

void ConvexPolygonShape3D::set_points(const std::vector<Vector3> &p_points) {

	points = p_points;
	_update_shape();
	notify_change_to_owners();
}

std::vector<Vector3> ConvexPolygonShape3D::get_points() const {

	return points;
}

void ConvexPolygonShape3D::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_points", "points"), &ConvexPolygonShape3D::set_points);
	ClassDB::bind_method(D_METHOD("get_points"), &ConvexPolygonShape3D::get_points);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "points"), "set_points", "get_points");
}

ConvexPolygonShape3D::ConvexPolygonShape3D() :
		Shape3D(PhysicsServer3D::get_singleton()->shape_create(PhysicsServer3D::SHAPE_CONVEX_POLYGON)) {
}
