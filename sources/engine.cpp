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

#include "engine.h"

extern BaseModel basemodel;

Engine::Engine() {}

std::pair<QPoint, QPoint> Engine::GetBestMove(color color)
{
    GenMove generatedMoves(basemodel.board.pieces, color);

    std::vector<std::pair<QPoint, QPoint>> pos;
    std::vector<std::pair<QPoint, QPoint>> posAll;

    // all valid moves
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
    {
            if (generatedMoves.GetColor({i, j}) == color) {
                pos = generatedMoves.IsValidPieceMove(QPoint{i, j});
                for (std::pair<QPoint, QPoint> p : pos) {
                    posAll.push_back(p);
                    //qDebug() << p.first.row << p.first.col;
                }
            }
        }

    int sizeOfPosAll = posAll.size();

    int random = QRandomGenerator::global()->bounded(sizeOfPosAll);

    return posAll.at(random);
}

QList<QPoint> Engine::GetPossibleMoves(Piece *piece)
{
    return QList<QPoint>();
}

QList<QPoint> Engine::GetPossibleMoves(color color)
{
    return QList<QPoint>();
}

void Engine::engineGo()
{
    std::pair<QPoint, QPoint> ownEngineMove;
    ownEngineMove = GetBestMove(basemodel.board.onMove);
    basemodel.currentMoves.push_back(ownEngineMove);
    emit updateView(ownEngineMove.first, ownEngineMove.second, "engine");
}
