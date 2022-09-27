#pragma once

#include "Algorithm.h"
#include <sstream>
#include <iomanip>

class Interface {
private:

	Font font; //шрифт
	std::vector<Text> strings; //отображаемые строки
	std::vector<RectangleShape> window_rectangles; //некоторые прямоугольники интерфейса

	struct Button { //структура кнопки
		Text text;
		RectangleShape rectangle;
	};
	Button algorithm_button; //кнопка, используемая для генерации определённого числа циклов алгоритма

	struct Switch { //структура переключателя
		std::vector<RectangleShape> rectangles;
		RectangleShape main_rectangle;
	};
	Switch mode_switch; //переключатель режима редактора

	struct PointerString { //строки, с помощью которых выводятся значения указателей на определённые переменные
		Text text;
		void* ptr;
		bool type;
	};
	std::vector<PointerString> string_pointers;

	struct InputField { //поле ввода
		Text text;
		void* ptr;
		bool type;
		RectangleShape rectangle;
	};
	std::vector<InputField> input_fields;

	//Создание стандартной строки
	Text text_preset(String, int, float, float);
	//Создание стандартного прямоугольника
	RectangleShape rectangle_preset(float, float, float, float, float);
	//Проверка наличия мыши в прямоугольнике
	bool if_mouse_in_rectangle(Event&, RectangleShape&);
	//Изменение выделенного переключателя
	void change_switch_selection(Switch&, int);

public:

	int p1_id = 0; //выделенный пункт

	int p2_id = 0; //пункт, к которому будет проведен путь, если возможно
	int connection_id = 0; //выделенный путь
	bool mode = 0; //режим редактора

	int input_field_id = 0; //выделенное поле ввода
	bool input_mode = 0; //режим ввода
	String temp_string = ""; //временная строка
	Text temp_text; //временный выводимый текст

	Population::Point selected_point; //копия выделенного пункта
	Population::Connection selected_connection; //копия выделенного пути

	//Конструктор
	Interface(std::string);
	//Добавление строки
	void add_string(String str, int, float, float);
	//Добавление переключателя редактора
	void add_mode_switch(std::vector<std::vector<float>>);
	//Добавление кнопки генерации
	void add_algorithm_button(String str, int, float, float, float, float);
	//Добавление строки из указателя
	void add_pointer_string(void*, bool, int, float, float);
	//Добавление поля ввода
	void add_input_field(void*, bool, int, float, float, float, float);
	//Добавления стандартного прямоугольника на экран
	void add_window_rectangle(float, float, float, float, float);
	//Обработка ввода
	void input_callback(Window&, Population&, Event&);
	//Отрисовка всех объектов интерфейса
	void draw(RenderWindow& window);

	friend class Population;
};
