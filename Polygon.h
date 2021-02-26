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

	void generateSegments();
	void smoothNormals();

private:
	Segment& findSegmentByPoint(Segment& s, bool& found, int p0_or_1, int f0_or_1);
	inline float VectorCmp(Vector2f& v0, Vector2f& v1);
};

