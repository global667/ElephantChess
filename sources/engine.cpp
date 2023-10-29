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

std::pair<position, position> Engine::GetBestMove(color color)
{
    GenMove generatedMoves(basemodel.board.pieces, color);

    std::vector<std::pair<position, position>> pos;
    std::vector<std::pair<position, position>> posAll;

    // all valid moves
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
    {
            if (generatedMoves.GetColor({i, j}) == color) {
                pos = generatedMoves.IsValidPieceMove(
                    position{i, j}); //generateLegalMoves(Color::Black);
                for (std::pair<position, position> p : pos) {
                    posAll.push_back(p);
                    //qDebug() << p.first.row << p.first.col;
                }
            }
        }

    // Is in Check?
    if (generatedMoves.IsCheck(color)) {
        qDebug() << "Check";
        //statusBar()->showMessage("Check");
        //return;
    }

    // Is in Checkmate?
    if (generatedMoves.IsCheckmate(color)) {
        qDebug() << "Checkmate";
        //statusBar()->showMessage("Checkmate");
        //return;
    }

    int sizeOfPosAll = posAll.size();

    int random = QRandomGenerator::global()->bounded(sizeOfPosAll);

    return posAll.at(random);
}

QList<position> Engine::GetPossibleMoves(Piece *piece)
{
    return QList<position>();
}

QList<position> Engine::GetPossibleMoves(color color)
{
    return QList<position>();
}

void Engine::engineGo()
{
    std::pair<position, position> ownEngineMove;
    ownEngineMove = GetBestMove(basemodel.board.onMove);
    basemodel.currentMoves.push_back(ownEngineMove);
    emit updateView(ownEngineMove.first, ownEngineMove.second, "engine");
}
