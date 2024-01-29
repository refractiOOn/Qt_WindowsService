cmake_minimum_required(VERSION 3.24)

project(QtService LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(CXX_SOURCE_FILES
    main.cpp
    ThreadPool.hpp
    ServiceInstaller.hpp ServiceInstaller.cpp
    ServiceBase.hpp ServiceBase.cpp
    SampleService.hpp SampleService.cpp
)

add_executable(${PROJECT_NAME} ${CXX_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt6::Core)
