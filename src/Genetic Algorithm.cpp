
#include "Functions.h"
#include "Algorithm.h"
#include "Window.h"

int point_id = 0;
int connection_id = 0;

int input_field_id = 0;

bool edit_mode = 0;
bool input_mode = 0;

bool interface_mode = 0;

int results_mode = 0;
int selected_route = 0;

Population::Point selected_point;
Population::Connection selected_connection;
Population::Route selected_result;

void interface_input_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			if (interface.temp_string.size() == 0)
				interface.temp_string = "0";
			try {
				if (interface.input_fields[input_field_id].type) //Изменение значения, на которое указывает ptr, на введённое
					*(float*)interface.input_fields[input_field_id].ptr = std::stof(std::string(interface.temp_string));
				else
					*(int*)interface.input_fields[input_field_id].ptr = std::stoi(std::string(interface.temp_string));
			}
			catch(std::invalid_argument& q) {
				interface.temp_string = "0";
				if (interface.input_fields[input_field_id].type)
					*(float*)interface.input_fields[input_field_id].ptr = std::stof(std::string(interface.temp_string));
				else
					*(int*)interface.input_fields[input_field_id].ptr = std::stoi(std::string(interface.temp_string));
			}
			interface.input_fields[input_field_id].rectangle.setFillColor(Color::Transparent);
			interface.temp_string = "";
			interface.temp_text.setString("");
			if (f.map.size() == 0) //Обновление параметров пункта, если возможно
				return;
			f.map[point_id].time = selected_point.time;
			f.map[point_id].price = selected_point.price;
			f.map[point_id].score = selected_point.score;
			if (f.connections.size() == 0) //Обновление параметров пути, если возможно
				return;
			f.map[f.connections[connection_id].id[0]].connections[f.connections[connection_id].id[1]]
				= selected_connection.time;
			f.map[f.connections[connection_id].id[1]].connections[f.connections[connection_id].id[0]]
				= selected_connection.time;
			return;
		}
		if (event.key.code == Keyboard::BackSpace) { //Нажатие backspase
			if (interface.temp_string.size() != 0) //Удаление 1 введённого символа, если возможно
				interface.temp_string.pop_back();
			interface.temp_text.setString(interface.temp_string);
			return;
		}
		if (event.type == Event::TextEntered) //Ввод остальных символов с клавиатуры
		{
			interface.temp_string += event.text.unicode; //Увеличение строки на введённый символ
			interface.temp_text.setString(interface.temp_string); //Обновление отображаемого текста
			return;
		}
	}
	if (event.type != Event::MouseButtonReleased || input_mode)
		return;
	//Работа с картой
	if (event.mouseButton.x < 1000) { //Проверка позиции
		if (event.mouseButton.button == Mouse::Right) { //Выделить точку при нажатии ПКМ
			for (int i = 0; i < f.map.size(); ++i) {
				if (!interface.if_mouse_in_point(event, f.map[i].obj, f))
					continue;
				f.map[point_id].obj.setFillColor(Color::Red);
				f.map[i].obj.setFillColor(IColor::Blue);
				point_id = i;
				selected_point = f.map[i];
				return;
			}
			return;
		}
		if (!edit_mode && event.mouseButton.button == Mouse::Left) { //Создать точку при нажатии ЛКМ в 1 режиме, если возможно
			for (auto& g : f.map) {
				Vector2f pos = g.obj.getPosition();
				float dx = event.mouseButton.x - pos.x - f.point_radius;
				float dy = event.mouseButton.y - pos.y - f.point_radius;
				if (sqrt(dx * dx + dy * dy) < (2.f * f.point_radius + 4.f))
					return;
			}
			f.add_point(event.mouseButton.x - f.point_radius, event.mouseButton.y - f.point_radius);
			return;
		}
		switch (event.mouseButton.button) { //Режим редактирования путей
		case Mouse::Left: //Провести путь от выделенной точки к данной, если возможно
			for (int i = 0; i < f.map.size(); ++i) {
				if ((!interface.if_mouse_in_point(event, f.map[i].obj, f)) || (i == point_id))
					continue;
				for (auto& m : f.connections)
					if ((m.id[0] == i && m.id[1] == point_id) || (m.id[0] == point_id && m.id[1] == i))
						return;
				f.add_connection(i, point_id);
				return;
			}
			break;
		case Mouse::Middle: //Выделение пути
			for (int i = 0; i < f.connections.size(); ++i) {
				Vector2i pos_m = Mouse::getPosition(window);
				Vector2f pos_r = f.connections[i].rectangle.getPosition();
				Vector2f size = f.connections[i].rectangle.getSize();
				float dx = pos_m.x - pos_r.x, dy = pos_m.y - pos_r.y;
				float ang = atan2(dy, dx) - f.connections[i].rectangle.getRotation() / 57.3f;
				float l = sqrt(dy * dy + dx * dx);
				pos_m.x = pos_r.x + l * cos(ang);
				pos_m.y = pos_r.y + l * sin(ang);
				if (pos_m.x >= pos_r.x && pos_m.y >= pos_r.y && pos_m.x <= pos_r.x + size.x && pos_m.y <= pos_r.y + size.y) {
					f.connections[connection_id].rectangle.setFillColor(IColor::Gray);
					f.connections[connection_id].time = selected_connection.time;
					f.connections[i].rectangle.setFillColor(IColor::Pink);
					connection_id = i;
					selected_connection = f.connections[i];
					return;
				}
			}
		}
		return;
	}
}

