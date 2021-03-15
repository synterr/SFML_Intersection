//Optical defines any optical object position and rectangulat boundary (mainly for mouse input detection and selection)

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Optical
{
public:
	Optical();
	~Optical();

public:
	Vector2f m_pos;
	float m_angle; 
	IntRect m_bounds;

private:
};

