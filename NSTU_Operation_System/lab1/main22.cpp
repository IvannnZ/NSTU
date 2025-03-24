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
std::string filename = "file.odt";

void startProcess() {
    pid_t pid = fork();
    if (pid == 0) {
        int devNull = open("/dev/null", O_WRONLY);
        if (devNull != -1) {
            dup2(devNull, STDOUT_FILENO);
            dup2(devNull, STDERR_FILENO);
            close(devNull);
        }
        execlp("gnome-calculator", "gnome-calculator", NULL);
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
    processes.erase(
        std::remove_if(processes.begin(), processes.end(), [](pid_t pid) {
            return !isProcessRunning(pid);
        }),
        processes.end()
    );
}

void listProcesses() {
    cleanupProcesses();
    std::cout << "Active processes:" << std::endl;
    for (pid_t pid : processes) {
        std::cout << "PID: " << pid << std::endl;
    }
}

void stopProcess(pid_t pid) {
    cleanupProcesses();
    auto it = std::find(processes.begin(), processes.end(), pid);
    if (it != processes.end()) {
        if (kill(pid, SIGTERM) == 0) {
            std::cout << "Process " << pid << " terminated." << std::endl;
            processes.erase(it);
        } else {
            std::cerr << "Failed to terminate process " << pid << std::endl;
        }
    } else {
        std::cerr << "Process " << pid << " is not running or already removed." << std::endl;
    }
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
            for (pid_t pid : processes) {
                if (isProcessRunning(pid)) {
                    kill(pid, SIGTERM);
                }
            }
            std::cout << "Exiting program." << std::endl;
            break;
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }
    return 0;
}
