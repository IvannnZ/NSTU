// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <chrono>
// #include <iomanip>
//
// using namespace std;
//
// // Первый способ - бинарный поиск
// int min_time_binary(int x, int y, int num)
// {
//     if (num == 1) return min(x, y);
//     int left = 0, right = num * min(x, y);
//     while (left < right)
//     {
//         int mid = (left + right) / 2;
//         int copies = mid / x + mid / y;
//         if (copies >= num - 1)
//             right = mid;
//         else
//             left = mid + 1;
//     }
//     return left + min(x, y);
// }
//
// // Второй способ - моделирование процесса копирования
// int min_time_simulation(int x, int y, int num)
// {
//     vector<int> times = {0};
//     for (int i = 1; i < num; ++i)
//     {
//         int t1 = times[i - 1] + x;
//         int t2 = times[i - 1] + y;
//         times.push_back(min(t1, t2));
//     }
//     return times[num - 1] + min(x, y);
// }
//
// // Функция для измерения времени выполнения
// long long measure_time(int (*func)(int, int, int), int x, int y, int num)
// {
//     auto start = chrono::high_resolution_clock::now();
//     func(x, y, num);
//     auto end = chrono::high_resolution_clock::now();
//     return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
// }
//
// int main()
// {
//     vector<int> nums = {8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
//     int x = 2, y = 3;
//
//     cout << left << setw(10) << "num" << setw(15) << "Тэ1" << setw(15) << "Тэ2"
//         << setw(10) << "Т1" << setw(10) << "Т2" << setw(15) << "ТЭ1/Т1" << setw(15) << "ТЭ2/Т2" << endl;
//     for (int num : nums)
//     {
//         long long Te1 = measure_time(min_time_binary, x, y, num);
//         long long Te2 = measure_time(min_time_simulation, x, y, num);
//         int T1 = min_time_binary(x, y, num);
//         int T2 = min_time_simulation(x, y, num);
//         cout << left << setw(10) << num << setw(15) << Te1 << setw(15) << Te2
//             << setw(10) << T1 << setw(10) << T2 << setw(15) << (double)Te1 / T1
//             << setw(15) << (double)Te2 / T2 << endl;
//     }
//     return 0;
// }

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
        // std::cout<<nx<<" "<<l<<" "<<r<<std::endl;
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
    // return func_solve(x, y, nx, num) + min(x, y);
}


int KSU(int a, int b)
{
    int count = 0;
    while (a!=b)
    {
        count ++;
        if(a>b)
        {
            a-=b;
        }else
        {
            b-=a;
        }
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
        // int te1 = min_time_sim(x, y, n);
        int te2 = min_time_bin(x, y, n);
        int te1 = KSU(x, n);
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


// int main()
// {
//     vector<int> nums = {8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
//
//     cout << left << setw(10) << "num" << setw(15) << "Тэ1" << setw(15) << "Тэ2"
//         << setw(10) << "Т1" << setw(10) << "Т2" << setw(15) << "ТЭ1/Т1" << setw(15) << "ТЭ2/Т2" << endl;
//     for (const int num : nums)
//     {
//         constexpr int y = 3, x = 2;
//         const long long Te2 = measure_time(min_time, x, y, num);
//         cout<<Te2;
//     }
//     return 0;
// }

// long long measure_time(int (*func)(int, int, int), int x, int y, int num)
// {
//     const auto start = chrono::high_resolution_clock::now();
//     func(x, y, num);
//     const auto end = chrono::high_resolution_clock::now();
//     return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
// }
