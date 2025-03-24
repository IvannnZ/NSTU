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
const int ITERATIONS = 10;
const int dellay_time = 100;
// Синхронизация
enum SyncType { NO_SYNC = 0, CRITICAL_SECTION = 1, MUTEX = 2};
atomic<SyncType> syncType(NO_SYNC);
mutex mtx;

void threadFunc1() {
    for (int i = 0; i < ITERATIONS; i++) {
        if (syncType == MUTEX) {
            unique_lock<mutex> lock(mtx, defer_lock);
            lock.lock();
            GlobalData += CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(dellay_time));
            GlobalData -= CONST_VALUE;
            GlobalLog1.push_back(GlobalData);
            lock.unlock();
            continue;
        } else if (syncType == CRITICAL_SECTION) {
            unique_lock<mutex> lock(mtx);
            GlobalData += CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(dellay_time));
            GlobalData -= CONST_VALUE;
            GlobalLog1.push_back(GlobalData);
            continue;
        }

        GlobalData += CONST_VALUE;
        this_thread::sleep_for(chrono::milliseconds(dellay_time));
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
            this_thread::sleep_for(chrono::milliseconds(dellay_time));
            GlobalData += CONST_VALUE;
            GlobalLog2.push_back(GlobalData);
            lock.unlock();
            continue;
        } else if (syncType == CRITICAL_SECTION) {
            unique_lock<mutex> lock(mtx);
            GlobalData -= CONST_VALUE;
            this_thread::sleep_for(chrono::milliseconds(dellay_time));
            GlobalData += CONST_VALUE;
            GlobalLog2.push_back(GlobalData);
            continue;
        }

        GlobalData -= CONST_VALUE;
        this_thread::sleep_for(chrono::milliseconds(dellay_time));
        GlobalData += CONST_VALUE;
        GlobalLog2.push_back(GlobalData);
    }
}

int main() {
    cout << "\nВыберите тип синхронизации (RadioGroup):\n0 - Без синхронизации\n1 - Критическая секция (unique_lock)\n2 - Мьютекс (lock/unlock)\n";
    int choice;
    cin >> choice;

    while (true) {
        GlobalData = 0;
        GlobalLog1.clear();
        GlobalLog2.clear();


        if (choice >= 0 && choice <= 2) {
            syncType = static_cast<SyncType>(choice);
        } else {
            cout << "Неверный выбор. Используется режим без синхронизации.\n";
            syncType = NO_SYNC;
        }

        // isRunning = true;
        thread t1(threadFunc1);
        thread t2(threadFunc2);

        cout << "Если не хотите запускать ещё, напишите < 0, иначе\nВыберите тип синхронизации (RadioGroup):\n0 - Без синхронизации\n1 - Критическая секция (unique_lock)\n2 - Мьютекс (lock/unlock)\n";
        cin >> choice;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Добавим небольшую задержку, чтобы избежать высокоскоростного зацикливания
        t1.join();
        t2.join();
        // isRunning = false;

        cout << "\nИстория изменений GlobalData потоком 1:" << endl;
        for (int val : GlobalLog1) {
            cout << val << " ";
        }
        cout << "\nИстория изменений GlobalData потоком 2:" << endl;
        for (int val : GlobalLog2) {
            cout << val << " ";
        }
        if (choice<0)
        {
            break;
        }
    }

    return 0;
}

