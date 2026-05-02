cmake_minimum_required(VERSION 3.16)
# Link-only smoke for installed awtk-mvvm. Set CMAKE_PREFIX_PATH to include
# both the awtk-mvvm install prefix and the AWTK install prefix (semicolon-separated).
# Windows cmd example:
#   cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/mvvm-install;C:/awtk-install
project(awtk_mvvm_smoke_find_package LANGUAGES C)

find_package(awtk-mvvm REQUIRED CONFIG)

add_executable(smoke_find_package main.c)
target_link_libraries(smoke_find_package PRIVATE awtk_mvvm::mvvm)
