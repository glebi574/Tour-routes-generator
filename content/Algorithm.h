#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class Population {
private:

	const float point_radius = 12.f;
	const float line_width = 5.f;

	int points = 0; //���������� �������
	int attempt = 1; //����� ���������

	struct Point {
		std::vector<float> connections;
		float time = 0.f;
		float price = 0.f;
		float score = 0.f;
		CircleShape obj;
	};
	std::vector<Point> map; //�����, ���������� ��� ������ � ���������� ����� ����

	struct Connection {
		int id[2];
		RectangleShape rectangle;
	};
	std::vector<Connection> connections;

	struct Route {
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

	struct RouteParametres {
		int first_point = 0;
		int last_point = 0;
	} route_parametres; //��������� � �������� ����� ��������

	struct UserRatios {
		int time = 1;
		int price = 1;
		int score = 1;
	} user_ratios; //�������� ������� ����������

	//��������� ���������������� �������������
	void set_user_ratios(int, int, int);
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
	//��������� �����
	void generate_map();
	//������� ����������� ��������
	void mutation(Route&);
	//������� ����������� ��������
	void count_result(Route&);
	//������� ����������� ������������ ��� ���� ���������
	void count_user_results();
	//���������� ����� ��� ����������� ������� �� ������� ������������� �����
	void add_point_in_route(Route&, int);
	//�������� ����� ��� ����������� ������� �� ��������
	void remove_point_from_route(Route&, int);
	//�������������� ������� ������������� �����
	void update_absent(Route&);
	//�������� ������������ ��������
	bool check_exceptions(std::vector<int>&);
	//�������� ����� �� �����
	void add_point(float, float);
	//�������� ���������� ����� ����� �������
	void add_connection(int, int);

	friend class EventCallback;
	friend class Interface;
};