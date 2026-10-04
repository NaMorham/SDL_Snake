//--------------------------------------------------------------------------------
template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator=(const Vec2DBase& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    return *this;
}

template <typename T, class FnEq>
const T& Vec2DBase<T, FnEq>::operator[](const size_t& idx) const
{
    if (idx >= 2) {
        return 0.0; // TODO: Throw and/or return NaN
    }
    return m_data[idx];
}

template <typename T, class FnEq>
bool Vec2DBase<T, FnEq>::operator==(const Vec2DBase<T, FnEq>& rhs) const
{
    return IsEqual(rhs);
}

template <typename T, class FnEq>
bool Vec2DBase<T, FnEq>::operator!=(const Vec2DBase<T, FnEq>& rhs) const
{
    return !IsEqual(rhs);
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator+(const Vec2DBase<T, FnEq>& rhs) const
{
    Vec2DBase<T, FnEq> pt{ m_x + rhs.m_x, m_y + rhs.m_y };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator+(const T& rhs) const
{
    Vec2DBase<T, FnEq> pt{ m_x + rhs, m_y + rhs };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator+=(const Vec2DBase<T, FnEq>& rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    return *this;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator+=(const T& rhs)
{
    m_x += rhs;
    m_y += rhs;
    return *this;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator-() const
{
    Vec2DBase<T, FnEq> pt{ -m_x, -m_y };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator-(const Vec2DBase<T, FnEq>& rhs) const
{
    Vec2DBase<T, FnEq> pt{ m_x - rhs.m_x, m_y - rhs.m_y };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator-(const T& rhs) const
{
    Vec2DBase<T, FnEq> pt{ m_x - rhs, m_y - rhs };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator-=(const Vec2DBase<T, FnEq>& rhs)
{
    m_x -= rhs.m_x;
    m_y -= rhs.m_y;
    return *this;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator-=(const T& rhs)
{
    m_x -= rhs;
    m_y -= rhs;
    return *this;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator*(const T& mul) const
{
    Vec2DBase<T, FnEq> pt{ m_x * mul, m_y * mul };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator*=(const T& mul)
{
    m_x *= mul;
    m_y *= mul;
    return *this;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::operator/(const T& div) const
{
    // TODO: Throw
    if (div == 0) {
        return Vec2DBase<T, FnEq>{ 0, 0 };
    }

    Vec2DBase<T, FnEq> pt{ m_x / div, m_y / div };
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::operator/=(const T& div)
{
    // TODO: Throw
    if (div != 0) {
        m_x /= div;
        m_y /= div;
    }
    return *this;
}

template <typename T, class FnEq>
double Vec2DBase<T, FnEq>::Magnitude() const
{
    return std::sqrt((m_x * m_x) + (m_y * m_y));
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq> Vec2DBase<T, FnEq>::Normalised() const
{
    // this is a bit stupid with integer types
    double mag{ this->Magnitude() };
    Vec2DBase<T, FnEq> pt;
    if (mag != 0.0) {
        pt.m_x = static_cast<T>(std::round(static_cast<double>(m_x) / mag));
        pt.m_y = static_cast<T>(std::round(static_cast<double>(m_y) / mag));
    }
    return pt;
}

template <typename T, class FnEq>
Vec2DBase<T, FnEq>& Vec2DBase<T, FnEq>::Normalise()
{
    double mag{ Magnitude() };
    if (mag != 0.0) {
        m_x = static_cast<T>(std::round(static_cast<double>(m_x) / mag));
        m_y = static_cast<T>(std::round(static_cast<double>(m_y) / mag));
    }
    return *this;
}

template <typename T, class FnEq>
double Vec2DBase<T, FnEq>::Dot(const Vec2DBase<T, FnEq>& rhs) const
{
    return ((m_x * rhs.m_x) + (m_y * rhs.m_y));
}

template <typename T, class FnEq>
double Vec2DBase<T, FnEq>::AngleBetween(const Vec2DBase<T, FnEq>& rhs) const
{
    return std::acos(this->Normalised().Dot(rhs.Normalised()));
}
