cmake_minimum_required(VERSION 3.28)
project(calendar_app C)

set(CMAKE_C_STANDARD 17)

add_executable(calendar_app src/main.c
        src/option_handlers.c
        include/option_handlers.h
        include/utils.h
        src/utils.c
        include/notes.h
        src/notes.c
        src/background_log.c
        include/background_log.h
        src/http_handler.c
        include/http_handler.h
)

find_package(CURL REQUIRED)
target_link_libraries(calendar_app PRIVATE CURL::libcurl)

find_package(cJSON CONFIG REQUIRED)
target_link_libraries(calendar_app PRIVATE cjson)
