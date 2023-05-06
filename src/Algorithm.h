#pragma once

#include "Functions.h"
using namespace sf;

namespace IColor { //Дополнительные цвета
	const Color Blue = Color(100, 100, 240);
	const Color Pink = Color(200, 140, 200);
	const Color Gray = Color(120, 120, 120);
}

class Population {
public:

	float point_radius = 12.f;
	float point_outline_thickness = 2.f;
	float path_width = 5.f;
	float path_outline_thickness = 2.f;

	Color
		point_color = Color::Red,
		point_selection_color = IColor::Blue,
		point_outline_color = Color::Black,
		path_color = IColor::Gray,
		path_selection_color = IColor::Pink,
		path_outline_color = Color::Black;
	bool draw_points_first = false;
	bool do_log = true;

	int points = 0; //количество пунктов
	int attempt = 1; //номер генерации
	int mutation_chance = 100;
	bool use_recombination = false;

	struct Point { //пункт
		std::vector<float> connections;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		std::wstring name = L"";
		CircleShape obj;
	};
	std::vector<Point*> map; //карта, содержащая все пункты и расстояния между ними

	struct Connection { //отображаемый путь
		int id[2];
		float time = 0.f;
		RectangleShape rectangle;
	};
	std::vector<Connection> connections; //массив со всеми отрисовываемыми путями

	struct Route { //маршрут
		std::vector<int> path;
		std::vector<int> absent;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		float user_result = 0.f;
	};
	std::vector<Route> generation; //популяция со всеми особями
	std::vector<Route> new_routes;
	std::vector<Route> best_results; //лучшие пути среди всех генераций

	int amount = 20; //количество особей
	int cycles_amount = 10; //количество циклов

	struct RouteParametres {
		int first_point = 0;
		int last_point = 0;
	} route_parametres; //начальная и конечная точки маршрута

	struct UserRatios {
		int time = 1;
		int price = 1;
		int score = 1;
	} user_ratios; //важность каждого показателя

	struct Results { //Все уникальные маршруты, сгенерированные программой
		std::vector<Route> routes;
		int amount = 0;
		std::vector<std::vector<Route>> best_routes = {};
		void reset() {
			routes.clear();
			amount = 0;
			for (auto& g : best_routes) {
				g.clear();
			}
			for (int i = 0; i < 4; ++i) {
				best_routes.push_back({});
			}
		}
	} results;

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

	friend std::ostream& operator<< (std::ostream&, const Route&);
	//Мутация конкретного маршрута
	void mutation(Route&);

	void recombination();
	//Подсчёт результатов маршрута
	void count_result(Route&);
	//Подсчёт результатов пользователя для всех маршрутов
	void count_user_results();
	//Перезаполнение массива отсутствующих точек
	void update_absent(Route&);
	//Проверка корректности маршрута
	bool check_exceptions(std::vector<int>&);
	//Добавить точку на карту
	Point* add_point(float, float);
	//Добавить соединение между двумя точками
	void add_connection(int, int);

	void save(std::wstring);

	void load(std::wstring);

	void load_settings();

	//~Population();

	friend class Interface;
};
