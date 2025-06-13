cmake_minimum_required(VERSION 3.15)
project(RGZ2)

set(CMAKE_CXX_STANDARD 20)

# Поиск GTK3 через pkg-config
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Добавление исполняемого файла
add_executable(RGZ2 main.cpp)

# Подключение GTK3
#target_include_directories(RGZ2 PRIVATE ${GTK3_INCLUDE_DIRS})
#target_link_directories(RGZ2 PRIVATE ${GTK3_LIBRARY_DIRS})
#target_link_libraries(RGZ2 PRIVATE ${GTK3_LIBRARIES})
#target_compile_options(RGZ2 PRIVATE ${GTK3_CFLAGS_OTHER})