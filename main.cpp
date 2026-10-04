/**
 * SDL2 Snake
 */
#include <iostream>
#include <iomanip>

#include "SDL_SnakeVer.h"
#include "Vec2D.h"
#include "RigidBody.h"
#include "Game.h"

// ----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    std::ostringstream oss;
    oss << "Snake v" << SDL_Snake_Version;
    std::string title{ oss.str() };
    Game gm;

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        gm.m_screenRect.w, gm.m_screenRect.h, SDL_WINDOW_ALLOW_HIGHDPI);

    // Check that the window was successfully created
    if (NULL == window)
    {
        // In the case that the window could not be made...
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* pRenderer = SDL_CreateRenderer(window, -1, 0);
    if (NULL == pRenderer)
    {
        // In the case that the window could not be made...
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (gm.Init())
    {
        gm.Loop(pRenderer);
    }
    gm.Finalise();

    if (pRenderer) { SDL_DestroyRenderer(pRenderer); pRenderer = nullptr; }
    if (window) { SDL_DestroyWindow(window); window = nullptr; }
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------
