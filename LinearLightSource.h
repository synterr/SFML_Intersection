#pragma once
#include <SFML/Graphics.hpp>
#include "LightSource.h"

using namespace sf;

class LinearLightSource : public LightSource
{
public:
	LinearLightSource(float intensity, long rayCount, Vector2f position, float angle, Color color, float spreadAngle);
	~LinearLightSource();
	void UpdateLight(Vector2f pos);
	void UpdateLight();

public :

	float spreadAngle;
};

