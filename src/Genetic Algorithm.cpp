
#include "Functions.h"
#include "Algorithm.h"
#include "Window.h"

int point_id = 0;
int connection_id = 0;

int switch_id = 0; //Номер нового переключателя
int input_field_id = 0;

bool edit_mode = 0; //Режим редактирования карты; 0 - точки, 1 - пути
bool input_mode = 0; //Ввод в поле ввода
bool name_visibility_mode = 0;

int interface_mode = 0; //Режим интерфейса; 0 - редактирование данных, 1 - отображение результатов, 2 - настройки

int results_mode = 0; //Выбранный критерий для отображения результатов
int selected_route = 0; //Выбранный маршрут в вкладке результатов

Color __gray(219, 239, 219);

Population::Point selected_point;
Population::Connection selected_connection;
Population::Route selected_result;

std::wstring file_name;

void input_field_wstring_tonumber(Interface& interface) {
	if (interface.input_fields[input_field_id].type)
		*(float*)interface.input_fields[input_field_id].ptr = std::stof(std::wstring(interface.temp_string));
	else
		*(int*)interface.input_fields[input_field_id].ptr = std::stoi(std::wstring(interface.temp_string));
}

void temp_string_to_input_field(Interface& interface) {
	if (interface.temp_string.size() == 0)
		interface.temp_string = L"0";
	try {
		input_field_wstring_tonumber(interface);
	}
	catch (std::invalid_argument& q) {
		interface.temp_string = L"0";
		input_field_wstring_tonumber(interface);
	}
}

void base_interface_input_handler(Interface& interface, Event& event) {
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

void update_input_field(Interface& interface) {
	interface.input_fields[input_field_id].rectangle.setFillColor(Color::Transparent);
	interface.temp_string = L"";
	interface.temp_text.setString("");
}

bool can_press_mouse_button(Event& event) {
	return event.type == Event::MouseButtonReleased && !input_mode;
}

bool was_switched(Interface::Switch* _switch, Interface& interface, Event& event) {
	for (int i = 0; i < _switch->rectangles.size(); ++i) {
		if (interface.if_mouse_in_rectangle(event, _switch->rectangles[i])) {
			switch_id = i;
			interface.change_switch_selection(_switch, i);
			return true;
		}
	}
	return false;
}

void check_input_fields_activation(Interface& interface, Event& event) {
	for (int i = 0; i < interface.input_fields.size(); ++i) {
		if (interface.if_mouse_in_rectangle(event, interface.input_fields[i].rectangle)) {
			input_mode = true;
			input_field_id = i;
			interface.temp_text.setPosition(interface.input_fields[input_field_id].rectangle.getPosition());
			interface.input_fields[input_field_id].rectangle.setFillColor(__gray);
		}
	}
}

void interface_input_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			if (interface.input_fields[input_field_id].is_str) {
				*(std::wstring*)interface.input_fields[input_field_id].ptr = interface.temp_string;
			} else {
				temp_string_to_input_field(interface);
			}
			update_input_field(interface);
			if (f.map.size() == 0) //Обновление параметров пункта, если возможно
				return;
			f.map[point_id]->name = selected_point.name;
			f.map[point_id]->time = selected_point.time;
			f.map[point_id]->price = selected_point.price;
			f.map[point_id]->score = selected_point.score;
			if (f.connections.size() == 0) //Обновление параметров пути, если возможно
				return;
			f.map[f.connections[connection_id].id[0]]->connections[f.connections[connection_id].id[1]]
				= selected_connection.time;
			f.map[f.connections[connection_id].id[1]]->connections[f.connections[connection_id].id[0]]
				= selected_connection.time;
			return;
		}
		base_interface_input_handler(interface, event);
	}
}

void main_interface_input_callback(Window& window, Interface interface, Population& f, Event& event) {

}

void results_interface_input_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			temp_string_to_input_field(interface);
			update_input_field(interface);
			if (selected_route >= f.results.amount) {
				selected_route = 0;
				return;
			}
			selected_result = f.results.best_routes[results_mode][selected_route];
			return;
		}
		base_interface_input_handler(interface, event);
	}
}

void settings_interface_input_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			temp_string_to_input_field(interface);
			update_input_field(interface);
			return;
		}
		base_interface_input_handler(interface, event);
	}
}

