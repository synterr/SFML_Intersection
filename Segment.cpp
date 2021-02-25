#include "Segment.h"

Segment::Segment(sf::Vector2f p0, sf::Vector2f p1)
{
	this->p0 = p0;
	this->p1 = p1;
	float dx = p1.x - p0.x;
	float dy = p1.y - p0.y;
	n1 = Vector2f(-dy, dx);
	//n2 = Vector2f(dy, -dx);
	float normalize = 1/sqrtf(VectorDotProduct(n1, n1));
	n1 = n1 * normalize;
	//n2 = n2 * normalize;
}

Segment::~Segment()
{
}

inline float Segment::VectorDotProduct(Vector2f v1, Vector2f v2) {	return v1.x * v2.x + v1.y * v2.y; }

inline Vector2f Segment::VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v));}