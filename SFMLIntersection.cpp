#pragma once
#include "globals.h"
#include <iostream>
#include "Polygon.h"
#include "PointLightSource.h"
#include "LinearLightSource.h"
#include "Lens.h"

Vector2f g_mouse_pos = Vector2f(0, 0);    //Last Left Mouse Position
Vector2f l_mouse_pos = Vector2f(10, 10);  //Last Right Mouse Position
float    g_mouse_scroll = 0.f;

bool redrawShape = true;

unsigned int	trace_density = 250;//500
unsigned int	maxdepth = 32;//8


const Vector2f	window_size(1280.f, 720.f);


int main()
{
	//smoothing = true;
	FPS fps;
	long total_rays = 0;
	Font font;
	if (!font.loadFromFile("segoeui.ttf"))
	{
		printf("Fonts not found \n");
	}

	unsigned int div = 40;
	unsigned int alphaint = div;

	if (trace_density > div)
		alphaint = trace_density;
	else
		alphaint = div;

	const Color		ray_color = Color(255, 255, 255, 255 / (alphaint / div));
	const Color		ray_color_hit = Color(255, 255, 255, 255 / (alphaint / div));
	const Color		ray_color_secondary = Color(255, 100, 50, 50);
	const Color		wall_color = Color(0, 255, 0, 200);
	VertexArray		ray_line(Lines, 2);

	//Create light sources
	g_mouse_pos = Vector2f(window_size.x - 20, window_size.y / 2);
	PointLightSource light1 = PointLightSource(1.f, trace_density, g_mouse_pos, TWO_PI / 2, ray_color, DegToRad(30.f));
	//LinearLightSource light1 = LinearLightSource(1.f, trace_density, g_mouse_pos, 0.f, ray_color, 60.f);
	light1.UpdateLight();

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode((unsigned int)window_size.x, (unsigned int)window_size.y), "2D Ray Casting", Style::Close, settings);
	window.setFramerateLimit(120);

	srand((unsigned int)time(NULL));

	Lens nulLens;
	vector<Lens*> lenses;
	Lens lens1(Vector2f(window_size.x / 2 + 50, window_size.y / 2), -200.0f, -200.0f, 200.0f, 80.0f, 1.5f);
	Lens lens2(Vector2f(window_size.x / 2 - 50, window_size.y / 2), 150.0f, 150.0f, 200.0f, 10.0f, 1.5f);
	Lens lens3(Vector2f(window_size.x / 2 - 200, window_size.y / 2), -950.0f, 110.0f, 200.0f, 20.0f, 1.5f);

	lens1.Update(lens1.m_pos);
	lens2.Update(lens2.m_pos);
	lens3.Update(lens3.m_pos);

	lenses.push_back(&lens1);
	lenses.push_back(&lens2);
	lenses.push_back(&lens3);

	Lens* selectedLens = &nulLens;

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

			case Event::MouseWheelScrolled:
				g_mouse_scroll = event.mouseWheelScroll.delta;

				break;

				//case Event::TextEntered:
				//	if (event.text.unicode < 128) //Normal characters
				//		printf("%c", event.text.unicode);
				//	if (event.text.unicode == 13) //Normal characters
				//		printf("\n");
				//	break;
			}

		}
		g_mouse_pos = Vector2f(Mouse::getPosition(window));

		if (!Mouse::isButtonPressed(Mouse::Button::Left) && !Mouse::isButtonPressed(Mouse::Button::Right)) {
			
			selectedLens->m_isSelected = false;
			selectedLens = &nulLens;
			for (int i = 0; i < lenses.size(); i++)
			{
				if (lenses[i]->m_bounds.contains((Vector2i)g_mouse_pos))
				{

					selectedLens = lenses[i];
					selectedLens->m_isSelected = true;
					break;
				}
			}
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left) && selectedLens != &nulLens)
		{
			g_mouse_pos = Vector2f(Mouse::getPosition(window));
			selectedLens->Update(g_mouse_pos, selectedLens->m_angle);
		}
		if (Mouse::isButtonPressed(Mouse::Button::Right) && selectedLens != &nulLens)
		{
			l_mouse_pos = Vector2f(Mouse::getPosition(window));
			selectedLens->m_angle = atan2(l_mouse_pos.y - selectedLens->m_pos.y, l_mouse_pos.x - selectedLens->m_pos.x);
			selectedLens->Update(selectedLens->m_pos, selectedLens->m_angle);
		}
		if (Keyboard::isKeyPressed(Keyboard::LAlt))
		{
			if (g_mouse_scroll > 0.f)
			{
				selectedLens->m_R1 += selectedLens->m_R1 / 10.f;
				if (selectedLens->m_R1 > 1000)
					selectedLens->m_R1 = -1000;

				selectedLens->Update(selectedLens->m_pos, selectedLens->m_angle);
			}
			else if (g_mouse_scroll < 0.f)
			{
				selectedLens->m_R1 -= selectedLens->m_R1 / 10.f;
				if (selectedLens->m_R1 < -1000)
					selectedLens->m_R1 = 1000;
				selectedLens->Update(selectedLens->m_pos, selectedLens->m_angle);
			}

		}
		else if (Keyboard::isKeyPressed(Keyboard::RAlt))
		{
			if (g_mouse_scroll > 0.f)
			{
				selectedLens->m_R2 += selectedLens->m_R2 / 10.f;
				if (selectedLens->m_R2 > 1000)
					selectedLens->m_R2 = -1000;

				selectedLens->Update(selectedLens->m_pos, selectedLens->m_angle);
			}
			else if (g_mouse_scroll < 0.f)
			{
				selectedLens->m_R2 -= selectedLens->m_R2 / 10.f;
				if (selectedLens->m_R2 < -1000)
					selectedLens->m_R2 = 1000;
				selectedLens->Update(selectedLens->m_pos, selectedLens->m_angle);
			}

		}
		else if (Keyboard::isKeyPressed(Keyboard::LControl))
		{
			
		}

		else
		{
			if (Mouse::isButtonPressed(Mouse::Button::Left) && selectedLens == &nulLens)
			{
				g_mouse_pos = Vector2f(Mouse::getPosition(window));
				//light1.angle = atan2(l_mouse_pos.y - g_mouse_pos.y, l_mouse_pos.x - g_mouse_pos.x);
				light1.UpdateLight(g_mouse_pos);
				//window.clear(Color(10, 10, 10));
			}

			if (Mouse::isButtonPressed(Mouse::Button::Right) && selectedLens == &nulLens)
			{
				l_mouse_pos = Vector2f(Mouse::getPosition(window));
				light1.angle = atan2(l_mouse_pos.y - light1.position.y, l_mouse_pos.x - light1.position.x);
				light1.UpdateLight(light1.position);
				//window.clear(Color(10, 10, 10));
			}
		}
		g_mouse_scroll = 0.f;
		// If mouse if the same as previous frame, move on to the next
		//if (mouse_snapshot == g_mouse_pos)
			//continue;

		window.clear(Color(10, 10, 10));
		//window.draw(convex);

		// Set start of ray-drawing line to mouse position
		light1.UpdateLight(light1.position);
		total_rays = 0;

		ray_line[0].color = ray_color_hit;
		ray_line[1].color = ray_color_hit;

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
					Segment testseg(Vector2f(0.f, 0.f), Vector2f(0.f, 0.f), false, 1.0f);
					if (testsegs.size() - 1 >= depth && testsegs.size() > 0)
						testseg = *testsegs[d];			//Set previously found segment if existing
					else
						testsegs.push_back(&testseg);	//Else push zero segment for further assigment

					newDepth = false;
					for (int r = 0; r < light1.traces[i].rays[d].size(); r++)	//Rays for specific depth
					{
						float ior = testseg.ior;
						Ray& rayHit = light1.traces[i].rays[d][r];

						rayHit.m_isHit = false;

						for (int l = 0; l < lenses.size(); l++)
						{
							if (testseg.lensIndex == l)
							{
								rayHit.calc_hit(testseg);
								break;
							}
						}
						if (!rayHit.m_isHit)
						{
							for (int l = 0; l < lenses.size(); l++)
							{

								if (!light1.traces[i].rays[d][r].calc_hit(testseg)) //Check last hited segment first
								{

									for (int j = 0; j < lenses[l]->m_poly.m_segments.size(); j++)
									{
										// Calculate ray end-point
										// When an intersection is found, the end-point is set to that intersection, meaning the next check will check for walls
										// between start and the new end-point. This means the ray will always go to the nearest wall

										if (rayHit.calc_hit(lenses[l]->m_poly.m_segments[j]))
										{
											ior = lenses[l]->m_ior;
											lenses[l]->m_poly.m_segments[j].lensIndex = l;
											testsegs[d] = &lenses[l]->m_poly.m_segments[j];
										}
									}
								}

							}
						}
						if (rayHit.m_isHit && rayHit.intensity > 0.02f) // 0.02 ignore transmission of very low intensities
						{
							if (!newDepth)
							{
								vector<Ray> rays;
								light1.traces[i].rays.push_back(rays);
								newDepth = true;
							}

							depth = (unsigned int)light1.traces[i].rays.size() - 1;


							float kr = 0.5f;
							FresnelS(rayHit.m_dir, rayHit.m_normal, ior, kr);

							// compute refraction if it is not a case of total internal reflection
							if (kr < 1)
							{
								Ray ray2(rayHit.m_end, Refract(rayHit.m_dir, rayHit.m_normal, ior));
								ray2.m_isHit = false;
								ray2.intensity = rayHit.intensity * (1 - kr);
								light1.traces[i].rays[depth].push_back(ray2);
							}

							//if (kr >= 1.f)
							kr = 0.1f; // 0.1 Reduce intensity for internal reflections to avoid calculations

							Ray ray1(rayHit.m_end, Reflect(rayHit.m_dir, rayHit.m_normal));
							ray1.m_isHit = false;
							ray1.intensity = rayHit.intensity * kr;
							light1.traces[i].rays[depth].push_back(ray1);
						}

						//float intensity = Clamp(rayHit.intensity, 0.05f, 1.0f);

						ray_line[0].color.a = (float)ray_color_hit.a * rayHit.intensity;
						ray_line[1].color.a = (float)ray_color_hit.a * rayHit.intensity;
						if (ray_line[0].color.a < 1)
						{
							ray_line[0].color.a = 1;
							ray_line[1].color.a = 1;
						}
						//ray_line[0].color = ray_color_hit;
						//ray_line[1].color = ray_color_hit;

						// Set drawing-line end to final intersection
						ray_line[0].position = rayHit.m_pos;
						ray_line[1].position = rayHit.m_end;

						// Draw ray
						/*if (rayHit.m_isHit)
						{
							ray_line[0].color = ray_color_hit;
							ray_line[1].color = ray_color_hit;
						}
						else
						{
							ray_line[0].color = ray_color;
							ray_line[1].color = ray_color;
						}*/
						total_rays++;
						window.draw(ray_line);
					}

				}
			} while (newDepth == true);
		}

		for (auto lens : lenses)
			lens->Draw(window);


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
