#pragma once
#include <SFML/Graphics.hpp>
#include "Polygon.h"

using namespace sf;
using namespace std;

class Lens
{
public:
	Lens();
	~Lens();

public:
	float ior;
	Polygon m_poly;

private:
};

