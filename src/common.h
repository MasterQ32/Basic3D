#pragma once

#include <basic3d/image.hpp>

static const int   screenSize_X = 320;
static const int   screenSize_Y = 240;

using Screen = Basic3D::Image<screenSize_X, screenSize_Y>;

extern void initFrame(Screen & screen);

extern void renderFrame(Screen & screen);
