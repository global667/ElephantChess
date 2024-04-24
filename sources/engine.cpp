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
    {32, 34, 36, 38, 40, 38, 36, 34, 32},
    {30, 32, 34, 36, 38, 36, 34, 32, 30},
    {28, 30, 32, 34, 36, 34, 32, 30, 28},
    {26, 28, 30, 32, 34, 32, 30, 28, 26},
    {24, 26, 28, 30, 32, 30, 28, 26, 24},
    {22, 24, 26, 28, 30, 28, 26, 24, 22},
    {20, 22, 24, 26, 28, 26, 24, 22, 20},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
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

Engine::Engine() { initializeZobrist(); }

const int kBoardSize = 10*9; // Anzahl der Felder auf einem Schachbrett
const int kPieceTypes = 14; // Anzahl unterschiedlicher Spielsteine (7 für Schwarz, 7 für Rot)
std::array<std::array<uint64_t, kBoardSize>, kPieceTypes> zobristTable;

void Engine::initializeZobrist() {
    std::mt19937_64 rng(0); // Zufallsgenerator mit einem festen Seed für Reproduzierbarkeit
    std::uniform_int_distribution<uint64_t> dist;

    for (int piece = 0; piece < kPieceTypes; ++piece) {
        for (int position = 0; position < kBoardSize; ++position) {
            zobristTable[piece][position] = dist(rng); // Zufällige Werte für jede Position und jeden Spielsteintyp
        }
    }
}

std::uint64_t Engine::hashBoard(const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
    std::uint64_t hash = 0;
    int i = 0;
    for (const std::vector<std::shared_ptr<Piece>>& row : board) {
        for (const auto& piece : row) {
            //auto name = std::uint64_t(*piece->getEuroName().toStdU16String().c_str());
            if (!piece || piece->color == Color::None)
                continue; // Kein Spielstein auf diesem Feld (leeres Feld
            std::uint64_t name = piece->getId();
            //hash ^= name;  // Hier sollte eine bessere Hash-Funktion verwendet werden
            hash ^= zobristTable[name][i++]; // XOR mit Zufallswert aus Zobrist-Hash-Tabelle
            hash = hash * 1099511628211ULL;  // Ein großer Prime, der oft in Hashing verwendet wird
        }
        i = 0;
    }
    return hash;
}

std::pair<Point, Point> Engine::GetBestMove(Color color) {
    const auto& posAll = basemodel.position.getAllValidMoves(color, basemodel.position.board);
    if (posAll.empty())
        return {Point(-1, -1), Point(-1, -1)};

    std::pair<Point, Point> bestMove = {};
    int bestScore = color == Color::Black ? INFINITY_SCORE : -INFINITY_SCORE;
    QVector<QFuture<int>> futures;
    TranspositionTable transpositionTable;

    for (const auto& move : posAll) {
        auto local_board = basemodel.position.board; // Erstelle eine Kopie des Boards für jeden Thread
        basemodel.position.movePiece(move.first, move.second, local_board);

        auto future = QtConcurrent::run([this, color/*, move*/, local_board, &transpositionTable]() mutable {
            //int minma =
            //basemodel.position.undoMove(move.first, move.second, local_board[move.first.x][move.first.y], local_board);
            return minimax(this->depth - 1, color == Color::Black ? false : true, local_board, &transpositionTable);//minma;
        });
        futures.append(future);
    }

    for (int i = 0; i < futures.size(); ++i) {
        futures[i].waitForFinished();
        int eval = futures[i].result();
        if ((color == Color::Black && eval < bestScore) || (color == Color::Red && eval > bestScore)) {
            bestScore = eval;
            bestMove = posAll[i];
        }
    }
    evaluation = bestScore;
    bMove = basemodel.posToken(bestMove.first.x, bestMove.first.y, bestMove.second.x, bestMove.second.y);
    emit updateFromThread();
    return bestMove;
}

int Engine::minimax(int depth, bool maximizingPlayer, std::vector<std::vector<std::shared_ptr<Piece>>> board, TranspositionTable *tt) {
    if (appClosing)
		return 0;
    
    std::uint64_t hash = hashBoard(board);
    nodes++;
    if (tt->find(hash) != tt->end()) {
        auto hs = tt->at(hash).value;
        qDebug() << "TT hit" << hs << " depth: " << depth << " hash: " << hash << " size: " << tt->size() << " nodes: " << nodes;
        return tt->at(hash).value;;
    }

    Color color = maximizingPlayer ? Color::Red : Color::Black;
    auto moves = basemodel.position.getAllValidMoves(color, board);

    if (depth == 0 || moves.empty()) {
        int evaluation = evaluatePosition(board);
        (*tt)[hash] = {depth, evaluation};
        return evaluation;
    }

    QVector<QFuture<int>> futures;

    int eval = maximizingPlayer ? INT_MIN : INT_MAX;
    auto updateEval = [&eval, maximizingPlayer](int result) {
        if (maximizingPlayer) {
            eval = std::max(eval, result);
        } else {
            eval = std::min(eval, result);
        }
    };

    for (auto& move : moves) {
        auto future = QtConcurrent::run([this, depth, move, board, tt, maximizingPlayer]() {
            std::vector<std::vector<std::shared_ptr<Piece>>> temp_board = board; // Erstelle lokale Kopie für den Thread
            basemodel.position.movePiece(move.first, move.second, temp_board);
            int result = minimax(depth - 1, !maximizingPlayer, temp_board, tt);
            basemodel.position.undoMove(move.first, move.second, board[move.first.x][move.first.y], temp_board);
            return result;
        });
        futures.append(future);
    }

    for (auto &future : futures) {
        future.waitForFinished();
        updateEval(future.result());
    }

    (*tt)[hash] = {depth, eval};  // Speichern der Bewertung
    emit updateFromThread();
    return eval;
}



