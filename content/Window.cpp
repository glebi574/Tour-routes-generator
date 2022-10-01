#pragma once

#include "Window.h"

Interface::Interface(std::string str) {
	font.loadFromFile(str);
	temp_text = Text("", font, 16);
	temp_text.setFillColor(Color::Black);
}

Text Interface::text_preset(String str, int size, float x, float y) {
	Text text(str, font, size);
	text.setFillColor(Color::Black);
	text.move(x, y);
	return text;
}

RectangleShape Interface::rectangle_preset(float width, float x1, float y1, float x2, float y2) {
	RectangleShape rectangle(Vector2f(x2 - x1, y2 - y1));
	rectangle.setOutlineThickness(width);
	rectangle.setOutlineColor(Color::Black);
	rectangle.move(x1, y1);
	return rectangle;
}

void Interface::change_switch_selection(Switch& s, int n) {
	auto vect = s.rectangles[n].getPosition();
	auto size = s.rectangles[n].getSize();
	s.main_rectangle = RectangleShape(Vector2f(size.x * 0.8f, size.y * 0.8f));
	s.main_rectangle.setFillColor(Color::Black);
	s.main_rectangle.move(vect.x + size.x * 0.1f, vect.y + size.y * 0.1f);
}

bool Interface::if_mouse_in_rectangle(Event& event, RectangleShape& rectangle) {
	auto vect = rectangle.getPosition();
	auto size = rectangle.getSize();
	if (vect.x <= event.mouseButton.x && event.mouseButton.x <= vect.x + size.x
		&& vect.y <= event.mouseButton.y && event.mouseButton.y <= vect.y + size.y) {
		return true;
	}
	return false;
}

bool Interface::if_mouse_in_point(Event& event, CircleShape& circle, Population& g) {
	Vector2f pos = circle.getPosition();
	float dx = event.mouseButton.x - pos.x - g.point_radius;
	float dy = event.mouseButton.y - pos.y - g.point_radius;
	return (sqrt(dx * dx + dy * dy) < (g.point_radius + 2.f));
}

void Interface::add_string(String str, int size, float x, float y) {
	strings.emplace_back(text_preset(str, size, x, y));
}

void Interface::add_mode_switch(std::vector<std::vector<float>> coords) {
	for (auto& g : coords) {
		mode_switch.rectangles.emplace_back(rectangle_preset(1.f, g[0], g[1], g[2], g[3]));
	}
	change_switch_selection(mode_switch, 0);
}

void Interface::add_algorithm_button(String str, int size, float x1, float y1, float x2, float y2) {
	algorithm_button.text = text_preset(str, size, (x2 + x1) / 2.f - str.getSize() * 5.f, y1);
	algorithm_button.rectangle = rectangle_preset(0.5f, x1, y1, x2, y2);
}

void Interface::add_pointer_string(void* ptr, bool type, int size, float x, float y) {
	PointerString ptr_str;
	ptr_str.text = text_preset("0", size, x, y);
	ptr_str.ptr = ptr;
	ptr_str.type = type;
	string_pointers.emplace_back(ptr_str);
}

void Interface::add_input_field(void* ptr, bool type, int size, float x1, float y1, float x2, float y2) {
	InputField input_field;
	input_field.text = text_preset("0", size, x1, y1);
	input_field.rectangle = rectangle_preset(0.5f, x1, y1, x2, y2);
	input_field.rectangle.setFillColor(Color::Transparent);
	input_field.ptr = ptr;
	input_field.type = type;
	input_fields.emplace_back(input_field);
}

void Interface::add_window_rectangle(float width, float x1, float y1, float x2, float y2) {
	window_rectangles.emplace_back(rectangle_preset(0.5f, x1, y1, x2, y2));
}

