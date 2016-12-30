cmake_minimum_required(VERSION 3.6)
project(filesystem)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c fs.c fs.h)
add_executable(filesystem ${SOURCE_FILES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(filesystem Threads::Threads)
