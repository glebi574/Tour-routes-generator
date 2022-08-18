
#include "Functions.h"
#include "Algorithm.h"
#include "Window.h"

unsigned int wtime = clock(); //время начала выполнения программы

int main() {

    srand(clock()); //начальное значение rand()
    system("color 08"); //цвет консоли
    setlocale(LC_ALL, ""); //использование русского языка

    Population g(40, 10);
    g.set_user_ratios(20, 20, 20);
    g.print_map();
    for (int i = 0; i < 2; ++i)
        g.cycle();

    ContextSettings s;
    s.antialiasingLevel = 8;

    RenderWindow window(VideoMode(988, 598), "Genetic Algorithm", Style::Default, s);

    window.setFramerateLimit(60);
    /*
    Font font;
    font.loadFromFile("C:\\Users\\user\\source\\repos\\Genetic algorithm\\FiraCode-Regular.ttf");
    std::vector<Text> strings = {
        Text(L"Пункт:", font, 16)
    };
    for (auto& i : strings) {
        i.setFillColor(Color(0, 0, 0));
    }
    strings[0].move(20, 20);*/
    //strings[1].move(20, 120);

    //create_graph(g);

    Texture map;
    map.loadFromFile("C:\\Users\\user\\source\\repos\\Genetic algorithm\\map.png");
    Sprite map_sprite(map);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
                add_object(event.mouseButton.x - 12.0f, event.mouseButton.y - 8.0f);
            if (event.type == Event::Closed or event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
                window.close();
        }

        window.clear(Color(110, 110, 110, 0));
        window.draw(map_sprite);
        draw_graph(window);
        window.display();

    }

}