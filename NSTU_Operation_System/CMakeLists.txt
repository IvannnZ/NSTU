cmake_minimum_required(VERSION 3.29)
project(NSTU_Operation_System)

# Устанавливаем стандарт C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем флаги компилятора для отладки и релиза
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Находим библиотеку pthread
find_package(Threads REQUIRED)

# Добавляем флаг для компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Добавляем исходные файлы проекта
add_executable(NSTU_Operation_System
        lab1/test.cpp)

# Линкуем с pthread
target_link_libraries(NSTU_Operation_System PRIVATE Threads::Threads)
