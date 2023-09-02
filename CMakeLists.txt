cmake_minimum_required(VERSION 3.5)

project(XiangQi VERSION 0.2 LANGUAGES CXX)
configure_file(Config.h.in Config.h)
enable_testing(true)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Widgets Test LinguistTools)

set(TS_FILES XiangQi_de_DE.ts)

set(PROJECT_WEBSITE "docs/lizenzen.html"
    "docs/robots.txt"
    "docs/Screenshot.jpg"
    "docs/sitemaps.txt"
    "docs/sitemaps.xml"
    "docs/style.css"
    "docs/test"
    "docs/.htaccess"
    "docs/apple-touch-icon-120x120-precomposed.png"
    "docs/apple-touch-icon-152x152-precomposed.png"
    "docs/datenschutz.html"
    "docs/download.html"
    "docs/favicon.ico"
    "docs/footer.html"
    "docs/header.html"
    "docs/index.html"
    "docs/jquery-3.7.0.min.js"
    "docs/links.html"
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        boardview.h boardview.cpp
        basemodel.h basemodel.cpp
        res.qrc
        uci.cpp uci.h
        genmove.h genmove.cpp
        types.h
        Roadmap.md
        engine.h engine.cpp
        deployment.txt
        settingsview.h settingsview.cpp
        board.h board.cpp
        ${TS_FILES}
        ${PROJECT_WEBSITE}
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(XiangQi
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET XiangQi APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(XiangQi SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(XiangQi
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_executable(test_xiangqi_app test_xiangqi_app.cpp test_xiangqi_app.h
    genmove.cpp genmove.h types.h boardview.cpp boardview.h board.cpp board.h
    basemodel.cpp basemodel.h res.qrc engine.cpp engine.h uci.cpp uci.h settingsview.cpp settingsview.h
    mainwindow.cpp mainwindow.h)
target_link_libraries(test_xiangqi_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

#add_executable(Chameleon Chameleon/LICENSE
#    Chameleon/README.md
#    Chameleon/bitboard.cpp
#    Chameleon/bitboard.h
#    Chameleon/bitcount.h
#    Chameleon/endgame.cpp
#    Chameleon/endgame.h
#    Chameleon/evaluate.cpp
#    Chameleon/evaluate.h
#    Chameleon/init.cpp
#    Chameleon/init.h
#    Chameleon/magics.h
#    Chameleon/main.cpp
#    Chameleon/material.cpp
#    Chameleon/material.h
#    Chameleon/misc.cpp
#    Chameleon/misc.h
#    Chameleon/movegen.cpp
#    Chameleon/movegen.h
#    Chameleon/movepick.cpp
#    Chameleon/movepick.h
#    Chameleon/pawns.cpp
#    Chameleon/pawns.h
#    Chameleon/position.cpp
#    Chameleon/position.h
#    Chameleon/psqt.cpp
#    Chameleon/search.cpp
#    Chameleon/search.h
#    Chameleon/thread.cpp
#    Chameleon/thread.h
#    Chameleon/thread_win32.h
#    Chameleon/timeman.cpp
#    Chameleon/timeman.h
#    Chameleon/tt.cpp
#    Chameleon/tt.h
#    Chameleon/types.h
#    Chameleon/uci.cpp
#   Chameleon/uci.h
#    Chameleon/ucioption.cpp
#)


target_link_libraries(XiangQi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.boringgames.XiangQi)
endif()
set_target_properties(XiangQi PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS XiangQi
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(XiangQi)
endif()

target_include_directories(XiangQi PUBLIC
    "${PROJECT_BINARY_DIR}")
                           #"${PROJECT_SOURCE_DIR}/pgnlib")
                           #"${PROJECT_SOURCE_DIR}/Chameleon")
