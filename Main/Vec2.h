#pragma once

template <typename T>
class Vec2
{
public:
	Vec2() = default;
	Vec2(T x, T y)
		:
		x(x),
		y(y)
	{  }
	T GetX() const
	{
		return x;
	}
	T GetY() const
	{
		return y;
	}
	void SetX(T x_in)
	{
		x = x_in;
	}
	void SetY(T y_in)
	{
		y = y_in;
	}
	Vec2 operator+(const Vec2& rhs)
	{
		return Vec2{ x + rhs.GetX(), y + rhs.GetY() };
	}
	Vec2 operator-(const Vec2& rhs)
	{
		return Vec2{ x - rhs.GetX(), y - rhs.GetY() };
	}
	template <typename T2>
	operator Vec2<T2>()
	{
		return Vec2{ (T2)x, (T2)y };
	}
	bool operator==(const Vec2& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}
private:
	T x;
	T y;
};