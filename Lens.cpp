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
	m_R2 = r2;
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
	unsigned int maxPoints = 100;
	float radius = m_R1;
	float rot =  -TWO_PI / 4;
	Vector2f center = Vector2f(0.f, 0.f);

	float step = TWO_PI/2 / (float)maxPoints * 2.f;


	for (float a = 0; a < TWO_PI/2 + step / 2.f; a += step)
	{
		float x = radius * cos(a + rot);
		float y = radius * sin(a + rot);

		m_poly.m_points.push_back(Vector2f(center.x + x, center.y + y));
	}
	
	m_poly.generateSegmentsNew(m_pos);
	//m_poly.smoothNormals();
}

void Lens::Draw(RenderWindow& window)
{
	Color lens_color = Color(0, 255, 0, 200);
	VertexArray lens_line(Lines, 2);
	lens_line[0].color = lens_color;
	lens_line[1].color = lens_color;

	for (int i = 0; i < m_poly.m_segments.size(); i++)
	{
		lens_line[0].position = m_poly.m_segments[i].m_p0;
		lens_line[1].position = m_poly.m_segments[i].m_p1;
		window.draw(lens_line);
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
