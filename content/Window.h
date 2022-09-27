#pragma once

#include "Algorithm.h"
#include <sstream>
#include <iomanip>

class Interface {
private:

	Font font; //�����
	std::vector<Text> strings; //������������ ������
	std::vector<RectangleShape> window_rectangles; //��������� �������������� ����������

	struct Button { //��������� ������
		Text text;
		RectangleShape rectangle;
	};
	Button algorithm_button; //������, ������������ ��� ��������� ������������ ����� ������ ���������

	struct Switch { //��������� �������������
		std::vector<RectangleShape> rectangles;
		RectangleShape main_rectangle;
	};
	Switch mode_switch; //������������� ������ ���������

	struct PointerString { //������, � ������� ������� ��������� �������� ���������� �� ����������� ����������
		Text text;
		void* ptr;
		bool type;
	};
	std::vector<PointerString> string_pointers;

	struct InputField { //���� �����
		Text text;
		void* ptr;
		bool type;
		RectangleShape rectangle;
	};
	std::vector<InputField> input_fields;

	//�������� ����������� ������
	Text text_preset(String, int, float, float);
	//�������� ������������ ��������������
	RectangleShape rectangle_preset(float, float, float, float, float);
	//�������� ������� ���� � ��������������
	bool if_mouse_in_rectangle(Event&, RectangleShape&);
	//��������� ����������� �������������
	void change_switch_selection(Switch&, int);

public:

	int p1_id = 0; //���������� �����

	int p2_id = 0; //�����, � �������� ����� �������� ����, ���� ��������
	int connection_id = 0; //���������� ����
	bool mode = 0; //����� ���������

	int input_field_id = 0; //���������� ���� �����
	bool input_mode = 0; //����� �����
	String temp_string = ""; //��������� ������
	Text temp_text; //��������� ��������� �����

	Population::Point selected_point; //����� ����������� ������
	Population::Connection selected_connection; //����� ����������� ����

	//�����������
	Interface(std::string);
	//���������� ������
	void add_string(String str, int, float, float);
	//���������� ������������� ���������
	void add_mode_switch(std::vector<std::vector<float>>);
	//���������� ������ ���������
	void add_algorithm_button(String str, int, float, float, float, float);
	//���������� ������ �� ���������
	void add_pointer_string(void*, bool, int, float, float);
	//���������� ���� �����
	void add_input_field(void*, bool, int, float, float, float, float);
	//���������� ������������ �������������� �� �����
	void add_window_rectangle(float, float, float, float, float);
	//��������� �����
	void input_callback(Window&, Population&, Event&);
	//��������� ���� �������� ����������
	void draw(RenderWindow& window);

	friend class Population;
};
