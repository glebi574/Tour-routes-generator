#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
using namespace sf;

namespace IColor { //�������������� �����
	const Color Blue = Color(100, 100, 240);
	const Color Pink = Color(200, 140, 200);
	const Color Gray = Color(120, 120, 120);
}

class Population {
private:

	const float point_radius = 12.f;
	const float line_width = 5.f;

	int points = 0; //���������� �������
	int attempt = 1; //����� ���������

	struct Point { //�����
		std::vector<float> connections;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		CircleShape obj;
	};
	std::vector<Point> map; //�����, ���������� ��� ������ � ���������� ����� ����

	struct Connection { //������������ ����
		int id[2];
		float time = 0.f;
		RectangleShape rectangle;
	};
	std::vector<Connection> connections; //������ �� ����� ��������������� ������

	struct Route { //�������
		std::vector<int> path;
		std::vector<int> absent;
		float time;
		float price;
		float score;
		float user_result;
	};
	std::vector<Route> generation; //��������� �� ����� �������
	std::vector<Route> best_results; //������ ���� ����� ���� ���������

public:

	int amount = 20; //���������� ������
	int cycles_amount = 0; //���������� ������

	struct RouteParametres {
		int first_point = 0;
		int last_point = 0;
	} route_parametres; //��������� � �������� ����� ��������

	struct UserRatios {
		int time = 1;
		int price = 1;
		int score = 1;
	} user_ratios; //�������� ������� ����������

	//����������� �������� ����������, ������������ ���������� � �������� ������
	void generate_routes();
	//����������� ��������� � ������� �����������
	void cycle();
	//����� �����
	void print_map();
	//����� ���� ���������
	void print_population();
	//��������� ���� ������� �� �����
	void draw(RenderWindow&);

private:
	friend std::ostream& operator<< (std::ostream&, const Route&);
	//������� ����������� ��������
	void mutation(Route&);
	//������� ����������� ��������
	void count_result(Route&);
	//������� ����������� ������������ ��� ���� ���������
	void count_user_results();
	//�������������� ������� ������������� �����
	void update_absent(Route&);
	//�������� ������������ ��������
	bool check_exceptions(std::vector<int>&);
	//�������� ����� �� �����
	void add_point(float, float);
	//�������� ���������� ����� ����� �������
	void add_connection(int, int);

	friend class Interface;
};
