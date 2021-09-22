#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::swap;

using sf::CircleShape;
using sf::Color;
using sf::Event;
using sf::RenderWindow;
using sf::Vertex;
using sf::VideoMode;
using sf::View;

#define city_size 6
#define maxiter 4

struct city
{
	double x;
	double y;
};

struct draw_city
{
	double x;
	double y;
};

struct max_X_Y
{
	double x;
	double y;
};

void beolvas(fstream &file, city *&v, int &n)
{
	n = 0;
	double i, x, y;
	v = (city *)malloc(sizeof(city));
	while (file >> i && file >> v[n].x && file >> v[n].y)
	{
		v = (city *)realloc(v, ((n++) + 2) * sizeof(city));
	}
}

int *Swap(int *route, int n, int i, int k)
{

	// Solution 1: Simple algorithm for swapping routes

	int *new_route = new int[n];
	for (int j = 0; j < i; ++j)
	{
		new_route[j] = route[j];
	}

	for (int j = i, jj = k; j <= k; j++, jj--)
	{
		new_route[j] = route[jj];
	}

	for (int j = k + 1; j < n; j++)
	{
		new_route[j] = route[j];
	}

	// Solution 2: Using the built in swap function for swapping routes

	// for(int j = i, jj = k; j < jj; j++, jj--){
	// 	swap(route[j], route[jj]);
	// }

	return new_route;
}

