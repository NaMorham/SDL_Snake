#include <SDL.h>
#include <SDL_ttf.h>

#include "Vec2D.h"
#include "Game.h"
#include "RigidBody.h"

template <typename T, class FnEq>
RigidBody<T, FnEq>::RigidBody(const T& mass/* = 1.0f*/,
    const Vec2DBase<T, FnEq>& pos/* = { 0.0f, 0.0f }*/,
    const Vec2DBase<T, FnEq>& velocity/* = { 0.0f, 0.0f }*/,
    const Vec2DBase<T, FnEq>& accel/* = { 0.0f, 0.0f }*/,
    const Vec2DBase<T, FnEq>& force/* = { 0.0f, 0.0f }*/)
    : m_mass{ mass },
    m_pos{ pos }, m_velocity{ velocity },
    m_accel{ accel }, m_force{ force },
    m_oldPos{ pos }, m_oldVelocity{ velocity },
    m_oldAccel{ accel }, m_oldForce{ force }
{}

template <typename T, class FnEq>
void RigidBody<T, FnEq>::Update(Game& gm)
{
    float dt2 = gm.m_deltaTime * gm.m_deltaTime;  // TODO: Move to Game object
    Vec2DBase<T, FnEq> accel{ Acceleration() + ExternalAcceleration() };
    if (gm.m_oss.str().size() > 0)
        gm.m_oss << std::endl;
    gm.m_oss << "RB: accel = [" << accel.X() << ", " << accel.Y() << "]";
    if (IsActive()) {
        m_oldForce = Force();
        m_oldAccel = Acceleration();
        m_oldVelocity = Velocity();

        // Verlet Integration so we do not gain energy and minimise loss
        // :: newPos = curPos + (curPos - oldPos) + a*dt*dt
        Vec2DBaseF newPos{
            2 * Position().X() - OldPosition().X() + (accel.X() * dt2),
            2 * Position().Y() - OldPosition().Y() + (accel.Y() * dt2)
        };

        m_velocity = (m_pos - m_oldPos) / gm.m_deltaTime;
        m_accel = accel;

        m_oldPos = m_pos;
        m_pos = newPos;
    }
}

template <typename T, class FnEq>
void RigidBody<T, FnEq>::Render(SDL_Renderer* pRenderer, Game& gm) const
{
    // TODO: Draw the collision box?
    // TODO: Handle rotation
    if (IsVisible()) {
        SDL_Point p{ WorldToScreen(Position().X(), Position().Y(), gm) };
        SDL_Point pv{ WorldToScreen(Position().X() + Velocity().X(),
                                    Position().Y() + Velocity().Y(), gm) };
        SDL_Point p2{ p.x + pv.x, p.y + pv.y };

        SDL_RenderDrawLine(pRenderer, p.x, p.y, p2.x, p2.y);
        SDL_RenderDrawLine(pRenderer, p.x, p.y + 2, p.x + 2, p.y);
        SDL_RenderDrawLine(pRenderer, p.x + 2, p.y, p.x, p.y - 2);
        SDL_RenderDrawLine(pRenderer, p.x, p.y - 2, p.x + 2, p.y);
        SDL_RenderDrawLine(pRenderer, p.x - 2, p.y, p.x, p.y + 2);
    }
}

template <typename T, class FnEq>
RigidBody<T, FnEq>& RigidBody<T, FnEq>::ResetOldValues()
{
    m_oldPos = m_pos;
    m_oldVelocity = m_accel;
    m_oldAccel = m_accel;
    m_oldForce = m_force;
    return *this;
}
