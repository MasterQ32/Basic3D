#pragma once

#include <cstdint>

namespace Basic3D
{
    namespace internals
    {
        template<typename T>
        struct NextBiggerInt;

        template<> struct NextBiggerInt<uint8_t>   { typedef uint16_t type; };
        template<> struct NextBiggerInt<uint16_t>  { typedef uint32_t type; };
        template<> struct NextBiggerInt<uint32_t>  { typedef uint64_t type; };

        template<> struct NextBiggerInt<int8_t>    { typedef int16_t  type; };
        template<> struct NextBiggerInt<int16_t>   { typedef int32_t  type; };
        template<> struct NextBiggerInt<int32_t>   { typedef int64_t  type; };

        template<typename T>
        struct NextSmallerInt;

        template<> struct NextSmallerInt<uint8_t>  { typedef uint8_t type;  };
        template<> struct NextSmallerInt<uint16_t> { typedef uint8_t type;  };
        template<> struct NextSmallerInt<uint32_t> { typedef uint16_t type; };
        template<> struct NextSmallerInt<uint64_t> { typedef uint32_t type; };

        template<> struct NextSmallerInt<int8_t>   { typedef int8_t type;   };
        template<> struct NextSmallerInt<int16_t>  { typedef int8_t type;   };
        template<> struct NextSmallerInt<int32_t>  { typedef int16_t type;  };
        template<> struct NextSmallerInt<int64_t>  { typedef int32_t type;  };


        template<int size>
        struct SignedIntBySize;

        template<> struct SignedIntBySize<1>       { typedef int8_t  type; };
        template<> struct SignedIntBySize<2>       { typedef int16_t type; };
        template<> struct SignedIntBySize<3>       { typedef int32_t type; };
        template<> struct SignedIntBySize<4>       { typedef int32_t type; };
        template<> struct SignedIntBySize<5>       { typedef int64_t type; };
        template<> struct SignedIntBySize<6>       { typedef int64_t type; };
        template<> struct SignedIntBySize<7>       { typedef int64_t type; };
        template<> struct SignedIntBySize<8>       { typedef int64_t type; };

        template<int size>
        struct UnsignedIntBySize;

        template<> struct UnsignedIntBySize<1>     { typedef uint8_t  type; };
        template<> struct UnsignedIntBySize<2>     { typedef uint16_t type; };
        template<> struct UnsignedIntBySize<3>     { typedef uint32_t type; };
        template<> struct UnsignedIntBySize<4>     { typedef uint32_t type; };
        template<> struct UnsignedIntBySize<5>     { typedef uint64_t type; };
        template<> struct UnsignedIntBySize<6>     { typedef uint64_t type; };
        template<> struct UnsignedIntBySize<7>     { typedef uint64_t type; };
        template<> struct UnsignedIntBySize<8>     { typedef uint64_t type; };

        template<typename T>
        struct MakeSigned;

        template<> struct MakeSigned<int8_t>       { typedef int8_t type; };
        template<> struct MakeSigned<uint8_t>      { typedef int8_t type; };
        template<> struct MakeSigned<int16_t>      { typedef int16_t type; };
        template<> struct MakeSigned<uint16_t>     { typedef int16_t type; };
        template<> struct MakeSigned<int32_t>      { typedef int32_t type; };
        template<> struct MakeSigned<uint32_t>     { typedef int32_t type; };
        template<> struct MakeSigned<int64_t>      { typedef int64_t type; };
        template<> struct MakeSigned<uint64_t>     { typedef int64_t type; };

        template<typename T>
        struct MakeUnsigned;

        template<> struct MakeUnsigned<int8_t>     { typedef uint8_t type; };
        template<> struct MakeUnsigned<uint8_t>    { typedef uint8_t type; };
        template<> struct MakeUnsigned<int16_t>    { typedef uint16_t type; };
        template<> struct MakeUnsigned<uint16_t>   { typedef uint16_t type; };
        template<> struct MakeUnsigned<int32_t>    { typedef uint32_t type; };
        template<> struct MakeUnsigned<uint32_t>   { typedef uint32_t type; };
        template<> struct MakeUnsigned<int64_t>    { typedef uint64_t type; };
        template<> struct MakeUnsigned<uint64_t>   { typedef uint64_t type; };
    }
}
