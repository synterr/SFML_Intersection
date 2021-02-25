#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Ray
{
public:
	Vector2f m_end;
	Vector2f m_pos;
	Vector2f m_dir;
	Color m_color;
	bool m_isHit;

	Ray() {};
	Ray(Vector2f pos, Vector2f dir);
	bool calc_hit(Vector2f wall1, Vector2f wall2);

private:
	Vector2f m_relative_end;
	inline Vector2f VectorNormalize(Vector2f v);
	inline float VectorDotProduct(Vector2f v1, Vector2f v2);
};
