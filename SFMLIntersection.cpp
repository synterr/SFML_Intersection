#pragma once
#include "globals.h"
#include <iostream>
#include "Polygon.h"

Vector2f g_mouse_pos = Vector2f(0, 0);    //Last Left Mouse Position
Vector2f l_mouse_pos = Vector2f(10, 10);  //Last Right Mouse Position

bool redrawShape = true;

unsigned int	trace_density = 500;
unsigned int	maxdepth = 8;


const Vector2f	window_size(900.f, 600.f);


int main()
{
	smoothing = true;
	FPS fps;
	long total_rays = 0;
	Font font;
	if (!font.loadFromFile("segoeui.ttf"))
	{
		printf("Fonts not found \n");
	}

	unsigned int alphaint = 20;

	if (trace_density > 30)
		alphaint = trace_density;
	else
		alphaint = 30;

	const Color		ray_color = Color(255, 255, 255, 255 / (alphaint / 20));
	const Color		ray_color_hit = Color(255, 255, 255, 255 / (alphaint / 20));
	const Color		ray_color_secondary = Color(255, 100, 50, 50);
	const Color		wall_color = Color(0, 255, 0, 200);

	//Create light sources
	PointLightSource light1 = PointLightSource(100.f, trace_density, g_mouse_pos, 0.f, ray_color, DegToRad(30.f));
	//LinearLightSource light1 = LinearLightSource(100.f, trace_density, g_mouse_pos, 0.f, ray_color, 120.f);

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode((unsigned int)window_size.x, (unsigned int)window_size.y), "2D Ray Casting", Style::Close, settings);
	window.setFramerateLimit(120);

	srand((unsigned int)time(NULL));

	Polygon circle;

	//unsigned int maxPoints = 720;
	//float radius = 200.f;
	//float rot = 0;
	//Vector2f center = Vector2f(window_size.x / 2, -100/*window_size.y / 2*/);
	//float lx = radius * cos(0 + rot);
	//float ly = radius * sin(0 + rot);

	//float step = TWO_PI / (float)maxPoints * 2.f;

	//for (float a = 0 + step; a < TWO_PI/2 + step / 100.f; a += step)
	//{
	//	float x = radius * cos(a + rot);
	//	float y = radius * sin(a + rot);

	//	circle.m_points.push_back(Vector2f(center.x + lx, center.y + ly));
	//	circle.m_points.push_back(Vector2f(center.x + x, center.y + y));

	//	lx = x;
	//	ly = y;
	//}

	unsigned int maxPoints = 720;
	float radius = 250.f;
	float rot = -TWO_PI / 8;
	Vector2f center = Vector2f(window_size.x / 2, window_size.y / 2);
	float lx = radius * cos(0 + rot);
	float ly = radius * sin(0 + rot);

	float step = TWO_PI / (float)maxPoints * 2.f;

	//for (float a = 0 + step; a < TWO_PI / 2.5 + step / 100.f; a += step)
	//{
	//	float x = radius * cos(a + rot);
	//	float y = radius * sin(a + rot);

	//	circle.m_points.push_back(Vector2f(center.x + lx, center.y + ly));
	//	circle.m_points.push_back(Vector2f(center.x + x, center.y + y));

	//	lx = x;
	//	ly = y;
	//}


	maxPoints = 360 / 2;
	radius = 50;
	rot = -TWO_PI / 9;
	center = Vector2f(window_size.x / 2-100, window_size.y / 2);
	lx = radius * cos(0 + rot);
	ly = radius * sin(0 + rot);

	step = TWO_PI / (float)maxPoints * 2.f;

	for (float a = 0 + step; a < TWO_PI+ step / 100.f; a += step)
	{
		float x = radius * cos(a + rot);
		float y = radius * sin(a + rot);

		circle.m_points.push_back(Vector2f(center.x + lx, center.y + ly));
		circle.m_points.push_back(Vector2f(center.x + x, center.y + y));

		lx = x;
		ly = y;
	}

	maxPoints = 360 / 2;
	radius = 100;
	rot = -TWO_PI / 9;
	center = Vector2f(window_size.x / 2 +200, window_size.y / 2);
	lx = radius * cos(0 + rot);
	ly = radius * sin(0 + rot);

	step = TWO_PI / (float)maxPoints * 2.f;

	for (float a = 0 + step; a < TWO_PI + step / 100.f; a += step)
	{
		float x = radius * cos(a + rot);
		float y = radius * sin(a + rot);

		circle.m_points.push_back(Vector2f(center.x + lx, center.y + ly));
		circle.m_points.push_back(Vector2f(center.x + x, center.y + y));

		lx = x;
		ly = y;
	}
	circle.generateSegments();
	if (smoothing)
		circle.smoothNormals();


	// Do this if want to have segment that is closing shape (last point to first point)
	//segments.push_back(Segment(lastPoint, points.at(0)));

	//for (auto it : circle.m_segments)
	//	printf("Segment: %f, %f - %f, %f\n", it.m_p0.x, it.m_p0.y, it.m_p1.x, it.m_p1.y);

	//light1.UpdateLight();

	// Make line used for drawing rays
	VertexArray ray_line(Lines, 2);

	// Make line used for drawing walls

	VertexArray wall_line(Lines, 2);
	wall_line[0].color = wall_color;
	wall_line[1].color = wall_color;

	// Main loop

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;

			case Event::Resized:
				printf("Window resized to: %i x %i \n", event.size.width, event.size.height);
				break;

				//case Event::TextEntered:
				//	if (event.text.unicode < 128) //Normal characters
				//		printf("%c", event.text.unicode);
				//	if (event.text.unicode == 13) //Normal characters
				//		printf("\n");
				//	break;
			}

		}

		if (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			g_mouse_pos = Vector2f(Mouse::getPosition(window));
			//light1.angle = atan2(l_mouse_pos.y - g_mouse_pos.y, l_mouse_pos.x - g_mouse_pos.x);
			//light1.UpdateLight(g_mouse_pos);
		}

		if (Mouse::isButtonPressed(Mouse::Button::Right))
		{
			l_mouse_pos = Vector2f(Mouse::getPosition(window));
			light1.angle = atan2(l_mouse_pos.y - g_mouse_pos.y, l_mouse_pos.x - g_mouse_pos.x);
			//light1.UpdateLight(g_mouse_pos);
		}
		// If mouse if the same as previous frame, move on to the next
		//if (mouse_snapshot == g_mouse_pos)
			//continue;

		window.clear(Color(50, 50, 50));
		//window.draw(convex);

		// Set start of ray-drawing line to mouse position
		light1.UpdateLight(g_mouse_pos);
		total_rays = 0;

		vector<Segment*> testsegs;

		for (int i = 0; i < light1.traces.size(); i++)
		{
			unsigned int depth = 0;
			bool newDepth = false;
			do
			{
				for (unsigned int d = depth; d < depth + 1; d++)			//Rays Depth loop
				{
					if (d > maxdepth)
					{
						newDepth = false;
						break;
					}
					//Prepare test segment
					Segment testseg(Vector2f(0.f, 0.f), Vector2f(0.f, 0.f));
					if (testsegs.size() - 1 >= depth && testsegs.size() > 0)
						testseg = *testsegs[d];			//Set previously found segment if existing
					else
						testsegs.push_back(&testseg);	//Else push zero segment for further assigment

					newDepth = false;
					for (int r = 0; r < light1.traces[i].rays[d].size(); r++)	//Rays for specific depth
					{
						Ray& rayHit = light1.traces[i].rays[d][r];

						rayHit.m_isHit = false;

						if (!light1.traces[i].rays[d][r].calc_hit(testseg)) //Check last hited segment first
						{
							for (int j = 0; j < circle.m_segments.size(); j++)
							{
								// Calculate ray end-point
								// When an intersection is found, the end-point is set to that intersection, meaning the next check will check for walls
								// between start and the new end-point. This means the ray will always go to the nearest wall

								if (rayHit.calc_hit(circle.m_segments[j]))
									testsegs[d] = &circle.m_segments[j];
							}
						}
						if (rayHit.m_isHit)
						{
							if (!newDepth)
							{
								vector<Ray> rays;
								light1.traces[i].rays.push_back(rays);
								newDepth = true;
							}

							depth = (unsigned int)light1.traces[i].rays.size() - 1;
							
							//Reflect
							Ray ray1(rayHit.m_end, Reflect(rayHit.m_dir, rayHit.m_normal));
							Ray ray2(rayHit.m_end, Refract(rayHit.m_dir, rayHit.m_normal, 1.3f));
							ray1.m_isHit = false;
							ray2.m_isHit = false;
							//light1.traces[i].rays[depth].push_back(ray1);
							light1.traces[i].rays[depth].push_back(ray2);
						}
						// Set drawing-line end to final intersection
						ray_line[0].position = rayHit.m_pos;
						ray_line[1].position = rayHit.m_end;

						// Draw ray
						if (rayHit.m_isHit)
						{
							ray_line[0].color = ray_color_hit;
							ray_line[1].color = ray_color_hit;
						}
						else
						{
							ray_line[0].color = ray_color;
							ray_line[1].color = ray_color;
						}
						total_rays++;
						window.draw(ray_line);
					}
				}
			} while (newDepth == true);
		}

		// Draw walls
		for (int i = 0; i < circle.m_segments.size(); i++)
		{
			wall_line[0].position = circle.m_segments[i].m_p0;
			wall_line[1].position = circle.m_segments[i].m_p1;
			window.draw(wall_line);
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


#ifdef NDEBUG
		sf::Text text;
		text.setFont(font); // font is a sf::Font
		stringstream fpss;
		fpss << "FPS:" << fps.getFPS();
		// set the string to display

		text.setPosition(10, 10);
		text.setString(fpss.str());
		text.setCharacterSize(16); // in pixels, not points!
		text.setFillColor(sf::Color(200, 200, 200, 100));
		window.draw(text);

		fpss.str(std::string());
		fpss << "Traces:" << trace_density;
		text.setPosition(10, 30);
		text.setString(fpss.str());
		window.draw(text);

		fpss.str(std::string());
		fpss << "Rays:" << total_rays;
		text.setPosition(10, 50);
		text.setString(fpss.str());
		window.draw(text);
		fps.update();
#else

#endif

		window.display();

	}


	return 0;
}
