#pragma once

#include "internals/typetraits.hpp"

#include <cstdint>
#include <cassert>

namespace Basic3D
{
    template<int integrals, int decimals>
    struct Fixed
    {
    public: // static types
        typedef typename internals::SignedIntBySize<int((integrals + decimals + 7) / 8)>::type word_t;
        typedef typename internals::NextBiggerInt<word_t>::type dword_t; // double word
        typedef typename internals::NextSmallerInt<word_t>::type hword_t; // half word

    public: // static members
        static const word_t decimalMask = ((1<<decimals) - 1);
        static const word_t integralMask = ((1<<decimals) - 1);
        static const word_t decimalFactor = (1<<decimals);

    public: // members
        word_t value;

    public: // constructors
        constexpr Fixed() = default;
        constexpr Fixed(float v)  : value(word_t(v * decimalFactor + 0.5)) { }
        constexpr Fixed(double v) : value(word_t(v * decimalFactor + 0.5)) { }
        constexpr Fixed(int i)    : value(word_t(i) << decimals) { }

    public: // functions

        constexpr static Fixed raw(word_t value) {
            Fixed f;
            f.value = value;
            return f;
        }

        constexpr int decimalPart() const {
            return this->value & decimalMask;
        }

        constexpr int integralPart() const {
            return this->value & integralMask;
        }

    public: // arithmetic operators

        constexpr Fixed operator+ (Fixed const & o) const {
            return Fixed::raw(this->value + o.value);
        }
        constexpr Fixed operator- (Fixed const & o) const {
            return Fixed::raw(this->value - o.value);
        }
        constexpr Fixed operator* (Fixed const & o) const {
            Fixed r = *this;
            r.mul(o);
            return r;
        }
        constexpr Fixed operator/ (Fixed const & o) const {
            Fixed r = *this;
            r.div(o);
            return r;
        }
        constexpr Fixed operator* (hword_t o) const {
            Fixed r = *this;
            r.mul(o);
            return r;
        }
        constexpr Fixed operator/ (hword_t o) const {
            Fixed r = *this;
            r.div(o);
            return r;
        }

    public: // relational operators
        constexpr bool operator ==(Fixed const & o) const {
            return (this->value == o.value);
        }
        constexpr bool operator !=(Fixed const & o) const {
            return (this->value != o.value);
        }
        constexpr bool operator >=(Fixed const & o) const {
            return (this->value >= o.value);
        }
        constexpr bool operator <=(Fixed const & o) const {
            return (this->value <= o.value);
        }
        constexpr bool operator >(Fixed const & o) const {
            return (this->value > o.value);
        }
        constexpr bool operator <(Fixed const & o) const {
            return (this->value < o.value);
        }

    public: // self-applying operators
        constexpr Fixed & operator += (Fixed const & o) {
            return (*this = *this + o);
        }

        constexpr Fixed & operator -= (Fixed const & o) {
            return (*this = *this - o);
        }

        constexpr Fixed & operator *= (Fixed const & o) {
            this->mul(o);
            return *this;
        }

        constexpr Fixed & operator /= (Fixed const & o) {
            this->div(o);
            return *this;
        }

        constexpr Fixed & operator *= (hword_t o) {
            this->mul(o);
            return *this;
        }

        constexpr Fixed & operator /= (hword_t o) {
            this->div(o);
            return *this;
        }

    public: // conversion operators
        explicit constexpr operator int() const {
            return (this->value >> decimals);
        }

        explicit constexpr operator float() const {
            return (this->value / float(decimalFactor));
        }

        explicit constexpr operator double() const {
            return (this->value / double(decimalFactor));
        }
    private: // arithmetic
        constexpr void mul(Fixed const & o)
        {
            dword_t const r = dword_t(this->value) * o.value;
            this->value = word_t(r >> decimals);
        }

        constexpr void mul(typename internals::MakeSigned<hword_t>::type o)
        {
            this->value *= o;
        }

        constexpr void mul(typename internals::MakeUnsigned<hword_t>::type o)
        {
            this->value *= o;
        }

        constexpr void div(const Fixed& o)
        {
            assert(o.value != 0);
            dword_t const x = dword_t(this->value) << decimals;
            this->value = word_t(x / o.value);
        }

        constexpr void div(typename internals::MakeSigned<hword_t>::type o)
        {
            assert(o != 0);
            this->value /= o;
        }

        constexpr void div(typename internals::MakeUnsigned<hword_t>::type o)
        {
            assert(o != 0);
            this->value /= o;
        }
    };
}
