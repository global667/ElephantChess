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
#include <cchess_rules.h>


extern BaseModel basemodel;

Engine::Engine() {}

std::pair<QPoint, QPoint> Engine::GetBestMove(color color)
{
    //GenMove generatedMoves(basemodel.board.pieces, color);
    Position position(basemodel.board.pieces, color);
 /*   std::vector<std::pair<QPoint, QPoint>> pos;
    std::vector<std::pair<QPoint, QPoint>> posAll;

    // all valid moves
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
    {
            if (generatedMoves.GetColor({i, j}) == color) {
                pos = generatedMoves.AllValidMoves(QPoint{i, j});
                for (std::pair<QPoint, QPoint> p : pos) {
                    posAll.push_back(p);
                    //qDebug() << p.first.row << p.first.col;
                }
            }
        }

    int sizeOfPosAll = posAll.size();

    int random = QRandomGenerator::global()->bounded(sizeOfPosAll);

    return posAll.at(random);*/

    std::vector<std::pair<int, int>> moves;
    std::vector<std::pair<int, int>> all_moves_to;
    std::vector<std::pair<int, int>> all_moves_from;

    srand(time(NULL));

    // find all possible moves
    for (int file1 = 0; file1 < 9; file1++) {
        for (int rank1 = 0; rank1 < 10; rank1++) {
            if (position.board[rank1][file1].piece != nullptr)
                if (position.board[rank1][file1].piece->color == position.players_color
                    && position.is_inside_board(file1, rank1)) {
                    PieceType piece_type = position.board[rank1][file1].piece->piece_type;
                    moves = position.generate_piece_moves(piece_type, file1, rank1);
                    for (auto m : moves) {
                        all_moves_from.push_back(std::make_pair(file1, rank1));
                        all_moves_to.push_back(m);
                    }
                }
        }
    }

    // select one valid move randomly
    int move = rand() % all_moves_from.size();

    char f1 = all_moves_from[move].first;
    f1 = f1 + 'a';
    char t1 = all_moves_to[move].first;
    t1 = t1 + 'a';

    std::string bestmove = f1 + std::to_string(all_moves_from[move].second) + t1
                           + std::to_string(all_moves_to[move].second);

    return std::make_pair(QPoint(all_moves_from[move].first,all_moves_from[move].second), QPoint(all_moves_to[move].first,all_moves_to[move].second)); //bestmove;
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
