#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "LightSource.h"

using namespace sf;

class PointLightSource : public LightSource
{
public:
	PointLightSource(float intensity, long rayCount, Vector2f position, float angle, Color color, float spreadAngle);
	~PointLightSource();
	void UpdateLight(Vector2f pos);
	void UpdateLight();

public :

	float spreadAngle;
};

