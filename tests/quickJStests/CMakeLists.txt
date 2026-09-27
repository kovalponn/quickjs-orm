cmake_minimum_required(VERSION 3.16)

# ВАЖНО: объявляем оба языка — C для QuickJS, CXX для main.cpp
project(quickjs_hltest LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QJS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/quickjs)

# ---------- Статическая библиотека QuickJS (только C) ----------
add_library(quickjs STATIC
    ${QJS_DIR}/quickjs.c
    ${QJS_DIR}/libregexp.c
    ${QJS_DIR}/libunicode.c
    ${QJS_DIR}/cutils.c
    ${QJS_DIR}/libbf.c
)

target_include_directories(quickjs PUBLIC ${QJS_DIR})

# _GNU_SOURCE нужен на Linux/macOS для strdup, strndup, pthread и т.п.
if(NOT WIN32)
    target_compile_definitions(quickjs PRIVATE _GNU_SOURCE)
endif()

# Отключаем предупреждения C-кода QuickJS (его много, и они не наши)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(quickjs PRIVATE -w)
endif()

# libm нужна для математики QuickJS
if(NOT WIN32)
    find_library(M_LIB m)
    if(M_LIB)
        target_link_libraries(quickjs PUBLIC ${M_LIB})
    endif()
endif()

# Потоки (нужны quickjs-libc и иногда самому рантайму)
find_package(Threads REQUIRED)
target_link_libraries(quickjs PUBLIC Threads::Threads)

# ---------- quickjs-libc (тоже C) ----------
add_library(quickjs_libc STATIC
    ${QJS_DIR}/quickjs-libc.c
)
target_include_directories(quickjs_libc PUBLIC ${QJS_DIR})
target_link_libraries(quickjs_libc PUBLIC quickjs)

# ---------- Наш C++ исполняемый файл ----------
add_executable(hltest _HLTEST/main.cpp)

# Включаем папку quickjs для заголовков
target_include_directories(hltest PRIVATE ${QJS_DIR})

# Линкуем QuickJS и libc-модуль
target_link_libraries(hltest PRIVATE quickjs_libc)

# QuickJS-заголовки написаны на C — чтобы компилятор C++ корректно
# понял extern "C", подключаем их через обёртку (см. ниже),
# либо полагаемся на то, что quickjs.h уже имеет extern "C" внутри.