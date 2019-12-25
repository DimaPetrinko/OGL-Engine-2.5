#include "Math.h"

Vector2::Vector2() : Vector2(0,0) {}

Vector2::Vector2(const float x, const float y)
{
	this->x = x;
	this->y = y;
}

void Vector2::Normalize()
{
	auto m = Magnitude();
	x /= m;
	y /= m;
}
Vector2 Vector2::Normalized() const
{
	auto m = Magnitude();
	return Vector2(x / m, y / m);
}
float Vector2::Magnitude() const
{
	return sqrt(x * x + y * y);
}

Vector2 Vector2::operator+ (const Vector2 other) const
{
	return Vector2(x + other.x, y + other.y);
}
void Vector2::operator+= (const Vector2 other)
{
	x += other.x;
	y += other.y;
}

Vector2 Vector2::operator- (const Vector2 other) const
{
	return Vector2(x - other.x, y - other.y);
}
void Vector2::operator-= (const Vector2 other)
{
	x -= other.x;
	y -= other.y;
}

Vector2 Vector2::operator* (const float scale) const
{
	return Vector2(x * scale, y * scale);
}
void Vector2::operator*= (const float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 Vector2::operator/ (const float scale) const
{
	return Vector2(x / scale, y / scale);
}
void Vector2::operator/= (const float scale)
{
	x /= scale;
	y /= scale;
}

Vector2 Vector2::operator* (const Vector2 other) const
{
	return Vector2(x * other.x, y * other.y);
}
void Vector2::operator*= (const Vector2 other)
{
	x *= other.x;
	y *= other.y;
}

Vector2 Vector2::operator/ (const Vector2 other) const
{
	return Vector2(x / other.x, y / other.y);
}
void Vector2::operator/= (const Vector2 other)
{
	x /= other.x;
	y /= other.y;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}
Vector2 Vector2::Cross(const Vector2& a)
{
	return Vector2(a.y, -a.x);
}
/*Angle in radians*/
float Vector2::Angle(const Vector2& a, const Vector2& b)
{
	return acosf(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
}