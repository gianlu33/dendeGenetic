cmake_minimum_required(VERSION 3.13)
project(dendeGenetic)

set(CMAKE_CXX_STANDARD 17)

add_executable(dendeGenetic main.cpp IOManager.cpp IOManager.h Solution.cpp Solution.h Genetic.cpp Genetic.h AnalysisManager.cpp AnalysisManager.h LocalSearchManager.cpp LocalSearchManager.h ctpl_stl.h ProcessManager.cpp ProcessManager.h)