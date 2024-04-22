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

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QObject>

#include "cchess_rules.h"

enum class markerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };

static constexpr int ROWS = 10;
static constexpr int COLS = 9;

enum class completePieceType {
    GeneralRot, AdvisorRot, ElephantRot,
    HorseRot, ChariotRot, CannonRot,
    SoldierRot, GeneralSchwarz, AdvisorSchwarz,
    ElephantSchwarz, HorseSchwarz, ChariotSchwarz,
    CannonSchwarz, SoldierSchwarz
};

struct BaseModel //: public QObject
{
    //Q_OBJECT
public:
    BaseModel()//QObject *parent = nullptr)
    {
        engineName
            = "PikaFish";//"C:/Users/wolfk/Downloads/Pikafish.2024-03-10/Windows/pikafish-ssse3.exe";//"built-in";//"F:/source/XiangQi/build-Dumbo-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/Dumbo"; //"/home/wsk/.vs/Dumbo/out/build/linux-debug/Dumbo"; //
        position.setupInitialPositions();
        moveHistory.append(position);
        currentMove++;
    }


    static constexpr short BoardRows = ROWS;
    static constexpr short BoardCols = COLS;
    static constexpr short BoardRowPoints = BoardRows - 1;
    static constexpr short BoardColPoints = BoardCols - 1;

    //Position position;
    Board position;

    enum class viewStyleMode { traditional_native, traditional_png, western_png };
    viewStyleMode viewStyleModeVar = viewStyleMode::traditional_native;

    int currentMove = 0;
    QStringList moves;
    QString engineName = "built-in";
    QList<Board> moveHistory;
    QString kind = "human";

    QList<QPair<Point, Point>> currentMoves;
    Point fromHuman = {-1, -1};
    Point toHuman = {-1, -1};
    Point fromUCI = {-1, -1};
    Point toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    Color gameView = Color::Red;

    QByteArray posToken(int fromX, int fromY, int toX, int toY)
    {
        char c[4];
        c[0] = fromY + 'a';
        c[1] = fromX + '0';
        c[2] = toY + 'a';
        c[3] = toX + '0';
        return QByteArray(c,sizeof(c));
    }

    long nodes;
};

#endif // BASEMODEL_H
