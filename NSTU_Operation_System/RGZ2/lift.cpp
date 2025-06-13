#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>

const std::string FIFO_PREFIX = "/tmp/lift_fifo_";

struct LiftStatus {
    int id;
    int currentFloor;
    int targetFloor;
    char direction[10]; // "UP", "DOWN", "STOP"
};

// Очистка экрана
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// Проверка корректности ввода числа
int inputFloor(int currentFloor) {
    int floor;
    while (true) {
        std::cout << "Введите целевой этаж (1–9): ";
        std::cin >> floor;
        if (std::cin.fail() || floor < 1 || floor > 9) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод. Попробуйте снова.\n";
        } else if (floor == currentFloor) {
            std::cout << "Вы уже на этом этаже.\n";
        } else {
            return floor;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: lift_child <lift_id>\n";
        return 1;
    }

    int lift_id = std::stoi(argv[1]);
    std::string fifoName = FIFO_PREFIX + std::to_string(lift_id);

    int fd = open(fifoName.c_str(), O_WRONLY);
    if (fd < 0) {
        perror("Ошибка открытия FIFO");
        return 1;
    }

    LiftStatus status;
    status.id = lift_id;
    status.currentFloor = 1;

    while (true) {
        clearScreen();
        std::cout << "=== ЛИФТ #" << lift_id << " ===\n";
        std::cout << "Текущий этаж: " << status.currentFloor << "\n";
        std::cout << "1. Вызвать лифт на другой этаж\n";
        std::cout << "2. Выход\n";
        std::cout << "Выберите действие: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 2) {
            std::cout << "Завершение работы лифта #" << lift_id << "\n";
            break;
        } else if (choice == 1) {
            status.targetFloor = inputFloor(status.currentFloor);

            if (status.targetFloor > status.currentFloor)
                strcpy(status.direction, "UP");
            else
                strcpy(status.direction, "DOWN");

            while (status.currentFloor != status.targetFloor) {
                sleep(4);
                status.currentFloor += (status.direction[0] == 'U') ? 1 : -1;
                write(fd, &status, sizeof(status));
            }

            strcpy(status.direction, "STOP");
            write(fd, &status, sizeof(status));

            std::cout << "Лифт прибыл на этаж " << status.currentFloor << ". Нажмите Enter для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
    }

    close(fd);
    return 0;
}
