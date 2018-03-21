#pragma once

#include <cstdint>
#define FORCEINLINE

#include "internals/typetraits.hpp"

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
        Fixed() = default;

        Fixed(float v)  : value(word_t(v * decimalFactor + 0.5)) { }
        Fixed(double v) : value(word_t(v * decimalFactor + 0.5)) { }
        Fixed(int i)    : value(word_t(i) << decimals) { }

    public: // functions

        static Fixed raw(word_t value) {
            Fixed f;
            f.value = value;
            return f;
        }

    public: // arithmetic operators

        Fixed operator+ (Fixed const & o) const {
            return Fixed::raw(this->value + o.value);
        }
        Fixed operator- (Fixed const & o) const {
            return Fixed::raw(this->value - o.value);
        }
        Fixed operator* (Fixed const & o) const {
            Fixed r = *this;
            r.mul(o);
            return r;
        }
        Fixed operator/ (Fixed const & o) const {
            Fixed r = *this;
            r.div(o);
            return r;
        }
        Fixed operator* (hword_t o) const {
            Fixed r = *this;
            r.mul(o);
            return r;
        }
        Fixed operator/ (hword_t o) const {
            Fixed r = *this;
            r.div(o);
            return r;
        }

        Fixed & operator += (Fixed const & o) {
            return (*this = *this + o);
        }

        Fixed & operator -= (Fixed const & o) {
            return (*this = *this - o);
        }

        Fixed & operator *= (Fixed const & o) {
            this->mul(o);
            return *this;
        }

        Fixed & operator /= (Fixed const & o) {
            this->div(o);
            return *this;
        }

        Fixed & operator *= (hword_t o) {
            this->mul(o);
            return *this;
        }

        Fixed & operator /= (hword_t o) {
            this->div(o);
            return *this;
        }

    public: // conversion operators
        explicit operator int() const {
            return (this->value >> decimals);
        }

        explicit operator float() const {
            return (this->value / float(decimalFactor));
        }

        explicit operator double() const {
            return (this->value / double(decimalFactor));
        }
    private: // arithmetic
        void mul(Fixed const & o)
        {
            dword_t const r = dword_t(this->value) * o.value;
            this->value = word_t(r >> decimals);
        }

        void mul(typename internals::MakeSigned<hword_t>::type o)
        {
            this->value *= o;
        }

        void mul(typename internals::MakeUnsigned<hword_t>::type o)
        {
            this->value *= o;
        }

        void div(const Fixed& o)
        {
            dword_t const x = dword_t(this->value) << decimals;
            this->value = x / o.value;
        }

        void div(typename internals::MakeSigned<hword_t>::type o)
        {
            this->value /= o;
        }

        void div(typename internals::MakeUnsigned<hword_t>::type o)
        {
            this->value /= o;
        }
    };
}
