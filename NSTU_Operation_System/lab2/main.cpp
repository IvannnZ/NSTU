#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

using namespace std;

// Глобальные переменные
int GlobalData = 0;
vector<int> GlobalLog1, GlobalLog2;
const int CONST_VALUE = 3;
const int ITERATIONS = 100;

// Синхронизация
enum SyncType { NO_SYNC = 0, CRITICAL_SECTION = 1, MUTEX = 2 };
atomic<SyncType> syncType(NO_SYNC);
mutex mtx;

void threadFunc1() {
    for (int i = 0; i < ITERATIONS; i++) {
        if (syncType == MUTEX) {
            unique_lock<mutex> lock(mtx, defer_lock);
            lock.lock();
            GlobalData += CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(10));
            GlobalData -= CONST_VALUE;
            GlobalLog1.push_back(GlobalData);
            lock.unlock();
            continue;
        } else if (syncType == CRITICAL_SECTION) {
            unique_lock<mutex> lock(mtx);
            GlobalData += CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(10));
            GlobalData -= CONST_VALUE;
            GlobalLog1.push_back(GlobalData);
            continue;
        }

        GlobalData += CONST_VALUE;
        this_thread::sleep_for(chrono::milliseconds(10));
        GlobalData -= CONST_VALUE;
        GlobalLog1.push_back(GlobalData);
    }
}

void threadFunc2() {
    for (int i = 0; i < ITERATIONS; i++) {
        if (syncType == MUTEX) {
            unique_lock<mutex> lock(mtx, defer_lock);
            lock.lock();
            GlobalData -= CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(10));
            GlobalData += CONST_VALUE;
            GlobalLog2.push_back(GlobalData);
            lock.unlock();
            continue;
        } else if (syncType == CRITICAL_SECTION) {
            unique_lock<mutex> lock(mtx);
            GlobalData -= CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(10));
            GlobalData += CONST_VALUE;
            GlobalLog2.push_back(GlobalData);
            continue;
        }

        GlobalData -= CONST_VALUE;
        this_thread::sleep_for(chrono::milliseconds(10));
        GlobalData += CONST_VALUE;
        GlobalLog2.push_back(GlobalData);
    }
}

int main() {
    cout << "Выберите тип синхронизации (RadioGroup):\n";
    cout << "0 - Без синхронизации\n";
    cout << "1 - Критическая секция (unique_lock)\n";
    cout << "2 - Мьютекс (lock/unlock через unique_lock с defer_lock)\n";
    int choice;
    cin >> choice;

    if (choice >= 0 && choice <= 2) {
        syncType = static_cast<SyncType>(choice);
    } else {
        cout << "Неверный выбор. Используется режим без синхронизации.\n";
        syncType = NO_SYNC;
    }

    thread t1(threadFunc1);
    thread t2(threadFunc2);

    t1.join();
    t2.join();

    // Вывод истории изменений
    cout << "История изменений GlobalData потоком 1:" << endl;
    for (int val : GlobalLog1) {
        cout << val << " ";
    }
    cout << endl;

    cout << "История изменений GlobalData потоком 2:" << endl;
    for (int val : GlobalLog2) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
