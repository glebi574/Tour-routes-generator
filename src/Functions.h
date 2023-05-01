#pragma once

#include "include.h"

//Случайное число типа int от a до b
int random(int, int);
//Случайное число типа float от a до b
float random(float, float);
//True или False с некоторым шансом
bool chance(int);
//Поиск элемента в векторе
template<typename T> int find(std::vector<T>& vec, T v) {
    for (T i = 0; i < vec.size(); ++i)
        if (vec[i] == v)
            return i;
    return -1;
}
/*
class sf::RectangleShape : public FixedPosition { };

class sf::CircleShape : public FixedPosition { };

class FixedPosition {
public:
  sf::Window* window;

  float kx = 1.f, ky = 1.f;
  float width, heigth;

  FixedPosition(sf::Window);

  void count_ratios();

  sf::Vector2f getPosition(sf::RectangleShape);

  sf::Vector2f getPosition(sf::CircleShape);
};
*/