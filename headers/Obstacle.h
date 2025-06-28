#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <SFML/Graphics.hpp>
#include <vector>

class Light;

class Obstacle
{
public:
    std::vector<std::vector<sf::Vector2f>> triangles;
    bool display = true;

public:
    Obstacle()
    {
        triangles = {
            {sf::Vector2f(300, 100), sf::Vector2f(100, 300), sf::Vector2f(400, 300)},
            {sf::Vector2f(600, 100), sf::Vector2f(800, 100), sf::Vector2f(900, 200)},
            {sf::Vector2f(1200, 200), sf::Vector2f(1000, 500), sf::Vector2f(1300, 600)},
            {sf::Vector2f(500, 500), sf::Vector2f(200, 600), sf::Vector2f(400, 700)},
            {sf::Vector2f(800, 600), sf::Vector2f(700, 700), sf::Vector2f(1000, 800)}};
    }
    ~Obstacle() {}

    void draw(sf::RenderWindow &window)
    {
        for (const auto &tri : triangles)
        {
            sf::VertexArray triangle(sf::Triangles, 3);
            triangle[0].position = tri[0];
            triangle[1].position = tri[1];
            triangle[2].position = tri[2];
            auto dark = sf::Color::Black;
            triangle[0].color = dark;
            triangle[1].color = dark;
            triangle[2].color = dark;
            window.draw(triangle);
        }
    }

    friend void rayCaster(Light &light, Obstacle &obstacle, sf::RenderWindow &window);
};

#endif
