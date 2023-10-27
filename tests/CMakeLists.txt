cmake_minimum_required(VERSION 3.16)
project(mytest LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set (Tests test_xiangqi_app.cpp
    test_xiangqi_app.h)

enable_testing(true)

qt_add_executable(mytest test_xiangqi_app.cpp test_xiangqi_app.h
        ../sources/mainwindow.cpp
        ../sources/mainwindow.h
        ../sources/boardview.h ../sources/boardview.cpp
        ../sources/basemodel.h ../sources/basemodel.cpp
        ../sources/uci.cpp ../sources/uci.h
        ../sources/genmove.h ../sources/genmove.cpp
        ../sources/types.h
        ../sources/engine.h ../sources/engine.cpp
        ../sources/settingsview.h ../sources/settingsview.cpp
        ../sources/board.h ../sources/board.cpp
        ../sources/contexmenu.h ../sources/contexmenu.cpp)

add_test(NAME mytest COMMAND mytest)

target_link_libraries(mytest PRIVATE Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets)


