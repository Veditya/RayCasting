#include "../headers/Window.h"
#include "../headers/Light.h"
#include "../headers/Obstacle.h"
#include <sstream>

Window::Window() : window(sf::VideoMode(1500, 900), "Shadow", sf::Style::Close | sf::Style::Resize)
{
    window.setFramerateLimit(60);
    if (!dejavu.loadFromFile("assets/DejaVuSans.ttf"))
    {
        // Make sure DejaVuSans.ttf is in your working directory
        throw std::runtime_error("Failed to load font");
    }
}
Window::~Window() {}

void Window::Start()
{
    // load all the objects here
    Light light;
    Obstacle obstacle;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
                gridView = !gridView;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
                light.illumine = !light.illumine;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O)
                obstacle.display = !obstacle.display;
        }

        window.clear(sf::Color::Black);

        // all drawing will happen here

        // grids
        if (gridView)
        {
            drawGrid(window, 100, dejavu);
        }
        // lighting
        if (light.illumine)
        {
            rayCaster(light, obstacle, window);
            light.draw(window);
        }
        // obstacle
        if (obstacle.display)
        {
            obstacle.draw(window);
        }

        window.display();
    }
}

void Window::drawGrid(sf::RenderWindow &window, int cellSize, sf::Font &font)
{
    int w = window.getSize().x;
    int h = window.getSize().y;

    // Draw vertical lines
    for (int x = 0; x <= w; x += cellSize)
    {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, 0), sf::Color(60, 60, 60)),
            sf::Vertex(sf::Vector2f(x, h), sf::Color(60, 60, 60))};
        window.draw(line, 2, sf::Lines);
    }
    // Draw horizontal lines
    for (int y = 0; y <= h; y += cellSize)
    {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, y), sf::Color(60, 60, 60)),
            sf::Vertex(sf::Vector2f(w, y), sf::Color(60, 60, 60))};
        window.draw(line, 2, sf::Lines);
    }
    // Draw coordinate points and labels
    for (int x = 0; x <= w; x += cellSize)
    {
        for (int y = 0; y <= h; y += cellSize)
        {
            sf::CircleShape point(4);
            point.setFillColor(sf::Color::Red);
            point.setOrigin(4, 4);
            point.setPosition(x, y);
            window.draw(point);

            std::ostringstream ss;
            ss << "(" << x << "," << y << ")";
            sf::Text text;
            text.setFont(font);
            text.setString(ss.str());
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::White);
            text.setPosition(x + 6, y + 2);
            window.draw(text);
        }
    }
}