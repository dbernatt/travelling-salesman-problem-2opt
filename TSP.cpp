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
using std::cerr;
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
using sf::Keyboard;
using sf::Vector2f;
using sf::Lines;

// 1080 720

#define CITY_CIRCLE_SIZE 6
#define ITER 4

const float WIDTH = VideoMode::getDesktopMode().width;
const float HEIGHT = VideoMode::getDesktopMode().height;

int x_offset = 0;
int y_offset = 0;

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

struct _X_Y
{
	double x;
	double y;
};

void read_nodes(fstream& file, city*& cities, int& numberOfCities)
{
	numberOfCities = 0;
	double i, x, y;
	cities = (city*)malloc(sizeof(city));
	while (file >> i && file >> cities[numberOfCities].x && file >> cities[numberOfCities].y)
	{
		cities = (city*)realloc(cities, ((numberOfCities++) + 2) * sizeof(city));
	}
}

int* Swap(int* route, int numberOfCities, int i, int k)
{

	// Solution 1: Simple algorithm for swapping routes

	int* new_route = new int[numberOfCities];
	for (int j = 0; j < i; ++j)
	{
		new_route[j] = route[j];
	}

	for (int j = i, jj = k; j <= k; j++, jj--)
	{
		new_route[j] = route[jj];
	}

	for (int j = k + 1; j < numberOfCities; j++)
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

double calculateTotalDistance(city* cities, int numberOfCities, int* route)
{
	double sum = 0;
	for (int i = 0; i < numberOfCities - 1; i++)
	{
		sum += dist(cities[route[i]], cities[route[i + 1]]);
	}
	sum += dist(cities[route[0]], cities[route[numberOfCities - 1]]);
	return sum;
}

int* greedy(city* cities, int numberOfCities, int start)
{
	int* route = new int[numberOfCities];

	double min;
	int minind;
	double new_min;
	bool visited[numberOfCities];
	int k = 1;
	int i;
	int ii;

	double best_distance;
	double new_distance;

	for (int j = 0; j < numberOfCities; j++)
	{
		route[j] = j;
		visited[j] = false;
	}
	visited[start] = true;
	route[0] = start;
	i = start;

	while (k < numberOfCities)
	{
		ii = 0;
		while (visited[ii] == true)
		{
			ii++;
		}
		min = dist(cities[i], cities[ii]);
		minind = ii;

		for (int j = 0; j < numberOfCities; j++)
		{
			if (!visited[j])
			{
				new_min = dist(cities[i], cities[j]);
				if (new_min < min)
				{
					min = new_min;
					minind = j;
				}
			}
		}
		visited[minind] = true;
		route[k] = minind;
		i = minind;
		k++;
	}

	return route;
}

int* greedy_best(city* cities, int numberOfCities)
{
	int* route = greedy(cities, numberOfCities, 0);
	double best_distance = calculateTotalDistance(cities, numberOfCities, route);
	double new_distance;

	for (int i = 1; i < numberOfCities; i++)
	{
		int* new_route = greedy(cities, numberOfCities, i);
		new_distance = calculateTotalDistance(cities, numberOfCities, new_route);
		if (new_distance < best_distance)
		{
			best_distance = new_distance;
			for (int j = 0; j < numberOfCities; j++)
			{
				route[j] = new_route[j];
			}
		}
	}
	return route;
}

_X_Y imum(city* cities, int numberOfCities)
{
	_X_Y m;
	m.x = cities[0].x;
	m.y = cities[0].y;
	for (int i = 1; i < numberOfCities; ++i)
	{
		if (cities[i].x > m.x)
		{
			m.x = cities[i].x;
		}
		if (cities[i].y > m.y)
		{
			m.y = cities[i].y;
		}
	}
	return m;
}

draw_city* resize(city* cities, int numberOfCities, _X_Y m, int w, int h)
{

	draw_city* aux = new draw_city[numberOfCities];

	for (int i = 0; i < numberOfCities; ++i)
	{
		aux[i].x = (cities[i].x / m.x) * w;
		aux[i].y = (cities[i].y / m.y) * h;
	}
	return aux;
}

void _2_opt(RenderWindow& window, CircleShape* circle, draw_city* resized_city, int numberOfCities, city* cities, int* route, int* new_route, int new_distance, int best_distance)
{
	int improvement = 0;
	bool ok;

	while (improvement <= ITER)
	{
		ok = true;
		for (int i = 1; i < numberOfCities - 1 && ok; i++)
		{
			for (int k = i + 1; k < numberOfCities && ok; k++)
			{
				new_route = Swap(route, numberOfCities, i, k);
				new_distance = calculateTotalDistance(cities, numberOfCities, new_route);
				if (new_distance < best_distance)
				{
					best_distance = new_distance;

					for (int ii = 0; ii < numberOfCities; ii++)
					{
						route[ii] = new_route[ii];
					}

					window.clear();

					for (int i = 0; i < numberOfCities; i++)
					{
						circle[i].setPosition(resized_city[i].x, resized_city[i].y);
						window.draw(circle[i]);
					}

					// Draw lines between the nodes
					for (int l = 0; l < numberOfCities - 1; l++)
					{
						Vertex line[] = {
							Vertex(Vector2f(resized_city[route[l]].x + CITY_CIRCLE_SIZE,
												resized_city[route[l]].y + CITY_CIRCLE_SIZE)),
							Vertex(Vector2f(resized_city[route[l + 1]].x + CITY_CIRCLE_SIZE,
												resized_city[route[l + 1]].y + CITY_CIRCLE_SIZE)) };
						window.draw(line, 2, Lines);
					}

					window.display();
					improvement = 0;
					ok = false;
				}
			}
		}
		improvement++;
	}



	window.display();
}

void draw_view(RenderWindow& window, CircleShape* circle, draw_city* resized_city, int numberOfCities, int* route)
{
	for (int i = 0; i < numberOfCities; i++)
	{
		circle[i].setPosition(resized_city[i].x, resized_city[i].y);
		window.draw(circle[i]);
	}

	Vertex line[] = {
		Vertex(Vector2f(resized_city[route[0]].x + CITY_CIRCLE_SIZE,
							resized_city[route[0]].y + CITY_CIRCLE_SIZE),
			   Color::Red),
		Vertex(Vector2f(resized_city[route[numberOfCities - 1]].x + CITY_CIRCLE_SIZE,
							resized_city[route[numberOfCities - 1]].y + CITY_CIRCLE_SIZE),
			   Color::Red) };

	window.draw(line, 2, Lines);

	for (int l = 0; l < numberOfCities - 1; l++)
	{
		Vertex line[] = {
			Vertex(Vector2f(resized_city[route[l]].x + CITY_CIRCLE_SIZE,
								resized_city[route[l]].y + CITY_CIRCLE_SIZE),
				   Color::Red),
			Vertex(Vector2f(resized_city[route[l + 1]].x + CITY_CIRCLE_SIZE,
								resized_city[route[l + 1]].y + CITY_CIRCLE_SIZE),
				   Color::Red) };
		window.draw(line, 2, Lines);
	}
}

void print_route(int* route, int numberOfCities, double best_distance)
{
	cout << "Best distance 	: " << best_distance << endl;
	cout << "Best route 	: " << endl;
	cout << route[0];
	for (int i = 1; i < numberOfCities; ++i)
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

int main(int argv, char** argc)
{
	srand(time(NULL));

	RenderWindow window(VideoMode(WIDTH, HEIGHT, 32),
		"Travelling Salesman");

	// Change input node coords here :)
	fstream file("tsp1.txt");

	View view;
	view.setSize(WIDTH, HEIGHT);

	int numberOfCities = 0;
	city* cities;

	read_nodes(file, cities, numberOfCities);
	_X_Y m = imum(cities, numberOfCities);
	draw_city* resized_city = new draw_city[numberOfCities];
	resized_city = resize(cities, numberOfCities, m, WIDTH, HEIGHT);

	double best_distance;
	double new_distance;
	int* route = new int[numberOfCities];
	int* new_route = new int[numberOfCities];

	CircleShape* circle = new CircleShape[numberOfCities];

	for (int i = 0; i < numberOfCities; i++)
	{
		circle[i].setRadius(CITY_CIRCLE_SIZE);
		circle[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	}

	route = greedy_best(cities, numberOfCities);
	best_distance = calculateTotalDistance(cities, numberOfCities, route);
	bool route_found = false;
	print_usage();


	while (window.isOpen())
	{
		Event event;
		view = window.getView();

		while (window.pollEvent(event))
		{

			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Escape:
					window.close();
					break;
				case Keyboard::Left:
					view.move(-10, 0);
					window.setView(view);
					break;
				case Keyboard::Right:
					view.move(10, 0);
					window.setView(view);
					break;
				case Keyboard::Up:
					view.move(0, -10);
					window.setView(view);
					break;
				case Keyboard::Down:
					view.move(0, 10);
					window.setView(view);
					break;
				case Keyboard::Z:
					view.zoom(0.98f);
					window.setView(view);
					break;
				case Keyboard::X:
					view.zoom(1.02f);
					window.setView(view);
					break;
				case Keyboard::A:
					view.rotate(1);
					window.setView(view);
					break;
				case Keyboard::S:
					view.rotate(-1);
					window.setView(view);
					break;
				case Keyboard::D:
					window.setView(window.getDefaultView());
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		if (!route_found) {
			_2_opt(window, circle, resized_city, numberOfCities, cities, route, new_route, new_distance, best_distance);
			print_route(route, numberOfCities, best_distance);
			route_found = true;
		}

		window.clear();
		draw_view(window, circle, resized_city, numberOfCities, route);
		window.display();
	}

	delete[] route;
	delete[] new_route;
	delete[] circle;
	delete[] resized_city;
	delete[] cities;

	return EXIT_SUCCESS;
}