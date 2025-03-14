#include <iostream>
#include <cstdlib>

int main() {
    std::string command = "libreoffice file.txt &"; // "&" чтобы не блокировать терминал
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Ошибка запуска LibreOffice!" << std::endl;
        return 1;
    }

    std::cout << "LibreOffice запущен." << std::endl;
    return 0;
}
