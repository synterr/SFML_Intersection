
#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include "ray.h"
#include "fps.h"
#include "PointLightSource.h"
#include "LinearLightSource.h"

//#include "wall.h"

using namespace sf;
using namespace std;

extern bool smoothing = true;

inline float RadToDeg(float Rad);
inline float DegToRad(float Deg);

constexpr auto TWO_PI = 3.14159265358979f * 2;
//extern sf::Vector2f g_mouse_pos;

inline int random(int max, int min = 0) { return rand() % (max - min) + min; }
inline float VectorDotProduct(Vector2f v1, Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; };
inline Vector2f VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v)); }