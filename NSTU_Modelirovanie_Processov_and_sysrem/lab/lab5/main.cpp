#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

class RandomNormal
{
    random_device rd;
    mt19937 gen;
    normal_distribution<> dist;

public:
    RandomNormal(double mean, double stddev) : gen(rd()), dist(mean, stddev)
    {
    }

    int operator()() { return max(1, (int)round(dist(gen))); }
};

// Function for synchronous pipeline
long long synchronousPipeline(int stages, int elements, RandomNormal& randGen)
{
    vector<int> processingTime(elements, 0);
    vector<int> buffer(stages, 0);
    long long totalTime = 0;

    for (int i = 0; i < stages; i++)
    {
        buffer[i] = randGen();
    }

    for (int i = 0; i < elements; i++)

    {
        totalTime += *std::max_element(buffer.begin(), buffer.end());
        for (int i = 0; i < stages; i++)
        {
            buffer[i] = randGen();
        }
    }
    return totalTime;
}

long long asynchronousPipeline(int stages, int elements, RandomNormal& randGen)
{
    vector<int> buffer(stages, 0);
    long long time = 0;
    int processed = 0;
    for (int i = 0; i < stages; ++i)
    {
        buffer[i] = randGen();
    }
    while (processed < elements)
    {
        // Process elements at each stage
        for (int i = stages - 1; i >= 0; --i)
        {
            if (buffer[i] > 0)
            {
                --buffer[i];
                if (buffer[i] == 0 && i == stages - 1)
                {
                    ++processed;
                    buffer[i] = -1;
                }
            }
        }

        for (int i = stages - 1; i > 0; --i)
        {
            if (buffer[i] == -1 && buffer[i - 1] == 0)
            {
                buffer[i] = randGen();
                buffer[i - 1] = -1;
                cout<<"buffer[i]"<<endl;
            }
        }
        if (processed == 98)
        {
            cout <<"processed"<<endl;
        }
        if (buffer[0] == -1){ buffer[0] = randGen(); }
        ++time;
    }
    return time;
}

int main()
{
    int stages = 5;
    double mean = 5.0, stddev = 2.0;
    cout << left << setw(10) << "num"
        << setw(20) << "Synchronous"
        << setw(20) << "Asynchronous"
        << setw(15) << "Asyn / Syn" << endl;
    cout << string(100, '-') << endl;

    RandomNormal randGen(mean, stddev);
    for (int elements = 100; elements < 1000; elements += 100)
    {
        long long syncTime = synchronousPipeline(stages, elements, randGen);
        long long asyncTime = asynchronousPipeline(stages, elements, randGen);

        cout << left << setw(10) << elements
            << setw(20) << syncTime
            << setw(20) << asyncTime
            << setw(15) << (double)asyncTime / syncTime << endl;
    }
    return 0;
}
