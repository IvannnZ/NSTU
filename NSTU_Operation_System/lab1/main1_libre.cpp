#include <iostream>
#include <cstdlib>   // Для system()
#include <unistd.h>  // Для fork() и execlp()
#include <sys/types.h>
#include <sys/wait.h>


int main() {
    std::string filename = "file.odt";
    // Создаем дочерний процесс
    pid_t pid = fork();

    if (pid < 0) {
        std::cout << "fork failed" << std::endl;
        std::cerr << "Ошибка создания процесса!" << std::endl;
        return 1;
    }
    else if (pid == 0) {
        std::cout<<"0"<<std::endl;
        // Дочерний процесс: запускаем LibreOffice Writer
        execlp("libreoffice", "libreoffice", filename.c_str(), NULL);

        // Если execlp() завершится с ошибкой
        std::cerr << "Ошибка запуска LibreOffice!" << std::endl;
        exit(1);
    }
    else {
        std::cout<<"1"<<std::endl;
        // Родительский процесс ждет завершения LibreOffice
        waitpid(pid, NULL, 0);
        std::cout << "LibreOffice закрыт." << std::endl;
    }

    return 0;
}
