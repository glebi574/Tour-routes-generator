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

Interface::Switch* Interface::add_switch(std::vector<std::vector<float>> coords) {
	Switch* _switch = new Switch;
	for (auto& g : coords) {
		_switch->rectangles.emplace_back(rectangle_preset(1.f, g[0], g[1], g[2], g[3]));
	}
	auto vect = _switch->rectangles[0].getPosition();
	auto size = _switch->rectangles[0].getSize();
	_switch->main_rectangle = RectangleShape(Vector2f(size.x * 0.8f, size.y * 0.8f));
	_switch->main_rectangle.setFillColor(Color::Black);
	_switch->main_rectangle.setPosition(vect.x + size.x * 0.1f, vect.y + size.y * 0.1f);
	switches.emplace_back(_switch);
	return _switch;
}

void Interface::change_switch_selection(Switch* s, int n) {
	s->mode = n;
	auto vect = s->rectangles[n].getPosition();
	auto size = s->rectangles[n].getSize();
	s->main_rectangle.setPosition(vect.x + size.x * 0.1f, vect.y + size.y * 0.1f);
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

Interface::Button* Interface::add_button(String str, int size, float x1, float y1, float x2, float y2) {
	Button* button = new Button;
	button->text = text_preset(str, size, (x2 + x1) / 2.f - str.getSize() * 5.f, y1);
	button->rectangle = rectangle_preset(0.5f, x1, y1, x2, y2);
	buttons.emplace_back(button);
	return button;
}

void Interface::add_pointer_string(void* ptr, bool type, int size, float x, float y, bool is_str) { //type == 1 - float, type == 0 - int
	PointerString ptr_str;
	ptr_str.text = text_preset("0", size, x, y);
	ptr_str.ptr = ptr;
	ptr_str.type = type;
	ptr_str.is_str = is_str;
	string_pointers.emplace_back(ptr_str);
}

void Interface::add_input_field(void* ptr, bool type, int size, float x1, float y1, float x2, float y2, bool is_str) {
	InputField input_field;
	input_field.text = text_preset("0", size, x1, y1);
	input_field.rectangle = rectangle_preset(0.5f, x1, y1, x2, y2);
	input_field.rectangle.setFillColor(Color::Transparent);
	input_field.ptr = ptr;
	input_field.type = type;
	input_field.is_str = is_str;
	input_fields.emplace_back(input_field);
}

void Interface::add_window_rectangle(float width, float x1, float y1, float x2, float y2) {
	window_rectangles.emplace_back(rectangle_preset(0.5f, x1, y1, x2, y2));
}

void Interface::draw(RenderWindow& window) {
	for (auto& g : window_rectangles) //Отрисовка некоторых прямоугольников
		window.draw(g);
	for (auto& g : string_pointers) { //Отрисовка строк, значение которых получено из указателя
		std::stringstream str;
		if (g.is_str) {
			str << *(std::string*)g.ptr;
			g.text.setString(*(std::wstring*)g.ptr);
		}
		else {
			if (g.type)
				str << std::fixed << std::setprecision(2) << *(float*)g.ptr;
			else
				str << *(int*)g.ptr;
			g.text.setString(str.str());
		}
		window.draw(g.text);
	}
	for (auto& g : input_fields) { //Отрисовка полей ввода
		std::stringstream str;
		if (g.is_str) {
			str << *(std::string*)g.ptr;
			g.text.setString(*(std::wstring*)g.ptr);
		} else {
			if (g.type)
				str << std::fixed << std::setprecision(2) << *(float*)g.ptr;
			else
				str << *(int*)g.ptr;
			g.text.setString(str.str());
		}
		window.draw(g.text);
		window.draw(g.rectangle);
	}
	for (auto& g : strings) //Отрисовка строк
		window.draw(g);
	for (auto& g : switches) {
		for (auto& u : g->rectangles)
			window.draw(u);
		window.draw(g->main_rectangle);
	}
	for (auto& g : buttons) {
		window.draw(g->rectangle);
		window.draw(g->text);
	}
	window.draw(temp_text); //Отрисовка введённого текста
}
