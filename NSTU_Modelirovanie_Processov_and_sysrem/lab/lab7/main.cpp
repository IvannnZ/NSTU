#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

int main() {
    const double L = 10.0;       // Толщина пластины
    const double p_abs = 0.3;    // Вероятность поглощения
    const int N = 1000000;       // Количество моделируемых нейтронов

    int transmitted = 0;         // Счётчик прошедших нейтронов
    int reflected = 0;           // Счётчик отражённых нейтронов
    int absorbed = 0;            // Счётчик поглощённых нейтронов

    // Инициализация генератора случайных чисел
    std::srand(std::time(0));

    for (int i = 0; i < N; ++i) {
        double x = 0.0;          // Начальная позиция (x=0)
        double mu = 1.0;         // Начальное направление (вперёд)
        bool alive = true;       // Флаг "нейтрон ещё в игре"

        while (alive) {
            // Разыгрываем длину свободного пробега (экспоненциальное распределение)
            double r = (double)std::rand() / RAND_MAX;  // [0, 1)
            double lambda = -std::log(r + 1e-10);       // +1e-10 чтобы избежать log(0)
            double x_next = x + lambda * mu;            // Новая позиция

            if (x_next >= L) {         // Нейтрон прошёл сквозь пластину
                transmitted++;
                alive = false;
            } else if (x_next < 0) {  // Нейтрон отразился
                reflected++;
                alive = false;
            } else {                   // Нейтрон остался внутри пластины
                // Проверяем, поглотился ли он
                double gamma = (double)std::rand() / RAND_MAX;
                if (gamma < p_abs) {
                    absorbed++;
                    alive = false;
                } else {
                    // Иначе рассеяние — меняем направление
                    x = x_next;
                    mu = 2.0 * ((double)std::rand() / RAND_MAX) - 1.0;  // [-1, 1]
                }
            }
        }
    }

    // Вывод результатов
    std::cout << "Simulated " << N << " neutron trajectories:\n";
    std::cout << "Transmitted:  " << transmitted << " (" << (100.0 * transmitted / N) << "%)\n";
    std::cout << "Reflected:    " << reflected << " (" << (100.0 * reflected / N) << "%)\n";
    std::cout << "Absorbed:     " << absorbed << " (" << (100.0 * absorbed / N) << "%)\n";

    return 0;
}