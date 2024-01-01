set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(C)
enable_language(CXX)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    3DCore
    3DRender
    3DInput
    3DLogic
    3DExtras
    3DAnimation
    #Test
    #LinguistTools
    HINTS "C:/Qt/6.6.1/mingw_64/"
)
find_library(CCHESS_RULES_LIBRARY
    NAMES libcchess_rules
    HINTS "f:/source/XiangQi/build-cchess_rules-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/"
)

set(PROJECT_CPP
        main.cpp
        mainwindow.cpp mainwindow.h
        boardview.h boardview.cpp
        basemodel.h basemodel.cpp
        uci.cpp uci.h        
        types.h
        engine.h engine.cpp
        settingsview.h settingsview.cpp
        board.h board.cpp
        config.h contexmenu.h contexmenu.cpp
        renderview.h renderview.cpp
        aboutview.h aboutview.cpp
        #res.qrc
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(ElephantChess ${PROJECT_CPP} ${RESOURCES})

target_link_libraries(ElephantChess PRIVATE
    Qt6::Core
    #Qt::Gui
    Qt::Widgets
    #Qt::Test
    Qt::3DCore
    Qt::3DRender
    Qt::3DLogic
    Qt::3DExtras
    Qt::3DAnimation
    ${CCHESS_RULES_LIBRARY}
)
#qt_add_translation(QM_FILES ${TS_FILES})

#target_link_directories(ElephantChess PUBLIC "F:/source/XiangQi/build-cchess_rules-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/")
target_include_directories(ElephantChess PUBLIC "../../cchess_rules/")

