#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>


unsigned width = 1000;
unsigned height = 1000;
float radius = 2.f;
float m = 1000.f;
float L = 400.f;
float l = 100.f;
float r = 0.f, F;
float elasticX = 1.f;
float elasticY = 1.f;
sf::Vector2f v = sf::Vector2f(0.02f, 0.f);
sf::Vector2f Ov = sf::Vector2f(0.f, 0.f);
float Oradius = radius;
float Maxspeed = 1.f;
int main()
{
    sf::RenderWindow window(sf::VideoMode({width, height}), "lab 1");
    sf::CircleShape ball(radius * 1.2);
    ball.setFillColor(sf::Color::White);

    sf::CircleShape backball(radius);
    backball.setFillColor(sf::Color::Blue);

    sf::CircleShape O(Oradius);
    O.setFillColor(sf::Color::Green);
    O.move({static_cast<float>(l), static_cast<float>(height)});
    ball.move({O.getPosition().x - l, O.getPosition().y - L});
    backball.move({O.getPosition().x - l, O.getPosition().y - L});
    
    //sf::View view1(sf::FloatRect({200.f, 200.f}, {300.f, 200.f}));
    //window.setView(view1);
    auto start = std::chrono::steady_clock::now();
    while (window.isOpen())
    {
        for(int i = 0; i < 2; i++) {
            while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        auto position = ball.getPosition();
        auto Opos = O.getPosition();
        auto backposs = backball.getPosition();
        /*
        if (position.x + 2 * radius >= width || position.x + radius <= 0) {
            if (position.x + 2 * radius >= width) ball.move({std::min(static_cast<float>(width) - position.x, 0.f),0.f});
            if (position.x + radius <= 0) ball.move({-1*std::min(position.x, 0.f), 0.f});
            v.x = -v.x * elasticX;
        }
        if (position.y + 2 * radius >= height || position.y + radius <= 0) {
            if (position.y + 2 * radius >= height) ball.move({0.f, std::min(static_cast<float>(width) - position.x, 0.f)});
            if (position.y + radius <= 0) ball.move({0.f, -1*std::min(position.y, 0.f)});
            v.y = -v.y * elasticY;
        }

        if (Opos.x + 2 * Oradius >= width || Opos.x <= 0) {
            if (Opos.x + 2 * radius >= width) ball.move({std::min(static_cast<float>(width) - Opos.x, 0.f),0.f});
            if (Opos.x + radius <= 0) ball.move({-1*std::min(Opos.x, 0.f), 0.f});
            Ov.x = -Ov.x;
        }
        if (Opos.y + 2 * Oradius >= height || Opos.y <= 0) {
            if (Opos.y + 2 * radius >= height) ball.move({0.f, std::min(static_cast<float>(width) - Opos.x, 0.f)});
            if (Opos.y + radius <= 0) ball.move({0.f, -1*std::min(Opos.y, 0.f)});
            Ov.y = -Ov.y;
        }*/
        //==========================================================


        //==========================================================




        if(position.y  < height) {
            r = std::sqrt(pow((position.x - O.getPosition().x), 2) + pow((position.y - O.getPosition().y), 2));
            F = m / pow(r, 2);

            double errHeight = (static_cast<int>(std::rand()) % 21 - 10) * 0.01f;
            //if(position.x + 2 * radius <= width + 1 && position.y + 2 * radius <= height + 1) {
            //v.x += F * (O.getPosition().x - position.x) / pow(r, 2);
            v.y += F * (O.getPosition().y - position.y) / pow(r, 2);

            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

            std::cout << "Time: " <<  static_cast<float>(elapsed_ms) / 100 << " | x: " << position.x << " | y: " << height - position.y << " | " << height - backposs.y << std::endl;

            v.x = std::min(std::max(-Maxspeed, v.x), std::min(v.x, Maxspeed));  v.y = std::min(std::max(-Maxspeed, v.y), std::min(v.y, Maxspeed));
            Ov.x = std::min(std::max(-Maxspeed, Ov.x), std::min(Ov.x, Maxspeed));  Ov.y = std::min(std::max(-Maxspeed, Ov.y), std::min(Ov.y, Maxspeed));


            ball.move(v);
            backball.move({v.x, v.y + errHeight});
            O.move(Ov);
            //window.clear(sf::Color::Black);
            window.draw(backball);
            window.draw(ball);
            window.draw(backball);
            //window.draw(O);
            window.display();
        }
        }




    }
}
