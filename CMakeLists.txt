cmake_minimum_required(VERSION 3.15)
project(SkittlesSim C)

set(CMAKE_C_STANDARD 99)

add_executable(SkittlesSim SkittlesSim.c SkittlesSim.h)
add_executable(SkittlesSimSerial SkittlesSimSerial.c SkittlesSim.h)