#pragma once
#include <SFML/Graphics.hpp>
#include "Segment.h"

using namespace sf;

class Ray
{
public:
	Vector2f m_end;
	Vector2f m_pos;
	Vector2f m_dir;
	float intensity;
	//When ray hits any segment, automatically stores a normal of this segment at hitpoint.
	bool m_isHit;
	Vector2f m_normal;

	Ray();
	Ray(Vector2f pos, Vector2f dir);
	bool calc_hit(Segment& seg);

private:
	Vector2f m_relative_end;
};