void Interface::input_callback(Window& window, Population& f, Event& event) {
	//Режим ввода
	if (input_mode) {
		if (event.key.code == Keyboard::Return) { //Нажатие клавиши ввода
			input_mode = false;
			if (input_fields[input_field_id].type) //Изменение значения, на которое указывает ptr, на введённое
				*(float*)input_fields[input_field_id].ptr = std::stof(std::string(temp_string));
			else
				*(int*)input_fields[input_field_id].ptr = std::stoi(std::string(temp_string));
			input_fields[input_field_id].rectangle.setFillColor(Color::Transparent);
			temp_string = "";
			temp_text.setString("");
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
			if (temp_string.size() != 0) //Удаление 1 введённого символа, если возможно
				temp_string.pop_back();
			return;
		}
		if (event.type == Event::TextEntered) //Ввод остальных символов с клавиатуры
		{
			temp_string += event.text.unicode; //Увеличение строки на введённый символ
			temp_text.setString(temp_string); //Обновление отображаемого текста
			return;
		}
	}
	if (event.type != Event::MouseButtonReleased)
		return;
	//Работа с картой
	if (event.mouseButton.x < 1000) { //Проверка позиции
		if (event.mouseButton.button == Mouse::Right) { //Выделить точку при нажатии ПКМ
			for (int i = 0; i < f.map.size(); ++i) {
				if (!if_mouse_in_point(event, f.map[i].obj, f))
					continue;
				f.map[point_id].obj.setFillColor(Color::Red);
				f.map[i].obj.setFillColor(IColor::Blue);
				point_id = i;
				selected_point = f.map[i];
				return;
			}
			return;
		}
		if (!mode && event.mouseButton.button == Mouse::Left) { //Создать точку при нажатии ЛКМ в 1 режиме, если возможно
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
				if ((!if_mouse_in_point(event, f.map[i].obj, f)) || (i == point_id))
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
	//Работа с интерфейсом
	for (int i = 0; i < mode_switch.rectangles.size(); ++i) { //Проверка переключения переключателя
		if (if_mouse_in_rectangle(event, mode_switch.rectangles[i])) {
			mode = !mode;
			change_switch_selection(mode_switch, mode);
			for (auto& g : f.connections)
				g.rectangle.setFillColor(IColor::Gray);
			return;
		}
	}
	if (if_mouse_in_rectangle(event, algorithm_button.rectangle)) { //Проверка нажатия кнопки
		f.generate_routes();
		for (int i = 0; i < f.cycles_amount; ++i) {
			f.cycle();
		}
		return;
	}
	for (int i = 0; i < input_fields.size(); ++i) { //Проверка нажатия на поле ввода
		if (if_mouse_in_rectangle(event, input_fields[i].rectangle)) {
			input_mode = true;
			input_field_id = i;
			temp_text.setPosition(input_fields[i].rectangle.getPosition());
			input_fields[i].rectangle.setFillColor(Color::White);
		}
	}
}

void Interface::draw(RenderWindow& window) {
	for (auto& g : window_rectangles) //Отрисовка некоторых прямоугольников
		window.draw(g);
	for (auto& g : string_pointers) { //Отрисовка строк, значение которых получено из указателя
		std::stringstream str;
		if (g.type)
			str << std::fixed << std::setprecision(2) << *(float*)g.ptr;
		else
			str << *(int*)g.ptr;
		g.text.setString(str.str());
		window.draw(g.text);
	}
	for (auto& g : input_fields) { //Отрисовка полей ввода
		std::stringstream str;
		if (g.type)
			str << std::fixed << std::setprecision(2) << *(float*)g.ptr;
		else
			str << *(int*)g.ptr;
		g.text.setString(str.str());
		window.draw(g.text);
		window.draw(g.rectangle);
	}
	for (auto& g : strings) //Отрисовка строк
		window.draw(g);
	for (auto& g : mode_switch.rectangles) //Отрисовка переключателей
		window.draw(g);
	window.draw(mode_switch.main_rectangle);
	window.draw(algorithm_button.rectangle); //Отрисовка кнопки
	window.draw(algorithm_button.text);
	window.draw(temp_text); //Отрисовка введённого текста
}
