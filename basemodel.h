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

struct BaseModel : public QObject
{
    Q_OBJECT
public:
    BaseModel(QObject *parent = nullptr)
    {
        engineName
            = "built-in";//"F:/source/XiangQi/build-Dumbo-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/Dumbo"; //"/home/wsk/.vs/Dumbo/out/build/linux-debug/Dumbo"; //
        position.initBoard();
        moveHistory.append(position);
        currentMove++;
    }

    static const short BoardRows = ROWS;
    static const short BoardCols = COLS;
    static const short BoardRowPoints = BoardRows - 1;
    static const short BoardColPoints = BoardCols - 1;

    Position position;

    viewStyleMode viewStyleModeVar = viewStyleMode::traditional_native;

    int currentMove = 0;
    QStringList moves;
    QString engineName = "built-in";
    QList<Position> moveHistory;
    QString kind = "uci";

    QList<QPair<QPoint, QPoint>> currentMoves;
    QPoint fromHuman = {-1, -1};
    QPoint toHuman = {-1, -1};
    QPoint fromUCI = {-1, -1};
    QPoint toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    Color gameView = Color::Red;

    QByteArray posToken(int fromX, int fromY, int toX, int toY)
    {
        QByteArray m;
        char c1 = fromX + 'a';
        char c2 = fromY + '0';
        char c3 = toX + 'a';
        char c4 = toY + '0';
        m.append(c1);
        m.append(c2);
        m.append(c3);
        m.append(c4);
        return m;
    }
};

#endif // BASEMODEL_H
