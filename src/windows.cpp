#include <SDL/SDL.h>
#include <Windows.h>

#include "common.h"

static Basic3D::Image<screenSize_X, screenSize_Y> screen;

bool keymap[SDLK_LAST];
bool keyhits[SDLK_LAST];

bool key_pressed(int vk) { return keymap[vk]; }
bool key_hit(int vk) { return keyhits[vk]; }

extern "C" FILE * __iob_func()
{
    static FILE data[3] = {*stdin,*stdout,*stderr };
    return data;
}

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
int main(int argc, char *argv[])
{
    fprintf(stdout, "");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface * surf = SDL_SetVideoMode(screenSize_X, screenSize_Y, 32, SDL_DOUBLEBUF | SDL_SWSURFACE);

    initFrame(screen);

    int framecounter = 0;

    while(true)
    {
        SDL_Event e;
        memset(keyhits, 0, sizeof keyhits);
        while(SDL_PollEvent(&e))
        {
            if((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                ExitProcess(0);
                return 0;
            }
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    keymap[e.key.keysym.sym] = true;
                    keyhits[e.key.keysym.sym] = true;
                    break;
                case SDL_KEYUP:
                    keymap[e.key.keysym.sym] = false;
                    break;
            }
        }

        renderFrame(screen, framecounter++);

        SDL_LockSurface(surf);

        memcpy(surf->pixels, screen.data(), screen.size());

        SDL_UnlockSurface(surf);

        SDL_Flip(surf);

        SDL_Delay(20);
    }
}
