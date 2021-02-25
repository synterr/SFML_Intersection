#include "globals.h"
#include <iostream>
#include "Segment.h"

Vector2f g_mouse_pos = Vector2f(0, 0);    //Last Left Mouse Position
Vector2f l_mouse_pos = Vector2f(10, 10);  //Last Right Mouse Position

long    ray_density = 1000;
const Color    ray_color = Color(255, 255, 255, 10);
const Color    ray_color_hit = Color(255, 255, 255, 100);
const Color    wall_color = Color(0, 255, 0, 200);;
const Vector2f window_size(900, 600);

//Create light sources
PointLightSource light1 = PointLightSource(100.f, ray_density, g_mouse_pos, 0.f,ray_color, 60.f);

int main()
{
	
	FPS fps;

	Font font;
	if (!font.loadFromFile("segoeui.ttf"))
	{
		printf("Fonts not found \n");
	}


	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(window_size.x, window_size.y), "2D Ray Casting", Style::Close,settings);
	window.setFramerateLimit(60);
	
	srand(time(NULL));
	
	std::vector<Vector2f> points;
	std::vector<Segment> segments;


	// create an empty shape
	ConvexShape convex;

	// resize it to 5 points
	convex.setPointCount(5);

	unsigned int maxPoints = 20;
	// define the points
	for(int cnt = 0; cnt < maxPoints/2; cnt++)
	{
		points.push_back(Vector2f(random(window_size.x, 50), random(window_size.y, 50)));
		points.push_back(Vector2f(random(window_size.x, 50), random(window_size.y, 50)));
	}
	
	int i = 0;
	Vector2f lastPoint;
	for (auto it : points)
	{
		if (i % 2 != 0) //if i > 0 for continous convex
			segments.push_back(Segment(lastPoint, it));
		//convex.setPoint(i, it);
		lastPoint = it;
		i++;
	}
	// Do this if want to have segment that is closing shape (last point to first point)
	//segments.push_back(Segment(lastPoint, points.at(0)));

	//for (auto it : segments)
	//	printf("Segment: %f, %f - %f, %f\n", it.p0.x, it.p0.y, it.p1.x, it.p1.y);

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

		for (int i = 0; i < light1.traces.size(); i++)
		{
			for (int r = 0; r < light1.traces[i].rays.size(); r++)
			{
				for (int j = 0; j < segments.size(); j++)
				{
					// Calculate ray end-point
					// When an intersection is found, the end-point is set to that intersection, meaning the next check will check for walls
					// between mouse and the new end-point. This means the ray will always go to the nearest wall
					light1.traces[i].rays[r][0].calc_hit(segments[j].p0, segments[j].p1);
				}

				// Set drawing-line end to final intersection
				ray_line[0].position = light1.traces[i].rays[r][0].m_pos;
				ray_line[1].position = light1.traces[i].rays[r][0].m_end;

				// Draw ray
				if (light1.traces[i].rays[r][0].isHit)
				{
					ray_line[0].color = ray_color_hit;
					ray_line[1].color = ray_color_hit;
				}
				else
				{
					ray_line[0].color = ray_color;
					ray_line[1].color = ray_color;
				}

				window.draw(ray_line);
			}

			
		}

		// Draw walls
		for (int i = 0; i < segments.size(); i++)
		{
			wall_line[0].position = segments[i].p0;
			wall_line[1].position = segments[i].p1;
			window.draw(wall_line);
		}


		sf::Text text;

		// select the font
		text.setFont(font); // font is a sf::Font


		stringstream fpss;
		fpss << "FPS:" << fps.getFPS();
		// set the string to display

		text.setString(fpss.str());

		// set the character size
		text.setCharacterSize(16); // in pixels, not points!

		// set the color
		text.setFillColor(sf::Color(200,200,200,100));

		window.draw(text);
		// Update window
		fps.update();
		window.display();

	}


	return 0;
}

