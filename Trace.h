#pragma once
#include <SFML/Graphics.hpp>
#include "Ray.h"

using namespace sf;
using namespace std;

class Trace
{
public:
	Trace();
	~Trace();

public:
	vector<vector<Ray>> rays;
};

