#pragma once

#include "Algorithm.h"

class Interface {
public:
	struct Button { //структура кнопки
		Text text;
		RectangleShape rectangle;
	};
	std::vector<Button*> buttons;

	struct Switch { //структура переключателя
		std::vector<RectangleShape> rectangles;
		RectangleShape main_rectangle;
		int mode = 0;
	};
	std::vector <Switch*> switches;

	struct PointerString { //строки, с помощью которых выводятся значения указателей на определённые переменные
		Text text;
		void* ptr;
		bool type;
		bool is_str;
	};
	std::vector<PointerString> string_pointers;

	struct InputField { //поле ввода
		Text text;
		void* ptr;
		bool type;
		RectangleShape rectangle;
		bool is_str;
	};
	std::vector<InputField> input_fields;

	Font font; //шрифт
	std::vector<Text> strings; //отображаемые строки
	std::vector<RectangleShape> window_rectangles; //некоторые прямоугольники интерфейса

	//Создание стандартной строки
	Text text_preset(String, int, float, float);
	//Создание стандартного прямоугольника
	RectangleShape rectangle_preset(float, float, float, float, float);
	//Проверка наличия мыши в прямоугольнике
	bool if_mouse_in_rectangle(Event&, RectangleShape&);
	//Проверка наличия мыши в пункте
	bool if_mouse_in_point(Event&, CircleShape&, Population&);

	std::wstring temp_string = L""; //временная строка
	Text temp_text; //временный выводимый текст

	//Конструктор
	Interface(std::string);
	//Создание кнопки
	Button* add_button(String str, int, float, float, float, float);
	//Создание переключателя
	Switch* add_switch(std::vector<std::vector<float>>);
	//Изменение выделенного переключателя
	void change_switch_selection(Switch*, int);
	//Добавление строки
	void add_string(String str, int, float, float);
	//Добавление строки из указателя
	void add_pointer_string(void*, bool, int, float, float, bool = false);
	//Добавление поля ввода
	void add_input_field(void*, bool, int, float, float, float, float, bool = false);
	//Добавления стандартного прямоугольника на экран
	void add_window_rectangle(float, float, float, float, float);
	//Отрисовка всех объектов интерфейса
	void draw(RenderWindow& window);
};
