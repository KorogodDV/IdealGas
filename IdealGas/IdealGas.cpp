#include "math.h"
#include <SFML/Graphics.hpp>
#include <cassert>

const float DT = 1.0;
const float speedboost = 0.001;
const int window_length = 1280;
const int window_width = 720;

struct vector2f
{
    float x;
    float y;
};

struct Sphere
{
    vector2f pos;
    int r;
    vector2f speed;
    int m;
    int red;
    int green;
    int blue;
};

void drawSphere(sf::RenderWindow* window, Sphere sphere, int lighting_detailing = 10)
{
    sf::CircleShape circle(sphere.r, 10);
    assert(sphere.pos.x > sphere.r & sphere.pos.x + sphere.r < window_length & sphere.pos.y > sphere.r & sphere.pos.y + sphere.r < window_width);
    for (int i = 0; i < lighting_detailing; i++)
    {
        circle.setRadius(sphere.r - sphere.r * i / lighting_detailing);
        circle.setPosition(sphere.pos.x - sphere.r + 1.4 * sphere.r * i / lighting_detailing, sphere.pos.y - sphere.r + 0.6 * sphere.r * i / lighting_detailing);
        circle.setFillColor(sf::Color(0.875 * sphere.red * i / lighting_detailing + 0.125 * sphere.red, 0.875 * sphere.green * i / lighting_detailing + 0.125 * sphere.green, 0.875 * sphere.blue * i / lighting_detailing + 0.125 * sphere.blue));
        window->draw(circle);
    }
}

void moveSphere(Sphere* sphere)
{
    sphere->pos.x += sphere->speed.x * DT;
    sphere->pos.y += sphere->speed.y * DT;
}


void checkSphereColideWithWalls(Sphere* sphere)
{
    if (sphere->pos.x + sphere->speed.x * DT + sphere->r > window_length)
    {
        sphere->speed.x *= -1;
        sphere->pos.x = 2 * window_length - sphere->pos.x - 2 * sphere->r;
    }
    if (sphere->pos.x + sphere->speed.x * DT - sphere->r < 0)
    {
        sphere->speed.x *= -1;
        sphere->pos.x = -sphere->pos.x + 2 * sphere->r;
    }
    if (sphere->pos.y + sphere->speed.y * DT + sphere->r > window_width)
    {
        sphere->speed.y *= -1;
        sphere->pos.y = 2 * window_width - sphere->pos.y - 2 * sphere->r;
    }
    if (sphere->pos.y + sphere->speed.y * DT - sphere->r < 0)
    {
        sphere->speed.y *= -1;
        sphere->pos.y = -sphere->pos.y + 2 * sphere->r;
    }
}

bool checkCollisionTwoSpheres(const Sphere* sphere1, const Sphere* sphere2)
{
    return pow(sphere1->pos.x - sphere2->pos.x, 2) + pow(sphere1->pos.y - sphere2->pos.y, 2) < pow(sphere1->r + sphere2->r, 2);
}

void collideSpheres(Sphere* sphere1, Sphere* sphere2)
{
    float vx10 = sphere1->speed.x;
    float vy10 = sphere1->speed.y;
    float vx20 = sphere2->speed.x;
    float vy20 = sphere2->speed.y;
    float dist = sqrt(pow(sphere2->pos.x - sphere1->pos.x, 2) + pow(sphere2->pos.y - sphere1->pos.y, 2));
    assert(dist);
    
    sphere1->pos.x -= (sphere1->r + sphere2->r - dist) * (sphere2->pos.x - sphere1->pos.x) / (2 * dist);
    sphere1->pos.y -= (sphere1->r + sphere2->r - dist) * (sphere2->pos.y - sphere1->pos.y) / (2 * dist);
    sphere2->pos.x += (sphere1->r + sphere2->r - dist) * (sphere2->pos.x - sphere1->pos.x) / (2 * dist);
    sphere2->pos.y += (sphere1->r + sphere2->r - dist) * (sphere2->pos.y - sphere1->pos.y) / (2 * dist); 

    sphere1->speed.x = (2 * sphere2->m * vx20 + (sphere1->m - sphere2->m) * vx10) / (sphere1->m + sphere2->m);
    sphere1->speed.y = (2 * sphere2->m * vy20 + (sphere1->m - sphere2->m) * vy10) / (sphere1->m + sphere2->m);
    sphere2->speed.x = (2 * sphere1->m * vx10 + (sphere2->m - sphere1->m) * vx20) / (sphere1->m + sphere2->m);
    sphere2->speed.y = (2 * sphere1->m * vy10 + (sphere2->m - sphere1->m) * vy20) / (sphere1->m + sphere2->m);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Chasing");

    Sphere particles[100];
    for (int i = 0; i < 100; i++)
    {
        particles[i] = { float(30 * i % (window_length - 100) + 50), float(100 * (30 * i - 30 * i % (window_length - 100)) / (window_length - 100) + 50), 12, float(0.1 * i), float(-0.1 * i), 1, 10 * i % 255, 15 * i % 255, 5 * i % 255};
    }

    while (window.isOpen())
    {
        sf::Event event;
        window.clear();
        for (int i = 0; i < 100; i++)
        {
            drawSphere(&window, particles[i]);
        }
        window.display();

        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
            {
                window.close();
                break;
            }
        }

        for (int i = 0; i < 100; i++)
        {
            checkSphereColideWithWalls(&particles[i]);
        }

        for (int i = 0; i < 100; i++)
        {
            for (int j = i + 1; j < 100; j++)
            {
                if (checkCollisionTwoSpheres(&particles[i], &particles[j]))
                {
                    collideSpheres(&particles[i], &particles[j]);
                }
            }
        }

        for (int i = 0; i < 100; i++)
        {
            moveSphere(&particles[i]);
        }
    }
    return 0;
}
