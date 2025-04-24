#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <termios.h>
#include <fcntl.h>
#include <chrono>

using namespace std;

class GasStation
{
private:
    sem_t sem_pistol;
    pthread_mutex_t mutex_stats;
    pthread_mutex_t mutex_queue;

    int waiting_cars;
    int total_served;
    unsigned long total_wait_time;
    unsigned long total_refuel_time;

    GasStation()
    {
        sem_init(&sem_pistol, 0, 2);
        pthread_mutex_init(&mutex_stats, nullptr);
        pthread_mutex_init(&mutex_queue, nullptr);
        waiting_cars = 0;
        total_served = 0;
        total_wait_time = 0;
        total_refuel_time = 0;
    }

public:
    static GasStation& get()
    {
        static GasStation instance;
        return instance;
    }

    void car_arrived()
    {
        pthread_mutex_lock(&mutex_queue);
        waiting_cars++;
        pthread_mutex_unlock(&mutex_queue);
    }

    void car_started_refuel()
    {
        pthread_mutex_lock(&mutex_queue);
        waiting_cars--;
        pthread_mutex_unlock(&mutex_queue);
    }

    void car_refueled(long wait_time, long refuel_time)
    {
        pthread_mutex_lock(&mutex_stats);
        total_served++;
        total_wait_time += wait_time;
        total_refuel_time += refuel_time;
        pthread_mutex_unlock(&mutex_stats);
    }

    void acquire_pistol()
    {
        sem_wait(&sem_pistol);
    }

    void release_pistol()
    {
        sem_post(&sem_pistol);
    }

    void get_stats(int& cars_waiting, int& cars_served,
                   double& avg_wait, double& forecast, int& get_refuiling)
    {
        pthread_mutex_lock(&mutex_queue);
        cars_waiting = waiting_cars;
        pthread_mutex_unlock(&mutex_queue);

        pthread_mutex_lock(&mutex_stats);
        cars_served = total_served;
        avg_wait = total_served > 0 ? (double)total_wait_time / total_served / 1000 : 0.0;
        double avg_refuel = total_served > 0 ? (double)total_refuel_time / total_served / 1000 : 0.0;
        forecast = avg_refuel * (cars_waiting + 1) / 2.0;
        pthread_mutex_unlock(&mutex_stats);
        get_refuiling = get_used_pistols();
    }

    int get_used_pistols()
    {
        int val;
        sem_getvalue(&sem_pistol, &val);
        return 2 - val;
    }

    ~GasStation()
    {
        sem_destroy(&sem_pistol);
        pthread_mutex_destroy(&mutex_stats);
        pthread_mutex_destroy(&mutex_queue);
    }
};

class Car
{
public:
    enum Status { WAITING, REFUELING, DONE };

private:
    pthread_t thread;
    Status status;
    int id;
    mutable pthread_mutex_t mutexstats;
    chrono::time_point<chrono::system_clock> create_time;
    long refuel_duration;

    static void* thread_func(void* arg)
    {
        Car* self = static_cast<Car*>(arg);
        self->run();
        return nullptr;
    }

    long get_elapsed_time()
    {
        auto now = chrono::system_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(
            now - create_time).count();
    }

    void run()
    {
        pthread_mutex_lock(&mutexstats);
        status = WAITING;
        pthread_mutex_unlock(&mutexstats);

        long wait_start = get_elapsed_time();

        GasStation::get().acquire_pistol();

        long wait_time = get_elapsed_time() - wait_start;
        pthread_mutex_lock(&mutexstats);
        status = REFUELING;
        pthread_mutex_unlock(&mutexstats);

        GasStation::get().car_started_refuel();

        refuel_duration = (rand() % 5 + 1) * 1000; // 1-5 seconds
        usleep(refuel_duration * 1000);

        GasStation::get().release_pistol();

        pthread_mutex_lock(&mutexstats);
        status = DONE;
        pthread_mutex_unlock(&mutexstats);

        GasStation::get().car_refueled(wait_time, refuel_duration);
    }

public:
    Car(int car_id) : id(car_id), status(WAITING),
                      create_time(chrono::system_clock::now())
    {
        pthread_mutex_init(&mutexstats, nullptr);
        GasStation::get().car_arrived();
        pthread_create(&thread, nullptr, &Car::thread_func, this);
    }

    ~Car()
    {
        pthread_join(thread, nullptr);
    }


    Status get_status() const
    {
        pthread_mutex_lock(&mutexstats);
        return status;
        pthread_mutex_unlock(&mutexstats);
    }

    int get_id() const { return id; }
};

vector<Car*> cars;
int next_id = 1;

void print_stats()
{
    int cars_waiting, cars_served, refueling;
    double avg_wait, forecast;
    GasStation::get().get_stats(cars_waiting, cars_served, avg_wait, forecast, refueling);

    cout << "\033[2J\033[1;1H";
    cout << "Заправляются: " << refueling << endl;
    cout << "Ожидают: " << cars_waiting << endl;
    cout << "Среднее время ожидания: " << avg_wait << "s" << endl;
    cout << "Прогноз для новой машины: " << forecast << "s" << endl;
    cout << "\n'a' - новая машина, 'q' - выход\n";
}

void cleanup_cars()
{
    auto it = cars.begin();
    while (it != cars.end())
    {
        if ((*it)->get_status() == Car::DONE)
        {
            delete *it;
            it = cars.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool kbhit()
{
    termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

int main()
{
    srand(time(nullptr));

    cout << "Бензоколонка - управление:" << endl;
    cout << "Нажмите 'enter' для добавления машины" << endl;
    cout << "Нажмите 'q' для выхода" << endl;

    while (true)
    {
        if (kbhit())
        {
            char c = getchar();
            if (c == 'q') break;
            cars.push_back(new Car(next_id++));
        }

        print_stats();
        cleanup_cars();
        usleep(1000000);
    }

    cleanup_cars();
    return 0;
}
