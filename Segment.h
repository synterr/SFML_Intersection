#pragma once
#include <SFML/Graphics.hpp>

class Segment
{
public:
	Segment(sf::Vector2f p0, sf::Vector2f p1);
	~Segment();

	sf::Vector2f p0;
	sf::Vector2f p1;
private:

};

