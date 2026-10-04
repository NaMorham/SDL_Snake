#pragma once

#include <cstdint>
#include <functional>

// predeclare
template<typename T, class FnEq>
class Vec2DBase;

class Game;

struct SDL_Renderer;

//-----------------------------------------------------------------------------
/*
   x = x0 + v0*t + 0.5*a*t*t

   F = J*dt
   a = F/m
   v = v0 + a*dt
   p = p0 + v*dt

   p = b + ((b-a)/t * ((J*t)/m) * t) * t ->
   p = (b*m + b*J*t*t - a*J*t*t) / m

 we have: p0, p1, J, m
   a = (J*dt)/m
   v = v0 + ((J*dt)/m)*dt
   v0 = (p1-p0)/dt
   v = (p1-p0)/dt * ((J*dt)/m) * dt    // save this so we don't recalc
   pn = p1 + ((p1-p0)/dt * ((J*dt)/m) * dt) * dt
   : pn = (p1*m + p1*J*dt*dt - p0*J*dt*dt)/m

   : Jtt = J*dt*dt
   : pn = (p1*m + p1*Jtt - p0*Jtt) / m

 Euler method
   update pos -> vel
   :: p1 = p0 + v*dt
   :: v1 = v0 + a*dt

 Semi-implicit Euler method
   update vel -> pos
   :: v1 = v0 + a*dt
   :: p1 = p0 + v1*dt

 Mid point method
   update mpos based on dT/2
   -> calc mvel from mpos
   -> calc pos from mvel
   ::  x1 = x0 + F(x0 + v0*(dt/2), t0+(dt/2))*dt  // F?

 TODO: RK4 method

 Verlet integration
   :: curVel = (curPos - oldPos)/dt
   ::-> newPos = curPos + curVel*dt + a*dt*dt
   :: newPos = curPos + (curPos - oldPos) + a*dt*dt
*/
template <typename T, class FnEq = std::equal_to<T>>
class RigidBody
{
private:
    float m_mass{ 1.0f };
    Vec2DBase<T, FnEq> m_pos{ 0.0f, 0.0f },
        m_velocity{ 0.0f, 0.0f },
        m_accel{ 0.0f, 0.0f },
        m_force{ 0.0f, 0.0f };
    Vec2DBase<T, FnEq> m_oldPos{ 0.0f, 0.0f },
        m_oldVelocity{ 0.0f, 0.0f },
        m_oldAccel{ 0.0f, 0.0f },
        m_oldForce{ 0.0f, 0.0f };
    Vec2DBase<T, FnEq> m_externalAccel{ 0.0f, 0.0f };
    bool m_visible{ true },
        m_active{ true };

public:
    RigidBody(const T& mass = 1.0,
        const Vec2DBase<T, FnEq>& pos = { 0.0, 0.0 },
        const Vec2DBase<T, FnEq>& velocity = { 0.0, 0.0 },
        const Vec2DBase<T, FnEq>& accel = { 0.0, 0.0 },
        const Vec2DBase<T, FnEq>& force = { 0.0, 0.0 });

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& Position() const { return m_pos; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& Position() { return m_pos; }
    inline RigidBody<T, FnEq>& Position(const Vec2DBase<T, FnEq>& rhs) { m_pos = rhs; return *this; }

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& Velocity() const { return m_velocity; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& Velocity() { return m_velocity; }
    inline RigidBody<T, FnEq>& Velocity(const Vec2DBase<T, FnEq>& rhs) { m_velocity = rhs; return *this; }

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& Acceleration() const { return m_accel; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& Acceleration() { return m_accel; }
    inline RigidBody<T, FnEq>& Acceleration(const Vec2DBase<T, FnEq>& rhs) { m_accel = rhs; return *this; }

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& Force() const { return m_force; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& Force() { return m_force; }
    inline RigidBody<T, FnEq>& Force(const Vec2DBase<T, FnEq>& rhs) { m_force = rhs; return *this; }

    [[nodiscard]] inline const Vec2DBase<T, FnEq>& ExternalAcceleration() const { return m_externalAccel; }
    [[nodiscard]] inline Vec2DBase<T, FnEq>& ExternalAcceleration() { return m_externalAccel; }
    inline RigidBody<T, FnEq>& ExternalAcceleration(const Vec2DBase<T, FnEq>& rhs) { m_externalAccel = rhs; return *this; }

    // old values
    [[nodiscard]] inline const Vec2DBase<T, FnEq>& OldPosition() const { return m_oldPos; }
    [[nodiscard]] inline const Vec2DBase<T, FnEq>& OldVelocity() const { return m_oldVelocity; }
    [[nodiscard]] inline const Vec2DBase<T, FnEq>& OldAcceleration() const { return m_oldAccel; }
    [[nodiscard]] inline const Vec2DBase<T, FnEq>& OldForce() const { return m_oldForce; }

    void Update(Game& gm);

    void Render(SDL_Renderer* pRenderer, Game& gm) const;

    [[nodiscard]] inline bool IsVisible() const { return m_visible; }
    inline bool ToggleVisible() { m_visible = !m_visible; return m_visible; }
    inline RigidBody& Show() { m_visible = true; return *this; }
    inline RigidBody& Hide() { m_visible = false; return *this; }

    [[nodiscard]] inline bool IsActive() const { return m_active; }
    inline bool ToggleActive() { m_active = !m_active; return m_active; }
    inline RigidBody<T, FnEq>& Activate() { m_active = true; return *this; }
    inline RigidBody<T, FnEq>& Deactivate() { m_active = false; return *this; }

    RigidBody<T, FnEq>& ResetOldValues();
};

using RigidBodyF = RigidBody<float, std::equal_to<float>>;
using RigidBodyD = RigidBody<double, std::equal_to<double> >;

#include "RigidBody.inl"
