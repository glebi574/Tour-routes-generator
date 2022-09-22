#pragma once

#include <cassert>
#include "Functions.h"
#include "Algorithm.h"
	
void Population::set_user_ratios(int t, int p, int s) {
	user_ratios.time = t;
	user_ratios.price = p;
	user_ratios.score = s;
}

void Population::print_map() {
	for (auto& p : map) {
		for (auto& l : p.connections)
			std::cout << l << "\t";
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Population::generate_map() {
	for (int i = 0; i < points; ++i) {
		if (true) { //если mode == mode::random, то карта генерируется случайно
			map[i].connections.resize(points);
			map[i].time = random(40.0f, 600.0f);
			map[i].price = random(1.0f, 10.0f);
			map[i].score = random(0.0f, 10.0f);
			for (int n = 0; n < map[i].connections.size(); ++n) {
				if (i == n) {
					map[i].connections[n] = 0.0f;
					continue;
				}
				if (i > n) {
					map[i].connections[n] = map[n].connections[i];
					continue;
				}
				map[i].connections[n] = chance(80) ? random(60.0f, 700.0f) : 0.0f;
			}
		}
		else { //иначе информация о точках заполняется при помощи готовой карты

		}
	}
}

void Population::count_result(Route &n) {
	n.time = 0;
	n.price	= 0;
	n.score = 0;
	for (int i = 1; i < n.path.size(); ++i)
		n.time += map[n.path[i - 1]].connections[n.path[i]];
	for (auto& g : n.path) {
		n.time += map[g].time;
		n.price += map[g].price;
		n.score += map[g].score;
	}
}

void Population::count_user_results() {
	float args[3] = {0, 0, 0};
	for (auto& g : generation) {
		if (g.time > args[0])
			args[0] = g.time;
		if (g.price > args[1])
			args[1] = g.price;
		if (g.score > args[2])
			args[2] = g.score;
	}
	for (auto& g : generation) {
		g.user_result = (1.0f - g.time	/ args[0]) * user_ratios.time  +
						(1.0f - g.price / args[1]) * user_ratios.price +
								g.score / args[2]  * user_ratios.score;
	}
}

void Population::mutation(Route& g) {
	int variant; //способ мутации особи
	int length = g.path.size();
	if (length == 4) //ограничение по способам мутации
		variant = random(2, 4);
	else if (length == points)
		variant = random(1, 2);
	else
		variant = random(1, 4);
	int x, y;
	switch (variant) {
	case 1: //удаление пункта
		x = random(1, g.path.size() - 2);
		remove_point_from_route(g, x);
		break;
	case 2: //смена мест двух пунктов
		do {
			x = random(1, g.path.size() - 2);
			y = random(1, g.path.size() - 2);
		} while (x == y);
		std::swap(g.path[x], g.path[y]);
		break;
	case 3: //добавление пункта
		x = random(0, g.absent.size() - 1);
		add_point_in_route(g, x);
		break;
	case 4: //замена пункта маршрута на пункт, которого в маршруте нет
		x = random(1, g.path.size() - 2);
		y = random(0, g.absent.size() - 1);
		std::swap(g.path[x], g.absent[y]);
		break;
	}
}

void Population::cycle() {
	for (auto& g : generation)
		do mutation(g);
		while (!check_exceptions(g.path));
	for (auto& g : generation)
		count_result(g);
	count_user_results();
	Route* best[]{&generation[0], &generation[0], &generation[0], &generation[0]};
	for (auto& g : generation) {
		if (g.time < best[0]->time)
			best[0] = &g;
		if (g.price < best[1]->price)
			best[1] = &g;
		if (g.score > best[2]->score)
			best[2] = &g;
		if (g.user_result > best[3]->user_result)
			best[3] = &g;
	}
	if (best[0]->time < best_results[0].time)
		best_results[0] = *best[0];
	if (best[1]->price < best_results[1].price)
		best_results[1] = *best[1];
	if (best[2]->score > best_results[2].score)
		best_results[2] = *best[2];
	if (best[3]->user_result > best_results[3].user_result)
		best_results[3] = *best[3];

	std::cout << ">\t>\t>\t>\t>\nПуть с наименьшим временем:\n" << best_results[0];
	std::cout << "\nПуть с наименьшей стоимостью:\n" << best_results[1];
	std::cout << "\nПуть с наибольшей привлекательностью:\n" << best_results[2];
	std::cout << "\nНаиболее подходящий для пользователя путь:\n" << best_results[3];
	std::cout << ">\t>\t>\t>\t>\n" << std::endl;
}

void Population::add_point_in_route(Route &obj, int r) {
	obj.path.insert(obj.path.begin() + random(1, obj.path.size() - 2), obj.absent[r]);
	obj.absent.erase(obj.absent.begin() + r);
}

void Population::remove_point_from_route(Route &obj, int r) {
	obj.absent.push_back(obj.path[r]);
	obj.path.erase(obj.path.begin() + r);
}

void Population::update_absent(Route& g) {
	for (int i = 0; i < points; ++i)
		if (find(g.path, i) == -1)
			g.absent.push_back(i);
}

bool Population::check_exceptions(std::vector<int> &vec) {
	for (int i = 1; i < vec.size(); ++i) {
		if (map[vec[i - 1]].connections[vec[i]] == 0) return false;
	}
	return true;
}

std::ostream& operator<< (std::ostream& out, const Population::Route& p) {
	for (auto& i : p.path)
		out << i << " ";
#ifndef NDEBUG
	out << "\nabsent:";
	for (auto& i : p.absent)
		out << " " << i;
#endif
	out << "\nОбщее время пути: " << p.time << "\nОбщая стоимость путешествия: " << p.price <<
		"\nОбщая оценка: " << p.score << "\nОценка этого пути относительно интересов пользователя: " << p.user_result << "\n";
	return out;
}

void Population::print_population() {
	for (auto& g : generation) {
		std::cout << g << '\n';
	}
	std::cout << std::endl;
}

void Population::add_point(float x, float y) {
	Point point;
	for (int i = 0; i < map.size(); ++i) {
		point.connections.emplace_back(0);
	}
	point.obj = CircleShape(point_radius);
	point.obj.setFillColor(Color::Red);
	point.obj.setOutlineThickness(2.f);
	point.obj.setOutlineColor(Color::Black);
	point.obj.move(x, y);
	map.emplace_back(point);
	for (auto& g : map) {
		g.connections.emplace_back(0);
	}
}

void Population::add_connection(int a, int b) {
	Connection connection;
	connection.id[0] = a;
	connection.id[1] = b;
	auto& p1 = map[a].obj.getPosition();
	auto& p2 = map[b].obj.getPosition();
	float l = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	connection.rectangle = RectangleShape(Vector2f(l, line_width));
	float ang = atan2(p2.y - p1.y, p2.x - p1.x);
	connection.rectangle.move(p1.x + point_radius - line_width * cos(ang + 1.57f) / 2.f, p1.y + point_radius - line_width * sin(ang + 1.57f) / 2.f);
	connection.rectangle.rotate(180.0f / 3.1416f * ang);
	connection.rectangle.setFillColor(Color(100, 100, 100));
	connection.rectangle.setOutlineThickness(2.f);
	connection.rectangle.setOutlineColor(Color::Black);
	connections.emplace_back(connection);
}

void Population::draw(RenderWindow& window) {
	for (auto& g : map)
		window.draw(g.obj);
	for (auto& g : connections)
		window.draw(g.rectangle);
}
