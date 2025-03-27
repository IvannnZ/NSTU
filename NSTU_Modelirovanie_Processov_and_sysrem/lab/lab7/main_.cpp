#include <iostream>
#include <cmath>
#include <random>

int main() {
    const double L = 10.0;
    const double p_abs = 0.3;
    const int N = 1000000;

    int transmitted = 0;
    int reflected = 0;
    int absorbed = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> uniform01(0.0, 1.0);
    std::uniform_real_distribution<double> uniformMinus1_1(-1.0, 1.0);

    for (int i = 0; i < N; ++i) {
        double x = 0.0;
        double mu = 1.0;
        bool alive = true;

        while (alive) {
            double r = uniform01(gen);
            double lambda = -std::log(r);
            double x_next = x + lambda * mu;

            if (x_next >= L) {
                transmitted++;
                alive = false;
            } else if (x_next < 0) {
                reflected++;
                alive = false;
            } else {
                double gamma = uniform01(gen);
                if (gamma < p_abs) {
                    absorbed++;
                    alive = false;
                } else {
                    x = x_next;
                    mu = uniformMinus1_1(gen);
                }
            }
        }
    }

    double prob_transmitted = static_cast<double>(transmitted) / N;
    double prob_reflected = static_cast<double>(reflected) / N;
    double prob_absorbed = static_cast<double>(absorbed) / N;

    std::cout << "Simulated " << N << " neutron trajectories:" << std::endl;
    std::cout << "Transmitted:  " << transmitted << " (" << prob_transmitted * 100 << "%)" << std::endl;
    std::cout << "Reflected:    " << reflected << " (" << prob_reflected * 100 << "%)" << std::endl;
    std::cout << "Absorbed:     " << absorbed << " (" << prob_absorbed * 100 << "%)" << std::endl;

    return 0;
}