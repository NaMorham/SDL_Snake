#pragma once

#include <cstdint>
#include <functional>

/*
 ROTATE p1 around origin by r radians to p2

 p1.x = cos(r0), p1.y = sin(r0)
 p2.x = cos(r0+r), p2y = sin(r0+r)

 p2.x = cos(r0) * cos(r) - sin(r0) * sin(r)
 p2.x = p1.x * cos(r) - P1.y * sin(r)

 p2.y = sin(r0) * cos(r) + cos(r0) * sin(r)
 p2.y = p1.y * cos(r) + p1.x *  sin(r)
*/

//-----------------------------------------------------------------------------
template <typename T, class FnEq = std::equal_to<T>>
class Vec2DBase
{
private:
    union {
        struct {
            T m_x, m_y;
        };
        T m_data[2];
    };
    FnEq IsEqual;

public:
    Vec2DBase(const T& x = 0, const T& y = 0) : m_x{ x }, m_y{ y } {}
    Vec2DBase(const Vec2DBase& orig) : m_x{ orig.m_x }, m_y{ orig.m_y } {}
    virtual ~Vec2DBase() = default;

    [[nodiscard]] inline const T& X() const { return m_x; }
    inline T& X() { return m_x; }
    inline T& incX(const T& increment) { m_x += increment; return m_x; }
    inline Vec2DBase& X(const T& newX) { m_x = newX; return *this; }

    [[nodiscard]] inline const T& Y() const { return m_y; }
    inline T& Y() { return m_y; }
    inline T& incY(const T& increment) { m_y += increment; return m_y; }
    inline Vec2DBase& Y(const T& newY) { m_y = newY; return *this; }

    // use with care
    [[nodiscard]] inline const T* Data() const { return m_data; }
    [[nodiscard]] inline T* Data() { return m_data; }

    [[nodiscard]] const T& operator[](const size_t& idx) const;

    // normal operators
    Vec2DBase<T, FnEq>& operator=(const Vec2DBase& rhs);

    bool operator==(const Vec2DBase& rhs) const;
    bool operator!=(const Vec2DBase& rhs) const;

    bool operator<(const Vec2DBase& rhs) const = delete;
    bool operator<=(const Vec2DBase& rhs) const = delete;
    bool operator>(const Vec2DBase& rhs) const = delete;
    bool operator>=(const Vec2DBase& rhs) const = delete;

    [[nodiscard]] Vec2DBase operator+(const Vec2DBase& rhs) const;
    [[nodiscard]] Vec2DBase operator+(const T& rhs) const;
    Vec2DBase& operator+=(const Vec2DBase& rhs);
    Vec2DBase& operator+=(const T& rhs);

    [[nodiscard]] Vec2DBase operator-() const;
    [[nodiscard]] Vec2DBase operator-(const Vec2DBase& rhs) const;
    [[nodiscard]] Vec2DBase operator-(const T& rhs) const;
    Vec2DBase& operator-=(const Vec2DBase& rhs);
    Vec2DBase& operator-=(const T& rhs);

    [[nodiscard]] Vec2DBase operator*(const T& mul) const;

    Vec2DBase& operator*=(const T& mul);

    [[nodiscard]] Vec2DBase operator/(const T& div) const;

    Vec2DBase& operator/=(const T& div);

    [[nodiscard]] double Magnitude() const;

    [[nodiscard]] Vec2DBase Normalised() const;

    Vec2DBase& Normalise();

    [[nodiscard]] double Dot(const Vec2DBase& rhs) const;

    double AngleBetween(const Vec2DBase& rhs) const;
};

using Vec2DBaseF = Vec2DBase<float, std::equal_to<float> >;
using Vec2DBaseD = Vec2DBase<double, std::equal_to<double> >;

#include "Vec2D.inl"
