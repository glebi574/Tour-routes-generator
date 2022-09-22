#pragma once

#include "Algorithm.h"

class EventCallback {
private:

public:
	int selected_p1_id = 0;
	int selected_p2_id = 0;
	int connection_id = 0;
	bool mode = 0;

	Population::Point selected_point;

	void change_mode();

	void input_callback(Population&, Event&, Interface&);

	friend class Population;
	friend class Interface;
};

class Interface {
private:
	Font font;

	std::vector<Text> strings;

	struct Button {
		Text text;
		RectangleShape obj;
		void (*func)(...);
	};
	std::vector<Button> buttons;

	struct Switch {
		std::vector<RectangleShape> objects;
		std::vector<void(*)()> funcs;
		RectangleShape MainRectangle;
		int selected = 0;
	};
	//std::vector<Switch> switches;
	Switch mode_switch;

	struct PointerString {
		Text obj;
		int* int_ptr;
		float* float_ptr;
	};
	std::vector<PointerString> string_pointers;

	std::vector<RectangleShape> window_rectangles;

	void string_preset(Text&, float, float);

public:
	void change_font(String);
	bool if_mouse_in_rectangle(Event&, RectangleShape);
	void add_string(String, int, float, float);
	//void add_button(String, int, float, float, float, float, void(*)(...));
	//void add_switch(std::vector<std::vector<float>>, std::vector<void(*)()>);
	void change_switch_selection(Switch&, int);
	void add_mode_switch(std::vector<std::vector<float>>);
	void add_string_from_pointer(int&, int, float, float);
	void add_string_from_pointer(float&, int, float, float);
	void add_window_rectangle(float, float, float, float, float);
	void draw(RenderWindow& window);

	friend class Population;
	friend class EventCallback;
};
