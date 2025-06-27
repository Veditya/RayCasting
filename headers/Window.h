#ifndef WINDOW_H
#define WINDOW_H
#include <SFML/Graphics.hpp>
#include <sstream>

class Window
{
public:
    sf::RenderWindow window;
    sf::Font dejavu;
    bool gridView = false;

public:
    Window();
    ~Window();

    void Start();
    void drawGrid(sf::RenderWindow &window, int cellSize, sf::Font &font);
};

#endif