cmake_minimum_required(VERSION 3.28)
project(calendar_app C)

set(CMAKE_C_STANDARD 17)

add_executable(calendar_app src/main.c
        src/option_handlers.c
        include/option_handlers.h
        include/utils.h
        src/utils.c
        include/notes.h
        src/notes.c)
