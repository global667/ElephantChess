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

//#include "cchess_rules.h"
//#include "board.h"
//#include "game.h"

#include "basetypes.h"
#include "board.h"
//#include "cchess_rules.h"



// TODO: delete doublettes i.e. engineName and engineData.engineName
struct BaseModel //: public QObject
{
    //Q_OBJECT
public:
    BaseModel() //QObject *parent = nullptr)
        : nodes(0), engineData({0, 0, 0, "", ""}) {
        //engineName
        //        = "ElephantChessEngine";

        position.setupInitialPositions();
        moveHistory.append(position);
        //currentMove++;
    }

    static constexpr short BoardRows = ROWS;
    static constexpr short BoardCols = COLS;
    static constexpr short BoardRowPoints = BoardRows - 1;
    static constexpr short BoardColPoints = BoardCols - 1;

    //Position position;
    Board position;

    enum class viewStyleMode { traditional_native, traditional_png, western_png };

    viewStyleMode viewStyleModeVar = viewStyleMode::traditional_native;
    bool nativePieces = true;

    int currentMove = 0;
    QStringList moves;
    //QString engineName = "built-in";
    QList<Board> moveHistory{};

    //QString kind = "human";
    enum class Mode { human, engine, uci, tipp, movenow };

    Mode mode = Mode::human;

    QList<QPair<Point, Point> > currentMoves;
    Point fromHuman = {-1, -1};
    Point toHuman = {-1, -1};
    Point fromUCI = {-1, -1};
    Point toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    Color gameView = Color::Red;

    static QByteArray posToken(const int fromX, const int fromY, const int toX, const int toY) {
        char c[4];
        c[0] = static_cast<char>(fromY + 'a');
        c[1] = static_cast<char>(fromX + '0');
        c[2] = static_cast<char>(toY + 'a');
        c[3] = static_cast<char>(toX + '0');
        return {c, sizeof(c)};
    }

    long nodes;

    // engine evaluation, search depth etc. for the gui
    struct EngineData {
        int evaluation{};
        int searchDepth{};
        long nodes{};
        QString bestMove;
        QString engineName;
    } engineData;
};

#endif // BASEMODEL_H
