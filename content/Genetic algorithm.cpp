
#include "Functions.h"
#include "Algorithm.h"
#include "Window.h"

unsigned int wtime = clock(); //время начала выполнения программы

int main() {

    srand(clock()); //начальное значение rand()
    system("color 08"); //цвет консоли
    setlocale(LC_ALL, ""); //использование русского языка

    Population g;
    EventCallback ec;
    Interface interface;
    interface.change_font("C:\\Users\\user\\source\\repos\\Genetic algorithm\\FiraCode-Regular.ttf");

    
    ContextSettings s;
    s.antialiasingLevel = 8; //параметры окна
    RenderWindow window(VideoMode(1320, 598), "Genetic Algorithm", Style::Default, s); //создание окна
    window.setFramerateLimit(60);

    interface.add_string(L"Пункт:", 16, 1000, 70);
    interface.add_string(L"Время посещения:", 16, 1000, 90);
    interface.add_string(L"Стоимость посещения:", 16, 1000, 110);
    interface.add_string(L"Оценка:", 16, 1000, 130);

    interface.add_string(L"Начальный пункт:", 16, 1000, 170);
    interface.add_string(L"Конечный пункт:", 16, 1000, 190);

    interface.add_string(L"Время пути:", 16, 1000, 270);

    interface.add_string(L"Важность времени:", 16, 1000, 350);
    interface.add_string(L"Важность стоимости:", 16, 1000, 370);
    interface.add_string(L"Важность оценки:", 16, 1000, 390);

    interface.add_string(L"Количество особей:", 16, 1000, 470);
    interface.add_string(L"Количество генераций:", 16, 1000, 490);

    interface.add_string(L"Параметры пункта", 24, 1000, 30);
    interface.add_string(L"Параметры пути", 24, 1000, 230);
    interface.add_string(L"Важность параметров", 24, 1000, 310);
    interface.add_string(L"Алгоритм", 24, 1000, 430);

    interface.add_string_from_pointer(ec.selected_p1_id, 16, 1220, 60);

    interface.add_string_from_pointer(ec.selected_point.time, 16, 1220, 80);
    interface.add_string_from_pointer(ec.selected_point.price, 16, 1220, 100);
    interface.add_string_from_pointer(ec.selected_point.score, 16, 1220, 120);

    interface.add_string_from_pointer(g.route_parametres.first_point, 16, 1220, 170);
    interface.add_string_from_pointer(g.route_parametres.last_point, 16, 1220, 190);

    interface.add_string_from_pointer(g.user_ratios.time, 16, 1220, 350);
    interface.add_string_from_pointer(g.user_ratios.price, 16, 1220, 370);
    interface.add_string_from_pointer(g.user_ratios.score, 16, 1220, 390);

    interface.add_string_from_pointer(g.amount, 16, 1220, 470);

    interface.add_window_rectangle(0.5f, 995.f, 25.f, 1305.f, 215.f);
    interface.add_window_rectangle(0.5f, 995.f, 225.f, 1305.f, 295.f);
    interface.add_window_rectangle(0.5f, 995.f, 305.f, 1305.f, 415.f);
    interface.add_window_rectangle(0.5f, 995.f, 425.f, 1305.f, 515.f);

    //interface.add_button(L"Где", 16, 1000.f, 520.f, 1100.f, 540.f, func);
    interface.add_mode_switch({ {1240.f, -10.f, 1260.f, 10.f}, {1270.f, -10.f, 1290.f, 10.f} });

    Texture map;
    map.loadFromFile("C:\\Users\\user\\source\\repos\\Genetic algorithm\\map.png");
    Sprite map_sprite(map);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            ec.input_callback(g, event, interface);
            if (event.type == Event::Closed or event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
                window.close();
        }

        window.clear(Color(255, 255, 255));
        window.draw(map_sprite);
        g.draw(window);
        interface.draw(window);
        window.display();

    }

}