#pragma once

#include <cassert>
#include "Functions.h"
#include "Algorithm.h"
	
	Population::Population(short a, short p, short fp, short lp) {
		assert(p > 4 && "There can't be fewer than 5 points in route.");
		amount = a;
		points = p;
		
		route_parametres.first_point = fp;
		if (lp == 0)
			route_parametres.last_point = points - 1;
		else
			route_parametres.last_point = lp;

		map.resize(points);
		generation.resize(amount);
		best_results.resize(4);
		
		generate_map();

		for (auto& g : generation) { //генерируем каждый маршрут
			g.path.resize(random(4, points)); //устанавливаем длину от 4 до points

			for (short i = 0; i < g.path.size(); ++i)
				g.path[i] = i; //заполнение по типу 0 1 ... points - 1
			if (g.path.front() != route_parametres.first_point) { //корректировка начального пункта маршрута
				short p = find(g.path, route_parametres.first_point);
				if (p == -1)
					g.path.front() = route_parametres.first_point;
				else
					std::swap(g.path.front(), g.path[p]);
			}
			if (g.path.back() != route_parametres.last_point) { //корректировка конечного пункта маршрута
				short p = find(g.path, route_parametres.last_point);
				if (p == -1)
					g.path.back() = route_parametres.last_point;
				else
					std::swap(g.path.back(), g.path[p]);
			}

			update_absent(g);

			while(!check_exceptions(g.path)) { //генерация маршрутов
				mutation(g);
			}
		}

		for (int i = 0; i < 4; ++i) {
			count_result(generation[i]);
			best_results[i] = generation[i];
		}
	}

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
			if (mode) { //если mode == mode::random, то карта генерируется случайно
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

	void Population::count_result(route &n) {
		n.time		  = 0;
		n.price		  = 0;
		n.score		  = 0;
		n.user_result = 0;
		for (short i = 1; i < n.path.size(); ++i)
			n.time	+= map[n.path[i - 1]].connections[n.path[i]];
		for (auto& g : n.path) {
			n.time	+= map[g].time;
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
			g.user_result = (1.0f - g.time / args[0]) * user_ratios.time + (1.0f - g.price / args[1]) * user_ratios.price
				+ g.score / args[2] * user_ratios.score;
		}
	}

	void Population::mutation(route& g) {
		int r;
		int l = g.path.size();
		if (l == 4)
			r = random(2, 4);
		else if (l == points)
			r = random(1, 2);
		else
			r = random(1, 4);
		int v = 0, y = 0;
		switch (r) {
		case 1:
			v = random(1, g.path.size() - 2);
			remove_point(g, v);
			break;
		case 2:
			while (v == y) {
				v = random(1, g.path.size() - 2);
				y = random(1, g.path.size() - 2);
			}
			std::swap(g.path[v], g.path[y]);
			break;
		case 3:
			v = random(0, g.absent.size() - 1);
			add_point(g, v);
			break;
		case 4:
			v = random(1, g.path.size() - 2);
			y = random(0, g.absent.size() - 1);
			std::swap(g.path[v], g.absent[y]);
			break;
		}
	}
	/*
	void Population::Mutation() {
		for (auto& g : generation) {
			do mutation(g);
			while (!check_exceptions(g.path));
		}
	}
	*/
	void Population::Crossing() {
		for (int a = 0; a < amount; ++a) {
			short attempt = 0;
			if (generation[a].path.size() > 5)
			{
				int b;
				do {
					++attempt;
					do
						b = random(0, amount - 1);
					while (a == b || generation[b].path.size() < 6);
					int sa = generation[a].path.size(), sb = generation[b].path.size();
					int s = random(2, (sa >= sb ? sb : sa) - 3);
					sa = random(1, sa - s - 1);
					sb = random(1, sb - s - 1);
					for (int i = 1; i < generation[a].path.size() - 1; ++i) {
						if (i >= sa && i < sa + s)
							continue;
						for (int k = 0; k < s; ++k) {
							if (find(generation[a].path, (short)generation[b].path[sb + k]) != -1)
								goto impossible;
						}
					}
					for (int i = 0; i < s; ++i)
						generation[a].path[sa + i] = generation[b].path[sb + i];
				impossible:;
				} while (attempt < 1000 && !check_exceptions(generation[a].path));
				generation[a].absent.clear();
				update_absent(generation[a]);
			}
			while (!check_exceptions(generation[a].path))
				mutation(generation[a]);
		}
	}

	void Population::cycle() {
		Crossing();
		for (auto& g : generation)
			count_result(g);
		count_user_results();
		float args[4] = {0xffffff, 0xffffff, 0, 0};
		int indexes[4] = { -1, -1, -1, -1 };
		for (int i = 0; i < amount; ++i) {
			if (generation[i].time < args[0]) {
				args[0] = generation[i].time;
				indexes[0] = i;
			}
			if (generation[i].price < args[1]) {
				args[1] = generation[i].price;
				indexes[1] = i;
			}
			if (generation[i].score > args[2]) {
				args[2] = generation[i].score;
				indexes[2] = i;
			}
			if (generation[i].user_result > args[3]) {
				args[3] = generation[i].user_result;
				indexes[3] = i;
			}
		}
		if(indexes[0] != -1 && args[0] < best_results[0].time)
			best_results[0] = generation[indexes[0]];
		if (indexes[1] != -1 && args[1] < best_results[1].price)
			best_results[1] = generation[indexes[1]];
		if (indexes[2] != -1 && args[2] > best_results[2].score)
			best_results[2] = generation[indexes[2]];
		if (indexes[3] != -1 && args[3] > best_results[3].user_result)
			best_results[3] = generation[indexes[3]];
		std::cout << ">\t>\t>\t>\t>\nПуть с наименьшим временем:\n" << best_results[0];
		std::cout << "\nПуть с наименьшей стоимостью:\n" << best_results[1];
		std::cout << "\nПуть с наибольшей привлекательностью:\n" << best_results[2];
		std::cout << "\nНаиболее подходящий для пользователя путь:\n" << best_results[3];
		std::cout << ">\t>\t>\t>\t>\n" << std::endl;
	}

	void Population::add_point(route &obj, int r) {
		obj.path.insert(obj.path.begin() + random(1, obj.path.size() - 2), obj.absent[r]);
		obj.absent.erase(obj.absent.begin() + r);
	}

	void Population::remove_point(route &obj, int r) {
		obj.absent.push_back(obj.path[r]);
		obj.path.erase(obj.path.begin() + r);
	}

	void Population::update_absent(route& g) {
		for (short i = 0; i < points; ++i)
			if (find(g.path, i) == -1)
				g.absent.push_back(i);
	}

	bool Population::check_exceptions(std::vector<short> &vec) {
		for (int i = 1; i < vec.size(); ++i) {
			if (map[vec[i - 1]].connections[vec[i]] == 0) return false;
		}
		return true;
	}

	std::ostream& operator<< (std::ostream& out, const Population::route& p) {
		for (auto& i : p.path)
			out << i << "\t";
#ifndef NDEBUG
		out << "\nabsent:";
		for (auto& i : p.absent)
			out << "\t" << i;
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
	