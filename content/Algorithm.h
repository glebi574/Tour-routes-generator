#pragma once

#include <iostream>
#include <vector>

class Population {
private:

	static constexpr float speed = 1.4f;
	short amount;
	short points;
	short attempt = 1;

	struct map_part {
		std::vector<float> connections;
		float time;
		float price;
		float score;
	};
	std::vector<map_part> map;

	struct route {
		std::vector<short> path;
		std::vector<short> absent;
		float time;
		float price;
		float score;
		float user_result;
	};
	std::vector<route> generation;
	std::vector<route> best_results;

	struct user_ratios {
		int time;
		int price;
		int score;
	};
	user_ratios user_ratios;

	struct route_parametres {
		short first_point;
		short last_point;
	};
	route_parametres route_parametres;

	enum class mode { manual, random };
	bool mode = (bool)mode::random;

public:
	//��������� ���������(���������� ������, ���������� �������, ��������� �����, �������� �����)
	//\param ���������� ���������, ������������ ���������� ����� � ����, ��������� �����, �������� �����
	Population(short, short, short fp = 0, short lp = 0);
	//��������� ���������������� �������������
	void set_user_ratios(int, int, int);
	//����������� ��������� � ������� �����������
	void cycle();
	//����� �����
	void print_map();
	//����� ���� ���������
	void print_population();
	//������� ���� ���������
	//void Mutation();

private:
	friend std::ostream& operator<< (std::ostream&, const route&);
	friend void create_graph(Population&);
	//��������� �����
	void generate_map();
	//������� ����������� ��������
	void mutation(route&);
	//�����������
	void Crossing();
	//������� ����������� ��������
	void count_result(route&);
	//������� ����������� ������������ ��� ���� ���������
	void count_user_results();
	//���������� ����� ��� ����������� ������� �� ������� ������������� �����
	void add_point(route&, int);
	//�������� ����� ��� ����������� ������� �� ��������
	void remove_point(route&, int);
	//�������������� ������� ������������� �����
	void update_absent(route&);
	//�������� ������������ ��������
	bool check_exceptions(std::vector<short>&);

};