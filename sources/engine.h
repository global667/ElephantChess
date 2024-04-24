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

#include <QtConcurrent>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QTimer>

#include "basemodel.h"

struct TTEntry {
    int depth;
    int value;
    // Sie können weitere Informationen speichern, wie z.B. den besten Zug
};

using TranspositionTable = std::unordered_map<std::uint64_t, TTEntry>;

// chinese chess engine
class Engine : public QObject
{
    Q_OBJECT
public:
    Engine();
    ~Engine() { appClosing = true; }

    std::pair<Point, Point> GetBestMove(Color color);
    std::pair<Point, Point> engineGo();
    //int search(int depth, Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    long nodes = 0;
    int evaluation = 0;
    int depth = 2;
    QString bMove = "";
    QString name = "ElephantChessEngine";

signals:
    void updateView(Point from, Point to, QString kind);
    void paintFromThread();
    void updateFromThread();

private:
    int evaluatePosition(const std::vector<std::vector<std::shared_ptr<Piece> > > &board);
    int getPieceValue(const std::shared_ptr<Piece> *piece);
    int getPositionValue(const std::shared_ptr<Piece> *piece, int x, int y);
    int quiesce(int alpha, int beta, Color color);

    static constexpr int INFINITY_SCORE = std::numeric_limits<int>::max();

    int getPossibleHits(const int x, const int y, const std::vector<std::vector<std::shared_ptr<Piece> > > &board, const std::vector<std::pair<Point, Point> > &moves);
    std::uint64_t hashBoard(const std::vector<std::vector<std::shared_ptr<Piece> > > &board);
    //int minimax(int depth, bool maximizingPlayer, const std::vector<std::vector<std::shared_ptr<Piece> > > board, TranspositionTable *tt);
    void initializeZobrist();
    int minimax(int depth, bool maximizingPlayer, std::vector<std::vector<std::shared_ptr<Piece> > > board, TranspositionTable *tt);
    bool appClosing = false;
public slots:
    void nodesPerSecond();
};

#endif // ENGINE_H