int Engine::evaluatePosition(const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
    int score = 0;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 9; y++) {
            auto piece = board[x][y];
            if (!piece->name.isEmpty()) {
                auto moves = basemodel.position.board[x][y].get()->generateValidMoves(Point(x, y), board);//getValidMovesForPiece(Point(x,y),board);
                int pieceValue = getPieceValue(&piece);
                int positionValue = getPositionValue(&piece, x, y);
                // add evaluation of hiting pieces
                //positionValue += getPossibleHits(x ,y , board, moves);
                score += piece->getColor() == Color::Red ? (pieceValue + positionValue) : -(pieceValue + positionValue);
                //qDebug() << "Score in Eval: " << score;
            }
        }
    }
    return score;
}

int Engine::getPossibleHits(const int x, const int y, const std::vector<std::vector<std::shared_ptr<Piece>>>& board, const std::vector<std::pair<Point, Point>>& moves)
{
    int hits = 0;
    for (const auto& move : moves)
    {
        if (!board[move.second.x][move.second.y]->name.isEmpty() && board[move.second.x][move.second.y]->color != board[x][y]->color)
            hits += (getPieceValue(&board[move.second.x][move.second.y]));
    }
    return hits;
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
        return INFINITY_SCORE;
    return 0;
}

int Engine::getPositionValue(const std::shared_ptr<Piece> *piece, int x, int y)
{
    if (piece->get()->euroName == "Soldier")
    {   //qDebug() << soldierValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
        //qDebug() << "X: " << x << " Y: " << y;
        //qDebug() << "Piece Name: " << piece->get()->euroName;
        return soldierValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "Cannon")    {
        return cannonValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "Chariot") {
        return chariotValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "Horse") {
        return horseValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "Elephant") {
        return elephantValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "Advisor") {
        return advisorValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    if (piece->get()->euroName == "General") {
        return generalValueTable[piece->get()->getColor() == Color::Red ? x : 9 - x][y];
    }
    return 0;
}

int Engine::quiesce(int alpha, int beta, Color color)
{
    return 0;
}
/*
int Engine::search(int depth, Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
    //qDebug() << "Depth: " << depth;
    //qDebug() << "Nodes: " << nodes;
    if (depth == 0)
        return evaluatePosition(board);

    if (basemodel.position.isCheckmate(color, board) ) {
        if (color == Color::Red)
            return INFINITY_SCORE;
        else
            return -INFINITY_SCORE;
    }

    std::vector<std::pair<Point, Point>> posAll = basemodel.position.getAllValidMoves(color, board);
    if (posAll.empty())
        return INFINITY_SCORE; // oder ein anderer spezieller Wert für Schachmatt/-pat

    //std::pair<Point, Point> bestMove;
    int bestScore = INFINITY_SCORE; // Für Minimax-Suche

    QVector<QFuture<int>> futures;
    std::vector<std::vector<std::shared_ptr<Piece>>> new_board = board;

    for (auto& move : posAll) {

        std::shared_ptr<Piece> piece = new_board[move.first.x][move.first.y];
        basemodel.position.movePiece(move.first, move.second, new_board);

        // Starten Sie die Suche in einem neuen Thread
        //auto future = QtConcurrent::run(&Engine::search, depth - 1, color == Color::Black ? Color::Red : Color::Black, new_board);
        auto future = QtConcurrent::run([this, color, depth, new_board] {return search(depth - 1, color == Color::Black ? Color::Red : Color::Black, new_board); });


        futures.append(future);

        basemodel.position.undoMove(move.first, move.second, piece, new_board);

    }

    // Warte auf alle Futures und evaluiere die Ergebnisse
    for (int i = 0; i < futures.size(); ++i) {
        futures[i].waitForFinished();
        int score = color == Color::Red ? futures[i].result() : -futures[i].result();
        if (score < bestScore) {
            evaluation = bestScore = score;
            //bestMove = posAll[i];
            emit updateFromThread();
        }
    }

    return bestScore;
}
*/
void Engine::nodesPerSecond()
{
    //qDebug() << "Nodes per second: " << nodes;
    basemodel.engineData.nodes = nodes;
    basemodel.engineData.searchDepth = depth;
    basemodel.engineData.evaluation = evaluation;
    basemodel.engineData.engineName = name;
    basemodel.engineData.bestMove = bMove;
    //nodes = 0;
}

std::pair<Point, Point> Engine::engineGo()
{    
    auto move = GetBestMove(basemodel.position.players_color);
    qDebug() << "Engine Move: " << basemodel.posToken(move.first.x, move.first.y, move.second.x, move.second.y);
    qDebug() << "Engine Evaluation: " << evaluation;
    qDebug() << "Engine Nodes: " << nodes;
    qDebug() << "Engine Depth: " << depth;
    qDebug() << "Engine Name: " << name;
    return move;
}
