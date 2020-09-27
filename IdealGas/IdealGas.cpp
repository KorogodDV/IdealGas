#include "math.h"
#include <SFML/Graphics.hpp>

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
    int R;
    vector2f speed;
    int m;
    int red;
    int green;
    int blue;
};

void drawSphere(sf::RenderWindow* window, Sphere sphere, int lighting_detailing = 10)
{
    sf::CircleShape circle(sphere.R);
    for (int i = 0; i < lighting_detailing; i++)
    {
        circle.setRadius(sphere.R - sphere.R * i / lighting_detailing);
        circle.setPosition(sphere.pos.x - sphere.R + 1.4 * sphere.R * i / lighting_detailing, sphere.pos.y - sphere.R + 0.6 * sphere.R * i / lighting_detailing);
        circle.setFillColor(sf::Color(0.875 * sphere.red * i / lighting_detailing + 0.125 * sphere.red, 0.875 * sphere.green * i / lighting_detailing + 0.125 * sphere.green, 0.875 * sphere.blue * i / lighting_detailing + 0.125 * sphere.blue));
        (*window).draw(circle);
    }
}

void speedBoostForCatchingSpheres(Sphere* sphere)
{
    sphere->speed.x += speedboost * sphere->speed.x / abs(sphere->speed.x);
}

void moveSphere(Sphere* sphere, float DT)
{
    sphere->pos.x += sphere->speed.x * DT;
    sphere->pos.y += sphere->speed.y * DT;
}


void checkSphereColideWithWalls(Sphere* sphere)
{
    if (sphere->pos.x + sphere->speed.x * DT + sphere->R > window_length)
    {
        sphere->speed.x *= -1;
        sphere->pos.x = 2 * window_length - sphere->pos.x - 2 * sphere->R;
    }
    if (sphere->pos.x + sphere->speed.x * DT - sphere->R < 0)
    {
        sphere->speed.x *= -1;
        sphere->pos.x = -sphere->pos.x + 2 * sphere->R;
    }
    if (sphere->pos.y + sphere->speed.y * DT + sphere->R > window_width)
    {
        sphere->speed.y *= -1;
        sphere->pos.y = 2 * window_width - sphere->pos.y - 2 * sphere->R;
    }
    if (sphere->pos.y + sphere->speed.y * DT - sphere->R < 0)
    {
        sphere->speed.y *= -1;
        sphere->pos.y = -sphere->pos.y + 2 * sphere->R;
    }
}

bool checkCollisionTwoSpheres(const Sphere* sphere1, const Sphere* sphere2)
{
    return pow(sphere1->pos.x - sphere2->pos.x, 2) + pow(sphere1->pos.y - sphere2->pos.y, 2) < pow(sphere1->R + sphere2->R, 2);
}

void collideSpheres(Sphere* sphere1, Sphere* sphere2)
{
    float vx10 = sphere1->speed.x;
    float vy10 = sphere1->speed.y;
    float vx20 = sphere2->speed.x;
    float vy20 = sphere2->speed.y;

    sphere1->speed.x = (2 * sphere2->m * vx20 + (sphere1->m - sphere2->m) * vx10) / (sphere1->m + sphere2->m);
    sphere1->speed.y = (2 * sphere2->m * vy20 + (sphere1->m - sphere2->m) * vy10) / (sphere1->m + sphere2->m);
    sphere2->speed.x = (2 * sphere1->m * vx10 + (sphere2->m - sphere1->m) * vx20) / (sphere1->m + sphere2->m);
    sphere2->speed.y = (2 * sphere1->m * vy10 + (sphere2->m - sphere1->m) * vy20) / (sphere1->m + sphere2->m);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Chasing");

    int N = 100;
    int asd = 100;
    Sphere particles[100];
    for (int i = 0; i < 100; i++)
    {
        particles[i] = { float(50 * i % (window_length - 100) + 50), float(100 * (50 * i - 50 * i % (window_length - 100)) / (window_length - 100)), 12, float(0.3 * i), float(-0.1 * i), 1, 10 * i % 255, 15 * i % 255, 5 * i % 255 };
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
            for (int k = i; k < 100; k++)
            {
                if (checkCollisionTwoSpheres(&particles[i], &particles[k]))
                {
                    collideSpheres(&particles[i], &particles[k]);
                }
            }
        }

        for (int i = 0; i < 100; i++)
        {
            moveSphere(&particles[i], DT);
        }
    }
    return 0;
}