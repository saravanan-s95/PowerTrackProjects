cmake_minimum_required(VERSION 3.16)

project(MP3_Tags
    VERSION 1.0
    DESCRIPTION "An MP3 Tag Reader and Editor"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(MP3_TagReader
    main.c
    view.c
    edit.c
    mp3.c
    render.c
    helpers.c
)

target_compile_options(MP3_TagReader
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
