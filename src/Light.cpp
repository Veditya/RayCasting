#include "../headers/Light.h"
#include "../headers/Obstacle.h"

Light::Light()
{
    src.setRadius(6);
    src.setFillColor(sf::Color::Green);
    src.setOrigin(6, 6);
    src.setPosition(700, 400);
}
Light::~Light()
{
}
void Light::draw(sf::RenderWindow &window)
{
    window.draw(src);
}

Intersect Light::getRayCollision(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &c, const sf::Vector2f &d)
{
    sf::Vector2f r = b - a; // ray vector
    sf::Vector2f s = d - c; // other line segment vector

    float rxs = r.x * s.y - r.y * s.x;
    if (rxs == 0)
    {
        return {false, sf::Vector2f(0, 0)}; // Lines are parallel
    }
    sf::Vector2f cma = c - a;
    float t = (cma.x * s.y - cma.y * s.x) / rxs;
    float u = (cma.x * r.y - cma.y * r.x) / rxs;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        // Lines intersect
        sf::Vector2f intersection = a + t * r;
        return {true, intersection};
    }
    else
    {
        return {false, sf::Vector2f(0, 0)}; // No intersection
    }
}

void rayCaster(Light &light, Obstacle &obstacle, sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float x = static_cast<float>(mousePos.x);
    float y = static_cast<float>(mousePos.y);
    light.src.setPosition(x, y);

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> allSides;
    for (const auto &tri : obstacle.triangles)
    {
        allSides.push_back({tri[0], tri[1]});
        allSides.push_back({tri[1], tri[2]});
        allSides.push_back({tri[2], tri[0]});
    }

    // caste rays on vertices of triangle obstacles
    for (const auto &tri : obstacle.triangles)
    {
        for (int vi = 0; vi < 3; ++vi)
        {
            sf::Vector2f destination = tri[vi];
            sf::Vector2f origin = light.src.getPosition();
            sf::Vector2f closestIntersection = destination;
            float minDist = std::numeric_limits<float>::max();
            bool found = false;

            for (const auto &side : allSides)
            {
                // Optionally skip the side if it contains the destination vertex
                if ((side.first == destination && side.second == origin) ||
                    (side.second == destination && side.first == origin))
                    continue;

                Intersect I = light.getRayCollision(origin, destination, side.first, side.second);
                if (I.result)
                {
                    float dist = std::hypot(I.pos.x - origin.x, I.pos.y - origin.y);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        closestIntersection = I.pos;
                        found = true;
                    }
                }
            }

            // Draw the ray to the closest intersection (or to the vertex if no intersection)
            sf::Vertex ray[] = {
                sf::Vertex(origin, sf::Color::Green),
                sf::Vertex(closestIntersection, sf::Color::Red)};
            sf::CircleShape endpointDot(8);
            endpointDot.setFillColor(sf::Color::Red);
            endpointDot.setOrigin(8, 8);
            endpointDot.setPosition(closestIntersection);
            window.draw(endpointDot);
            window.draw(ray, 2, sf::Lines);
        }
    }
}