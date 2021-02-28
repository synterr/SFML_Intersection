#include "LightSource.h"

LightSource::LightSource()
{
	this->angle = 0;
	this->color = Color::Magenta;
	this->intensity = 1;
	this->position = Vector2f(0, 0);
	this->rayCount = 1;
	
}

LightSource::~LightSource()
{
}

void LightSource::UpdateLight()
{
}
