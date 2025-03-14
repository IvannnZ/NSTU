#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Процесс запущен, PID: " << getpid() << std::endl;

    fork(); // Первый fork()
    fork(); // Второй fork()

    std::cout << "Я новый процесс, мой PID: " << getpid() << std::endl;

    return 0;
}
