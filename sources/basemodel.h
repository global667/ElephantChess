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

#include "board.h"
#include "types.h"

class BaseModel : public QObject
{
    Q_OBJECT
public:
    explicit BaseModel(QObject *parent = nullptr){};

    static const short BoardRows = ROWS;
    static const short BoardCols = COLS;
    static const short BoardRowPoints = BoardRows - 1;
    static const short BoardColPoints = BoardCols - 1;

    // a class that holds all board data
    Board board;
    Board boardCopy;

    int currentMove = 0;
    QStringList moves;
    QString engineName = "native";
    QList<Board> moveHistory;

    position fromHuman = {-1, -1};
    position toHuman = {-1, -1};
    position fromUCI = {-1, -1};
    position toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    color gameView = color::Red;

    // legt fest welche Farbe der Mensch spielt
    color humanColor = color::Red;

    QString engine = "chameleon";
};

#endif // BASEMODEL_H
