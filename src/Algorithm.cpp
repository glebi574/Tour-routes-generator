#include "Algorithm.h"

void Population::generate_routes() {
	generation.clear();
	generation.resize(amount);
	best_results.clear();
	best_results.resize(4);
	for (auto& g : generation) {
		g.path.resize(random(4, points));

		for (int i = 0; i < g.path.size(); ++i)
			g.path[i] = i;
		if (g.path.front() != route_parametres.first_point) {
			int p = find(g.path, route_parametres.first_point);
			if (p == -1)
				g.path.front() = route_parametres.first_point;
			else
				std::swap(g.path.front(), g.path[p]);
		}
		if (g.path.back() != route_parametres.last_point) {
			int p = find(g.path, route_parametres.last_point);
			if (p == -1)
				g.path.back() = route_parametres.last_point;
			else
				std::swap(g.path.back(), g.path[p]);
		}

		update_absent(g);

		while (!check_exceptions(g.path)) {
			mutation(g);
		}
	}
	for (int i = 0; i < 4; ++i) {
		count_result(generation[i]);
		best_results[i] = generation[i];
	}
}

void Population::print_map() {
	for (auto& p : map) {
		for (auto& l : p->connections)
			std::cout << l << "\t";
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Population::count_result(Route& n) {
	n.time = 0;
	n.price = 0;
	n.score = 0;
	for (int i = 1; i < n.path.size(); ++i)
		n.time += map[n.path[i - 1]]->connections[n.path[i]];
	for (auto& g : n.path) {
		n.time += map[g]->time;
		n.price += map[g]->price;
		n.score += map[g]->score;
	}
}

void Population::count_user_results() {
	float args[3] = { best_results[0].time, best_results[1].price, best_results[2].score };
	for (auto& g : generation) {
		if (g.time  > best_results[0].time)
			args[0] = g.time;
		if (g.price > best_results[1].price)
			args[1] = g.price;
		if (g.score > best_results[2].score)
			args[2] = g.score;
	}
	for (auto& g : generation) {
		if (g.price == 0)
			g.user_result = (1.0f - g.time  / args[0]) * user_ratios.time + g.score / args[2]  * user_ratios.score;
		else
			g.user_result = (1.0f - g.time  / args[0]) * user_ratios.time  +
							(1.0f - g.price / args[1]) * user_ratios.price +
									g.score / args[2]  * user_ratios.score;
	}
	for (auto& g : best_results) {
		if (g.price == 0)
			g.user_result = (1.0f - g.time / args[0]) * user_ratios.time + g.score / args[2] * user_ratios.score;
		else
			g.user_result = (1.0f - g.time  / args[0]) * user_ratios.time  +
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
		g.absent.push_back(g.path[x]);
		g.path.erase(g.path.begin() + x);
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
		g.path.insert(g.path.begin() + random(1, g.path.size() - 2), g.absent[x]);
		g.absent.erase(g.absent.begin() + x);
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
	Route* best[]{ &generation[0], &generation[0], &generation[0], &generation[0] };
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

	print_population();

	std::cout << "\n\n\nПуть с наименьшим временем:\n" << best_results[0];
	if (best_results[1].price != 0)
		std::cout << "\nПуть с наименьшей стоимостью:\n" << best_results[1];
	std::cout << "\nПуть с наибольшей привлекательностью:\n" << best_results[2];
	std::cout << "\nНаиболее подходящий для пользователя путь:\n" << best_results[3];
	std::cout << "\n\n\n" << std::endl;

	for (auto& g : generation) {
		bool is_unique = true;
		for (auto& u : results.routes) {
			if (g.path == u.path) {
				is_unique = false;
				break;
			}
		}
		if (is_unique) {
			results.routes.push_back(g);
		}
	}

	float args[3] = { best_results[0].time, best_results[1].price, best_results[2].score };
	for (auto& g : results.routes) {
		if (g.time > best_results[0].time)
			args[0] = g.time;
		if (g.price > best_results[1].price)
			args[1] = g.price;
		if (g.score > best_results[2].score)
			args[2] = g.score;
	}
	for (auto& g : results.routes) {
		if (g.price == 0)
			g.user_result = (1.0f - g.time / args[0]) * user_ratios.time + g.score / args[2] * user_ratios.score;
		else
			g.user_result = (1.0f - g.time / args[0]) * user_ratios.time +
			(1.0f - g.price / args[1]) * user_ratios.price +
			g.score / args[2] * user_ratios.score;
	}

}

void Population::update_absent(Route& g) {
	for (int i = 0; i < points; ++i)
		if (find(g.path, i) == -1)
			g.absent.push_back(i);
}

bool Population::check_exceptions(std::vector<int>& vec) {
	for (int i = 1; i < vec.size(); ++i) {
		if (map[vec[i - 1]]->connections[vec[i]] == 0) return false;
	}
	return true;
}

std::ostream& operator<< (std::ostream& out, const Population::Route& p) {
	for (auto& i : p.path)
		out << i << "  ";
#ifndef NDEBUG
	if (p.absent.size() != 0) {
		out << "\nabsent:";
		for (auto& i : p.absent)
			out << "  " << i;
	}
#endif
	out << "\nОбщее время пути: " << p.time << "\nОбщая оценка: " << p.score;
	if (p.price != 0)
		out << "\nОбщая стоимость путешествия: " << p.price;
	out << "\nОценка этого пути относительно интересов пользователя: " << p.user_result << "\n";
	return out;
}

void Population::print_population() {
	for (auto& g : generation) {
		std::cout << g << '\n';
	}
	std::cout << std::endl;
}

Population::Point* Population::add_point(float x, float y) {
	Point* point = new Point;
#ifndef NDEBUG
	point->time  = random(0.f, 20.f);
	point->score = random(0.f, 10.f);
#endif
	for (int i = 0; i < map.size(); ++i) {
		point->connections.emplace_back(0);
	}
	point->obj = CircleShape(point_radius);
	point->obj.setFillColor(Color::Red);
	point->obj.setOutlineThickness(2.f);
	point->obj.setOutlineColor(Color::Black);
	point->obj.move(x, y);
	map.push_back(point);
	for (auto& g : map) {
		g->connections.emplace_back(0);
	}
	points += 1;
	return point;
}

void Population::add_connection(int a, int b) {
	Connection connection;
	connection.id[0] = a;
	connection.id[1] = b;
#ifndef NDEBUG
	float u = random(0.f, 20.f);
	map[a]->connections[b] = u;
	map[b]->connections[a] = u;
	connection.time = u;
#endif
	auto& p1 = map[a]->obj.getPosition();
	auto& p2 = map[b]->obj.getPosition();
	float l = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	connection.rectangle = RectangleShape(Vector2f(l, line_width));
	float ang = atan2(p2.y - p1.y, p2.x - p1.x);
	connection.rectangle.move(p1.x + point_radius - line_width * cos(ang + 1.57f) / 2.f, p1.y + point_radius - line_width * sin(ang + 1.57f) / 2.f);
	connection.rectangle.rotate(180.0f / 3.1416f * ang);
	connection.rectangle.setFillColor(IColor::Gray);
	connection.rectangle.setOutlineThickness(2.f);
	connection.rectangle.setOutlineColor(Color::Black);
	connections.emplace_back(connection);
}

void Population::draw(RenderWindow& window) {
	for (auto& g : map)
		window.draw(g->obj);
	for (auto& g : connections)
		window.draw(g.rectangle);
}

void Population::save(std::wstring file_name) {
	std::wofstream file(L"saves/" + file_name + L".lua");
	if (file.is_open()) {
		std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>());
		file.imbue(loc);
		file << "first_point = " << route_parametres.first_point <<
			"\nlast_point = " << route_parametres.last_point <<
			"\namount = " << amount <<
			"\ncycles_amount = " << cycles_amount <<
			"\ntime_ratio = " << user_ratios.time <<
			"\nprice_ratio = " << user_ratios.price <<
			"\nscore_ratio = " << user_ratios.score;
		file << "\n\nmap = {\n";
		for (auto& g : map) {
			file << "\t{\n\t\ttime = " << g->time << ",\n\t\tprice = " << g->price << ",\n\t\tscore = " << g->score <<
				",\n\t\tname = \"" << g->name << "\",\n\t\tx = " << g->obj.getPosition().x << ",\n\t\ty = " << g->obj.getPosition().y <<
				",\n\t\tconnections = { ";
			for (auto& t : g->connections) {
				file << t << ", ";
			}
			file << "}\n\t},\n";
		}
		file << "}\n\nconnections = {\n";
		for (auto& g : connections) {
			file << "\t{ id1 = " << g.id[0] << ", id2 = " << g.id[1] << ", time = " << g.time << " },\n";
		}
		file << "}";
		file.close();
		std::cout << "Успешно сохранено.\n";
	}
	else {
		std::cout << "Ошибка при открытии файла.\n";
	}
}

void Population::load(std::wstring file_name) {
	for (auto& g : map) {
		delete g;
	}
	map.clear();
	connections.clear();
	generation.clear();
	best_results.clear();
	results.routes.clear();
	results.amount = 0;
	route_parametres.first_point = 0;
	route_parametres.last_point = 1;
	user_ratios.time = 1;
	user_ratios.price = 1;
	user_ratios.score = 1;
	points = 0;
	attempt = 1;
	amount = 20;
	cycles_amount = 10;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	std::wstring wpath = "saves/" + file_name + L".lua";
	if (luaL_dofile(L, std::string(wpath.begin(), wpath.end()).c_str()) != LUA_OK) {
		const char* error_message = lua_tostring(L, -1);
		std::cerr << "Ошибка: " << error_message << std::endl;
		lua_pop(L, 1);
		lua_close(L);
		return;
	}
	
	lua_getglobal(L, "first_point");
	lua_getglobal(L, "last_point");
	lua_getglobal(L, "amount");
	lua_getglobal(L, "cycles_amount");
	lua_getglobal(L, "time_ratio");
	lua_getglobal(L, "price_ratio");
	lua_getglobal(L, "score_ratio");

	route_parametres.first_point = lua_tointeger(L, -7);
	route_parametres.last_point = lua_tointeger(L, -6);
	amount = lua_tointeger(L, -5);
	cycles_amount = lua_tointeger(L, -4);
	user_ratios.time = lua_tointeger(L, -3);
	user_ratios.price = lua_tointeger(L, -2);
	user_ratios.score = lua_tointeger(L, -1);

	lua_pop(L, 7);

	lua_getglobal(L, "map");
	if (!lua_istable(L, -1)) {
		lua_close(L);
		return;
	}
	
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter16;

	int n = luaL_len(L, -1);
	for (int i = 1; i <= n; ++i) {
		lua_rawgeti(L, -1, i);
		if (lua_istable(L, -1)) {
			lua_getfield(L, -1, "time");
			lua_getfield(L, -2, "price");
			lua_getfield(L, -3, "score");
			lua_getfield(L, -4, "name");
			lua_getfield(L, -5, "x");
			lua_getfield(L, -6, "y");

			int x = lua_tointeger(L, -2), y = lua_tointeger(L, -1);
			Point* p = add_point(x, y);
			p->time = lua_tonumber(L, -6);
			p->price = lua_tonumber(L, -5);
			p->score = lua_tonumber(L, -4);
			p->name = converter16.from_bytes(lua_tostring(L, -3));
			lua_pop(L, 6);
		}
		lua_pop(L, 1);
	}
	/*
	for (int i = 1; i <= n; ++i) {
		lua_rawgeti(L, -1, i);
		if (lua_istable(L, -1)) {
			lua_getfield(L, -1, "connections");
			if (lua_istable(L, -1)) {
				int q = luaL_len(L, -1);
				for (int r = 1; r <= q; ++r) {
					lua_rawgeti(L, -1, r);
					map[i - 1]->connections[r - 1] = lua_tonumber(L, -1);
					lua_pop(L, 1);
				}
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	*/
	lua_pop(L, 1);

	lua_getglobal(L, "connections");
	if (!lua_istable(L, -1)) {
		lua_close(L);
		return;
	}

	n = luaL_len(L, -1);
	for (int i = 1; i <= n; ++i) {
		lua_rawgeti(L, -1, i);
		if (lua_istable(L, -1)) {
			lua_getfield(L, -1, "id1");
			lua_getfield(L, -2, "id2");
			lua_getfield(L, -3, "time");

			int id1 = lua_tointeger(L, -3), id2 = lua_tointeger(L, -2);
			float time = lua_tonumber(L, -1);
			lua_pop(L, 3);
			add_connection(id1, id2);
			connections[connections.size() - 1].time = time;
			map[id1]->connections[id2] = time;
			map[id2]->connections[id1] = time;
		}
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	lua_close(L);
}
/*
Population::~Population() {
	for (auto& g : map) {
		delete g;
	}
}
*/