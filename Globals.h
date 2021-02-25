
#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include "ray.h"
#include "fps.h"
#include "PointLightSource.h"

//#include "wall.h"

using namespace sf;
using namespace std;

void UpdateLight();

constexpr auto TWO_PI = 3.14159265358979f * 2;
extern sf::Vector2f g_mouse_pos;

inline int random(int max, int min = 0) { return rand() % (max - min) + min; }