#include "../headers/Light.h"
#include "../headers/Obstacle.h"

Light::Light()
{
    src.setRadius(6);
    src.setFillColor(mainColor);
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
    auto transparent = light.rayColor; // transparent color for rays

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float x = static_cast<float>(mousePos.x);
    float y = static_cast<float>(mousePos.y);
    light.src.setPosition(x, y);

    std::vector<sf::Vector2f> endpoints; // points stored here will be later used for triangle fanning

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> allSides;
    for (const auto &tri : obstacle.triangles)
    {
        allSides.push_back({tri[0], tri[1]}); // v01
        allSides.push_back({tri[1], tri[2]}); // v12
        allSides.push_back({tri[2], tri[0]}); // v20
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
                sf::Vertex(origin, transparent),
                sf::Vertex(closestIntersection, transparent)}; // save closestIntersection point coords
            sf::CircleShape endpointDot(8);

            window.draw(ray, 2, sf::Lines);

            endpoints.push_back(closestIntersection);
            // === this block is for extra shadow rays for shadow effect ===
            if (light.shadowfx)
            {
                float angle = std::atan2(destination.y - origin.y, destination.x - origin.x);
                float offsets[2] = {-0.05f, 0.05f}; // Small angle offsets (radians)

                for (float offset : offsets)
                {
                    float newAngle = angle + offset;
                    sf::Vector2f dir(std::cos(newAngle), std::sin(newAngle));
                    sf::Vector2f farPoint = origin + dir * 2000.0f; // Large distance

                    sf::Vector2f closestExtraIntersection = farPoint;
                    float minExtraDist = std::numeric_limits<float>::max();
                    bool extraFound = false;

                    for (const auto &side : allSides)
                    {
                        Intersect I = light.getRayCollision(origin, farPoint, side.first, side.second);
                        if (I.result)
                        {
                            float dist = std::hypot(I.pos.x - origin.x, I.pos.y - origin.y);
                            if (dist < minExtraDist)
                            {
                                minExtraDist = dist;
                                closestExtraIntersection = I.pos;
                                extraFound = true;
                            }
                        }
                    }

                    // If no intersection with obstacles, extend to window border
                    if (!extraFound)
                    {
                        closestExtraIntersection = light.getRayEndpoint(origin, newAngle, window);
                    }

                    // Draw the extra ray and endpoint
                    sf::Vertex extraRay[] = {
                        sf::Vertex(origin, transparent),
                        sf::Vertex(closestExtraIntersection, transparent)};
                    sf::CircleShape extraDot(6);
                    window.draw(extraRay, 2, sf::Lines);
                    endpoints.push_back(closestExtraIntersection); // save the closestExtraIntersection coords
                }
            }
            // === End of extra rays block ===
        }
    }

    // Sort endpoints by angle around the light source
    sf::Vector2f origin = light.src.getPosition();
    std::sort(endpoints.begin(), endpoints.end(), [origin](const sf::Vector2f &a, const sf::Vector2f &b)
              {
        float angleA = std::atan2(a.y - origin.y, a.x - origin.x);
        float angleB = std::atan2(b.y - origin.y, b.x - origin.x);
        return angleA < angleB; });

    // Draw triangles between the light and each pair of points
    for (size_t i = 0; i < endpoints.size(); ++i)
    {
        sf::VertexArray triangle(sf::Triangles, 3);
        triangle[0].position = origin;
        triangle[1].position = endpoints[i];
        triangle[2].position = endpoints[(i + 1) % endpoints.size()];
        triangle[0].color = sf::Color(255, 255, 255, 200); // semi-transparent yellow
        triangle[1].color = sf::Color(255, 255, 255, 200);
        triangle[2].color = sf::Color(255, 255, 255, 200);
        window.draw(triangle);
    }
}

sf::Vector2f Light::getRayEndpoint(const sf::Vector2f &origin, float angle, const sf::RenderWindow &window)
{
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);

    float dx = std::cos(angle);
    float dy = std::sin(angle);

    float tMin = std::numeric_limits<float>::max();

    // Left (x = 0)
    if (dx != 0)
    {
        float t = (0 - origin.x) / dx;
        float y = origin.y + t * dy;
        if (t > 0 && y >= 0 && y <= h)
            tMin = std::min(tMin, t);
    }
    // Right (x = w)
    if (dx != 0)
    {
        float t = (w - origin.x) / dx;
        float y = origin.y + t * dy;
        if (t > 0 && y >= 0 && y <= h)
            tMin = std::min(tMin, t);
    }
    // Top (y = 0)
    if (dy != 0)
    {
        float t = (0 - origin.y) / dy;
        float x = origin.x + t * dx;
        if (t > 0 && x >= 0 && x <= w)
            tMin = std::min(tMin, t);
    }
    // Bottom (y = h)
    if (dy != 0)
    {
        float t = (h - origin.y) / dy;
        float x = origin.x + t * dx;
        if (t > 0 && x >= 0 && x <= w)
            tMin = std::min(tMin, t);
    }

    if (tMin == std::numeric_limits<float>::max())
        return origin; // No intersection found

    return origin + sf::Vector2f(dx * tMin, dy * tMin);
}