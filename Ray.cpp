#include "Ray.h"

Ray::Ray(Vector2f pos, Vector2f dir)
{
	// Set end point relative to mouse cursor
	// Add arbitrary length
	//m_color = Color::Magenta;
	
	m_dir = dir;
	m_relative_end = m_dir * 3000.f;
	m_pos = pos + m_dir;
	// Set end-point to (default length) distance away from mouse in set direction
	m_end = m_pos + m_relative_end;

	m_isHit = false;
}

Ray::Ray()
{
	m_isHit = false;
}

bool Ray::calc_hit(Segment &seg)
{
	Vector2f p1 = m_pos;
	const Vector2f p2 = m_end;

	// Calculates denominator of equations
	const float dpx = p1.x - p2.x;
	const float dpy = p1.y - p2.y;

	const float dsx = seg.m_p0.x - seg.m_p1.x;
	const float dsy = seg.m_p0.y - seg.m_p1.y;

	const float den = (dpx) * (dsy) - (dpy) * (dsx);

	//parallel vectors
	if (abs(den) < 0.01f)
		return false;
		
	const float t = ((p1.x - seg.m_p0.x) * (dsy) - (p1.y - seg.m_p0.y) * (dsx)) / den;
	const float u = -((dpx) * (p1.y - seg.m_p0.y) - (dpy) * (p1.x - seg.m_p0.x)) / den;

	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1)
	{
		// Gets intersection point
		m_end.x = p1.x + t * -dpx;
		m_end.y = p1.y + t * -dpy;

		float l;
		if (abs(dsy) > abs(dsx))
			l = ((m_end.y - seg.m_p0.y) / -dsy );	//linear blend of normals
		else
			l = ((m_end.x - seg.m_p0.x) / -dsx );	//linear blend of normals

		//store calculated normal from blend calculations
		m_normal = (seg.m_n0 * (1.f-l ) + seg.m_n1 * (l));
		m_isHit = true;

		//printf("n0: %f, %f n1: %f, %f \n", seg.m_n0.x, seg.m_n0.y, seg.m_n1.x, seg.m_n1.y);
		//printf("l: %f, end: %f, absdsx: %f, segx: %f \n", l, m_end.x, dsx, seg.m_p0.x);
		//printf("main0: %f, %f \n\n", m_normal.x, m_normal.y);

		return true;
	}
	return false;
}
