// Polygon is defined by points and segments

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

	void generateSegmentsNew(Vector2f pos); //probably should be moved to Lens class
	void generateSegments(); //probably should be moved to Lens class
	void smoothNormals();	 //probably should be moved to Lens class

private:
	//probably should be moved to Lens class
	Segment& findSegmentByPoint(Segment& s, bool& found, int p0_or_1, int f0_or_1);
	//probably should be moved to Lens class
	inline float VectorCmp(Vector2f& v0, Vector2f& v1);
	inline float VectorDotProduct(Vector2f v1, Vector2f v2);
	inline Vector2f VectorNormalize(Vector2f v);
};