double dist(city a, city b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

double calculateTotalDistance(city *v, int n, int *route)
{
	double sum = 0;
	for (int i = 0; i < n - 1; i++)
	{
		sum += dist(v[route[i]], v[route[i + 1]]);
	}
	sum += dist(v[route[0]], v[route[n - 1]]);
	return sum;
}

int *greedy(city *v, int n, int start)
{

	int *route = new int[n];

	double min;
	int minind;
	double ujmin;
	bool volt[n];
	int k = 1;
	int i;
	int ii;

	double best_distance;
	double new_distance;

	for (int j = 0; j < n; j++)
	{
		route[j] = j;
		volt[j] = false;
	}
	volt[start] = true;
	route[0] = start;
	i = start;

	while (k < n)
	{
		ii = 0;
		while (volt[ii] == true)
		{
			ii++;
		}
		min = dist(v[i], v[ii]);
		minind = ii;

		for (int j = 0; j < n; j++)
		{
			if (!volt[j])
			{
				ujmin = dist(v[i], v[j]);
				if (ujmin < min)
				{
					min = ujmin;
					minind = j;
				}
			}
		}
		volt[minind] = true;
		route[k] = minind;
		i = minind;
		k++;
	}

	return route;
}

int *greedy_best(city *v, int n)
{
	int *route = greedy(v, n, 0);
	double best_distance = calculateTotalDistance(v, n, route);
	double new_distance;

	for (int i = 1; i < n; i++)
	{
		int *new_route = greedy(v, n, i);
		new_distance = calculateTotalDistance(v, n, new_route);
		if (new_distance < best_distance)
		{
			best_distance = new_distance;
			for (int j = 0; j < n; j++)
			{
				route[j] = new_route[j];
			}
		}
	}
	return route;
}

max_X_Y maximum(city *v, int n)
{
	max_X_Y m;
	m.x = v[0].x;
	m.y = v[0].y;
	for (int i = 1; i < n; ++i)
	{
		if (v[i].x > m.x)
		{
			m.x = v[i].x;
		}
		if (v[i].y > m.y)
		{
			m.y = v[i].y;
		}
	}
	return m;
}

draw_city *resize(city *v, int n, max_X_Y m, int w, int h)
{

	draw_city *aux = new draw_city[n];

	for (int i = 0; i < n; ++i)
	{
		aux[i].x = (v[i].x / m.x) * w;
		aux[i].y = (v[i].y / m.y) * h;
	}
	return aux;
}

void print_route(int *route, int n, double best_distance)
{
	cout << "Best distance 	: " << best_distance << endl;
	cout << "Best route 	: " << endl;
	cout << route[0];
	for (int i = 1; i < n; ++i)
	{
		cout << " -> " << route[i];
	}
	cout << " -> " << route[0] << endl;
}

void print_usage()
{
	cout << "Welcome to Travelling Salesman Problem" << endl;
	cout << "Commands: " << endl;
	cout << "	-> Left 				Key: Move left" << endl;
	cout << "	-> Right 				Key: Move right" << endl;
	cout << "	-> Up 					Key: Move up" << endl;
	cout << "	-> Down 				Key: Move down" << endl;
	cout << "	-> Z 					Key: Zoom in" << endl;
	cout << "	-> X 					Key: Zoom out" << endl;
	cout << "	-> S					Key: Rotate clockwise" << endl;
	cout << "	-> A 					Key: Rotate anti-clockwise" << endl;
	cout << "	-> D 					Key: Reset to default view" << endl;
	cout << "	-> Esc 					Key: Close the application" << endl;
}

int main(int argv, char **argc)
{

	srand(time(NULL));

	RenderWindow window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height, 32),
						"Travelling Salesman",
						sf::Style::Titlebar | sf::Style::Fullscreen);

	// Change input node coords here :)
	fstream file("tsp1.txt");

	View view;
	view.setSize(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

	int n = 0;
	city *v;

	beolvas(file, v, n);
	max_X_Y m = maximum(v, n);

	draw_city *resized_city = new draw_city[n];
	double w = VideoMode::getDesktopMode().width;
	double h = VideoMode::getDesktopMode().height;

	resized_city = resize(v, n, m, w, h);

	int sorrend[n];
	double tav;
	int k = 0;
	double max;
	int ind1, ind2;

	double best_distance;
	double new_distance;
	bool ok;
	int improvement = 0;
	int *route = new int[n];
	int *new_route = new int[n];

	sf::CircleShape *circle = new sf::CircleShape[n];
	for (int i = 0; i < n; i++)
	{
		circle[i].setRadius(city_size);
		circle[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	}

	// First run the greedy algorithm
	route = greedy_best(v, n);

	best_distance = calculateTotalDistance(v, n, route);

	window.clear();
	for (int i = 0; i < n; i++)
	{
		circle[i].setPosition(resized_city[i].x, resized_city[i].y);
		window.draw(circle[i]);
	}
	for (int l = 0; l < n - 1; l++)
	{
		Vertex line[] = {
			Vertex(sf::Vector2f(resized_city[route[l]].x + city_size,
								resized_city[route[l]].y + city_size)),
			Vertex(sf::Vector2f(resized_city[route[l + 1]].x + city_size,
								resized_city[route[l + 1]].y + city_size))};
		window.draw(line, 2, sf::Lines);
	}
	window.display();

	print_usage();

	View traverse_view;

	while (window.isOpen())
	{
		Event event;
		View view = window.getView();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			view.move(-2, 0);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			view.move(2, 0);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			view.move(0, -2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			view.move(0, 2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			view.zoom(0.997f);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			view.zoom(1.003f);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			view.rotate(0.2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			view.rotate(-0.2);
			window.setView(view);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			window.setView(window.getDefaultView());
		}

		// 2-opt algorithm to optimize the route length
		while (improvement <= maxiter)
		{
			ok = true;

			for (int i = 1; i < n - 1 && ok; i++)
			{
				for (int k = i + 1; k < n && ok; k++)
				{
					new_route = Swap(route, n, i, k);
					new_distance = calculateTotalDistance(v, n, new_route);

					if (new_distance < best_distance)
					{
						best_distance = new_distance;

						for (int ii = 0; ii < n; ii++)
						{
							route[ii] = new_route[ii];
						}

						window.clear();

						for (int i = 0; i < n; i++)
						{
							circle[i].setPosition(resized_city[i].x, resized_city[i].y);
							window.draw(circle[i]);
						}

						// Draw lines between the nodes
						for (int l = 0; l < n - 1; l++)
						{
							Vertex line[] = {
								Vertex(sf::Vector2f(resized_city[route[l]].x + city_size,
													resized_city[route[l]].y + city_size)),
								Vertex(sf::Vector2f(resized_city[route[l + 1]].x + city_size,
													resized_city[route[l + 1]].y + city_size))};
							window.draw(line, 2, sf::Lines);
						}

						window.display();
						improvement = 0;
						ok = false;
					}
				}
			}
			improvement++;
		}

		window.clear();

		for (int i = 0; i < n; i++)
		{
			circle[i].setPosition(resized_city[i].x, resized_city[i].y);
			window.draw(circle[i]);
		}

		Vertex line[] = {
			Vertex(sf::Vector2f(resized_city[route[0]].x + city_size,
								resized_city[route[0]].y + city_size),
				   Color::Red),
			Vertex(sf::Vector2f(resized_city[route[n - 1]].x + city_size,
								resized_city[route[n - 1]].y + city_size),
				   Color::Red)};
		window.draw(line, 2, sf::Lines);

		for (int l = 0; l < n - 1; l++)
		{
			Vertex line[] = {
				Vertex(sf::Vector2f(resized_city[route[l]].x + city_size,
									resized_city[route[l]].y + city_size),
					   Color::Red),
				Vertex(sf::Vector2f(resized_city[route[l + 1]].x + city_size,
									resized_city[route[l + 1]].y + city_size),
					   Color::Red)};
			window.draw(line, 2, sf::Lines);
		}

		window.display();
	}

	print_route(route, n, best_distance);

	delete[] route;
	delete[] new_route;
	delete[] circle;
	delete[] resized_city;
	delete[] v;

	return 0;
}