#include "iostream"
// #include <stdio.h>
#include <unistd.h>

int main()
{
    int i = 4;
    while (i)
    {
        --i;
        sleep(5);
        std::cout << "My PID: " << getpid() << " " << i << std::endl;
    }
}
