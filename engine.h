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

// chinese chess engine
class Engine : public QObject
{
    Q_OBJECT
public:
    Engine();
    ~Engine() {}

    std::pair<Point, Point> GetBestMove(Color color);
    std::pair<Point, Point> engineGo();

signals:
    void updateView(Point from, Point to, QString kind);
private:
    int evaluatePosition(const std::vector<std::vector<std::shared_ptr<Piece> > > &board);
    int getPieceValue(const std::shared_ptr<Piece> *piece);
    int getPositionValue(const std::shared_ptr<Piece> *piece, int x, int y);
    int minimax(int depth, int alpha, int beta, Color color);
    int quiesce(int alpha, int beta, Color color);
    int search(int depth, Color color);
};

#endif // ENGINE_H
