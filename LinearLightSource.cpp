#include "LinearLightSource.h"

LinearLightSource::LinearLightSource(float intensity, long rayCount, Vector2f position, float angle, Color color, float spreadAngle)
{
	this->intensity = intensity;
	this->rayCount = rayCount;
	this->position = position;
	this->angle = angle;
	this->color = color;

	this->spreadAngle = spreadAngle;
}

LinearLightSource::~LinearLightSource()
{
}

void LinearLightSource::UpdateLight(Vector2f pos)
{
	this->position = pos;
		
	this->UpdateLight();
}

void LinearLightSource::UpdateLight()
{
	constexpr float TWO_PI = 3.14159265358979f * 2;
	float raySpread = this->spreadAngle; //Spread angle of light cone

	traces.clear();			//Clean all traces before light parameters will be updated

	if (this->rayCount > 0) //Minimum two rays
	{
		float step = raySpread / this->rayCount;
		for (float l = -raySpread/2; l <= raySpread/2; l += step)
		{
			const float x = cos(this->angle);
			const float y = sin(this->angle);

			const float xp = cos(this->angle + TWO_PI / 4);
			const float yp = sin(this->angle + TWO_PI / 4);
			
			Vector2f pos = Vector2f(this->position.x + l * xp, this->position.y + l * yp);

			Trace trace;							//Creating new trace
			vector<Ray> rays;						//Creating [0] rays vector (first cascade)
			trace.rays.push_back(rays);				//Push initial cascade into trace
			Ray ray = Ray(pos, Vector2f(x, y));
			ray.intensity = this->intensity;
			trace.rays[0].push_back(ray);		//Push ray[0,0] into trace
			traces.push_back(trace);				//Push trace to traces vector
		}
	}
	else if (this->rayCount == 0) //Only one ray
	{
		const float x = cos(this->angle);
		const float y = sin(this->angle);

		Trace trace;							//Creating new trace
		vector<Ray> rays;						//Creating [0] rays vector (first cascade)
		trace.rays.push_back(rays);				//Push initial cascade into trace
		trace.rays[0].push_back(Ray(this->position, Vector2f(x, y)));		//Push ray[0,0] into trace
		traces.push_back(trace);				//Push trace to traces vector		
	}
}
