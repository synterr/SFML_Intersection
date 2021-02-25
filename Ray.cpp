#include "Ray.h"

Ray::Ray(Vector2f pos, Vector2f dir)
{
	// Set end point relative to mouse cursor
	// Add arbitrary length
	m_relative_end = dir * 3000.f;
	m_pos = pos;
	// Set end-point to (default length) distance away from mouse in set direction
	m_end = m_pos + m_relative_end;

	isHit = false;
}

// Reset end-point of Ray
void Ray::reset(Vector2f pos)
{
	//m_pos = pos;
	// Set end-point to (default length) distance away from mouse in set direction
	//m_end = m_pos + m_relative_end;
}

// Calculates intersection-point two lines
// Used for getting intersection between Ray and wall
// More or less black-box code
void Ray::calc_hit(Vector2f p3, Vector2f p4)
{
	const Vector2f p1 = m_pos;
	const Vector2f p2 = m_end;

	// Calculates denominator of equations
	const float den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

	if (den == 0)
		return;

	const float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
	const float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / den;

	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1)
	{
		// Gets intersection point
		m_end.x = p1.x + t * (p2.x - p1.x);
		m_end.y = p1.y + t * (p2.y - p1.y);
		isHit = true;
	}
}