#include <iostream>
#include <unistd.h>  // Для fork() и execlp()
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    std::string filename = "file.txt";

    // Создаем дочерний процесс
    pid_t pid = fork();
    std::cout << "pid = " << pid << std::endl;

    if (pid < 0) {
      std::cout << "0" << std::endl;
        std::cerr << "Ошибка создания процесса!" << std::endl;
        return 1;
    }
    else if (pid == 0) {
        std::cout << "1" << std::endl;

        // Запускаем nano в новом окне терминала
        execlp("gnome-terminal", "gnome-terminal", "--", "nano", filename.c_str(), NULL);

        // Если execlp() завершится с ошибкой
        std::cerr << "Ошибка запуска gnome-terminal!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "2" << std::endl;

        std::cout << "Открытие nano в новом окне терминала..." << std::endl;
    }

    return 0;
}
