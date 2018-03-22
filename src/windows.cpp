#include <SDL/SDL.h>
#include <Windows.h>

#include "common.h"

static Basic3D::Image<screenSize_X, screenSize_Y> screen;

extern "C" FILE * __iob_func()
{
    static FILE data[3] = {*stdin,*stdout,*stderr };
    return data;
}

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
int main(int argc, char *argv[])
{
    fprintf(stdout, "Hello from SDL!\n");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface * surf = SDL_SetVideoMode(screenSize_X, screenSize_Y, 32, SDL_DOUBLEBUF | SDL_SWSURFACE);

    initFrame(screen);

    while(true)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                return 0;
        }

        renderFrame(screen);

        SDL_LockSurface(surf);

        memcpy(surf->pixels, screen.data(), screen.size());

        SDL_UnlockSurface(surf);

        SDL_Flip(surf);

        SDL_Delay(20);
    }
}
