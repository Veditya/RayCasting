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
    std::vector<sf::Vector2f> endpoints; // for triangle fanning
    sf::Color mainColor = sf::Color(255, 255, 255, 255);
    sf::Color shadowColor = sf::Color(255, 255, 255, 100);
    sf::Color rayColor = sf::Color(0, 0, 0, 0);
    bool shadowfx = true;
    bool illumine = true;

public:
    Light();

    ~Light();

    void draw(sf::RenderWindow &window);

    void updateSrc(sf::RenderWindow &window);

    Intersect getRayCollision(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &c, const sf::Vector2f &d);

    sf::Vector2f getRayEndpoint(const sf::Vector2f &origin, float angle, const sf::RenderWindow &window);

    friend void rayCaster(Light &light, Obstacle &obstacle, sf::RenderWindow &window);
};

#endif