#pragma once
#include <SFML/Graphics.hpp>
#include "Segment.h"

using namespace sf;
using namespace std;

class Polygon
{
public:
	Polygon();
	~Polygon();

public:
	vector<Vector2f> m_points;
	vector<Segment>  m_segments;
};

