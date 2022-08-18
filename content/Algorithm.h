#pragma once

#include <iostream>
#include <vector>

class Population {
private:

	static constexpr float speed = 1.4f;
	short amount;
	short points;
	short attempt = 1;

	struct map_part {
		std::vector<float> connections;
		float time;
		float price;
		float score;
	};
	std::vector<map_part> map;

	struct route {
		std::vector<short> path;
		std::vector<short> absent;
		float time;
		float price;
		float score;
		float user_result;
	};
	std::vector<route> generation;
	std::vector<route> best_results;

	struct user_ratios {
		int time;
		int price;
		int score;
	};
	user_ratios user_ratios;

	struct route_parametres {
		short first_point;
		short last_point;
	};
	route_parametres route_parametres;

	enum class mode { manual, random };
	bool mode = (bool)mode::random;

public:
	//Генерация маршрутов(количество особей, количество пунктов, начальный пункт, конечный пункт)
	//\param количество маршрутов, максимальное количество точек в пути, начальный пункт, конечный пункт
	Population(short, short, short fp = 0, short lp = 0);
	//Установка пользовательских коэффициентов
	void set_user_ratios(int, int, int);
	//Модификация маршрутов и подсчёт результатов
	void cycle();
	//Вывод карты
	void print_map();
	//Вывод всех маршрутов
	void print_population();
	//Мутация всех маршрутов
	//void Mutation();

private:
	friend std::ostream& operator<< (std::ostream&, const route&);
	friend void create_graph(Population&);
	//Генерация карты
	void generate_map();
	//Мутация конкретного маршрута
	void mutation(route&);
	//Скрещивание
	void Crossing();
	//Подсчёт результатов маршрута
	void count_result(route&);
	//Подсчёт результатов пользователя для всех маршрутов
	void count_user_results();
	//Добавление точки под определённым номером из массива отсутствующих точек
	void add_point(route&, int);
	//Удаление точки под определённым номером из маршрута
	void remove_point(route&, int);
	//Перезаполнение массива отсутствующих точек
	void update_absent(route&);
	//Проверка корректности маршрута
	bool check_exceptions(std::vector<short>&);

};