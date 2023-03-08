#pragma once

#include "Functions.h"

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

float random(float a, float b) {
    return (rand() % (int)((b - a + 1) * 100.0f)) / 100.0f + a;
}

bool chance(int c) {
    return random(0, 100) < c ? true : false;
}

sf::Vector2f FixedPosition::getPosition(sf::RectangleShape obj) {
  sf::Vector2f pos = obj.getPosition();
  pos.x *= kx, pos.y *= ky;
  return pos;
}

sf::Vector2f FixedPosition::getPosition(sf::CircleShape obj) {
  sf::Vector2f pos = obj.getPosition();
  pos.x *= kx, pos.y *= ky;
  return pos;
}

FixedPosition::FixedPosition(sf::Window __window) {
  window = &__window;
  sf::Vector2u size = __window.getSize();
  width = size.x, heigth = size.y;
}

void FixedPosition::count_ratios() {
  sf::Vector2u size = window->getSize();
  kx = size.x / width, ky = size.y / heigth;
}
