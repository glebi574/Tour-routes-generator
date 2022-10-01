#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
using namespace sf;

namespace IColor { //Дополнительные цвета
	const Color Blue = Color(100, 100, 240);
	const Color Pink = Color(200, 140, 200);
	const Color Gray = Color(120, 120, 120);
}

class Population {
private:

	const float point_radius = 12.f;
	const float line_width = 5.f;

	int points = 0; //количество пунктов
	int attempt = 1; //номер генерации

	struct Point { //пункт
		std::vector<float> connections;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		CircleShape obj;
	};
	std::vector<Point> map; //карта, содержащая все пункты и расстояния между ними

	struct Connection { //отображаемый путь
		int id[2];
		float time = 0.f;
		RectangleShape rectangle;
	};
	std::vector<Connection> connections; //массив со всеми отрисовываемыми путями

	struct Route { //маршрут
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
	int cycles_amount = 0; //количество циклов

	struct RouteParametres {
		int first_point = 0;
		int last_point = 0;
	} route_parametres; //начальная и конечная точки маршрута

	struct UserRatios {
		int time = 1;
		int price = 1;
		int score = 1;
	} user_ratios; //важность каждого показателя

	//Регенерация основных параметров, используемых алгоритмом и создание особей
	void generate_routes();
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
	//Мутация конкретного маршрута
	void mutation(Route&);
	//Подсчёт результатов маршрута
	void count_result(Route&);
	//Подсчёт результатов пользователя для всех маршрутов
	void count_user_results();
	//Перезаполнение массива отсутствующих точек
	void update_absent(Route&);
	//Проверка корректности маршрута
	bool check_exceptions(std::vector<int>&);
	//Добавить точку на карту
	void add_point(float, float);
	//Добавить соединение между двумя точками
	void add_connection(int, int);

	friend class Interface;
};
