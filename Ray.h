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
	Vector2f m_normal; 
	Color m_color;
	bool m_isHit;

	Ray();
	Ray(Vector2f pos, Vector2f dir);
	bool calc_hit(Segment& seg);

private:
	Vector2f m_relative_end;
};
