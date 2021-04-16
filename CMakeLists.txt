cmake_minimum_required(VERSION 2.6)
project(mergin)

add_executable(mergin mergin.c unifile.h unifile.c)

add_custom_command(TARGET mergin POST_BUILD COMMAND "${PROJECT_SOURCE_DIR}/test.sh")
