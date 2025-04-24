// lissajous.cpp
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <iostream>

constexpr float PI = 3.1415926535f;

class LissajousThread {
public:
    LissajousThread(float A, float W, float D)
        : A(A), W(W), D(D), t(0), running(true), value(0) {}


    void start() {
        thread = std::thread(&LissajousThread::run, this);
    }

    void stop() {
        running = false;
        if (thread.joinable()) thread.join();
    }

    void updateParams(float newA, float newW, float newD) {
        std::lock_guard<std::mutex> lock(mutex);
        A = newA; W = newW; D = newD;
    }

    float getValue() {
        std::lock_guard<std::mutex> lock(mutex);
        return value;
    }

private:
    std::thread thread;
    std::mutex mutex;
    float A, W, D;
    float t;
    float value;
    bool running;

    void run() {
        while (running) {
            {
                std::lock_guard<std::mutex> lock(mutex);
                value = A * std::sin(W * t + D);
                t += 0.01f;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};

class Button {
public:
    Button(float x, float y, float width, float height, const std::string& label) {
        rect.setSize({width, height});
        rect.setPosition(x, y);
        rect.setFillColor(sf::Color(200, 200, 200));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2);

        font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x + 5, y + 5);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(rect);
        window.draw(text);
    }

    bool isClicked(const sf::Vector2f& mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }

private:
    sf::RectangleShape rect;
    sf::Text text;
    sf::Font font;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lissajous Figures");

    // Создаем потоки
    LissajousThread xThread(150, 3, 0);
    LissajousThread yThread(150, 2, PI / 2);

    xThread.start();
    yThread.start();

    Button startButton(10, 10, 100, 30, "Start");
    Button stopButton(10, 50, 100, 30, "Stop");

    bool running = true;
    std::vector<sf::Vertex> points;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (startButton.isClicked(mousePos)) {
                    xThread.updateParams(150, 3, 0);
                    yThread.updateParams(150, 2, PI / 2);
                    running = true;
                } else if (stopButton.isClicked(mousePos)) {
                    running = false;
                }
            }
        }

        if (running) {
            float x = xThread.getValue();
            float y = yThread.getValue();
            sf::Vector2f point(400 + x, 300 + y);
            points.emplace_back(point, sf::Color::Red);
        }

        window.clear(sf::Color::White);
        for (auto& p : points)
            window.draw(&p, 1, sf::Points);

        startButton.draw(window);
        stopButton.draw(window);
        window.display();
    }

    xThread.stop();
    yThread.stop();

    return 0;
}
