configure_file(config.h.in config.h)

set(PROJECT_CPP
        main.cpp
        mainwindow.cpp mainwindow.h
        boardview.h boardview.cpp
        basemodel.h basemodel.cpp
        uci.cpp uci.h
        genmove.h genmove.cpp
        types.h
        engine.h engine.cpp
        settingsview.h settingsview.cpp
        board.h board.cpp
        res.qrc
        )

qt_add_resources(RESOURCES res.qrc)

#qt_add_translation(QM_FILES ${TS_FILES})

add_executable(ElephantChess ${PROJECT_CPP} ${RESOURCES}
    contexmenu.h contexmenu.cpp)
target_link_libraries(ElephantChess PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

set_target_properties(ElephantChess PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE FALSE
)




