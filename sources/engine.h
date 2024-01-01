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

#ifndef ENGINE_H
#define ENGINE_H

#include <QRandomGenerator>
#include "basemodel.h"
//#include "genmove.h"
//#include "maschines.h"
#include "types.h"

// chinese chess engine
class Engine : public QObject
{
    Q_OBJECT
public:
    Engine();
    ~Engine() {}

    std::pair<QPoint, QPoint> GetBestMove(color color);

    // get all possible moves for a piece
    // return a list of positions
    QList<QPoint> GetPossibleMoves(Piece *piece);

    // get all possible moves for a color
    // return a list of positions
    QList<QPoint> GetPossibleMoves(color color);

    void engineGo();

    //void MovePiece(position from, position to);

    // For Players
    //void MovePiece(int fromX, int fromY, int toX, int toY);
    //void start(){};

private:
    //QByteArray posToken(int fromX, int fromY, int toX, int toY);

signals:
    void updateView(QPoint from, QPoint to, QString kind);
};

#endif // ENGINE_H
