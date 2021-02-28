
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
inline float VectorDotProduct(Vector2f &v1, Vector2f &v2) { return v1.x * v2.x + v1.y * v2.y; };
inline Vector2f VectorNormalize(Vector2f &v) { return v / sqrtf(VectorDotProduct(v, v)); }
inline Vector2f Reflect(Vector2f dir, Vector2f normal) { return dir - 2.0f * VectorDotProduct(dir, normal) * normal; }
inline float Clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
//Vector2f Refract(Vector2f dir, Vector2f normal, float eta) 
//{
//	normal = -normal;
//	float N_dot_I = VectorDotProduct(normal, dir);
//	eta = 2.0f - eta;
//	return (dir * eta - normal * (-N_dot_I + eta * N_dot_I));
//}

Vector2f Refract(Vector2f dir, Vector2f normal, float eta)
{
    normal = -normal;
    
    float cosi = Clamp(VectorDotProduct(dir, normal),-1.f,1.f);
    float etai = 1, etat = eta;
    Vector2f n = normal;
    if (cosi < 0) 
    { 
        cosi = -cosi; 
    }
    else 
    { 
        std::swap(etai, etat); n = -normal; 
    }
    float etac = etai / etat;
    float k = 1 - etac * etac * (1 - cosi * cosi);
    
    if (k < 0.f)
        return Vector2f(0.f, 0.f);
    else
        return etac * dir + (etac * cosi - sqrtf(k)) * n;
}

inline float RadToDeg(float Rad) {	return Rad / TWO_PI * 90.f;}
inline float DegToRad(float Deg) {	return Deg * TWO_PI / 360.f;}
