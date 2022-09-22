#pragma once

#include "Window.h"

void EventCallback::change_mode() {
	mode = !mode;
}

void EventCallback::input_callback(Population& f, Event& event, Interface& interface) {
	if (event.type == Event::MouseButtonReleased) { //Нажатие мыши
		if (event.mouseButton.x < 988) { //Проверка позиции
			if (mode) {
				if (event.mouseButton.button == Mouse::Left) { //Выделить точку при нажатии ЛКМ
					for (int i = 0; i < f.map.size(); ++i) {
						Vector2f pos = f.map[i].obj.getPosition();
						float dx = event.mouseButton.x - pos.x - f.point_radius;
						float dy = event.mouseButton.y - pos.y - f.point_radius;
						if (sqrt(dx * dx + dy * dy) < f.point_radius + 2.f) {
							f.map[selected_p2_id].obj.setFillColor(Color::Red);
							f.map[i].obj.setFillColor(Color::Blue);
							selected_p2_id = i;
							selected_point = f.map[i];
							break;
						}
					}
				}
				if (event.mouseButton.button == Mouse::Right) { //Создать соединение, если возможно при нажатии ПКМ
					for (int i = 0; i < f.map.size(); ++i) {
						Vector2f pos = f.map[i].obj.getPosition();
						float dx = event.mouseButton.x - pos.x - f.point_radius;
						float dy = event.mouseButton.y - pos.y - f.point_radius;
						if (sqrt(dx * dx + dy * dy) < f.point_radius + 2.f) {
							if (i != selected_p2_id) {
								for (auto& m : f.connections) {
									if ((m.id[0] == i && m.id[1] == selected_p2_id) || (m.id[0] == selected_p2_id && m.id[1] == i))
										return;
								}
								f.add_connection(i, selected_p2_id);
							}
							break;
						}
					}
				}
			}
			else {
				if (event.mouseButton.button == Mouse::Left) { //Создать точку при нажатии ЛКМ
					for (auto& g : f.map) {
						Vector2f pos = g.obj.getPosition();
						float dx = event.mouseButton.x - pos.x - f.point_radius;
						float dy = event.mouseButton.y - pos.y - f.point_radius;
						if (sqrt(dx * dx + dy * dy) < 2.f * f.point_radius + 4.f)
							return;
					}
					f.add_point(event.mouseButton.x - f.point_radius, event.mouseButton.y - f.point_radius);
				}
				if (event.mouseButton.button == Mouse::Right) { //Выделить точку при нажатии ПКМ
					for (int i = 0; i < f.map.size(); ++i) {
						Vector2f pos = f.map[i].obj.getPosition();
						float dx = event.mouseButton.x - pos.x - f.point_radius;
						float dy = event.mouseButton.y - pos.y - f.point_radius;
						if (sqrt(dx * dx + dy * dy) < f.point_radius + 2.f) {
							f.map[selected_p1_id].obj.setFillColor(Color::Red);
							f.map[i].obj.setFillColor(Color::Green);
							selected_p1_id = i;
							selected_point = f.map[i];
							break;
						}
					}
				}
			}
		}
		else { //Работа с интерфейсом
			/*for (auto& g : interface.buttons) {
				if (interface.if_mouse_in_rectangle(event, g.obj)) {
					g.func();
				}
			}*/
			/*for (auto& g : interface.switches) {
				for (int i = 0; i < g.objects.size(); ++i) {
					if (interface.if_mouse_in_rectangle(event, g.objects[i])) {
						g.funcs[i]();
						...
					}
				}
			}
			*/
			for (int i = 0; i < interface.mode_switch.objects.size(); ++i) {
				if (interface.if_mouse_in_rectangle(event, interface.mode_switch.objects[i])) {
					mode = !mode;
					interface.change_switch_selection(interface.mode_switch, (int)mode);
					for (auto& g : f.map) {
						g.obj.setFillColor(Color::Red);
					}
				}
			}
		}
	}
}


void Interface::change_font(String str) {
	font.loadFromFile(str);
}

bool Interface::if_mouse_in_rectangle(Event& event, RectangleShape rectangle) {
	auto vect = rectangle.getPosition();
	auto size = rectangle.getSize();
	if (vect.x <= event.mouseButton.x && event.mouseButton.x <= vect.x + size.x
		&& vect.y <= event.mouseButton.y && event.mouseButton.y <= vect.y + size.y) {
		return true;
	}
	return false;
}

void Interface::string_preset(Text& line, float x, float y) {
	line.setFillColor(Color::Black);
	line.move(x, y);
}

