#include "PointLightSource.h"

PointLightSource::PointLightSource(float intensity, long rayCount, Vector2f position, float angle, Color color, float spreadAngle)
{
	this->intensity = intensity;
	this->rayCount = rayCount;
	this->position = position;
	this->angle = angle;
	this->color = color;

	this->spreadAngle = spreadAngle;
}

PointLightSource::~PointLightSource()
{
}

void PointLightSource::UpdateLight(Vector2f pos)
{
	this->position = pos;
		
	this->UpdateLight();
}

void PointLightSource::UpdateLight()
{

	float raySpread = this->spreadAngle; //Spread angle of light cone

	traces.clear();			//Clean all traces before light parameters will be updated

	if (this->rayCount > 0) //Minimum two rays
	{
		float step = raySpread / this->rayCount;
		for (float a = 0; a <= raySpread; a += step)
		{
			//float randomizer = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			//const float x = cos(randomizer * raySpread - raySpread / 2 + this->angle);
			//const float y = sin(randomizer * raySpread - raySpread / 2 + this->angle);

			const float x = cos(a  - raySpread / 2 + this->angle);
			const float y = sin(a  - raySpread / 2 + this->angle);

			Trace trace;							//Creating new trace
			vector<Ray> rays;						//Creating [0] rays vector (first cascade)
			trace.rays.push_back(rays);				//Push initial cascade into trace
			Ray ray = Ray(this->position, Vector2f(x, y));
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
		Ray ray = Ray(this->position, Vector2f(x, y));
		ray.intensity = this->intensity;
		trace.rays[0].push_back(ray);		//Push ray[0,0] into trace
		traces.push_back(trace);				//Push trace to traces vector		
	}
}
