#pragma once

#include "Window.h"

float line_width = 2.0f;
float object_radius = 16.0f;
float global_radius = 250.0f;
float d_centre = 400.0f - object_radius;

void add_object(float x, float y) {
	CircleShape c(object_radius);
	c.setFillColor(Color(255, 112, 94));
	c.move(x, y);
	graph.objects.emplace_back(c);
}

void create_graph(Population& obj) {
	float ang = 0.0f;
	float d_ang = 6.2832f / obj.points;
	Event event;
	for (auto& g : obj.map) {
		add_object(d_centre - cos(ang) * global_radius, d_centre - sin(ang) * global_radius);
		ang += d_ang;
	}
	for (int a = 0; a < obj.points; ++a) {
		for (int b = a + 1; b < obj.points; ++b) {
			if (obj.map[a].connections[b] != 0.0f) {
				auto& p1 = graph.objects[a].getPosition();
				auto& p2 = graph.objects[b].getPosition();
				float l = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
				RectangleShape r(Vector2f(l, line_width));
				r.move(p1.x + object_radius, p1.y + object_radius);
				r.rotate(180.0f / 3.1416f * atan2(p2.y - p1.y, p2.x - p1.x));
				r.setFillColor(Color(229, 255, 0));
				graph.lines.emplace_back(r);
			}
		}
	}
}

void draw_graph(RenderWindow& window) {
	for (auto& obj : graph.lines)
		window.draw(obj);
	for (auto& obj : graph.objects)
		window.draw(obj);
}