void main_interface_input_callback(Window& window, Interface interface, Population& f, Event& event) {

}

void results_interface_input_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			if (interface.temp_string.size() == 0)
				interface.temp_string = "0";
			try {
				if (interface.input_fields[input_field_id].type) //Изменение значения, на которое указывает ptr, на введённое
					*(float*)interface.input_fields[input_field_id].ptr = std::stof(std::string(interface.temp_string));
				else
					*(int*)interface.input_fields[input_field_id].ptr = std::stoi(std::string(interface.temp_string));
			}
			catch (std::invalid_argument& q) {
				interface.temp_string = "0";
				if (interface.input_fields[input_field_id].type)
					*(float*)interface.input_fields[input_field_id].ptr = std::stof(std::string(interface.temp_string));
				else
					*(int*)interface.input_fields[input_field_id].ptr = std::stoi(std::string(interface.temp_string));
			}
			interface.input_fields[input_field_id].rectangle.setFillColor(Color::Transparent);
			interface.temp_string = "";
			interface.temp_text.setString("");
			if (selected_route >= f.results.amount) {
				selected_route = 0;
				return;
			}
			selected_result = f.results.best_routes[results_mode][selected_route];
			return;
		}
		if (event.key.code == Keyboard::BackSpace) { //Нажатие backspase
			if (interface.temp_string.size() != 0) //Удаление 1 введённого символа, если возможно
				interface.temp_string.pop_back();
			interface.temp_text.setString(interface.temp_string);
			return;
		}
		if (event.type == Event::TextEntered) //Ввод остальных символов с клавиатуры
		{
			interface.temp_string += event.text.unicode; //Увеличение строки на введённый символ
			interface.temp_text.setString(interface.temp_string); //Обновление отображаемого текста
			return;
		}
	}
}

