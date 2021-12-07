cmake_minimum_required(VERSION 3.13)

project(glad LANGUAGES CXX)

add_library(glad STATIC "glad.c" "glad.h" "KHR/khrplatform.h")

target_include_directories(glad PRIVATE "${PROJECT_SOURCE_DIR}/../")
