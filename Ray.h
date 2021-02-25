#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Ray
{
public:
	Vector2f m_end;
	Vector2f m_pos;
	bool isHit;

	Ray() {};
	Ray(Vector2f pos, Vector2f dir);
	void calc_hit(Vector2f wall1, Vector2f wall2);

private:
	Vector2f m_relative_end;
};
