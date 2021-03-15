#include "Segment.h"

Segment::Segment(sf::Vector2f p0, sf::Vector2f p1, bool isSmooth, float ior)
{
	this->lensIndex = -1;
	this->m_smooth = isSmooth;
	this->m_p0 = p0;
	this->m_p1 = p1;
	this->ior = ior;

	float dx = m_p1.x - m_p0.x;
	float dy = m_p1.y - m_p0.y;
	m_n0 = Vector2f(-dy, dx);
	m_n1 = Vector2f(-dy, dx); //Vector2f(dy, -dx);
	m_n0 = VectorNormalize(m_n0);
	m_n1 = VectorNormalize(m_n1);
}

Segment::Segment()
{
	this->lensIndex = -1;
	this->m_smooth = false;
	this->ior = 1.0f;
	this->m_p0 = Vector2f(0, 0);
	this->m_p1 = Vector2f(0, 0);
	this->m_n0 = Vector2f(0, 0);
	this->m_n1 = Vector2f(0, 0);
}

Segment::~Segment()
{
}

inline float Segment::VectorDotProduct(Vector2f v1, Vector2f v2) {	return v1.x * v2.x + v1.y * v2.y; }

inline Vector2f Segment::VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v));}