#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cmath>

using namespace std;

int min_time_sim(int x, int y, int num)
{
    int nx = 0, ny = 0;
    int count = 0;
    int n_t_x = 0, n_t_y = 0;
    num -= 1;
    while (0 < num)
    {
        count ++;
        if (n_t_x >= x)
        {
            n_t_x -= x;
            n_t_y += x;
            --num;
            nx++;
        }
        if (n_t_y >= y)
        {
            n_t_y -= y;
            n_t_x += y;
            --num;
            ny++;
        }
        if (n_t_x < x && n_t_y < y)
        {
            const int add = min(x - n_t_x, y - n_t_y);
            n_t_x += add;
            n_t_y += add;
        }
    }
    return count;
    // return max(x*nx. y*ny) + min(x, y);
}

int func_solve(int x, int y, int nx, int num)
{
    return y * (num - nx) + nx * x;
}

int min_time_bin(int x, int y, int num)
{
    int count = 0;
    num--;
    int nx = -1;
    int l = 0, r = num;
    while (l + 1 < r)
    {
        count ++;
        nx = (l + r) << 1;
        if (func_solve(x, y, nx, num) < func_solve(nx, y, nx + 1, num))
        {
            r = nx;
        }
        else
        {
            if (func_solve(x, y, nx, num) > func_solve(nx, y, nx + 1, num))
            {
                l = nx;
            }
        }
    }
    if (nx == -1)
    {
        return 0;
    }
    return count;
}

int main()
{
    int B[] = {8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16000, 32000, 64000, 128000, 256000, 512000};
    int size = sizeof(B) / sizeof(B[0]);
    std::cout << std::setw(8) << "num" << std::setw(11) << "Тэ1" << std::setw(12) << "Тэ2" << std::setw(12) << "Т1" <<
        std::setw(11) << "Т2" << std::setw(12) << "Тэ1/Т1" << std::setw(15) << "Тэ2/Т2" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    for (int i = 0; i < size; ++i)
    {
        int x = 50, y = 1;
        int n = B[i];
        int te2 = min_time_bin(x, y, n);
        int te1 = min_time_sim(x,y, n);
        double t1 = n;
        double t2 = log2(n);
        double tet1 = te1 / t1;
        double tet2 = te2 / t2;
        std::cout << std::setw(8) << n << std::setw(9) << te1 << std::setw(10) << te2 << std::setw(10) << t1 <<
            std::setw(10) << t2
            << std::setw(10) << std::fixed << std::setprecision(2) << tet1 << std::setw(10) << std::fixed <<
            std::setprecision(2) << tet2 << std::endl;
    }
    return 0;
}
