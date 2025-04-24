#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <memory>

class GasStation {
private:
    sem_t sem_pistol;
    pthread_mutex_t mutex_stats;
    pthread_mutex_t mutex_queue;
    pthread_mutex_t mutex_refueling;

    int waiting_cars;
    int refueling_cars;
    int total_served;
    unsigned long total_wait_time;
    unsigned long total_refuel_time;

    GasStation() {
        sem_init(&sem_pistol, 0, 2);
        pthread_mutex_init(&mutex_stats, NULL);
        pthread_mutex_init(&mutex_queue, NULL);
        waiting_cars = 0;
        total_served = 0;
        total_wait_time = 0;
        total_refuel_time = 0;
    }

public:
    static GasStation& get() {
        static GasStation instance;
        return instance;
    }

    void car_arrived() {
        pthread_mutex_lock(&mutex_queue);
        waiting_cars++;
        pthread_mutex_unlock(&mutex_queue);
    }

    void car_started_refuel() {
        pthread_mutex_lock(&mutex_queue);
        waiting_cars--;
        pthread_mutex_unlock(&mutex_queue);
    }

    void car_refueled(long wait_time, long refuel_time) {
        pthread_mutex_lock(&mutex_stats);
        total_served++;
        total_wait_time += wait_time;
        total_refuel_time += refuel_time;
        pthread_mutex_unlock(&mutex_stats);
    }

    void acquire_pistol() {
        sem_wait(&sem_pistol);
    }

    void release_pistol() {
        sem_post(&sem_pistol);
    }

    void get_stats(int& cars_waiting, int& cars_refueling, int& cars_served,
                  double& avg_wait, double& forecast) {
        pthread_mutex_lock(&mutex_queue);
        cars_waiting = waiting_cars;
        pthread_mutex_unlock(&mutex_queue);

        pthread_mutex_lock(&mutex_stats);
        cars_served = total_served;
        avg_wait = total_served > 0 ?
            (double)total_wait_time / total_served : 0.0;
        double avg_refuel = total_served > 0 ?
            (double)total_refuel_time / total_served : 0.0;
        forecast = avg_refuel * (cars_waiting + 1) / 2.0;
        pthread_mutex_unlock(&mutex_stats);
        cars_refueling = get_used_pistols();
    }

    int get_used_pistols() {
        int val;
        sem_getvalue(&sem_pistol, &val);
        return 2 - val;
    }

    ~GasStation() {
        sem_destroy(&sem_pistol);
        pthread_mutex_destroy(&mutex_stats);
        pthread_mutex_destroy(&mutex_queue);
    }
};

class Car {
private:
    struct timespec arrive_time;
    long refuel_duration;

public:
    Car() {
        clock_gettime(CLOCK_MONOTONIC, &arrive_time);
        refuel_duration = (rand() % 5 + 1) * 1000000;
    }

    void run() {
        GasStation& station = GasStation::get();

        station.car_arrived();
        station.acquire_pistol();
        station.car_started_refuel();

        struct timespec start_refuel;
        clock_gettime(CLOCK_MONOTONIC, &start_refuel);

        long wait_time = (start_refuel.tv_sec - arrive_time.tv_sec) * 1000 +
                        (start_refuel.tv_nsec - arrive_time.tv_nsec) / 1000000;

        usleep(refuel_duration);

        long refuel_time = refuel_duration / 1000;
        station.car_refueled(wait_time, refuel_time);
        station.release_pistol();
    }
};

void* car_generator(void* arg) {
    while (true) {
        std::unique_ptr<Car> car = std::make_unique<Car>();
        pthread_t thread;
        pthread_create(&thread, NULL, [](void* arg) -> void* {
            std::unique_ptr<Car> car(static_cast<Car*>(arg));
            car->run();
            return NULL;
        }, car.release());

        sleep(rand() % 3 + 1);
    }
    return NULL;
}

void print_stats() {
    GasStation& station = GasStation::get();
    int cars_waiting, cars_served, cars_refueling;
    double avg_wait, forecast;

    station.get_stats(cars_waiting, cars_refueling, cars_served, avg_wait, forecast);

    std::cout << "\nТекущая статистика:\n"
              << "Машин в очереди: " << cars_waiting << std::endl
              << "Машин на заправке: " << cars_refueling<<std::endl
              << "Всего обслужено: " << cars_served <<std::endl
              << "Среднее время ожидания: " << avg_wait << " мс\n"
              << "Прогноз для новой машины: " << forecast << " мс\n"
              << "-------------------------" << std::endl;
}

void* stats_printer(void* arg) {
    while (true) {
        print_stats();
        sleep(1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t generator_thread, stats_thread;


    pthread_create(&generator_thread, NULL, car_generator, NULL);
    pthread_create(&stats_thread, NULL, stats_printer, NULL);


    std::cout << "Управление заправкой:\n"
              << "Нажмите Enter для добавления машины\n"
              << "Введите 'q' и нажмите Enter для выхода\n";

    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "q") break;

        std::unique_ptr<Car> car = std::make_unique<Car>();
        pthread_t thread;
        pthread_create(&thread, NULL, [](void* arg) -> void* {
            std::unique_ptr<Car> car(static_cast<Car*>(arg));
            car->run();
            return NULL;
        }, car.release());
    }

    // Завершение работы
    pthread_cancel(generator_thread);
    pthread_cancel(stats_thread);

    return 0;
}