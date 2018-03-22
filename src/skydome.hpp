#pragma once

#include <array>
#include <cstdint>
#include <violent3d.hpp>
#include <basic3d.hpp>

namespace skydome
{
	using index_t = std::uint16_t;
	using real = Basic3D::real_t;
	using vec2_t = Basic3D::Vector2<real>;
	using vec3_t = Basic3D::Vector3<real>;

	std::array<Violent3D::Vertex<real>, 79> vertices
	{
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, -10.66287), vec2_t(0.3749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, -9.234316), vec2_t(0.3749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(13.05929, 6.539787, -2E-06), vec2_t(0.2499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(5.331431, 12.05929, 5.33143), vec2_t(0.1249,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, 7.539783), vec2_t(-0.0001,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, 10.66286), vec2_t(0.1249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, 6.539786, 13.05929), vec2_t(-0.0001,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234311, 6.539786, 9.23431), vec2_t(0.8749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-5.331433, 12.05929, 5.331429), vec2_t(0.8749,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(-7.539784, 12.05929, -3E-06), vec2_t(0.7499,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, 10.66286), vec2_t(0.8749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-9.234311, 6.539786, 9.23431), vec2_t(0.8749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-13.05929, 6.539785, -2E-06), vec2_t(0.7499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, -10.66287), vec2_t(0.6249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-9.234312, 6.539786, -9.234313), vec2_t(0.6249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-1E-06, 6.539786, -13.05929), vec2_t(0.4999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234312, 6.539786, -9.234313), vec2_t(0.6249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-5.331432, 12.05929, -5.331435), vec2_t(0.6249,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, -7.539787), vec2_t(0.4999,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, -9.234316), vec2_t(0.3749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, -10.66287), vec2_t(0.3749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-1E-06, 6.539786, -13.05929), vec2_t(0.4999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, -1.000001, -15.07957), vec2_t(0.4999,1.0001) },
		Violent3D::Vertex<real> { vec3_t(7.163692, 12.36513, -1.902866), vec2_t(0.2912,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, -9.234316), vec2_t(0.3749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, -7.539787), vec2_t(0.4999,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, -9.234316), vec2_t(0.3749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-1E-06, 6.539786, -13.05929), vec2_t(0.4999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, -7.539787), vec2_t(0.4999,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(13.05929, 6.539787, -2E-06), vec2_t(0.2499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, -9.234316), vec2_t(0.3749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(7.163692, 12.36513, -1.902866), vec2_t(0.2912,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(15.07957, -1.000001, -2E-06), vec2_t(0.2499,1.0001) },
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, -10.66287), vec2_t(0.3749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(13.05929, 6.539787, -2E-06), vec2_t(0.2499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(5.331431, 12.05929, 5.33143), vec2_t(0.1249,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(7.163692, 12.36513, -1.902866), vec2_t(0.2912,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(13.05929, 6.539787, -2E-06), vec2_t(0.2499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(7.163692, 12.36513, -1.902866), vec2_t(0.2912,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, 10.66286), vec2_t(0.1249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(13.05929, 6.539787, -2E-06), vec2_t(0.2499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(15.07957, -1.000001, -2E-06), vec2_t(0.2499,1.0001) },
		Violent3D::Vertex<real> { vec3_t(0, 6.539786, 13.05929), vec2_t(-0.0001,0.5025) },
		Violent3D::Vertex<real> { vec3_t(9.234313, 6.539786, 9.23431), vec2_t(0.1249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, -1.000001, 15.07957), vec2_t(-0.0001,1.0001) },
		Violent3D::Vertex<real> { vec3_t(10.66287, -1.000001, 10.66286), vec2_t(0.1249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(0, 6.539786, 13.05929), vec2_t(-0.0001,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-5.331433, 12.05929, 5.331429), vec2_t(0.8749,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(-9.234311, 6.539786, 9.23431), vec2_t(0.8749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, 7.539783), vec2_t(0.9999,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(0, 6.539786, 13.05929), vec2_t(0.9999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234311, 6.539786, 9.23431), vec2_t(0.8749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, 10.66286), vec2_t(0.8749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(0, 6.539786, 13.05929), vec2_t(0.9999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, -1.000001, 15.07957), vec2_t(0.9999,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-13.05929, 6.539785, -2E-06), vec2_t(0.7499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234311, 6.539786, 9.23431), vec2_t(0.8749,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-15.07957, -1.000001, -2E-06), vec2_t(0.7499,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, 10.66286), vec2_t(0.8749,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-13.05929, 6.539785, -2E-06), vec2_t(0.7499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-5.331432, 12.05929, -5.331435), vec2_t(0.6249,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(-9.234312, 6.539786, -9.234313), vec2_t(0.6249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-7.539784, 12.05929, -3E-06), vec2_t(0.7499,0.0001000166) },
		Violent3D::Vertex<real> { vec3_t(-13.05929, 6.539785, -2E-06), vec2_t(0.7499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234312, 6.539786, -9.234313), vec2_t(0.6249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, -10.66287), vec2_t(0.6249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-13.05929, 6.539785, -2E-06), vec2_t(0.7499,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-15.07957, -1.000001, -2E-06), vec2_t(0.7499,1.0001) },
		Violent3D::Vertex<real> { vec3_t(0, -1.000001, -15.07957), vec2_t(0.4999,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-10.66286, -1.000001, -10.66287), vec2_t(0.6249,1.0001) },
		Violent3D::Vertex<real> { vec3_t(-1E-06, 6.539786, -13.05929), vec2_t(0.4999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-1E-06, 6.539786, -13.05929), vec2_t(0.4999,0.5025) },
		Violent3D::Vertex<real> { vec3_t(-9.234312, 6.539786, -9.234313), vec2_t(0.6249,0.5025) },
		Violent3D::Vertex<real> { vec3_t(0, 12.05929, -7.539787), vec2_t(0.4999,0.0001000166) },
	};

	std::array<index_t, 93> indices
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 22, 24, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 44, 46, 45, 47, 48, 5, 49, 50, 51, 52, 53, 54, 53, 55, 54, 56, 57, 58, 57, 59, 58, 60, 61, 11, 62, 63, 64, 65, 66, 67, 66, 68, 67, 69, 70, 71, 70, 72, 71, 73, 74, 75, 76, 77, 78
	};
}
