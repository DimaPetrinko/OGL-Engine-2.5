#pragma once

namespace Utils
{
	const float PI = 3.14159265359f;
	const float RAD_TO_DEG = 57.2957795131f;
	const float DEG_TO_RAD = 0.01745329251f;

	struct Vector2
	{
		float x, y;

		Vector2();
		Vector2(float x, float y);

		void Normalize();
		Vector2 Normalized() const;
		float Magnitude() const;

		Vector2 operator+ (Vector2 other) const;
		void operator+= (Vector2 other);

		Vector2 operator- (Vector2 other) const;
		void operator-= (Vector2 other);

		Vector2 operator* (float scale) const;
		void operator*= (float scale);

		Vector2 operator/ (float scale) const;
		void operator/= (float scale);

		Vector2 operator* (Vector2 other) const;
		void operator*= (Vector2 other);

		Vector2 operator/ (Vector2 other) const;
		void operator/= (Vector2 other);

		static float Dot(const Vector2& a, const Vector2& b);
		static Vector2 Cross(const Vector2& a);
		static float Angle(const Vector2& a, const Vector2& b);

		inline static Vector2 One() { return Vector2(1, 1); }
		inline static Vector2 Zero() { return Vector2(0, 0); }
		inline static Vector2 Up() { return Vector2(0, 1); }
		inline static Vector2 Right() { return Vector2(1, 0); }
	};
}