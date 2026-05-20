cmake_minimum_required(VERSION 3.16)

project(lab3_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(lab3_client
    main.cpp
)

target_link_libraries(lab3_client
    PRIVATE Qt6::Core Qt6::Network
)