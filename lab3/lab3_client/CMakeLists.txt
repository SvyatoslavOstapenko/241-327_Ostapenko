cmake_minimum_required(VERSION 3.16)

project(lab3_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

qt_standard_project_setup()

qt_add_executable(lab3_client
    main.cpp

    book.h
    book.cpp

    httpclient.h
    httpclient.cpp

    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(lab3_client
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Widgets
)