int main() {

	srand(clock()); //начальное значение rand()
	system("color 08"); //цвет консоли
	setlocale(LC_ALL, ""); //использование русского языка

	Population g;
	Texture map;
	Interface interface("FiraCode-Regular.ttf");
	Interface main_interface("FiraCode-Regular.ttf");
	Interface results_interface("FiraCode-Regular.ttf");
	map.loadFromFile("map.png");
	Sprite map_sprite(map);

	ContextSettings s;
	s.antialiasingLevel = 4; //параметры окна
	RenderWindow window(VideoMode(1400, 800), "Genetic Algorithm", Style::Default, s); //создание окна
	window.setFramerateLimit(60);

	float x = 1040.f, y = 120.f;

	interface.add_string(L"Режим редактора:", 18, x, y - 80);

	interface.add_string(L"Параметры пункта", 24, x, y - 40);
	interface.add_string(L"Пункт:", 16, x, y);
	interface.add_string(L"Время посещения:", 16, x, y + 20);
	interface.add_string(L"Стоимость посещения:", 16, x, y + 40);
	interface.add_string(L"Оценка:", 16, x, y + 60);

	interface.add_string(L"Начальный пункт:", 16, x, y + 100);
	interface.add_string(L"Конечный пункт:", 16, x, y + 120);

	interface.add_string(L"Параметры пути", 24, x, y + 160);
	interface.add_string(L"Время пути:", 16, x, y + 200);

	interface.add_string(L"Важность параметров", 24, x, y + 240);
	interface.add_string(L"Важность времени:", 16, x, y + 280);
	interface.add_string(L"Важность стоимости:", 16, x, y + 300);
	interface.add_string(L"Важность оценки:", 16, x, y + 320);

	interface.add_string(L"Алгоритм", 24, x, y + 360);
	interface.add_string(L"Количество особей:", 16, x, y + 400);
	interface.add_string(L"Количество генераций:", 16, x, y + 420);

	interface.add_pointer_string(&point_id, 0, 16, x + 220, y);

	interface.add_input_field(&selected_point.time, 1, 16, x + 220, y + 20, x + 300, y + 40);
	interface.add_input_field(&selected_point.price, 1, 16, x + 220, y + 40, x + 300, y + 60);
	interface.add_input_field(&selected_point.score, 1, 16, x + 220, y + 60, x + 300, y + 80);

	interface.add_input_field(&g.route_parametres.first_point, 0, 16, x + 220, y + 100, x + 300, y + 120);
	interface.add_input_field(&g.route_parametres.last_point, 0, 16, x + 220, y + 120, x + 300, y + 140);

	interface.add_input_field(&selected_connection.time, 1, 16, x + 220, y + 200, x + 300, y + 220);

	interface.add_input_field(&g.user_ratios.time, 0, 16, x + 220, y + 280, x + 300, y + 300);
	interface.add_input_field(&g.user_ratios.price, 0, 16, x + 220, y + 300, x + 300, y + 320);
	interface.add_input_field(&g.user_ratios.score, 0, 16, x + 220, y + 320, x + 300, y + 340);

	interface.add_input_field(&g.amount, 0, 16, x + 220, y + 400, x + 300, y + 420);
	interface.add_input_field(&g.cycles_amount, 0, 16, x + 220, y + 420, x + 300, y + 440);

	interface.add_window_rectangle(0.5, x - 5, y - 45, x + 305, y + 145);
	interface.add_window_rectangle(0.5, x - 5, y + 155, x + 305, y + 225);
	interface.add_window_rectangle(0.5, x - 5, y + 235, x + 305, y + 345);
	interface.add_window_rectangle(0.5, x - 5, y + 355, x + 305, y + 445);

	Interface::Button& algorithm_button = interface.add_button(L"Сгенерировать", 16, x - 5, y + 460, x + 170, y + 480);
	Interface::Switch& edit_switch = interface.add_switch({ {x + 255, y - 80, x + 275, y - 60}, {x + 285, y - 80, x + 305, y - 60} });

	RectangleShape interface_bg(Vector2f(800.f, 800.f));
	interface_bg.move(1000.f, 0.f);

	main_interface.add_string(L"Режим интерфейса:", 18, x, y + 630);
	Interface::Switch& interface_switch = main_interface.add_switch({ {x + 255, y + 630, x + 275, y + 650}, {x + 285, y + 630, x + 305, y + 650} });

	results_interface.add_string(L"Уникальные маршруты:", 18, x, y - 80);
	results_interface.add_string(L"Выбранный маршрут:", 18, x, y - 60);

	results_interface.add_string(L"Лучшие маршруты", 24, x, y - 30);
	results_interface.add_string(L"По времени:", 16, x, y + 10);
	results_interface.add_string(L"По стоимости:", 16, x, y + 35);
	results_interface.add_string(L"По привлекательности:", 16, x, y + 60);
	results_interface.add_string(L"По оптимальности:", 16, x, y + 85);

	results_interface.add_string(L"Параметры маршрута", 24, x, y + 130);
	results_interface.add_string(L"Время:", 16, x, y + 170);
	results_interface.add_string(L"Стоимость:", 16, x, y + 190);
	results_interface.add_string(L"Оценка:", 16, x, y + 210);
	results_interface.add_string(L"Оптимальность:", 16, x, y + 230);

	results_interface.add_pointer_string(&g.results.amount, 0, 18, x + 240, y - 80);
	results_interface.add_pointer_string(&selected_result.time, 1, 16, x + 240, y + 170);
	results_interface.add_pointer_string(&selected_result.price, 1, 16, x + 240, y + 190);
	results_interface.add_pointer_string(&selected_result.score, 1, 16, x + 240, y + 210);
	results_interface.add_pointer_string(&selected_result.user_result, 1, 16, x + 240, y + 230);

	results_interface.add_input_field(&selected_route, 0, 18, x + 240, y - 58, x + 305, y - 38);

	results_interface.add_window_rectangle(0.5, x - 5, y - 35, x + 305, y + 115);
	results_interface.add_window_rectangle(0.5, x - 5, y + 125, x + 305, y + 260);

	Interface::Switch& results_switch = results_interface.add_switch({
			{x + 275, y + 10, x + 295, y + 30},
			{x + 275, y + 35, x + 295, y + 55},
			{x + 275, y + 60, x + 295, y + 80},
			{x + 275, y + 85, x + 295, y + 105}
		});

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			main_interface_input_callback(window, main_interface, g, event);
			if (interface_mode) {
				if (event.type == Event::MouseButtonReleased && !input_mode) {
					for (int i = 0; i < results_switch.rectangles.size(); ++i) { //Проверка переключения переключателя
						if (results_interface.if_mouse_in_rectangle(event, results_switch.rectangles[i])) {
							results_interface.change_switch_selection(results_switch, i);
							results_mode = results_switch.mode;
							selected_result = g.results.best_routes[results_mode][selected_route];
							break;
						}
					}
					for (int i = 0; i < results_interface.input_fields.size(); ++i) { //Проверка нажатия на поле ввода
						if (results_interface.if_mouse_in_rectangle(event, results_interface.input_fields[i].rectangle)) {
							input_mode = true;
							input_field_id = i;
							results_interface.temp_text.setPosition(results_interface.input_fields[i].rectangle.getPosition());
							results_interface.input_fields[i].rectangle.setFillColor(Color::White);
						}
					}
				}
				results_interface_input_callback(window, results_interface, g, event);
			}
			else {
				if (event.type == Event::MouseButtonReleased && !input_mode) {
					for (int i = 0; i < edit_switch.rectangles.size(); ++i) { //Проверка переключения переключателя
						if (interface.if_mouse_in_rectangle(event, edit_switch.rectangles[i])) {
							interface.change_switch_selection(edit_switch, i);
							edit_mode = edit_switch.mode;
							for (auto& u : g.connections)
								u.rectangle.setFillColor(IColor::Gray);
							break;
						}
					}
					if (interface.if_mouse_in_rectangle(event, algorithm_button.rectangle)) { //Проверка нажатия кнопки
						g.generate_routes();
						g.results.reset();
						for (int i = 0; i < g.cycles_amount; ++i) {
							g.cycle();
						}
						g.results.amount = g.results.routes.size();
						for (auto& u : g.results.best_routes) {
							u.resize(g.results.routes.size());
							u = g.results.routes;
						}
						std::sort(g.results.best_routes[0].begin(), g.results.best_routes[0].end(),
							[](const Population::Route& a, const Population::Route& b) {return a.time < b.time; });
						std::sort(g.results.best_routes[1].begin(), g.results.best_routes[1].end(),
							[](const Population::Route& a, const Population::Route& b) {return a.price < b.price; });
						std::sort(g.results.best_routes[2].begin(), g.results.best_routes[2].end(),
							[](const Population::Route& a, const Population::Route& b) {return a.score > b.score; });
						std::sort(g.results.best_routes[3].begin(), g.results.best_routes[3].end(),
							[](const Population::Route& a, const Population::Route& b) {return a.user_result > b.user_result; });
					}
					for (int i = 0; i < interface.input_fields.size(); ++i) { //Проверка нажатия на поле ввода
						if (interface.if_mouse_in_rectangle(event, interface.input_fields[i].rectangle)) {
							input_mode = true;
							input_field_id = i;
							interface.temp_text.setPosition(interface.input_fields[i].rectangle.getPosition());
							interface.input_fields[i].rectangle.setFillColor(Color::White);
						}
					}
				}
				interface_input_callback(window, interface, g, event);
			}
			if (event.type == Event::MouseButtonReleased && !input_mode) {
				for (int i = 0; i < interface_switch.rectangles.size(); ++i) { //Проверка переключения переключателя
					if (main_interface.if_mouse_in_rectangle(event, interface_switch.rectangles[i])) {
						main_interface.change_switch_selection(interface_switch, i);
						interface_mode = interface_switch.mode;
						for (auto& u : g.map)
							u.obj.setFillColor(Color::Red);
						for (auto& u : g.connections)
							u.rectangle.setFillColor(IColor::Gray);
						break;
					}
				}
			}
			if (event.type == Event::Closed or event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
				window.close();
		}

		window.clear(Color::White);
		window.draw(interface_bg);
		window.draw(map_sprite);
		main_interface.draw(window);

		if (interface_mode) {
			results_interface.draw(window);
			for (auto& u : g.map)
				window.draw(u.obj);
			std::vector<std::vector<int>> connected;
			for (int i = 1; i < selected_result.path.size(); ++i) {
				std::vector<int> k1 = { selected_result.path[i - 1], selected_result.path[i]};
				std::vector<int> k2 = { selected_result.path[i], selected_result.path[i - 1] };
				for (auto& u : connected) {
					if (u == k1 || u == k2)
						continue;
				}
				connected.push_back(k1);
			}
			for (auto& u : connected) {
				for (auto& f : g.connections) {
					if ((u[0] == f.id[0] && u[1] == f.id[1]) || (u[0] == f.id[1] && u[1] == f.id[0])) {
						window.draw(f.rectangle);
					}
				}
			}
		}
		else {
			interface.draw(window);
			g.draw(window);
		}

		window.display();

	}

}