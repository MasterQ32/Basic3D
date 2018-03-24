#pragma once

#include "vector3.hpp"
#include "matrix.hpp"

namespace Basic3D
{
    //! transforms a direction with an affice transformation
    template<typename real>
    static Vector3<real> transformDirection(Vector3<real> vec, Matrix4<real> const & matrix)
    {
        real x = vec.x * matrix.v[0][0] + vec.y * matrix.v[1][0] + vec.z * matrix.v[2][0];
        real y = vec.x * matrix.v[0][1] + vec.y * matrix.v[1][1] + vec.z * matrix.v[2][1];
        real z = vec.x * matrix.v[0][2] + vec.y * matrix.v[1][2] + vec.z * matrix.v[2][2];
        return Basic3D::Vector3<real>(x, y, z);
    }

    //! transforms a position with an affice transformation
    template<typename real>
    static Vector3<real> transformPoint(Vector3<real> vec, Matrix4<real> const & matrix)
    {
        real x = vec.x * matrix.v[0][0] + vec.y * matrix.v[1][0] + vec.z * matrix.v[2][0] + matrix.v[3][0];
        real y = vec.x * matrix.v[0][1] + vec.y * matrix.v[1][1] + vec.z * matrix.v[2][1] + matrix.v[3][1];
        real z = vec.x * matrix.v[0][2] + vec.y * matrix.v[1][2] + vec.z * matrix.v[2][2] + matrix.v[3][2];
        return Basic3D::Vector3<real>(x, y, z);
    }

    //! Multiplies a vector with a matrix and returns the perspective corrected result.
    template<typename real>
    static Vector3<real> transformPerspective(Vector3<real> vec, Matrix4<real> const & matrix)
    {
        real x = vec.x * matrix.v[0][0] + vec.y * matrix.v[1][0] + vec.z * matrix.v[2][0] + matrix.v[3][0];
        real y = vec.x * matrix.v[0][1] + vec.y * matrix.v[1][1] + vec.z * matrix.v[2][1] + matrix.v[3][1];
        real z = vec.x * matrix.v[0][2] + vec.y * matrix.v[1][2] + vec.z * matrix.v[2][2] + matrix.v[3][2];
        real w = vec.x * matrix.v[0][3] + vec.y * matrix.v[1][3] + vec.z * matrix.v[2][3] + matrix.v[3][3];
        return Basic3D::Vector3<real>(x / w, y / w, z / w);
    }
}
