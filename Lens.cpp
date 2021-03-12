#include "Lens.h"

Lens::Lens()
{
	m_pos = Vector2f(100.f, 100.f);
	m_R1 = 1.0f;
	m_R2 = 1.0f;
	m_D = 1.0f;
	m_L = 1.0f;
	m_ior = 1.0f;
}

Lens::Lens(Vector2f pos, float r1, float r2, float d, float l, float ior)
{
	m_pos = pos;
	m_R1 = r1;
	m_R2 = -r2;
	m_D  = d;
	m_L  = l;
	m_ior = ior;
}

Lens::~Lens()
{
}

void Lens::Update(Vector2f pos)
{
	m_pos = pos;
	constexpr auto TWO_PI = 3.14159265358979f * 2;
	int currPointIndex = 0;
	
	//Construct bulk rectangle top surface
	{
		m_poly.m_points.push_back(Vector2f(-m_L/2, -m_D/2));
		m_poly.m_points.push_back(Vector2f(m_L / 2, -m_D / 2));

		m_poly.generateSegmentsNew(m_pos, false, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size() + 1;
	}
	//Construct bulk rectangle bottom surface
	{
		m_poly.m_points.push_back(Vector2f(m_L / 2, m_D / 2));
		m_poly.m_points.push_back(Vector2f(-m_L / 2, m_D / 2));

		m_poly.generateSegmentsNew(m_pos, false, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size() + 1;
	}

	//Construct Left lens surface
	{
		unsigned int maxPoints = 100;
		float radius = abs(m_R1);
		float rot = TWO_PI / 2;
		float maxAngle = asinf((m_D / 2) / abs(m_R1));
		float angleBegin;
		float angleEnd;
		float step;
		Vector2f center;

		center = Vector2f(m_R1 * cos(maxAngle) - m_L / 2, 0.f);
		step = maxAngle * 2 / (float)maxPoints * 2.f;

		if (m_R1 > 0)
		{
			angleBegin = -maxAngle;
			angleEnd = maxAngle;

			for (float a = angleBegin; a < angleEnd + step / 2.f; a += step)
			{
				float x = radius * cos(a + rot);
				float y = radius * sin(a + rot);

				m_poly.m_points.push_back(Vector2f(center.x + x, center.y + y));
			}
		}
		else
		{
			angleBegin = TWO_PI / 2 + maxAngle;
			angleEnd   = TWO_PI / 2 - maxAngle;

			for (float a = angleBegin; a > angleEnd - step / 2.f; a -= step)
			{
				float x = radius * cos(a + rot);
				float y = radius * sin(a + rot);

				m_poly.m_points.push_back(Vector2f(center.x + x, center.y + y));
			}
		}
				

		m_poly.generateSegmentsNew(m_pos, true, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size()+1;
	}
	//Construct Right lens surface
	{
		unsigned int maxPoints = 100;
		float radius = abs(m_R2);
		float rot = TWO_PI / 2;
		float maxAngle = asinf((m_D / 2) / abs(m_R2));
		float angleBegin;
		float angleEnd;
		float step;
		Vector2f center;

		center = Vector2f(m_R2 * cos(maxAngle) + m_L / 2, 0.f);
		step = maxAngle * 2 / (float)maxPoints * 2.f;

		if (m_R2 > 0)
		{
			angleBegin = maxAngle;
			angleEnd = -maxAngle;

			for (float a = angleBegin; a > angleEnd - step / 2.f; a -= step)
			{
				float x = radius * cos(a + rot);
				float y = radius * sin(a + rot);

				m_poly.m_points.push_back(Vector2f(center.x + x, center.y + y));
			}
		}
		else
		{
			angleBegin = TWO_PI / 2 - maxAngle;
			angleEnd = TWO_PI / 2 + maxAngle;

			for (float a = angleBegin; a < angleEnd + step / 2.f; a += step)
			{
				float x = radius * cos(a + rot);
				float y = radius * sin(a + rot);

				m_poly.m_points.push_back(Vector2f(center.x + x, center.y + y));
			}
		}


		m_poly.generateSegmentsNew(m_pos, true, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size() + 1;
	}
	m_poly.smoothNormalsNew();
}

void Lens::Draw(RenderWindow& window)
{
	Color lens_color = Color(0, 255, 0, 200);
	Color normal_color = Color(255, 255, 0, 200);
	VertexArray lens_line(Lines, 2);
	VertexArray normal_line(Lines, 2);
	lens_line[0].color = lens_color;
	lens_line[1].color = lens_color;
	normal_line[0].color = normal_color;
	normal_line[1].color = normal_color;

	for (int i = 0; i < m_poly.m_segments.size(); i++)
	{
		lens_line[0].position = m_poly.m_segments[i].m_p0;
		lens_line[1].position = m_poly.m_segments[i].m_p1;
		window.draw(lens_line);
		normal_line[0].position = m_poly.m_segments[i].m_p0;
		normal_line[1].position = m_poly.m_segments[i].m_p0 + m_poly.m_segments[i].m_n0*20.f;
		//window.draw(normal_line);
		/*CircleShape cir;
		cir.setRadius(1);
		cir.setFillColor(Color::Transparent);
		cir.setOutlineThickness(1);
		cir.setOutlineColor(Color::Red);
		cir.setPosition(circle.m_segments[i].m_p0);
		window.draw(cir);
		cir.setPosition(circle.m_segments[i].m_p1);
		window.draw(cir);*/

	}

}
