#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

int main()
{
    const double L = 10.0;
    const double p_abs = 0.3;
    const int N = 1000000;

    int transmitted = 0;
    int reflected = 0;
    int absorbed = 0;

    std::srand(std::time(0));

    for (int i = 0; i < N; ++i)
    {
        double x = 0.0;
        double mu = 1.0;
        bool alive = true;

        while (alive)
        {
            double r = (double)std::rand() / RAND_MAX;
            double lambda = -std::log(r + 1e-10);
            double x_next = x + lambda * mu;

            if (x_next >= L)
            {
                transmitted++;
                alive = false;
            }
            else if (x_next < 0)
            {
                reflected++;
                alive = false;
            }
            else
            {
                double gamma = (double)std::rand() / RAND_MAX;
                if (gamma < p_abs)
                {
                    absorbed++;
                    alive = false;
                }
                else
                {
                    x = x_next;
                    mu = 2.0 * ((double)std::rand() / RAND_MAX) - 1.0; // [-1, 1]
                }
            }
        }
    }

    std::cout << "Simulated " << N << " neutron trajectories:\n";
    std::cout << "Transmitted:  " << transmitted << " (" << (100.0 * transmitted / N) << "%)\n";
    std::cout << "Reflected:    " << reflected << " (" << (100.0 * reflected / N) << "%)\n";
    std::cout << "Absorbed:     " << absorbed << " (" << (100.0 * absorbed / N) << "%)\n";

    return 0;
}
