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
bool Ray::calc_hit(Segment &seg)
{
	Vector2f p1 = m_pos;
	//p1 = p1 + m_dir * .1f; //Slight offset to avoid multiple collisions from the same segment
	const Vector2f p2 = m_end;

	// Calculates denominator of equations
	const float dpx = p1.x - p2.x;
	const float dpy = p1.y - p2.y;

	const float dsx = seg.m_p0.x - seg.m_p1.x;
	const float dsy = seg.m_p0.y - seg.m_p1.y;

	const float den = (dpx) * (dsy) - (dpy) * (dsx);

	if (den == 0)
		return false;
		
	const float t = ((p1.x - seg.m_p0.x) * (dsy) - (p1.y - seg.m_p0.y) * (dsx)) / den;
	const float u = -((dpx) * (p1.y - seg.m_p0.y) - (dpy) * (p1.x - seg.m_p0.x)) / den;

	// Make normal smoothing below!!!!!!!!!!!!!!!!!!!!!
	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1)
	{
		// Gets intersection point
		m_end.x = p1.x + t * -dpx;
		m_end.y = p1.y + t * -dpy;
		m_isHit = true;
		
		m_n1 = seg.m_n0 * t + seg.m_n1 * (1-t);
		return true;
	}
	return false;
}

inline float Ray::VectorDotProduct(Vector2f v1, Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }
inline Vector2f Ray::VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v)); }