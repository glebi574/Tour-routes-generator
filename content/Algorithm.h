#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class Population {
private:

	const float point_radius = 12.f;
	const float line_width = 5.f;

	int points = 0; //количество пунктов
	int attempt = 1; //номер генерации

	struct Point {
		std::vector<float> connections;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		CircleShape obj;
	};
	std::vector<Point> map; //карта, содержащая все пункты и расстояния между ними

	struct Connection {
		int id[2];
		RectangleShape rectangle;
	};
	std::vector<Connection> connections;

	struct Route {
		std::vector<int> path;
		std::vector<int> absent;
		float time;
		float price;
		float score;
		float user_result;
	};
	std::vector<Route> generation; //популяция со всеми особями
	std::vector<Route> best_results; //лучшие пути среди всех генераций

public:

	int amount = 20; //количество особей

	struct RouteParametres {
		int first_point = 0;
		int last_point = 0;
	} route_parametres; //начальная и конечная точки маршрута

	struct UserRatios {
		int time = 1;
		int price = 1;
		int score = 1;
	} user_ratios; //важность каждого показателя

	//Установка пользовательских коэффициентов
	void set_user_ratios(int, int, int);
	//Модификация маршрутов и подсчёт результатов
	void cycle();
	//Вывод карты
	void print_map();
	//Вывод всех маршрутов
	void print_population();
	//Отрисовка всех пунктов на карте
	void draw(RenderWindow&);

private:
	friend std::ostream& operator<< (std::ostream&, const Route&);
	//Генерация карты
	void generate_map();
	//Мутация конкретного маршрута
	void mutation(Route&);
	//Подсчёт результатов маршрута
	void count_result(Route&);
	//Подсчёт результатов пользователя для всех маршрутов
	void count_user_results();
	//Добавление точки под определённым номером из массива отсутствующих точек
	void add_point_in_route(Route&, int);
	//Удаление точки под определённым номером из маршрута
	void remove_point_from_route(Route&, int);
	//Перезаполнение массива отсутствующих точек
	void update_absent(Route&);
	//Проверка корректности маршрута
	bool check_exceptions(std::vector<int>&);
	//Добавить точку на карту
	void add_point(float, float);
	//Добавить соединение между двумя точками
	void add_connection(int, int);

	friend class EventCallback;
	friend class Interface;
};