void map_interface_callback(Window& window, Interface& interface, Population& f, Event& event) {
	if (event.mouseButton.x < 1000) { //Проверка позиции
		if (event.mouseButton.button == Mouse::Right) { //Выделить точку при нажатии ПКМ
			for (int i = 0; i < f.map.size(); ++i) {
				if (!interface.if_mouse_in_point(event, f.map[i]->obj, f))
					continue;
				f.map[point_id]->obj.setFillColor(f.point_color);
				f.map[i]->obj.setFillColor(f.point_selection_color);
				point_id = i;
				selected_point = *f.map[i];
				return;
			}
			return;
		}
		if (!edit_mode && event.mouseButton.button == Mouse::Left) { //Создать точку при нажатии ЛКМ в 1 режиме, если возможно
			for (auto& g : f.map) {
				Vector2f pos = g->obj.getPosition();
				float dx = event.mouseButton.x - pos.x - f.point_radius;
				float dy = event.mouseButton.y - pos.y - f.point_radius;
				if (sqrt(dx * dx + dy * dy) < (2.f * f.point_radius + 4.f))
					return;
			}
			Population::Point* point = f.add_point(event.mouseButton.x - f.point_radius, event.mouseButton.y - f.point_radius);
			auto& vect = point->obj.getPosition(); //Добавление текста к точке при её создании вручную -_-
			interface.add_pointer_string(&(point->name), 0, 14, vect.x + 3.f * f.point_radius, vect.y + 3.f, true);
			interface.string_pointers[interface.string_pointers.size() - 1].text.setFillColor(Color::White);
			interface.string_pointers[interface.string_pointers.size() - 1].text.setOutlineColor(Color::Black);
			interface.string_pointers[interface.string_pointers.size() - 1].text.setOutlineThickness(1.3f);
			return;
		}
		switch (event.mouseButton.button) { //Режим редактирования путей
		case Mouse::Left: //Провести путь от выделенной точки к данной, если возможно
			for (int i = 0; i < f.map.size(); ++i) {
				if ((!interface.if_mouse_in_point(event, f.map[i]->obj, f)) || (i == point_id))
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
					f.connections[connection_id].rectangle.setFillColor(f.path_color);
					f.connections[connection_id].time = selected_connection.time;
					f.connections[i].rectangle.setFillColor(f.path_selection_color);
					connection_id = i;
					selected_connection = f.connections[i];
					return;
				}
			}
		}
		return;
	}
}

