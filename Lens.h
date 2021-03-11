// Lens is defined by Polygon shape and ior.

#pragma once
#include <SFML/Graphics.hpp>
#include "Polygon.h"
#include "Optical.h"

using namespace sf;
using namespace std;

class Lens : public Optical
{
public:
	Lens();
	Lens(Vector2f pos, float r1, float r2, float d, float l, float ior);
	~Lens();
	void Update(Vector2f pos);
	void Draw(RenderWindow& window);

public:
	float m_R1; // radius of left side surface  (if R1 < 0 flip normals for that surface!)
	float m_R2; // radius of right side surface (if R2 < 0 flip normals for that surface!)
	float m_D;  // diameter of the lens
	float m_L;  // lenght of the lens (square prism region)
	float m_ior;
	Polygon m_poly;

private:
};

