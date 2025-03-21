
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

pid_t child_pid = -1; // Глобальная переменная для хранения PID дочернего процесса
std::string filename = "file.txt";

void startProcess()
{
    if (child_pid > 0)
    {
        std::cout << "Процесс уже запущен! PID: " << child_pid << std::endl;
        return;
    }

    child_pid = fork();

    if (child_pid == 0)
    {
        // Дочерний процесс
        execlp("gnome-terminal", "gnome-terminal", "--", "nano", filename.c_str(), NULL);
//         execlp("libreoffice", "libreoffice", filename.c_str(), NULL);

        std::cerr << "Ошибка: не удалось запустить nano!" << std::endl;
        exit(1);
    }
    else if (child_pid > 0)
    {
        std::cout << "Запущен процесс nano с PID: " << child_pid << std::endl;
    }
    else
    {
        std::cerr << "Ошибка при fork()!" << std::endl;
    }
}

void stopProcess()
{
    if (child_pid > 0)
    {
        kill(child_pid, SIGTERM); // Отправляем сигнал завершения
        waitpid(child_pid, NULL, 0); // Ждём завершения процесса
        std::cout << "Процесс nano (PID: " << child_pid << ") завершён." << std::endl;
        child_pid = -1;
    }
    else
    {
        std::cout << "Нет запущенного процесса для остановки!" << std::endl;
    }
}

int main()
{
    std::string command;

    std::cout << "Команды: start (запуск nano), stop (закрытие nano), exit (выход)" << std::endl;

    while (true)
    {
        std::cout << "> ";
        std::cin >> command;

        if (command == "start")
        {
            startProcess();
        }
        else if (command == "stop")
        {
            stopProcess();
        }
        else if (command == "exit")
        {
            stopProcess();
            std::cout << "Выход из программы." << std::endl;
            break;
        }
        else
        {
            std::cout << "Неизвестная команда!" << std::endl;
        }
    }

    return 0;
}
