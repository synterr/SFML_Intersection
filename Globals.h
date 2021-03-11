
#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include "ray.h"
#include "fps.h"

//#include "wall.h"

using namespace sf;
using namespace std;

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

Vector2f Refract(Vector2f& dir, Vector2f& normal, float ior)
{
    normal = -normal;
    
    float cosi = Clamp(VectorDotProduct(dir, normal),-1.f,1.f);
    float etai = 1, etat = ior;
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


void Fresnel(Vector2f &dir, Vector2f &normal, float ior, float& kr)
{
    float cosi = Clamp(VectorDotProduct(dir, normal), -1.f, 1.f);
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1.0f - cosi * cosi));
    // Total internal reflection
    if (sint >= 1.f)
    {
        kr = 1;
    }
    else
    {
        float cost = (std::max(0.f, 1.0f - abs(sint) ));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs*Rs + Rp*Rp)/2.f;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

void FresnelS(Vector2f& dir, Vector2f& normal, float ior, float& kr)
{
    // Schlick aproximation
    float etai = 1.0f, etat = ior;
    float r0 = (etai - etat) / (etai + etat);
    r0 *= r0;
    float cosi = -VectorDotProduct(normal, dir);
    if (cosi > 0) { std::swap(etai, etat); }
    else { cosi = -cosi; }
    if (etai > etat)
    {
        float n = etai / etat;
        float sinT2 = n * n * (1.0f - cosi * cosi);
        // Total internal reflection
        if (sinT2 > 1.0f)
        {
            kr = 1.0f;
            return;
        }
        cosi = sqrt(1.0 - sinT2);
    }
    float x = 1.0f - cosi;
    float ret = r0 + (1.0f - r0) * x * x * x * x * x;

    // adjust reflect multiplier for object reflectivity
    //float OBJECT_REFLECTIVITY = 0.3f;
        //kr = (OBJECT_REFLECTIVITY + (1.0f - OBJECT_REFLECTIVITY) * ret);
        kr = ret;
    return;
}

inline float RadToDeg(float Rad) {	return Rad / TWO_PI * 90.f;}
inline float DegToRad(float Deg) {	return Deg * TWO_PI / 360.f;}
