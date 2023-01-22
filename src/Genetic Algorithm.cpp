
#include "Functions.h"
#include "Algorithm.h"
#include "Window.h"

int main() {

    srand(clock()); //начальное значение rand()
    system("color 08"); //цвет консоли
    setlocale(LC_ALL, ""); //использование русского языка

    Population g;
    Texture map;
    Interface interface("FiraCode-Regular.ttf");
    Interface main_interface("FiraCode-Regular.ttf");
    Interface results_interface("FiraCode-Regular.ttf");
    map.loadFromFile("map.png");
    Sprite map_sprite(map);

    ContextSettings s;
    s.antialiasingLevel = 4; //параметры окна
    RenderWindow window(VideoMode(1400, 800), "Genetic Algorithm", Style::Default, s); //создание окна
    window.setFramerateLimit(60);

    float x = 1040.f, y = 120.f;

    interface.add_string(L"Режим редактора:", 18, x, y - 80);

    interface.add_string(L"Пункт:", 16, x, y);
    interface.add_string(L"Время посещения:", 16, x, y + 20);
    interface.add_string(L"Стоимость посещения:", 16, x, y + 40);
    interface.add_string(L"Оценка:", 16, x, y + 60);

    interface.add_string(L"Начальный пункт:", 16, x, y + 100);
    interface.add_string(L"Конечный пункт:", 16, x, y + 120);

    interface.add_string(L"Время пути:", 16, x, y + 200);

    interface.add_string(L"Важность времени:", 16, x, y + 280);
    interface.add_string(L"Важность стоимости:", 16, x, y + 300);
    interface.add_string(L"Важность оценки:", 16, x, y + 320);

    interface.add_string(L"Количество особей:", 16, x, y + 400);
    interface.add_string(L"Количество генераций:", 16, x, y + 420);

    interface.add_string(L"Параметры пункта", 24, x, y - 40);
    interface.add_string(L"Параметры пути", 24, x, y + 160);
    interface.add_string(L"Важность параметров", 24, x, y + 240);
    interface.add_string(L"Алгоритм", 24, x, y + 360);

    interface.add_pointer_string(&interface.point_id, 0, 16, x + 220, y);
    
    interface.add_input_field(&interface.selected_point.time, 1, 16, x + 220, y + 20, x + 300, y + 40);
    interface.add_input_field(&interface.selected_point.price, 1, 16, x + 220, y + 40, x + 300, y + 60);
    interface.add_input_field(&interface.selected_point.score, 1, 16, x + 220, y + 60, x + 300, y + 80);

    interface.add_input_field(&g.route_parametres.first_point, 0, 16, x + 220, y + 100, x + 300, y + 120);
    interface.add_input_field(&g.route_parametres.last_point, 0, 16, x + 220, y + 120, x + 300, y + 140);

    interface.add_input_field(&interface.selected_connection.time, 1, 16, x + 220, y + 200, x + 300, y + 220);

    interface.add_input_field(&g.user_ratios.time, 0, 16, x + 220, y + 280, x + 300, y + 300);
    interface.add_input_field(&g.user_ratios.price, 0, 16, x + 220, y + 300, x + 300, y + 320);
    interface.add_input_field(&g.user_ratios.score, 0, 16, x + 220, y + 320, x + 300, y + 340);

    interface.add_input_field(&g.amount, 0, 16, x + 220, y + 400, x + 300, y + 420);
    interface.add_input_field(&g.cycles_amount, 0, 16, x + 220, y + 420, x + 300, y + 440);
    
    interface.add_window_rectangle(0.5, x - 5, y - 45, x + 305, y + 145);
    interface.add_window_rectangle(0.5, x - 5, y + 150, x + 305, y + 225);
    interface.add_window_rectangle(0.5, x - 5, y + 235, x + 305, y + 345);
    interface.add_window_rectangle(0.5, x - 5, y + 355, x + 305, y + 445);

    interface.add_algorithm_button(L"Сгенерировать", 16, x - 5, y + 460, x + 170, y + 480);
    interface.add_mode_switch({ {x + 255, y - 80, x + 275, y - 60}, {x + 285, y - 80, x + 305, y - 60} });

    RectangleShape interface_bg(Vector2f(800.f, 800.f));
    interface_bg.move(1000.f, 0.f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            main_interface.input_callback(window, g, event);
            interface.input_callback(window, g, event);
            if (event.type == Event::Closed or event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
                window.close();
        }

        window.clear(Color::White);
        window.draw(interface_bg);
        window.draw(map_sprite);
        interface.draw(window);
        g.draw(window);
        window.display();

    }

}