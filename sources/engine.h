#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"
#include "basemodel.h"
#include "genmove.h"
#include <QRandomGenerator>

// chinese chess engine
class engine
{
public:
    engine();
    std::pair<Position, Position> getBestMove(Color color);

    // get all possible moves for a piece
    // return a list of positions
    QList<Position> getPossibleMoves(Piece *piece);

    // get all possible moves for a color
    // return a list of positions
    QList<Position> getPossibleMoves(Color color);

private:
};

#endif // ENGINE_H
