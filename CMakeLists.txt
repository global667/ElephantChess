cmake_minimum_required(VERSION 3.6)

find_package(Git REQUIRED)
# Get the latest commit hash
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

project(ElephantChess VERSION 0.4.4 DESCRIPTION "${COMMIT_ID}" LANGUAGES CXX HOMEPAGE_URL "https/www.elephant-chess.com")

set(PROJECT_DESC "-alpha (commit ${GIT_COMMIT_HASH})")

configure_file(sources/config.h.in sources/config.h)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)
enable_language(CXX)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets    
    Concurrent
    #3DCore
    #3DRender
    #3DInput
    #3DLogic
    #3DExtras
    #3DAnimation
    #Qml
    #Quick
    #Quick3D
    #Test
    #LinguistTools
    HINTS "C:/Qt/6.7.0/msvc2019_64"
)

qt6_standard_project_setup()
qt_add_resources(RESOURCES sources/res.qrc)

set(PROJECT_CPP
        sources/main.cpp #sources/config.h
        sources/mainwindow.cpp sources/mainwindow.h
        sources/boardview.h sources/boardview.cpp
        sources/basemodel.h sources/basemodel.cpp
        sources/uci.cpp sources/uci.h
        sources/engine.h sources/engine.cpp
        sources/settingsview.h sources/settingsview.cpp
        sources/contexmenu.h sources/contexmenu.cpp
        sources/aboutview.h sources/aboutview.cpp
        sources/cchess_rules.h sources/cchess_rules.cpp
)

add_executable(ElephantChess WIN32 ${PROJECT_CPP} ${RESOURCES}
)

target_link_libraries(ElephantChess PUBLIC
    Qt6::Core
    Qt::Widgets
    Qt::Concurrent
    #Qt::Test
    #Qt::3DCore
    #Qt::3DRender
    #Qt::3DLogic
    #Qt::3DExtras
    #Qt::3DAnimation
    #Qt::Qml
    #Qt::Quick
    #Qt::Quick3D
)

set_target_properties(ElephantChess PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

#qt_finalize_executable(ElephantChess)

#enable_testing(true)
#add_subdirectory(tests)
#add_test(NAME ElephantChess COMMAND ElephantChess)





#target_include_directories(ElephantChess PUBLIC "${CMAKE_SOURCE_DIR}../../cchess_rules"
#)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(XiangQi
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET XiangQi APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

#    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#else()
#    if(ANDROID)
#        add_library(XiangQi SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(XiangQi
#            ${PROJECT_SOURCES}
#        )
#    endif()

#    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#endif()

#target_link_libraries(XiangQi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
#if(${QT_VERSION} VERSION_LESS 6.1.0)
#  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.boringgames.XiangQi)
#endif()
##set_target_properties(XiangQi PROPERTIES
##    ${BUNDLE_ID_OPTION}
##    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
##    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
##    MACOSX_BUNDLE FALSE
##    WIN32_EXECUTABLE TRUE
##)

#include(GNUInstallDirs)
#install(TARGETS XiangQi
#    BUNDLE DESTINATION .
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(XiangQi)
#endif()

#target_include_directories(XiangQi PUBLIC
#    "${PROJECT_BINARY_DIR}")
#                           #"${PROJECT_SOURCE_DIR}/pgnlib")
#                           #"${PROJECT_SOURCE_DIR}/Chameleon")

