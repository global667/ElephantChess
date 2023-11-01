/*
  ElephantChess, a UCI chinese chess playing GUI with builtin engine
  Copyright (C) 2022-2023 Wolf S. Kappesser

  ElephantChess is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ElephantChess is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TYPES_H
#define TYPES_H

#include <QImage>
#include <QPoint>

// A chinese chess board in ascii
//    a b c d e f g h i
//    -------------------
//    9|r h e a g a e h r|9
//    8| | | | | | | | | |8
//    7| | c | | | | c | |7
//    6|s| |s| |s| |s| |s|6
//    5| | | | | | | | | |5
//    4| | | | | | | | | |4
//    3|s| |s| |s| |s| |s|3
//    2| | c | | | | c | |2
//    1| | | | | | | | | |1
//    -------------------
//    a b c d e f g h i

#define ROWS 10
#define COLS 9

enum class viewStyleMode { traditional_native, traditional_png, western_png };

enum class pieceType { Empty, General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier };

enum class color { Red, Black };

// class Piece
class Piece
{
public:
    Piece() {}
    Piece(color colr, pieceType type, QPoint pos, QImage img, QString name)
        : colr(colr)
        , type(type)
        , pos(pos)
        , img(img)
        , name(name)
    {
    }

    Piece(color colr, pieceType type, QPoint pos, QString name)
        : colr(colr)
        , type(type)
        , pos(pos)
        , name(name)
    {
    }

    virtual ~Piece() {}

    QImage img;
    color colr;
    pieceType type;
    QPoint pos;
    QString name;
};

enum class markerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };

enum class completePieceType {
    GeneralRot,
    AdvisorRot,
    ElephantRot,
    HorseRot,
    ChariotRot,
    CannonRot,
    SoldierRot,
    GeneralSchwarz,
    AdvisorSchwarz,
    ElephantSchwarz,
    HorseSchwarz,
    ChariotSchwarz,
    CannonSchwarz,
    SoldierSchwarz
};

#endif // TYPES_H
