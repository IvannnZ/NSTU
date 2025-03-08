#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

using namespace std;


int weightedRandomIndex(const std::vector<int>& priorities)
{
    if (priorities.empty()) return -1;

    int totalWeight = std::accumulate(priorities.begin(), priorities.end(), 0);
    if (totalWeight == 0) return -1;

    int randomValue = rand() % totalWeight;
    int cumulativeWeight = 0;
    for (size_t i = 0; i < priorities.size(); ++i)
    {
        cumulativeWeight += priorities[i];
        if (randomValue < cumulativeWeight)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void fillPriorities(std::vector<int>& priorities, int size, int ReadFlowSize)
{
    if (size <= 0 || ReadFlowSize <= 0) return;

    priorities.resize(size);
    for (int& value : priorities)
    {
        value = rand() % ReadFlowSize + 1;
    }
}

int main()
{
    int size = 100;
    int ReadFlowSize = size;
    int SizeFile = 10;
    int chanceCraeteWrite = 10;

    vector<int> priorities;
    fillPriorities(priorities, size, ReadFlowSize);
    vector<int> SavePriorities = priorities;
    vector<int> Value(size, 0);
    int AbsoluteValue = 0;

    int temp;
    while (AbsoluteValue < size)
    {
        temp = weightedRandomIndex(priorities);
        if (temp == -1) break;
        Value[temp]++;
        if (Value[temp] == SizeFile)
        {
            AbsoluteValue++;
            priorities[temp] = 0;
            if (rand() % chanceCraeteWrite == 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (priorities[i] != 0) { Value[i] = 0; }
                }
                SizeFile++;
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        cout << "Process " << i << " with priority " << SavePriorities[i] << " read " << Value[i] << " characters." <<
            endl;
    }

    return 0;
}
