#pragma once

#include <vector>

//��������� ����� ���� int �� a �� b
int random(int , int);
//��������� ����� ���� float �� a �� b
float random(float , float);
//True ��� False � ��������� ������
bool chance(int);
//����� �������� � �������
template<typename T> int find(std::vector<T>& vec, T v) {
    for (T i = 0; i < vec.size(); ++i)
        if (vec[i] == v)
            return i;
    return -1;
}
