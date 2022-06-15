#include "Lens.h"

Lens::Lens()
{
	m_isSelected = false;
	m_angle = 0;
	m_pos = Vector2f(100.f, 100.f);
	m_R1 = 1.0f;
	m_R2 = 1.0f;
	m_D = 1.0f;
	m_L = 1.0f;
	m_ior = 1.0f;
}

Lens::Lens(Vector2f pos, float r1, float r2, float d, float l, float ior)
{
	m_isSelected = false;
	m_angle = 0;
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

void Lens::Update(Vector2f pos, float angle)
{
	m_poly.reset();

	m_pos = pos;
	m_angle = angle;

	constexpr auto TWO_PI = 3.14159265358979f * 2;
	int currPointIndex = 0;
	
	unsigned int maxPoints = 30;
	
	//Construct bulk rectangle top surface
	{
		for (float l = 0; l <= m_L; l += m_L / ((float)maxPoints / 10.f))
		{
			Vector2f point = Vector2f(l - m_L / 2, -m_D / 2);
			RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
			m_poly.m_points.push_back(point);
		}

		m_poly.generateSegmentsNew(m_pos, true, currPointIndex, m_ior); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size()+1;
	}
	//Construct bulk rectangle bottom surface
	{
		for (float l = m_L; l >= 0; l -= m_L / ((float)maxPoints / 10.f))
		{
			Vector2f point = Vector2f(l - m_L / 2, m_D / 2);
			RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
			m_poly.m_points.push_back(point);
		}

		m_poly.generateSegmentsNew(m_pos, true, m_ior, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size()+1;
	}

	//Construct Left lens surface
	{
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
				Vector2f point = Vector2f(center.x + radius * cos(a + rot), center.y + radius * sin(a + rot));
				RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
				m_poly.m_points.push_back(point);
			}
		}
		else
		{
			angleBegin = TWO_PI / 2 + maxAngle;
			angleEnd   = TWO_PI / 2 - maxAngle;

			for (float a = angleBegin; a > angleEnd - step / 2.f; a -= step)
			{
				Vector2f point = Vector2f(center.x + radius * cos(a + rot), center.y + radius * sin(a + rot));
				RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
				m_poly.m_points.push_back(point);

			}
		}
		
		m_poly.generateSegmentsNew(m_pos, true, m_ior, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size()+1;
	}
	//Construct Right lens surface
	{
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
				Vector2f point = Vector2f(center.x + radius * cos(a + rot), center.y + radius * sin(a + rot));
				RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
				m_poly.m_points.push_back(point);

			}
		}
		else
		{
			angleBegin = TWO_PI / 2 - maxAngle;
			angleEnd = TWO_PI / 2 + maxAngle;

			for (float a = angleBegin; a < angleEnd + step / 2.f; a += step)
			{
				Vector2f point = Vector2f(center.x + radius * cos(a + rot), center.y + radius * sin(a + rot));
				RotatePoint(point, Vector2f(0.f, 0.f), m_angle);
				m_poly.m_points.push_back(point);

			}
		}

		m_poly.generateSegmentsNew(m_pos, true, m_ior, currPointIndex); // true if smoothing enabled
		currPointIndex = m_poly.m_points.size() + 1;
	}
	m_poly.smoothNormals();

	this->m_bounds = m_poly.m_bounds;

	//this->m_bounds.top = m_pos.y - m_poly.m_bounds.height / 2;
	//this->m_bounds.left = m_pos.x - m_poly.m_bounds.width / 2;
	//this->m_bounds.width = m_poly.m_bounds.width;
	//this->m_bounds.height = m_poly.m_bounds.height;

}

void Lens::Draw(RenderWindow& window)
{
	Color lens_color;
	if (m_isSelected)
		lens_color = Color(0, 100, 200, 255);
	else
		lens_color = Color(100, 200, 200, 200);

	Color normal_color = Color(255, 255, 0, 200);
	Color normal_color1 = Color(255, 0, 255, 200);
	VertexArray lens_line(Lines, 2);
	VertexArray normal_line(Lines, 2);
	VertexArray normal_line1(Lines, 2);
	lens_line[0].color = lens_color;
	lens_line[1].color = lens_color;
	normal_line[0].color = normal_color;
	normal_line[1].color = normal_color;
	normal_line1[0].color = normal_color1;
	normal_line1[1].color = normal_color1;


	for (int i = 0; i < m_poly.m_segments.size(); i++)
	{
		lens_line[0].position = m_poly.m_segments[i].m_p0;
		lens_line[1].position = m_poly.m_segments[i].m_p1;
		window.draw(lens_line);
		//normal_line[0].position = m_poly.m_segments[i].m_p0;
		//normal_line[1].position = m_poly.m_segments[i].m_p0 + m_poly.m_segments[i].m_n0*20.f;
		//window.draw(normal_line);
		//normal_line1[0].position = m_poly.m_segments[i].m_p1;
		//normal_line1[1].position = m_poly.m_segments[i].m_p1 + m_poly.m_segments[i].m_n1 * 30.f;
		//window.draw(normal_line1);

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

void Lens::RotatePoint(Vector2f& p, Vector2f c, float angle)
{
		float si = sin(angle);
		float cs = cos(angle);

		Vector2f point = p;

		// translate point back to origin:
		point.x -= c.x;
		point.y -= c.y;

		// rotate and translate point back:
		p.x = point.x * cs - point.y * si + c.x;
		p.y = point.x * si + point.y * cs + c.y;
}