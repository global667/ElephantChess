cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pgnLib VERSION 0.1 LANGUAGES CXX)

add_executable(pngLib
    PGNCommentText.cpp
    PGNCommentText.h
    PGNException.cpp
    PGNException.h
    PGNGame.cpp
    PGNGame.h
    PGNGameCollection.cpp
    PGNGameCollection.h
    PGNGameResult.cpp
    PGNGameResult.h
    PGNMove.cpp
    PGNMove.h
    PGNMoveList.cpp
    PGNMoveList.h
    PGNParser.cpp
    PGNParser.h
    PGNPiece.cpp
    PGNPiece.h
    PGNPly.cpp
    PGNPly.h
    PGNPosition.cpp
    PGNPosition.h
    PGNSquare.cpp
    PGNSquare.h
    PGNTag.cpp
    PGNTag.h
    PGNTagList.cpp
    PGNTagList.h
    main.cpp
    sample.pgn
)
