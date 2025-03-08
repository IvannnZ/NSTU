#include <algorithm>
#include <iostream>
//#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string.h>
#include <ctime>
//#include <io.h>
#include <numeric>
#include <random>


using namespace std;

unsigned int width = 1000;
unsigned int height = 1000;
//sf::RenderWindow window(sf::VideoMode({width, height}), "Lab 3");


std::string randomStrGen(const int length) {
    static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    result.resize(length);
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];
    return result;
}

int write(ofstream& in, const int count) {
    int temp = 0;
    if(!in.is_open()) cout << "Error in opening file" << endl;
    int size{};
    for (int i = 0; i < count; i++) {
        size = rand() % 10 + 3;
        temp += size;
        in << randomStrGen(size) << endl;
    }
    in.close();
    return temp;
}

int weightedRandomIndex(const std::vector<int>& priorities) {
    if (priorities.empty()) return -1;

    std::random_device rd;
    std::mt19937 gen(rd());

    const int totalWeight = std::accumulate(priorities.begin(), priorities.end(), 0);
    if (totalWeight == 0) return -1;

    std::uniform_int_distribution<int> dist(0, totalWeight - 1);
    const int randomValue = dist(gen);

    int cumulativeWeight = 0;
    for (size_t i = 0; i < priorities.size(); ++i) {
        cumulativeWeight += priorities[i];
        if (randomValue < cumulativeWeight) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
void fillPriorities(std::vector<int>& priorities, int size, int ReadFlowSize) {
    if (size <= 0 || ReadFlowSize <= 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, ReadFlowSize);

    priorities.resize(size);
    for (int& value : priorities) {
        value = dist(gen);
    }
}

void read(ifstream& out, vector<int>& ReadFlow) {
    out.open("input.txt");
    string line;
    out >> line;
}


int main()
{
//    window.setPosition(sf::Vector2i(0, 0));
    //Chart.setPosition(sf::Vector2i(static_cast<int>(width), 0));

    ofstream WriteFile;
    WriteFile.open("input.txt");
    ifstream ReadFile;
    int CountIn = 100;
    int size = 100;
    int ReadFlowSize = size;


        int SizeFile = write(WriteFile, CountIn);

        vector<int> ReadFlow;
        read(ReadFile, ReadFlow);


        vector<int> priorities;
        fillPriorities(priorities, size, ReadFlowSize);
        auto SavePriorities = priorities;
        vector<int> Value(size, 0);
        int AbsoluteValue = 0;

        int temp = 0;
        while (1) {
            if(AbsoluteValue == size) break;
            temp = weightedRandomIndex(priorities);
            Value[temp]++;
            if(Value[temp] == SizeFile) AbsoluteValue++, priorities[temp] = 0;

        }


    for (int i = 0; i < size; i++) {
        cout << "Process " << i << " with priority " << SavePriorities[i] << " read " << Value[i] << " characters." << endl;
    }

    return 0;
}