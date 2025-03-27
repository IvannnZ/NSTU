#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/wait.h>

std::vector<pid_t> processes;

bool isProcessRunning(pid_t pid) {
    return (kill(pid, 0) == 0);
}

void startProcess() {
    pid_t pid = fork();
    if (pid == 0) {
        // Запуск приложения, например, текстового редактора или другой программы
        // Можете заменить './main' на имя нужной вам программы
        int devNull = open("/dev/null", O_WRONLY);
        if (devNull != -1) {
            dup2(devNull, STDOUT_FILENO);
            dup2(devNull, STDERR_FILENO);
            close(devNull);
        }

        // Здесь можно указать любую другую программу
        execlp("./gnome-calculator","gnome-calculator", NULL);
        exit(1);
    } else if (pid > 0) {
        processes.push_back(pid);
        std::cout << "Started process with PID: " << pid << std::endl;
    } else {
        std::cerr << "Failed to fork process." << std::endl;
    }
}

void cleanupProcesses() {
    waitpid(0, NULL, WNOHANG);
    std::vector<pid_t> activeProcesses;
    for (size_t i = 0; i < processes.size(); ++i) {
        if (isProcessRunning(processes[i])) {
            activeProcesses.push_back(processes[i]);
        }
    }
    processes = activeProcesses;
}

void listProcesses() {
    cleanupProcesses();
    std::cout << "Active processes:" << std::endl;
    for (size_t i = 0; i < processes.size(); ++i) {
        std::cout << "PID: " << processes[i] << std::endl;
    }
}

void stopProcess(pid_t pid) {
    cleanupProcesses();
    for (size_t i = 0; i < processes.size(); ++i) {
        if (processes[i] == pid) {
            if (kill(pid, SIGTERM) == 0) {
                std::cout << "Process " << pid << " terminated." << std::endl;
                processes.erase(processes.begin() + i);
            } else {
                std::cerr << "Failed to terminate process " << pid << std::endl;
            }
            return;
        }
    }
    std::cerr << "Process " << pid << " is not running or already removed." << std::endl;
}

int main() {
    std::string command;
    while (true) {
        std::cout << "Enter command (start, stop [pid], list, exit): ";
        std::cin >> command;

        if (command == "start") {
            startProcess();
        } else if (command == "stop") {
            pid_t pid;
            std::cin >> pid;
            stopProcess(pid);
        } else if (command == "list") {
            listProcesses();
        } else if (command == "exit") {
            for (size_t i = 0; i < processes.size(); ++i) {
                if (isProcessRunning(processes[i])) {
                    kill(processes[i], SIGTERM);
                }
            }
            std::cout << "Exiting program." << std::endl;
            break;
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }
    return 0;
}
