#include "Polygon.h"

Polygon::Polygon()
{
}

Polygon::~Polygon()
{
}
//probably should be moved to Lens class
void Polygon::generateSegments(bool isSmooth, float ior)
{
	int i = 0;
	Vector2f lastPoint;
	for (auto it : this->m_points)
	{
		if (i % 2 != 0) //if i > 0 for continous convex
			this->m_segments.push_back(Segment(lastPoint, it, isSmooth, ior));
		lastPoint = it;
		i++;
	}

#ifdef NDEBUG
	//printf("Points: %i Segments: %i\n", (int)m_points.size(), (int)m_segments.size());

	//for (int i = 0; i < this->m_segments.size(); i++)
	//{
		/*printf("Segment: %i\n", i);
		printf("p0: %f, %f p1: %f, %f \n", this->m_segments[i].m_p0.x, this->m_segments[i].m_p0.y, this->m_segments[i].m_p1.x, this->m_segments[i].m_p1.y);
		printf("n0: %f, %f n1: %f, %f \n", this->m_segments[i].m_n0.x, this->m_segments[i].m_n0.y, this->m_segments[i].m_n1.x, this->m_segments[i].m_n1.y);*/
	//}
#endif	
}

void Polygon::generateSegmentsNew(Vector2f pos, bool isSmooth, float ior, int startIndex)
{
	int i = 0;
	Vector2f lastPoint;
	Vector2f curPoint;

	for (auto it : this->m_points)
	{
		if (i > 0) //if i > 0 for continous convex
		{
			curPoint = it;
			curPoint.x += pos.x;
			curPoint.y += pos.y;

			if (i >= startIndex)
				this->m_segments.push_back(Segment(lastPoint, curPoint, isSmooth, ior)); //New segment already has normals calculated in constructor
			/*if (m_segments.size() > 1 && smoothNormals)
			{
				Vector2f smoothedNormal;
				smoothedNormal = VectorNormalize(m_segments[m_segments.size() - 2].m_n1 + m_segments[m_segments.size()-1].m_n0);
				m_segments[m_segments.size() - 2].m_n1 = smoothedNormal;
				m_segments[m_segments.size()-1].m_n0 = smoothedNormal;
			}*/
		}
		lastPoint = it;
		lastPoint.x += pos.x;
		lastPoint.y += pos.y;
		i++;
	}

	/*printf("Points: %i Segments: %i\n", (int)m_points.size(), (int)m_segments.size());

	for (int i = 0; i < this->m_segments.size(); i++)
	{
		printf("Segment: %i\n", i);
		printf("p0: %f, %f p1: %f, %f \n", this->m_segments[i].m_p0.x, this->m_segments[i].m_p0.y, this->m_segments[i].m_p1.x, this->m_segments[i].m_p1.y);
		printf("n0: %f, %f n1: %f, %f \n", this->m_segments[i].m_n0.x, this->m_segments[i].m_n0.y, this->m_segments[i].m_n1.x, this->m_segments[i].m_n1.y);
	}*/
}

//probably should be moved to Lens class
void Polygon::smoothNormals()
{

	for (int i = 0; i < this->m_segments.size(); i++)
	{
		Segment* ptr;
		Vector2f smoothedNormal;
		bool found = false;
		ptr = &findSegmentByPoint(this->m_segments[i], found, 0, 0); //p0 from this segment, p0 from found segment
		if (found)
		{
			smoothedNormal = ptr->VectorNormalize(this->m_segments[i].m_n0 + ptr->m_n0);
			this->m_segments[i].m_n0 = smoothedNormal;
			ptr->m_n0 = smoothedNormal;
		}
		found = false;
		ptr = &findSegmentByPoint(this->m_segments[i], found, 1, 0); //p0 from this segment, p0 from found segment
		if (found)
		{
			smoothedNormal = ptr->VectorNormalize(this->m_segments[i].m_n1 + ptr->m_n0);
			this->m_segments[i].m_n1 = smoothedNormal;
			ptr->m_n0 = smoothedNormal;
		}
		found = false;
		ptr = &findSegmentByPoint(this->m_segments[i], found, 0, 1); //p0 from this segment, p0 from found segment
		if (found)
		{
			smoothedNormal = ptr->VectorNormalize(this->m_segments[i].m_n0 + ptr->m_n1);
			this->m_segments[i].m_n0 = smoothedNormal;
			ptr->m_n1 = smoothedNormal;
		}
		found = false;
		ptr = &findSegmentByPoint(this->m_segments[i], found, 1, 1); //p0 from this segment, p0 from found segment
		if (found)
		{
			smoothedNormal = ptr->VectorNormalize(this->m_segments[i].m_n1 + ptr->m_n1);
			this->m_segments[i].m_n1 = smoothedNormal;
			ptr->m_n1 = smoothedNormal;
		}

		/*printf("Segment: %i\n", i);
		printf("p0: %f, %f p1: %f, %f \n", this->m_segments[i].m_p0.x, this->m_segments[i].m_p0.y, this->m_segments[i].m_p1.x, this->m_segments[i].m_p1.y);
		printf("n0: %f, %f n1: %f, %f \n", this->m_segments[i].m_n0.x, this->m_segments[i].m_n0.y, this->m_segments[i].m_n1.x, this->m_segments[i].m_n1.y);*/
	}
}

void Polygon::smoothNormalsNew()
{

	for (int i = 0; i < this->m_segments.size() - 1; i++)
	{
		if (m_segments.size() > 1)
		{
			if (m_segments[i].m_smooth && m_segments[i + 1].m_smooth)
			{
				Vector2f smoothedNormal;
				smoothedNormal = VectorNormalize(m_segments[i].m_n1 + m_segments[i + 1].m_n0);
				m_segments[i].m_n1 = smoothedNormal;
				m_segments[i + 1].m_n0 = smoothedNormal;
			}
		}
	}
}

//probably should be moved to Lens class
Segment& Polygon::findSegmentByPoint(Segment& s, bool& found, int p0_or_1, int f0_or_1)
{
	Vector2f* point;

	if (p0_or_1 == 0)
		point = &s.m_p0;
	else
		point = &s.m_p1;

	for (int i = 0; i < this->m_segments.size(); i++)
	{

		if (VectorCmp(m_segments[i].m_p0, s.m_p0) < 0.1f && VectorCmp(m_segments[i].m_p1, s.m_p1) < 0.01f) //Skip the same segment
			continue;
		if (VectorCmp(m_segments[i].m_p0, *point) < 0.1f && f0_or_1 == 0)
		{
			found = true;
			return m_segments[i];
		}
		if (VectorCmp(m_segments[i].m_p1, *point) < 0.1f && f0_or_1 == 1)
		{
			found = true;
			return m_segments[i];
		}
	}
	found = false;
	return s;
}
//probably should be moved to Lens class
float Polygon::VectorCmp(Vector2f& v0, Vector2f& v1)
{
	float dx = abs(v0.x - v1.x);
	float dy = abs(v0.y - v1.y);

	return dx + dy;
}

inline float Polygon::VectorDotProduct(Vector2f v1, Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }

inline Vector2f Polygon::VectorNormalize(Vector2f v) { return v / sqrtf(VectorDotProduct(v, v)); }