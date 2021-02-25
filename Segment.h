#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Segment
{
public:
	Segment(sf::Vector2f p0, sf::Vector2f p1);
	~Segment();

	Vector2f p0;
	Vector2f p1;
	Vector2f n1; //normal vector1
	//Vector2f n2; //normal vector2
	
	inline float VectorDotProduct(Vector2f v1, Vector2f v2);
	inline Vector2f VectorNormalize(Vector2f v);
private:

};

