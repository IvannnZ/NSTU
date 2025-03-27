#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

int randRange(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

long long synchronousPipeline(int stages, int elements)
{
    vector<int> processingTime(elements, 0);
    long long totalTime = 0;

    for (int stage = 0; stage < stages; ++stage)
    {
        for (int i = 0; i < elements; ++i)
        {
            processingTime[i] += randRange(1, 10);
        }
    }

    for (int t : processingTime)
    {
        totalTime = max(totalTime, (long long)t);
    }
    return totalTime;
}

long long asynchronousPipeline(int stages, int elements)
{
    vector<int> buffer(stages, 0);
    vector<int> processing(elements, 0);
    long long time = 0;
    int processed = 0;

    while (processed < elements)
    {
        for (int i = stages - 1; i >= 0; --i)
        {
            if (buffer[i] > 0)
            {
                --buffer[i];
                if (buffer[i] == 0 && i == stages - 1)
                {
                    ++processed;
                }
            }
        }


        for (int i = stages - 1; i > 0; --i)
        {
            if (buffer[i] == 0 && buffer[i - 1] > 0)
            {
                buffer[i] = buffer[i - 1];
                buffer[i - 1] = 0;
            }
        }


        if (processed + buffer[0] < elements)
        {
            buffer[0] = randRange(1, 10);
        }

        ++time;
    }
    return time;
}

int main()
{
    srand(time(0));

    int stages = 5;
    cout << left << setw(10) << "Elements"
        << setw(20) << "Synchronous"
        << setw(20) << "Asynchronous"
        << setw(15) << "Async/Sync" << endl;
    cout << string(65, '-') << endl;

    for (int elements = 100; elements < 1000; elements += 100)
    {
        long long syncTime = synchronousPipeline(stages, elements);
        long long asyncTime = asynchronousPipeline(stages, elements);

        cout << left << setw(10) << elements
            << setw(20) << syncTime
            << setw(20) << asyncTime
            << setw(15) << fixed << setprecision(2)
            << (double)asyncTime / syncTime << endl;
    }
    return 0;
}
