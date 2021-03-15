//Segment is defined by points P0, P1 and normals for each of these points

#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Segment
{
public:
	
	int lensIndex; //store lens index for rayhit optimize algorithm

	bool m_smooth;   //tru if segment should be smoothed
	float ior;     //IOR for current segment - defined by Lens class
	Vector2f m_p0;
	Vector2f m_p1;
	Vector2f m_n0; //normal vector1
	Vector2f m_n1; //normal vector2
	


public:
	Segment(sf::Vector2f p0, sf::Vector2f p1, bool isSmooth, float ior);
	Segment();
	~Segment();
	
	inline float VectorDotProduct(Vector2f v1, Vector2f v2);
	inline Vector2f VectorNormalize(Vector2f v);
private:

};

