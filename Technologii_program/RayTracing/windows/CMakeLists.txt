cmake_minimum_required(VERSION 3.15)
project(RealTimeRayTracing)

# Кросс-компиляция для Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Пути к SFML
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/Libraries/sfml-windows/SFML-2.6.2")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

# Создание исполняемого файла и линковка с библиотеками
add_executable(RealTimeRayTracing Main.cpp)

target_link_libraries(RealTimeRayTracing
        ${SFML_LIBRARY_DIR}/libsfml-graphics.a
        ${SFML_LIBRARY_DIR}/libsfml-window.a
        ${SFML_LIBRARY_DIR}/libsfml-system.a
)
