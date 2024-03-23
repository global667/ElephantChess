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

std::pair<QPoint, QPoint> Engine::GetBestMove(Color color)
{
    std::vector<std::pair<QPoint, QPoint>> posAll = basemodel.position.generate_all_valid_moves();

    size_t sizeOfPosAll = posAll.size();

    qDebug () << "Depth: 1" << "Nodes: " << sizeOfPosAll;

    if (sizeOfPosAll == 0)
        qDebug() << "Checkmate for black";

    int random = 0;
    if (sizeOfPosAll > 0) {
       random = QRandomGenerator::global()->bounded(sizeOfPosAll);
       return posAll.at(random);
    } else
       return std::pair(QPoint(-1,-1),QPoint(-1,-1));
}

std::pair<QPoint, QPoint> Engine::engineGo()
{
    std::pair<QPoint, QPoint> ownEngineMove;
    ownEngineMove = GetBestMove(basemodel.position.players_color);
    basemodel.currentMoves.push_back(ownEngineMove);
    return ownEngineMove;
}
