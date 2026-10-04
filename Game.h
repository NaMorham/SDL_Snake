#pragma once

#if defined(_WIN32) || defined(_WIN64)
#  include <SDL.h>
#  include <SDL_ttf.h>
#else
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_ttf.h>
#endif

#include <sstream>
#include <vector>
#include <list>

#include "Vec2D.h"

template <typename T, class FnEq = std::equal_to<T>>
class WorldRect
{
private:
    Vec2DBase<T, FnEq> m_topLeft;
    Vec2DBase<T, FnEq> m_sides;

public:
    WorldRect(const T& left = 0, const T& top = 0, const T& width = 0, const T& height = 0)
        : m_topLeft{ left, top }, m_sides{ width, height } {}
    WorldRect(const Vec2DBase<T, FnEq>& topLeft = { 0, 0 }, const Vec2DBase<T, FnEq>& sides = { 0, 0 })
        : m_topLeft{ topLeft.X(), topLeft.Y() }, m_sides{ sides.X(), sides.Y() } {}
    virtual ~WorldRect() = default;

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& TopLeft() const { return m_topLeft; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& TopLeft() { return m_topLeft; }
    const WorldRect<T, FnEq>& TopLeft(const Vec2DBase<T, FnEq>& newTL)
    {
        m_topLeft = newTL;
        return *this;
    }

    [[nodiscard]] inline const Vec2DBase<T, FnEq> BottomRight() const { return m_topLeft + m_sides; }
    [[nodiscard]] inline Vec2DBase<T, FnEq> BottomRight() { return m_topLeft + m_sides; }
    const WorldRect<T, FnEq>& BottomRight(const Vec2DBase<T, FnEq>& newBR)
    {
        m_sides = Vec2DBase<T, FnEq>{ m_topLeft - newBR };
        return *this;
    }

    [[nodiscard]] inline const T Width() const { return m_sides.X(); }
    [[nodiscard]] inline const T Height() const { return m_sides.Y(); }
};

using WorldRectF = WorldRect<float, std::equal_to<float>>;
using WorldRectD = WorldRect<double>; // , std::equal_to<double >> ;

struct RGBAColour
{
    RGBAColour(const uint32_t rgba = 0x000000ff)
        : RGBA(rgba) {}
    RGBAColour(const uint8_t r = 0x00, const uint8_t g = 0x00, const uint8_t b = 0x00, const uint8_t a = 0xff)
        : red{ r }, green{ g }, blue{ b }, alpha{ a } {}
    union
    {
        uint32_t RGBA{ 0x000000ff };
        struct
        {
            uint8_t red,
                green,
                blue,
                alpha;
        };
    };
};

// ----------------------------------------------------------------------------
class Game
{
public:
    static int DEFAULT_WIDTH;
    static int DEFAULT_HEIGHT;

    SDL_Rect m_screenRect{ DEFAULT_WIDTH, DEFAULT_HEIGHT };

    WorldRectF m_worldRect{ 2000.0f, 10000.0f };
    float m_xScale{ m_worldRect.Width() / static_cast<float>(m_screenRect.w) },
        m_yScale{ m_worldRect.Height() / static_cast<float>(m_screenRect.h) },
        m_deltaTime{ 0 };
    Vec2DBaseF m_gravity{ 0.0f, -9.81f };

    uint32_t m_desiredFps{ 64 },
        m_msPerFrame{ 16 },
        m_lastFrameTime{ 0 },
        m_deltaTicks{ 0 };

    bool m_isRunning{ true };

    // default font stuff
    TTF_Font* m_pCourierFont{ nullptr };
    SDL_Surface* m_pFontSurface{ nullptr };
    SDL_Rect m_textRect{ 0, 0, 0, 0 };
    SDL_Texture* m_pTextTexture{ nullptr };
    std::vector<std::string> m_screenText;
    std::ostringstream m_oss;

    Game(const int w = DEFAULT_WIDTH, const int h = DEFAULT_HEIGHT, const uint32_t dfps = 64)
        : m_screenRect{ 0, 0, w, h }, m_desiredFps{ dfps },
        m_msPerFrame{ static_cast<uint32_t>(1 / static_cast<float>(dfps) * 1000.0f) } {}

    ~Game();

    bool Init();

    void Finalise();

    void Loop(SDL_Renderer* pRenderer);

    void Input();

    void Update();

    void Render(SDL_Renderer* pRenderer);

    template<typename T>
    void WorldToScreen(const T xWldPos, const T yWldPos, int& scrX, int& scrY) const;

    template<typename T>
    [[nodiscard]] SDL_Point WorldToScreen(const T& xWldPos, const T& yWldPos) const;

    template<typename T, class FnEq = std::equal_to<T>>
    [[nodiscard]] SDL_Point WorldToScreen(const Vec2DBase<T, FnEq>& wldPos) const;

    template<typename T>
    void ScreenToWorld(const int scrPtX, const int scrPtY, T& xWldPos, T& yWldPos) const;

    template<typename T, class FnEq = std::equal_to<T>>
    [[nodiscard]] Vec2DBase<T, FnEq> ScreenToWorld(const int& scrPtX, const int& scrPtY) const;

    template<typename T, class FnEq = std::equal_to<T>>
    [[nodiscard]] Vec2DBase<T, FnEq> ScreenToWorld(const SDL_Point& scrPt) const;
};
