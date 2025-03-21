#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Attraction {
    string name;
    double time;
    int score;
};

vector<Attraction> optimalRouteDP(const vector<Attraction>& attractions, double maxDays = 2.0) {
    int n = attractions.size();
    int maxTime = static_cast<int>(maxDays * 10); // Преобразуем время в десятые доли для DP
    vector<vector<int>> dp(n + 1, vector<int>(maxTime + 1, 0));

    // Заполняем таблицу динамического программирования
    for (int i = 1; i <= n; ++i) {
        int time = static_cast<int>(attractions[i - 1].time * 10);
        int score = attractions[i - 1].score;
        for (int j = 0; j <= maxTime; ++j) {
            if (j >= time)
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - time] + score);
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    // Восстанавливаем маршрут
    vector<Attraction> bestPlan;
    int j = maxTime;
    for (int i = n; i > 0 && j > 0; --i) {
        if (dp[i][j] != dp[i - 1][j]) {
            bestPlan.push_back(attractions[i - 1]);
            j -= static_cast<int>(attractions[i - 1].time * 10);
        }
    }

    return bestPlan;
}

int main() {
    vector<Attraction> attractions = {
        {"Кремль", 0.5, 10},
        {"Большой театр", 0.2, 5},
        {"Третьяковская галерея", 1.0, 7},
        {"Зоопарк", 0.5, 9},
        {"ВДНХ", 0.7, 8},
        {"Останкинская башня", 0.5, 6},
        {"Парк Горького", 0.3, 7},
        {"Музей космонавтики", 0.4, 9},
        {"Арбат", 0.3, 6},
        {"Красная площадь", 0.2, 10}
    };

    vector<Attraction> bestPlan = optimalRouteDP(attractions);

    cout << "Лучший маршрут:" << endl;
    for (const auto& place : bestPlan) {
        cout << place.name << endl;
    }

    int totalScore = 0;
    for (const auto& place : bestPlan) {
        totalScore += place.score;
    }
    cout << "Общая удовлетворенность: " << totalScore << endl;

    return 0;
}
