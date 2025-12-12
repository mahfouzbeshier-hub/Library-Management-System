cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/BookList.cpp
    src/BookList.h
)

add_executable(LibraryManagementSystem
    ${PROJECT_SOURCES}
)

target_link_libraries(LibraryManagementSystem PRIVATE Qt6::Widgets)

# Set properties for Windows to avoid console window if needed,
# but for debugging let's keep it or make it WIN32
set_target_properties(LibraryManagementSystem PROPERTIES
    WIN32_EXECUTABLE ON
)
