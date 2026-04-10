set(Qt6_DIR "C:/Qt/qt_sample_app/ext/qt-6.12.0-win10-22H2-x64-msvc-194/lib/cmake/Qt6")
cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_add_executable(client
    main.cpp
    mainwindow.cpp
    api_client.cpp
)

target_link_libraries(client PRIVATE
    Qt6::Widgets
    Qt6::Network
)