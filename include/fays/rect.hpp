#pragma once

namespace Fays
{
    //! A 2d rectangle in screen coordinates.
    struct Rect
    {
        int x, y;
        int width, height;

        //! returns the first inclusive column of the rectangle
        int left() const { return this->x; }

        //! returns the first inclusive row of the rectangle
        int top()  const { return this->y; }

        //! returns the first exclusive column right of the rectangle
        int right() const  { return this->x + this->width; }

        //! returns the first exclusive row below the rectangle
        int bottom() const { return this->y + this->height; }
    };
}
