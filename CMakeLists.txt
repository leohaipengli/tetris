cmake_minimum_required(VERSION 3.8)
project(A1_code)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/Angel.h
        include/CheckError.h
        include/InitShader.cpp
        include/mat.h
        include/vec.h
        Gasket.cpp)

add_executable(A1_code ${SOURCE_FILES})