#ifndef LIGHT_H
#define LIGHT_H
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

class Obstacle;

struct Intersect
{
    bool result;
    sf::Vector2f pos;
};

class Light
{
public:
    sf::CircleShape src;

    bool illumine = true;

public:
    Light();
    ~Light();
    void draw(sf::RenderWindow &window);

    Intersect getRayCollision(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &c, const sf::Vector2f &d);

    friend void rayCaster(Light &light, Obstacle &obstacle, sf::RenderWindow &window);
};

#endif