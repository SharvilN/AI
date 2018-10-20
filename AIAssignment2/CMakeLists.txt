cmake_minimum_required(VERSION 3.8)
project(AIAssignment2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Session.cpp Session.h SimulatedAnnealing.cpp SimulatedAnnealing.h)
add_executable(AIAssignment2 ${SOURCE_FILES})