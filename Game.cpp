#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Vec2D.h"
#include "Game.h"

int Game::DEFAULT_WIDTH{ 800 };
int Game::DEFAULT_HEIGHT{ 600 };

Game::~Game()
{
    Finalise();
}

bool Game::Init()
{
    // 2000 world units in gm.m_screenWidth pixels
    // 10000 world units in gm.m_screenHeight pixels
    m_xScale = 2000.0f / static_cast<float>(m_screenRect.w);
    m_yScale = 10000.0f / static_cast<float>(m_screenRect.h);

    m_pCourierFont = TTF_OpenFont("fonts/courbd.ttf", 12);
    if (m_pCourierFont == nullptr)
    {
        std::cerr << "Could not load courier bold ttf file" << std::endl;
    }
    return (m_pCourierFont != nullptr);
}

void Game::Finalise()
{
    if (m_pCourierFont) {
        TTF_CloseFont(m_pCourierFont);
        m_pCourierFont = nullptr;
    }
}

void Game::Loop(SDL_Renderer* pRenderer)
{
    while (m_isRunning)
    {
        const uint32_t ticks{ SDL_GetTicks() };
        // Get m_deltaTime factor converted to seconds to be used to update objects
        m_deltaTicks = ticks - m_lastFrameTime;
        m_deltaTime = static_cast<float>(m_deltaTicks) / 1000.0;
        // Store the milliseconds of the current frame to be used in the next one
        m_lastFrameTime = ticks;

        // check for input
        m_oss.str("");
        Input();
        // we're still going, so lets update and render
        if (m_isRunning)
        {
            Update();
            Render(pRenderer);

            if (m_deltaTicks < m_msPerFrame)
            {
                SDL_Delay(m_msPerFrame - m_deltaTicks);
            }
        }
    }
}

void Game::Input()
{
    SDL_Event windowEvent;

    if (SDL_PollEvent(&windowEvent))
    {
        if (SDL_QUIT == windowEvent.type) {
            m_isRunning = false;
        }
        else if (SDL_KEYDOWN == windowEvent.type) {
            SDL_Scancode scanCode{ windowEvent.key.keysym.scancode };
            // NOTE: Not elseif since we want to allow more than one key?
            /*
            if (SDL_SCANCODE_W == scanCode) {
            }
            if (SDL_SCANCODE_UP == scanCode) {
            }
            if (SDL_SCANCODE_LEFT == scanCode) {
            }
            if (SDL_SCANCODE_A == scanCode) {
            }
            if (SDL_SCANCODE_RIGHT == scanCode) {
            }
            if (SDL_SCANCODE_D == scanCode) {
            }
            */
        }
        else if (SDL_KEYUP == windowEvent.type)
        {
            SDL_Scancode scanCode{ windowEvent.key.keysym.scancode };

            if ((SDL_SCANCODE_ESCAPE == scanCode) ||
                (SDL_SCANCODE_Q == scanCode)) {
                m_isRunning = false;
            }
            else {
                /*
                if (SDL_SCANCODE_W == scanCode) {
                }
                if (SDL_SCANCODE_R == scanCode) {
                }
                */
            }
        }
    }
}

void Game::Update()
{
    m_oss.str("");
    float fps{ m_deltaTime != 0.0f ? (60 / m_deltaTime) : this->m_desiredFps };
    m_oss << "fps: " << fps;
}

void Game::Render(SDL_Renderer* pRenderer)
{
    if (pRenderer)
    {
        SDL_SetRenderDrawColor(pRenderer, 10, 12, 32, 255);
        SDL_RenderClear(pRenderer);

        std::string ss(m_oss.str());
        if (ss.size() > 0)
        {
            m_pFontSurface = TTF_RenderText_Blended_Wrapped(m_pCourierFont, ss.c_str(), SDL_Color{ 255, 255, 255, 255 }, 750);
            if (m_pTextTexture)
            {
                SDL_DestroyTexture(m_pTextTexture);
            }
            m_pTextTexture = SDL_CreateTextureFromSurface(pRenderer, m_pFontSurface);
            if (!m_pTextTexture)
            {
                std::cerr << "Failed to create texture" << std::endl;
                m_isRunning = false;
                return;
            }
            m_textRect = { 10, 10, m_pFontSurface->w, m_pFontSurface->h };
            SDL_FreeSurface(m_pFontSurface);
            m_pFontSurface = nullptr;
            SDL_RenderCopy(pRenderer, m_pTextTexture, NULL, &m_textRect);
        }

        SDL_RenderPresent(pRenderer);
    }
}
