#include "Ray.h"

Ray::Ray(Vector2f pos, Vector2f dir)
{
	// Set end point relative to mouse cursor
	// Add arbitrary length
	m_color = Color::Magenta;
	m_dir = VectorNormalize(dir);
	m_relative_end = m_dir * 3000.f;
	m_pos = pos + m_dir*1.f;
	// Set end-point to (default length) distance away from mouse in set direction
	m_end = m_pos + m_relative_end;

	m_isHit = false;
}


// Calculates intersection-point two lines
// Used for getting intersection between Ray and wall
// More or less black-box code
bool Ray::calc_hit(Vector2f p3, Vector2f p4)
{
	Vector2f p1 = m_pos;
	//p1 = p1 + m_dir * .1f; //Slight offset to avoid multiple collisions from the same segment
	const Vector2f p2 = m_end;

	// Calculates denominator of equations
	const double den = ((double)p1.x - p2.x) * ((double)p3.y - p4.y) - ((double)p1.y - p2.y) * ((double)p3.x - p4.x);

	if (den == 0)
		return false;

	const double t = (((double)p1.x - p3.x) * ((double)p3.y - p4.y) - ((double)p1.y - p3.y) * ((double)p3.x - p4.x)) / den;
	const double u = -(((double)p1.x - p2.x) * ((double)p1.y - p3.y) - ((double)p1.y - p2.y) * ((double)p1.x - p3.x)) / den;

	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1)
	{
		// Gets intersection point
		m_end.x = (float)(p1.x + t * ((double)p2.x - p1.x));
		m_end.y = (float)(p1.y + t * ((double)p2.y - p1.y));
		m_isHit = true;
		return true;
	}
	return false;
}

inline float Ray::VectorDotProduct(Vector2f v1, Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }
inline Vector2f Ray::VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v)); }