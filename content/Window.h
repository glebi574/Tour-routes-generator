#pragma once

#include "Algorithm.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
using namespace sf;

struct vertexes {

};

struct Graph {
	std::vector<CircleShape> objects;
	std::vector<RectangleShape> lines;
};

static Graph graph;

//добавление объекта для отрисовки
void add_object(float, float);
//содание карты
void create_graph(Population&);
//отрисовка карты
void draw_graph(RenderWindow&);
