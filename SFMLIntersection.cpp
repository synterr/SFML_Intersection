#include "globals.h"
#include <iostream>
#include "Polygon.h"

Vector2f g_mouse_pos = Vector2f(0, 0);    //Last Left Mouse Position
Vector2f l_mouse_pos = Vector2f(10, 10);  //Last Right Mouse Position

int  trace_density = 500;
int	  maxdepth = 10;

const Color    ray_color = Color(255, 255, 255, 10);
const Color    ray_color_secondary = Color(255, 100, 50, 50);
const Color    ray_color_hit = Color(255, 255, 255, 10);
const Color    wall_color = Color(0, 255, 0, 200);;
const Vector2f window_size(900, 600);

//Create light sources
PointLightSource light1 = PointLightSource(100.f, trace_density, g_mouse_pos, 0.f,ray_color, TWO_PI/64);

int main()
{
	
	FPS fps;
	long total_rays = 0;
	Font font;
	if (!font.loadFromFile("segoeui.ttf"))
	{
		printf("Fonts not found \n");
	}


	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(window_size.x, window_size.y), "2D Ray Casting", Style::Close,settings);
	window.setFramerateLimit(120);
	
	srand(time(NULL));
	
	Polygon circle;

	
	unsigned int maxPoints = 180;
	float step = TWO_PI / maxPoints;
		
	float lx = 200 * cos(0);
	float ly = 200 * sin(0);

	for (float a = step; a < TWO_PI +step/2; a += step)
	{
		float x = 200* cos(a);
		float y = 200 * sin(a);
		circle.m_points.push_back(Vector2f(window_size.x/2+x, window_size.y/2+y));
		circle.m_points.push_back(Vector2f(window_size.x / 2 + lx, window_size.y / 2 + ly));
		
		lx = x;
		ly = y;
	}


	int i = 0;
	Vector2f lastPoint;
	for (auto it : circle.m_points)
	{
		if (i % 2 != 0) //if i > 0 for continous convex
			circle.m_segments.push_back(Segment(lastPoint, it));
		//convex.setPoint(i, it);
		lastPoint = it;
		i++;
	}
	// Do this if want to have segment that is closing shape (last point to first point)
	//segments.push_back(Segment(lastPoint, points.at(0)));

	for (auto it : circle.m_segments)
		printf("Segment: %f, %f - %f, %f\n", it.m_p0.x, it.m_p0.y, it.m_p1.x, it.m_p1.y);

	light1.UpdateLight();

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
				printf ("Window resized to: %i x %i \n", event.size.width, event.size.height);
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
			light1.angle = atan2(l_mouse_pos.y - g_mouse_pos.y, l_mouse_pos.x - g_mouse_pos.x);
			light1.UpdateLight(g_mouse_pos);
		}
		
		if (Mouse::isButtonPressed(Mouse::Button::Right))
		{
			l_mouse_pos = Vector2f(Mouse::getPosition(window));
			light1.angle = atan2(l_mouse_pos.y - g_mouse_pos.y, l_mouse_pos.x - g_mouse_pos.x);
			light1.UpdateLight(g_mouse_pos);
		}
		// If mouse if the same as previous frame, move on to the next
		//if (mouse_snapshot == g_mouse_pos)
			//continue;

		window.clear(Color(50, 50, 50));
		//window.draw(convex);

		// Set start of ray-drawing line to mouse position
		light1.UpdateLight(g_mouse_pos);
		total_rays = 0;

		for (int i = 0; i < light1.traces.size(); i++)
		{
			int depth = 0;
			bool newDepth = false;
			do
			{
				for (int d = depth; d < depth+1; d++)			//Rays Depth loop - should be do-loop with 
				{
					if (d > maxdepth)
					{
						newDepth = false;
						break;
					}
					// Curently not working must be testseg array with elements per depth.
					//Segment testseg(Vector2f(0.f,0.f), Vector2f(0.f, 0.f));
					newDepth = false;
					for (int r = 0; r < light1.traces[i].rays[d].size(); r++)	//Rays for specific depth
					{
						light1.traces[i].rays[d][r].m_isHit = false;
						// Curently not working must be testseg array with elements per depth.
						//if (!light1.traces[i].rays[d][r].calc_hit(testseg.p0, testseg.p1))
						//{
							for (int j = 0; j < circle.m_segments.size(); j++)
							{
								// Calculate ray end-point
								// When an intersection is found, the end-point is set to that intersection, meaning the next check will check for walls
								// between start and the new end-point. This means the ray will always go to the nearest wall
								
								if (light1.traces[i].rays[d][r].calc_hit(circle.m_segments[j]))
								{
									// Curently not working must be testseg array with elements per depth.
									//testseg = segments[j];
								}
							}
						//}
						if (light1.traces[i].rays[d][r].m_isHit)
						{
							if (!newDepth)
							{
								vector<Ray> rays;
								light1.traces[i].rays.push_back(rays);
								newDepth = true;
							}
							
							depth = light1.traces[i].rays.size()-1;
							//Vector2f newdir(light1.traces[i].rays[d][r].m_dir.x+light1.traces[i].rays[d][r].m_dir.x*sin(TWO_PI/16), light1.traces[i].rays[d][r].m_dir.y+light1.traces[i].rays[d][r].m_dir.y *cos(TWO_PI / 16));
							Ray ray1(light1.traces[i].rays[d][r].m_end, light1.traces[i].rays[d][r].m_dir - 2.0f * VectorDotProduct(light1.traces[i].rays[d][r].m_dir, light1.traces[i].rays[d][r].m_n1) * light1.traces[i].rays[d][r].m_n1);
							//Vector2f newdir1(light1.traces[i].rays[d][r].m_dir.x + light1.traces[i].rays[d][r].m_dir.x * -sin(TWO_PI / 16), light1.traces[i].rays[d][r].m_dir.y + light1.traces[i].rays[d][r].m_dir.y * -cos(TWO_PI /16));
							//Ray ray2(light1.traces[i].rays[d][r].m_end, newdir1);
							ray1.m_isHit = false;
							//ray2.m_isHit = false;
							light1.traces[i].rays[depth].push_back(ray1);
							//light1.traces[i].rays[depth].push_back(ray2);
						}
						// Set drawing-line end to final intersection
						ray_line[0].position = light1.traces[i].rays[d][r].m_pos;
						ray_line[1].position = light1.traces[i].rays[d][r].m_end;

						// Draw ray
						if (light1.traces[i].rays[d][r].m_isHit)
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
			CircleShape cir;
			cir.setRadius(1);
			cir.setFillColor(Color::Transparent);
			cir.setOutlineThickness(1);
			cir.setOutlineColor(Color::Red);
			cir.setPosition(circle.m_segments[i].m_p0);
			window.draw(cir);
			cir.setPosition(circle.m_segments[i].m_p1);
			window.draw(cir);

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
		text.setFillColor(sf::Color(200,200,200,100));
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

