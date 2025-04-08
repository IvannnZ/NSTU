#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <cmath>
#include <chrono>
#include <iomanip>

// Константы
constexpr double PI = 3.141592653589793;
constexpr int HOURS_IN_DAY = 24;
constexpr int SIMULATION_SPEED = 3600; // 1 сек = 1 час

// Мощности генераторов (МВт)
constexpr double GG1_POWER = 100.0;
constexpr double GG2_POWER = 100.0;
constexpr double GG3_POWER = 120.0;
constexpr double GG4_POWER = 120.0;

// Глобальные данные
std::mutex data_mutex;
double p1_power = 0.0, p2_power = 0.0, p3_power = 0.0;
bool gg4_enabled = false;
std::atomic<bool> stop_simulation(false);

// Функция вычисления мощности потребителя
double calculate_power(double base, double amplitude, int hour) {
    return base + amplitude * std::sin(PI / 2 * (hour / 6.0 - 1));
}

// Поток для потребителя
void consumer_thread(int id, double base_power, double amplitude) {
    while (!stop_simulation) {
        auto now = std::chrono::system_clock::now();
        auto since_epoch = now.time_since_epoch();
        int current_hour = (std::chrono::duration_cast<std::chrono::seconds>(since_epoch).count() / SIMULATION_SPEED) % HOURS_IN_DAY;

        double power = calculate_power(base_power, amplitude, current_hour);

        {
            std::lock_guard<std::mutex> lock(data_mutex);
            if (id == 1) p1_power = power;
            else if (id == 2) p2_power = power;
            else if (id == 3) p3_power = power;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Поток управления генераторами
void generator_controller_thread() {
    while (!stop_simulation) {
        double total_power_needed, total_power_available;

        {
            std::lock_guard<std::mutex> lock(data_mutex);
            total_power_needed = p1_power + p2_power + p3_power;
            total_power_available = GG1_POWER + GG2_POWER + GG3_POWER + (gg4_enabled ? GG4_POWER : 0.0);
        }

        // Включаем/выключаем ГГ4
        bool need_gg4 = (total_power_needed > (GG1_POWER + GG2_POWER + GG3_POWER));

        {
            std::lock_guard<std::mutex> lock(data_mutex);
            gg4_enabled = need_gg4;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Функция вывода статистики
void print_stats() {
    while (!stop_simulation) {
        std::system("clear"); // Очистка консоли (для Linux)

        double p1, p2, p3, total_power;
        bool gg4_on;

        {
            std::lock_guard<std::mutex> lock(data_mutex);
            p1 = p1_power;
            p2 = p2_power;
            p3 = p3_power;
            gg4_on = gg4_enabled;
            total_power = p1 + p2 + p3;
        }

        auto now = std::chrono::system_clock::now();
        auto since_epoch = now.time_since_epoch();
        int current_hour = (std::chrono::duration_cast<std::chrono::seconds>(since_epoch).count() / SIMULATION_SPEED % HOURS_IN_DAY);

        std::cout << "⏰ Текущий час: " << current_hour << "\n\n";
        std::cout << "⚡ Потребляемая мощность:\n";
        std::cout << "  P1: " << std::fixed << std::setprecision(2) << p1 << " МВт\n";
        std::cout << "  P2: " << p2 << " МВт\n";
        std::cout << "  P3: " << p3 << " МВт\n";
        std::cout << "  Общая: " << total_power << " МВт\n\n";

        std::cout << "🔧 Генераторы:\n";
        std::cout << "  ГГ1: " << GG1_POWER << " МВт (вкл)\n";
        std::cout << "  ГГ2: " << GG2_POWER << " МВт (вкл)\n";
        std::cout << "  ГГ3: " << GG3_POWER << " МВт (вкл)\n";
        std::cout << "  ГГ4: " << GG4_POWER << " МВт (" << (gg4_on ? "вкл" : "выкл") << ")\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::cout << "🚀 Запуск моделирования ГЭС...\n";

    // Запуск потоков
    std::thread consumer1(consumer_thread, 1, 100.0, 20.0);
    std::thread consumer2(consumer_thread, 2, 180.0, 50.0);
    std::thread consumer3(consumer_thread, 3, 90.0, 20.0);
    std::thread controller(generator_controller_thread);
    std::thread stats(print_stats);

    // Ожидание Ctrl+C для остановки
    std::cin.get();
    stop_simulation = true;

    // Завершение потоков
    consumer1.join();
    consumer2.join();
    consumer3.join();
    controller.join();
    stats.join();

    std::cout << "\n🛑 Моделирование завершено.\n";
    return 0;
}