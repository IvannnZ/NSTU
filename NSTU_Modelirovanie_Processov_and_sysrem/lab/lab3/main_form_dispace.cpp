#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

const int NUM_READERS = 3; // Количество процессов чтения
const int BUFFER_SIZE = 10; // Размер буфера записи
const string FILENAME = "data.txt";

struct Reader {
    vector<char> buffer;
    int read_count = 0;
};

void initializeFile() {
    ofstream file(FILENAME, ios::trunc);
    if (!file) {
        cerr << "Ошибка открытия файла!" << endl;
        exit(1);
    }
    file << "abcdefghijklmnopqrstuvwxyz"; // Записываем тестовые данные
    file.close();
}

int weightedRandomSelection(const vector<double>& probabilities) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);
    double r = dist(gen);
    double sum = 0;
    for (int i = 0; i < probabilities.size(); i++) {
        sum += probabilities[i];
        if (r < sum) return i;
    }
    return probabilities.size() - 1;
}

int main() {
    srand(time(0));
    initializeFile();

    Reader readers[NUM_READERS];
    vector<char> writeBuffer;
    ifstream inFile(FILENAME);
    ofstream outFile;
    bool writing = false;

    vector<double> probabilities(NUM_READERS + 1, 1.0 / (NUM_READERS + 1));

    int totalWritten = 0;
    while (true) {
        int selectedProcess = weightedRandomSelection(probabilities);

        if (selectedProcess < NUM_READERS) { // Читающий процесс
            if (!writing) {
                char ch;
                if (!(inFile >> ch)) {
                    inFile.clear();
                    inFile.seekg(0, ios::beg);
                    inFile >> ch;
                }
                readers[selectedProcess].buffer.push_back(ch);
                readers[selectedProcess].read_count++;
            }
        } else { // Процесс записи
            if (!writing) {
                writing = true;
                outFile.open(FILENAME, ios::trunc);
                if (!outFile) {
                    cerr << "Ошибка открытия файла для записи!" << endl;
                    exit(1);
                }
            }
            if (!writeBuffer.empty()) {
                outFile << writeBuffer.back();
                writeBuffer.pop_back();
                totalWritten++;
            } else {
                outFile.close();
                break;
            }
        }
    }

    cout << "Результаты чтения:" << endl;
    for (int i = 0; i < NUM_READERS; i++) {
        cout << "Читатель " << i + 1 << " считал " << readers[i].read_count << " символов." << endl;
    }
    return 0;
}
