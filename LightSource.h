#pragma once
#include <SFML/Graphics.hpp>
#include "Trace.h"

using namespace sf;
using namespace std;

class LightSource
{
public:
	LightSource();
	~LightSource();

	virtual void UpdateLight();

public:
	
	long rayCount;
	Vector2f position;
	float angle;
	float intensity;
	Color color;
	vector<Trace> traces;

};