int main() {

	srand(clock()); //начальное значение rand()
	system("color 08"); //цвет консоли
	setlocale(LC_ALL, ""); //использование русского языка

	Population g;
	g.load_settings();
	Texture map;
	Texture* map_ptr = &map;
	Interface interface("FiraCode-Regular.ttf");
	Interface main_interface("FiraCode-Regular.ttf");
	Interface results_interface("FiraCode-Regular.ttf");
	Interface map_interface("FiraCode-Regular.ttf");
	Interface settings_interface("FiraCode-Regular.ttf");
	map.loadFromFile("map.png");
	Sprite map_sprite(map);

	ContextSettings s;
	s.antialiasingLevel = 4; //параметры окна
	RenderWindow window(VideoMode(1400, 800), "Genetic Algorithm", Style::Default, s); //создание окна
	window.setFramerateLimit(60);

	float x = 1040.f, y = 120.f;

	interface.add_string(L"Режим редактора:", 18, x, y - 80);

	interface.add_string(L"Параметры пункта", 24, x, y - 40);
	interface.add_string(L"Номер:", 16, x, y);
	interface.add_string(L"Название:", 16, x, y + 20);
	interface.add_string(L"Время посещения:", 16, x, y + 40);
	interface.add_string(L"Стоимость посещения:", 16, x, y + 60);
	interface.add_string(L"Оценка:", 16, x, y + 80);

	interface.add_string(L"Начальный пункт:", 16, x, y + 120);
	interface.add_string(L"Конечный пункт:", 16, x, y + 140);

	interface.add_string(L"Параметры пути", 24, x, y + 180);
	interface.add_string(L"Время пути:", 16, x, y + 220);

	interface.add_string(L"Важность параметров", 24, x, y + 260);
	interface.add_string(L"Важность времени:", 16, x, y + 300);
	interface.add_string(L"Важность стоимости:", 16, x, y + 320);
	interface.add_string(L"Важность оценки:", 16, x, y + 340);

	interface.add_string(L"Алгоритм", 24, x, y + 380);
	interface.add_string(L"Количество особей:", 16, x, y + 420);
	interface.add_string(L"Количество генераций:", 16, x, y + 440);

	interface.add_pointer_string(&point_id, 0, 16, x + 220, y);

	interface.add_input_field(&selected_point.name, 1, 14, x + 120, y + 22, x + 300, y + 38, true);
	interface.add_input_field(&selected_point.time, 1, 16, x + 220, y + 40, x + 300, y + 60);
	interface.add_input_field(&selected_point.price, 1, 16, x + 220, y + 60, x + 300, y + 80);
	interface.add_input_field(&selected_point.score, 1, 16, x + 220, y + 80, x + 300, y + 100);

	interface.add_input_field(&g.route_parametres.first_point, 0, 16, x + 220, y + 120, x + 300, y + 140);
	interface.add_input_field(&g.route_parametres.last_point, 0, 16, x + 220, y + 140, x + 300, y + 160);

	interface.add_input_field(&selected_connection.time, 1, 16, x + 220, y + 220, x + 300, y + 240);

	interface.add_input_field(&g.user_ratios.time, 0, 16, x + 220, y + 300, x + 300, y + 320);
	interface.add_input_field(&g.user_ratios.price, 0, 16, x + 220, y + 320, x + 300, y + 340);
	interface.add_input_field(&g.user_ratios.score, 0, 16, x + 220, y + 340, x + 300, y + 360);

	interface.add_input_field(&g.amount, 0, 16, x + 220, y + 420, x + 300, y + 440);
	interface.add_input_field(&g.cycles_amount, 0, 16, x + 220, y + 440, x + 300, y + 460);

	interface.add_window_rectangle(0.5, x - 5, y - 45, x + 305, y + 165); //Между линиями - 10
	interface.add_window_rectangle(0.5, x - 5, y + 175, x + 305, y + 245); //Между первым текстом и линией - 35
	interface.add_window_rectangle(0.5, x - 5, y + 255, x + 305, y + 365); //Между линией и заголовком - 5
	interface.add_window_rectangle(0.5, x - 5, y + 375, x + 305, y + 465);

	Interface::Button* algorithm_button = interface.add_button(L"Сгенерировать", 16, x - 5, y + 480, x + 170, y + 500);
	Interface::Switch* edit_switch = interface.add_switch({ {x + 255, y - 80, x + 275, y - 60}, {x + 285, y - 80, x + 305, y - 60} });

	interface.add_string(L"Файл:", 16, x, y + 540);
	interface.add_input_field(&file_name, 1, 14, x + 120, y + 540, x + 300, y + 560, true);
	Interface::Button* save_button = interface.add_button(L"Сохранить", 16, x - 5, y + 570, x + 145, y + 590);
	Interface::Button* load_button = interface.add_button(L"Загрузить", 16, x + 150, y + 570, x + 300, y + 590);

	RectangleShape interface_bg(Vector2f(800.f, 800.f));
	interface_bg.move(1000.f, 0.f);

	main_interface.add_string(L"Режим интерфейса:", 18, x, y + 630); //Между строкой и переключателем - 0 -> 20
	Interface::Switch* interface_switch = main_interface.add_switch({ {x + 255, y + 630, x + 275, y + 650}, {x + 285, y + 630, x + 305, y + 650}, {x + 315, y + 630, x + 335, y + 650} });
	
	results_interface.add_string(L"Уникальные маршруты:", 18, x, y - 80);
	results_interface.add_string(L"Выбранный маршрут:", 18, x, y - 60);

	results_interface.add_string(L"Лучшие маршруты", 24, x, y - 30); //Между названием и строкой - 40
	results_interface.add_string(L"По времени:", 16, x, y + 10); //Между строками - 20 - 25
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
	results_interface.add_window_rectangle(0.5, x - 5, y + 125, x + 305, y + 260); //Между строкой и линией - 30

	Interface::Switch* results_switch = results_interface.add_switch({
			{x + 275, y + 10, x + 295, y + 30},
			{x + 275, y + 35, x + 295, y + 55},
			{x + 275, y + 60, x + 295, y + 80},
			{x + 275, y + 85, x + 295, y + 105}
		});

	settings_interface.add_string(L"Настройки программы", 24, x, y - 80);
	settings_interface.add_string(L"Отображение названий:", 16, x, y - 40);

	settings_interface.add_string(L"Настройки алгоритма", 24, x, y + 5);
	settings_interface.add_string(L"Cкрещивание:", 16, x, y + 45);
	settings_interface.add_string(L"Шанс мутации:", 16, x, y + 70);

	Interface::Switch* name_visibility_switch = settings_interface.add_switch({ {x + 240, y - 40, x + 260, y - 20}, {x + 270, y - 40, x + 290, y - 20} });
	Interface::Switch* recombination_switch = settings_interface.add_switch({ {x + 240, y + 45, x + 260, y + 65}, {x + 270, y + 45, x + 290, y + 65} });

	settings_interface.add_window_rectangle(0.5, x - 5, y - 85, x + 305, y - 10);
	settings_interface.add_window_rectangle(0.5, x - 5, y, x + 305, y + 100);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)) {
			main_interface_input_callback(window, main_interface, g, event);
			switch (interface_mode) {
			case 0:
				if (can_press_mouse_button(event)) {
					if (was_switched(edit_switch, interface, event)) {
						edit_mode = edit_switch->mode;
						for (auto& u : g.connections)
							u.rectangle.setFillColor(g.path_color);
					}
					if (interface.if_mouse_in_rectangle(event, algorithm_button->rectangle) && g.map.size() > 4 && g.route_parametres.first_point != g.route_parametres.last_point) {
						g.generate_routes(); //Нажатие кнопки 'Сгенерировать' и проверка невозможности некоторых случаев(проверки возможности создания маршрута нет - а как)
						g.results.reset();
						for (int i = 0; i < g.cycles_amount; ++i)
							g.cycle();
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
					if (interface.if_mouse_in_rectangle(event, save_button->rectangle))
						g.save(file_name);
					if (interface.if_mouse_in_rectangle(event, load_button->rectangle)) {
						g.load(file_name);
						map.loadFromFile("saves\\" + file_name + ".png");
						Sprite __map_sprite(map);
						map_sprite = __map_sprite;
						map_interface.string_pointers.clear();
						for (int i = 0; i < g.map.size(); ++i) {
							auto& vect = g.map[i]->obj.getPosition();
							map_interface.add_pointer_string(&(g.map[i]->name), 0, 14, vect.x + 3.f * g.point_radius, vect.y + 3.f, true); //Добавление названий
							map_interface.string_pointers[i].text.setFillColor(Color::White);
							map_interface.string_pointers[i].text.setOutlineColor(Color::Black);
							map_interface.string_pointers[i].text.setOutlineThickness(1.3f);
						}
					}
					check_input_fields_activation(interface, event);
					map_interface_callback(window, map_interface, g, event);
				}
				interface_input_callback(window, interface, g, event);
				break;
			case 1:
				if (can_press_mouse_button(event)) {
					if (was_switched(results_switch, results_interface, event)) {
						results_mode = results_switch->mode;
						if (!g.results.best_routes.empty())
							selected_result = g.results.best_routes[results_mode][selected_route];
					}
					check_input_fields_activation(results_interface, event);
				}
				results_interface_input_callback(window, results_interface, g, event);
				break;
			case 2:
				if (can_press_mouse_button(event)) {
					if (was_switched(name_visibility_switch, settings_interface, event)) {
						name_visibility_mode = name_visibility_switch->mode;
					}
				}
				settings_interface_input_callback(window, results_interface, g, event);
				break;
			}
			if (can_press_mouse_button(event)) {
				if (was_switched(interface_switch, main_interface, event)) {
					interface_mode = interface_switch->mode;
					for (auto& u : g.map)
						u->obj.setFillColor(g.point_color);
					for (auto& u : g.connections)
						u.rectangle.setFillColor(g.path_color);
				}
			}
			if (event.type == Event::Closed or event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
				window.close();
		}

		window.clear(Color::White);
		window.draw(interface_bg);
		window.draw(map_sprite);
		main_interface.draw(window);

		std::vector<std::vector<int>> connected;
		switch (interface_mode) {
		case 0:
			interface.draw(window);
			g.draw(window);
			break;
		case 1:
			results_interface.draw(window);
			for (auto& u : g.map)
				window.draw(u->obj);
			for (int i = 1; i < selected_result.path.size(); ++i) {
				std::vector<int> k1 = { selected_result.path[i - 1], selected_result.path[i] };
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
			break;
		case 2:
			settings_interface.draw(window);
			g.draw(window);
			break;
		}

		if (name_visibility_mode) {
			map_interface.draw(window);
		}

		window.display();

	}

}