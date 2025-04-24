#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

sem_t pumpSemaphore;
pthread_mutex_t statsMutex = PTHREAD_MUTEX_INITIALIZER;

int carsInQueue = 0;
int carsRefueling = 0;
double totalWaitTime = 0.0;
int totalCarsProcessed = 0;
double totalRefuelTime = 0.0;
int totalCarsRefueled = 0;

void* carThread(void* arg) {
    struct timespec startTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    pthread_mutex_lock(&statsMutex);
    carsInQueue++;
    pthread_mutex_unlock(&statsMutex);

    sem_wait(&pumpSemaphore);

    struct timespec startRefuelTime;
    clock_gettime(CLOCK_MONOTONIC, &startRefuelTime);

    pthread_mutex_lock(&statsMutex);
    carsInQueue--;
    carsRefueling++;
    double waitTime = (startRefuelTime.tv_sec - startTime.tv_sec) +
                      (startRefuelTime.tv_nsec - startTime.tv_nsec) / 1e9;
    totalWaitTime += waitTime;
    totalCarsProcessed++;
    pthread_mutex_unlock(&statsMutex);

    unsigned int seed = time(NULL) ^ pthread_self();
    int refuelTime = rand_r(&seed) % 5 + 1;
    sleep(refuelTime);

    pthread_mutex_lock(&statsMutex);
    carsRefueling--;
    totalRefuelTime += refuelTime;
    totalCarsRefueled++;
    pthread_mutex_unlock(&statsMutex);

    sem_post(&pumpSemaphore);

    return NULL;
}

void* inputThread(void* arg) {
    char command;
    while (true) {
        std::cin >> command;
        if (command == 'a') {
            pthread_t thread;
            pthread_create(&thread, NULL, carThread, NULL);
            pthread_detach(thread);
        } else if (command == 'q') {
            exit(0);
        }
    }
    return NULL;
}

void* statsThread(void* arg) {
    while (true) {
        pthread_mutex_lock(&statsMutex);
        int currentInQueue = carsInQueue;
        int currentRefueling = carsRefueling;
        double averageWait = (totalCarsProcessed > 0) ?
            round(totalWaitTime / totalCarsProcessed * 100) / 100 : 0.0;
        double avgRefuel = (totalCarsRefueled > 0) ?
            round(totalRefuelTime / totalCarsRefueled * 100) / 100 : 0.0;
        double predictedWait = round(currentInQueue * avgRefuel / 2 * 100) / 100;
        pthread_mutex_unlock(&statsMutex);

        std::cout << "\nCurrent status:" << std::endl;
        std::cout << "Cars in queue: " << currentInQueue << std::endl;
        std::cout << "Cars refueling: " << currentRefueling << std::endl;
        std::cout << "Average waiting time: " << averageWait << "s" << std::endl;
        std::cout << "Predicted wait for new car: " << predictedWait << "s" << std::endl;
        std::cout << "------------------------" << std::endl;

        sleep(1);
    }
    return NULL;
}

int main() {
    sem_init(&pumpSemaphore, 0, 2);

    pthread_t inputTid, statsTid;
    pthread_create(&inputTid, NULL, inputThread, NULL);
    pthread_create(&statsTid, NULL, statsThread, NULL);

    pthread_join(inputTid, NULL);
    pthread_join(statsTid, NULL);

    sem_destroy(&pumpSemaphore);
    return 0;
}