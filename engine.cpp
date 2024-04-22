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

std::vector<std::vector<int>> soldierValueTable = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20, 22, 24, 26, 28, 26, 24, 22, 20},
    {22, 24, 26, 28, 30, 28, 26, 24, 22},
    {24, 26, 28, 30, 32, 30, 28, 26, 24},
    {26, 28, 30, 32, 34, 32, 30, 28, 26},
    {28, 30, 32, 34, 36, 34, 32, 30, 28},
    {30, 32, 34, 36, 38, 36, 34, 32, 30},
    {32, 34, 36, 38, 40, 38, 36, 34, 32}
};


const std::vector<std::vector<int>> generalValueTable = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 10, 12, 10, 0, 0, 0},
    {0, 0, 0, 8, 10, 8, 0, 0, 0},
    {0, 0, 0, 10, 12, 10, 0, 0, 0}
};

const std::vector<std::vector<int>> advisorValueTable = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 6, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 9, 0, 8, 0, 0},
    {0, 0, 0, 6, 0, 6, 0, 0, 0}
};

const std::vector<std::vector<int>> elephantValueTable = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 15, 0, 0, 0, 15, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {5, 0, 0, 0, 15, 0, 0, 0, 5},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 10, 0, 0, 0, 10, 0, 0}
};

const std::vector<std::vector<int>> horseValueTable = {
    {0, 4, 6, 8, 10, 8, 6, 4, 0},
    {4, 6, 8, 10, 12, 10, 8, 6, 4},
    {6, 8, 10, 12, 14, 12, 10, 8, 6},
    {8, 10, 12, 14, 16, 14, 12, 10, 8},
    {10, 12, 14, 16, 18, 16, 14, 12, 10},
    {10, 12, 14, 16, 18, 16, 14, 12, 10},
    {8, 10, 12, 14, 16, 14, 12, 10, 8},
    {6, 8, 10, 12, 14, 12, 10, 8, 6},
    {4, 6, 8, 10, 12, 10, 8, 6, 4},
    {0, 4, 6, 8, 10, 8, 6, 4, 0}
};


std::vector<std::vector<int>> cannonValueTable = {
    {18, 18, 18, 18, 18, 18, 18, 18, 18},
    {14, 14, 14, 14, 14, 14, 14, 14, 14},
    {10, 10, 10, 10, 10, 10, 10, 10, 10},
    {8, 8, 8, 8, 8, 8, 8, 8, 8},
    {6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 8, 8, 8, 8, 8, 8, 8},
    {10, 10, 10, 10, 10, 10, 10, 10, 10},
    {14, 14, 14, 14, 14, 14, 14, 14, 14},
    {18, 18, 18, 18, 18, 18, 18, 18, 18}
};

const std::vector<std::vector<int>> chariotValueTable = {
    {20, 20, 20, 20, 20, 20, 20, 20, 20},
    {15, 15, 15, 15, 15, 15, 15, 15, 15},
    {12, 12, 12, 12, 12, 12, 12, 12, 12},
    {10, 10, 10, 10, 10, 10, 10, 10, 10},
    {8, 8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8, 8},
    {10, 10, 10, 10, 10, 10, 10, 10, 10},
    {12, 12, 12, 12, 12, 12, 12, 12, 12},
    {15, 15, 15, 15, 15, 15, 15, 15, 15},
    {20, 20, 20, 20, 20, 20, 20, 20, 20}
};

Engine::Engine() {}

std::pair<Point, Point> Engine::GetBestMove(Color color)
{
    std::vector<std::pair<Point, Point>> posAll = basemodel.position.getAllValidMoves(color, basemodel.position.board);

    if (posAll.empty())
        return std::make_pair(Point(-1, -1), Point(-1, -1));

    std::pair<Point, Point> bestmove = posAll[0];
    int bestScore = 0;

    for (auto &move : posAll) {
         std::vector<std::vector<std::shared_ptr<Piece>>> new_board = basemodel.position.board;
         std::shared_ptr<Piece> piece = new_board[move.first.x][move.first.y];
         basemodel.position.movePiece(move.first, move.second, new_board);
         int score = evaluatePosition(new_board); //-search(depth - 1, color == Color::Red ? Color::Black : Color::Red);
         basemodel.position.undoMove(move.first, move.second, piece, new_board);
         if (score > bestScore) {
            bestScore = score;
            bestmove = move;
         }
    }
    return bestmove;
}

int Engine::evaluatePosition(const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
    int score = 0;

    if (basemodel.position.isCheckmate(Color::Black, board))
        return 1000000;

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 9; y++) {
            auto piece = board[x][y];
            if (!piece->name.isEmpty()) {
                int pieceValue = getPieceValue(&piece);
                int positionValue = getPositionValue(&piece, x, y);
                //score += piece->getColor() == Color::Red ? (pieceValue + positionValue) : -(pieceValue + positionValue);
                score += pieceValue + positionValue;
            }
        }
    }
    return score;
}

int Engine::getPieceValue(const std::shared_ptr<Piece> *piece)
{
    if (piece->get()->euroName.contains( "Soldier"))
        return 100;
    if (piece->get()->euroName.contains("Cannon"))
        return 300;
    if (piece->get()->getEuroName() == "Chariot")
        return 500;
    if (piece->get()->euroName.contains("Horse"))
        return 300;
    if (piece->get()->euroName.contains("Elephant"))
        return 200;
    if (piece->get()->euroName.contains("Advisor"))
        return 100;
    if (piece->get()->euroName.contains("General"))
        return 1000;
    return 0;
}

int Engine::getPositionValue(const std::shared_ptr<Piece> *piece, int x, int y)
{
    if (piece->get()->euroName == "Soldier")
        return soldierValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "Cannon")
        return cannonValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "Chariot")
        return chariotValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "Horse")
        return horseValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "Elephant")
        return elephantValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "Advisor")
        return advisorValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    if (piece->get()->euroName == "General")
        return generalValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    return 0;
}

int Engine::minimax(int depth, int alpha, int beta, Color color)
{
    return 0;
}

int Engine::quiesce(int alpha, int beta, Color color)
{
    return 0;
}

int Engine::search(int depth, Color color)
{
    std::vector<std::pair<Point, Point>> posAll = basemodel.position.getAllValidMoves(color, basemodel.position.board);

    size_t sizeOfPosAll = posAll.size();
    qDebug() << "Depth: " << depth << "Nodes: " << sizeOfPosAll;
    if (sizeOfPosAll == 0)
        qDebug() << "Checkmate for black";
    if (depth == 0)
        return evaluatePosition(basemodel.position.board);




    return 0;
}


std::pair<Point, Point> Engine::engineGo()
{
    std::pair<Point, Point> ownEngineMove = {};
    ownEngineMove = GetBestMove(basemodel.position.players_color);
    return ownEngineMove;
}