void Interface::add_string(String str, int size, float x, float y) {
	Text line(str, font, size);
	string_preset(line, x, y - 1.f);
	strings.emplace_back(line);
}
/*
void Interface::add_button(String str, int size, float x1, float y1, float x2, float y2, void(*func)(...)) {
	Text text(str, font, size);
	string_preset(text, (x2 + x1) / 2.f - str.getSize() * 5.f, y1);
	RectangleShape rectangle(Vector2f(x2 - x1, y2 - y1));
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineThickness(2.f);
	rectangle.setOutlineColor(Color::Black);
	rectangle.move(x1, y1);
	Button button;
	button.text = text;
	button.obj = rectangle;
	button.func = func;
	buttons.emplace_back(button);
}
*/
/*
void Interface::add_switch(std::vector<std::vector<float>> coords, std::vector<void(*)()> funcs) {
	Switch s;
	for (auto& g : coords) {
		RectangleShape rectangle(Vector2f(g[2] - g[0], g[3] - g[1]));
		rectangle.setFillColor(Color::White);
		rectangle.setOutlineThickness(1.f);
		rectangle.setOutlineColor(Color::Black);
		rectangle.move(g[2], g[3]);
		s.objects.emplace_back(rectangle);
	}
	for (auto& g : funcs) {
		s.funcs.emplace_back(g);
	}
	auto vect = s.objects[0].getPosition();
	auto size = s.objects[0].getSize();
	s.MainRectangle = RectangleShape(Vector2f(size.x * 0.8f, size.y * 0.8f));
	s.MainRectangle.setFillColor(Color::Black);
	s.MainRectangle.move(vect.x + size.x * 0.1f, vect.y + size.y * 0.1f);
	switches.emplace_back(s);
}
*/
void Interface::add_mode_switch(std::vector<std::vector<float>> coords) {
	for (auto& g : coords) {
		RectangleShape rectangle(Vector2f(g[2] - g[0], g[3] - g[1]));
		rectangle.setFillColor(Color::White);
		rectangle.setOutlineThickness(1.f);
		rectangle.setOutlineColor(Color::Black);
		rectangle.move(g[2], g[3]);
		mode_switch.objects.emplace_back(rectangle);
	}
	change_switch_selection(mode_switch, 0);
}

void Interface::change_switch_selection(Switch& s, int n) {
	auto vect = s.objects[n].getPosition();
	auto size = s.objects[n].getSize();
	s.MainRectangle = RectangleShape(Vector2f(size.x * 0.8f, size.y * 0.8f));
	s.MainRectangle.setFillColor(Color::Black);
	s.MainRectangle.move(vect.x + size.x * 0.1f, vect.y + size.y * 0.1f);
	s.selected = n;
}

void Interface::add_string_from_pointer(int& ptr, int size, float x, float y) {
	PointerString line;
	line.obj = Text("0", font, size);
	string_preset(line.obj, x, y);
	line.int_ptr = &ptr;
	line.float_ptr = NULL;
	string_pointers.emplace_back(line);
}

void Interface::add_string_from_pointer(float& ptr, int size, float x, float y) {
	PointerString line;
	line.obj = Text("0", font, size);
	string_preset(line.obj, x, y);
	line.int_ptr = NULL;
	line.float_ptr = &ptr;
	string_pointers.emplace_back(line);
}

void Interface::add_window_rectangle(float width, float x1, float y1, float x2, float y2) {
	RectangleShape rectangle(Vector2f(x2 - x1, y2 - y1));
	rectangle.setFillColor(Color::White);
	rectangle.setOutlineThickness(width);
	rectangle.setOutlineColor(Color::Black);
	rectangle.move(x1, y1);
	window_rectangles.emplace_back(rectangle);
}

void Interface::draw(RenderWindow& window) {
	for (auto& g : window_rectangles) {
		window.draw(g);
	}
	/*for (auto& g : buttons) {
		window.draw(g.obj);
		window.draw(g.text);
	}*/
	/*for (auto& g : switches) {
		for (auto& r : g.objects) {
			window.draw(r);
		}
		window.draw(g.MainRectangle);
	}*/
	for (auto& g : mode_switch.objects) {
		window.draw(g);
	}
	window.draw(mode_switch.MainRectangle);
	for (auto& g : strings) {
		window.draw(g);
	}
	for (auto& g : string_pointers) {
		std::string str;
		if (g.int_ptr == NULL)
			str = std::to_string(*g.float_ptr);
		else
			str = std::to_string(*g.int_ptr);
		g.obj.setString(str);
		window.draw(g.obj);
	}
}
