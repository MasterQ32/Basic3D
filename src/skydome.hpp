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
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, -10.662870f), vec2_t(0.375f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, -9.234316f), vec2_t(0.375f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(13.059290f, 6.5397870f, -0.000002f), vec2_t(0.250f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(5.331431f, 12.0592900f, 5.331430f), vec2_t(0.125f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, 7.539783f), vec2_t(0.000f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, 10.662860f), vec2_t(0.125f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 6.5397860f, 13.059290f), vec2_t(0.000f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234311f, 6.5397860f, 9.234310f), vec2_t(0.875f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-5.331433f, 12.0592900f, 5.331429f), vec2_t(0.875f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(-7.539784f, 12.0592900f, -0.000003f), vec2_t(0.750f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, 10.662860f), vec2_t(0.875f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-9.234311f, 6.5397860f, 9.234310f), vec2_t(0.875f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-13.059290f, 6.5397850f, -0.000002f), vec2_t(0.750f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, -10.662870f), vec2_t(0.625f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-9.234312f, 6.5397860f, -9.234313f), vec2_t(0.625f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-0.000001f, 6.5397860f, -13.059290f), vec2_t(0.500f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234312f, 6.5397860f, -9.234313f), vec2_t(0.625f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-5.331432f, 12.0592900f, -5.331435f), vec2_t(0.625f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, -7.539787f), vec2_t(0.500f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, -9.234316f), vec2_t(0.375f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, -10.662870f), vec2_t(0.375f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-0.000001f, 6.5397860f, -13.059290f), vec2_t(0.500f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, -1.0000010f, -15.079570f), vec2_t(0.500f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(7.163692f, 12.3651300f, -1.902866f), vec2_t(0.291f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, -9.234316f), vec2_t(0.375f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, -7.539787f), vec2_t(0.500f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, -9.234316f), vec2_t(0.375f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-0.000001f, 6.5397860f, -13.059290f), vec2_t(0.500f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, -7.539787f), vec2_t(0.500f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(13.059290f, 6.5397870f, -0.000002f), vec2_t(0.250f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, -9.234316f), vec2_t(0.375f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(7.163692f, 12.3651300f, -1.902866f), vec2_t(0.291f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(15.079570f, -1.0000010f, -0.000002f), vec2_t(0.250f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, -10.662870f), vec2_t(0.375f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(13.059290f, 6.5397870f, -0.000002f), vec2_t(0.250f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(5.331431f, 12.0592900f, 5.331430f), vec2_t(0.125f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(7.163692f, 12.3651300f, -1.902866f), vec2_t(0.291f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(13.059290f, 6.5397870f, -0.000002f), vec2_t(0.250f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(7.163692f, 12.3651300f, -1.902866f), vec2_t(0.291f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, 10.662860f), vec2_t(0.125f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(13.059290f, 6.5397870f, -0.000002f), vec2_t(0.250f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(15.079570f, -1.0000010f, -0.000002f), vec2_t(0.250f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 6.5397860f, 13.059290f), vec2_t(0.000f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(9.234313f, 6.5397860f, 9.234310f), vec2_t(0.125f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, -1.0000010f, 15.079570f), vec2_t(0.000f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(10.662870f, -1.0000010f, 10.662860f), vec2_t(0.125f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 6.5397860f, 13.059290f), vec2_t(0.000f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-5.331433f, 12.0592900f, 5.331429f), vec2_t(0.875f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(-9.234311f, 6.5397860f, 9.234310f), vec2_t(0.875f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, 7.539783f), vec2_t(1.000f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 6.5397860f, 13.059290f), vec2_t(1.000f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234311f, 6.5397860f, 9.234310f), vec2_t(0.875f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, 10.662860f), vec2_t(0.875f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 6.5397860f, 13.059290f), vec2_t(1.000f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, -1.0000010f, 15.079570f), vec2_t(1.000f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-13.059290f, 6.5397850f, -0.000002f), vec2_t(0.750f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234311f, 6.5397860f, 9.234310f), vec2_t(0.875f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-15.079570f, -1.0000010f, -0.000002f), vec2_t(0.750f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, 10.662860f), vec2_t(0.875f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-13.059290f, 6.5397850f, -0.000002f), vec2_t(0.750f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-5.331432f, 12.0592900f, -5.331435f), vec2_t(0.625f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(-9.234312f, 6.5397860f, -9.234313f), vec2_t(0.625f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-7.539784f, 12.0592900f, -0.000003f), vec2_t(0.750f,0.000f) },
		Violent3D::Vertex<real> { vec3_t(-13.059290f, 6.5397850f, -0.000002f), vec2_t(0.750f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234312f, 6.5397860f, -9.234313f), vec2_t(0.625f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, -10.662870f), vec2_t(0.625f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-13.059290f, 6.5397850f, -0.000002f), vec2_t(0.750f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-15.079570f, -1.0000010f, -0.000002f), vec2_t(0.750f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, -1.0000010f, -15.079570f), vec2_t(0.500f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-10.662860f, -1.0000010f, -10.662870f), vec2_t(0.625f,1.000f) },
		Violent3D::Vertex<real> { vec3_t(-0.000001f, 6.5397860f, -13.059290f), vec2_t(0.500f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-0.000001f, 6.5397860f, -13.059290f), vec2_t(0.500f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(-9.234312f, 6.5397860f, -9.234313f), vec2_t(0.625f,0.503f) },
		Violent3D::Vertex<real> { vec3_t(0.000000f, 12.0592900f, -7.539787f), vec2_t(0.500f,0.000f) },
	};

	std::array<index_t, 93> indices
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 22, 24, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 44, 46, 45, 47, 48, 5, 49, 50, 51, 52, 53, 54, 53, 55, 54, 56, 57, 58, 57, 59, 58, 60, 61, 11, 62, 63, 64, 65, 66, 67, 66, 68, 67, 69, 70, 71, 70, 72, 71, 73, 74, 75, 76, 77, 78
	};
}
