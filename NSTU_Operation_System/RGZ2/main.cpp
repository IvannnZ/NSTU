#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

const int NUM_LIFTS = 4;
const int NUM_FLOORS = 9;

struct Lift {
    int id;
    int currentFloor = 1;
    int targetFloor = 1;
    std::string direction = "STOP";
    sf::Clock clock;
    float moveInterval = 4.f; // seconds between floors
};

sf::Font loadFont() {
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Ошибка загрузки шрифта.\n";
        exit(1);
    }
    return font;
}

sf::RectangleShape makeButton(float x, float y, float w, float h, sf::Color color) {
    sf::RectangleShape button(sf::Vector2f(w, h));
    button.setPosition(x, y);
    button.setFillColor(color);
    return button;
}

bool isClicked(const sf::RectangleShape& rect, sf::Vector2f mouse) {
    return rect.getGlobalBounds().contains(mouse);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Управление лифтами");
    sf::Font font = loadFont();

    std::vector<Lift> lifts(NUM_LIFTS);
    for (int i = 0; i < NUM_LIFTS; ++i) {
        lifts[i].id = i;
    }

    // Кнопки вызова: [лифты][этажи]
    std::vector<std::vector<sf::RectangleShape>> callButtons(NUM_LIFTS);
    std::vector<std::vector<sf::Text>> buttonLabels(NUM_LIFTS);

    for (int i = 0; i < NUM_LIFTS; ++i) {
        for (int f = 0; f < NUM_FLOORS; ++f) {
            float x = 190.f + i * 150;
            float y = 140.f + f * 40;

            auto button = makeButton(x, y, 40, 30, sf::Color(100, 150, 200));
            callButtons[i].push_back(button);

            sf::Text label;
            label.setFont(font);
            label.setCharacterSize(14);
            label.setFillColor(sf::Color::White);
            label.setString(std::to_string(f + 1));
            label.setPosition(x + 12, y + 4);
            buttonLabels[i].push_back(label);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < NUM_LIFTS; ++i) {
                    for (int f = 0; f < NUM_FLOORS; ++f) {
                        if (isClicked(callButtons[i][f], mousePos)) {
                            lifts[i].targetFloor = f + 1;
                            if (lifts[i].targetFloor > lifts[i].currentFloor)
                                lifts[i].direction = "UP";
                            else if (lifts[i].targetFloor < lifts[i].currentFloor)
                                lifts[i].direction = "DOWN";
                            else
                                lifts[i].direction = "STOP";
                            lifts[i].clock.restart();
                        }
                    }
                }
            }
        }

        // Обновление движения лифтов
        for (auto& lift : lifts) {
            if (lift.direction != "STOP" &&
                lift.clock.getElapsedTime().asSeconds() >= lift.moveInterval) {

                if (lift.direction == "UP") lift.currentFloor++;
                else if (lift.direction == "DOWN") lift.currentFloor--;

                if (lift.currentFloor == lift.targetFloor) {
                    lift.direction = "STOP";
                }

                lift.clock.restart();
            }
        }

        window.clear(sf::Color(30, 30, 40));

        for (int i = 0; i < NUM_LIFTS; ++i) {
            // Шахта
            float x = 150 + i * 150;
            float yBase = 500;
            float floorH = 40;

            sf::RectangleShape shaft(sf::Vector2f(40, -floorH * NUM_FLOORS));
            shaft.setPosition(x, yBase);
            shaft.setFillColor(sf::Color(80, 80, 100));
            window.draw(shaft);

            // Кабина
            float cabY = yBase - floorH * lifts[i].currentFloor;
            sf::RectangleShape cab(sf::Vector2f(40, 30));
            cab.setPosition(x, cabY);
            cab.setFillColor(sf::Color::Yellow);
            window.draw(cab);

            // Инфо
            sf::Text label;
            label.setFont(font);
            label.setCharacterSize(14);
            label.setFillColor(sf::Color::White);
            label.setString("Лифт " + std::to_string(i) +
                            "\nЭтаж: " + std::to_string(lifts[i].currentFloor) +
                            "\nЦель: " + std::to_string(lifts[i].targetFloor) +
                            "\n" + lifts[i].direction);
            label.setPosition(x - 20, yBase + 10);
            window.draw(label);

            // Кнопки
            for (int f = 0; f < NUM_FLOORS; ++f) {
                window.draw(callButtons[i][f]);
                window.draw(buttonLabels[i][f]);
            }
        }

        window.display();
        usleep(10000);
    }

    return 0;
}
