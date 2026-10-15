cmake_minimum_required(VERSION 3.16)
project(recruit_stats LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(main main.c stats.c format.c)
