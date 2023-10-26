#include "engine.h"

extern BaseModel basemodel;

engine::engine()
{
    
}

std::pair<Position, Position> engine::getBestMove(Color color)
{
    GenMove generatedMoves(basemodel.board.pieces, color);

    std::vector<std::pair<Position, Position>> pos;
    std::vector<std::pair<Position, Position>> posAll;

    // all valid moves
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
    {
            if (generatedMoves.getColor({i,j}) == color)
            {
            pos = generatedMoves.isValidPieceMove(Position {i,j});//generateLegalMoves(Color::Black);
            for (std::pair<Position, Position> p : pos)
            {
                posAll.push_back(p);
                qDebug() << p.first.row << p.first.col;
            }
            }
    }

    int sizeOfPosAll = posAll.size();

    int random = QRandomGenerator::global()->bounded(sizeOfPosAll);

    return posAll.at(random);
}

QList<Position> engine::getPossibleMoves(Piece *piece)
{
    return QList<Position>();
}

QList<Position> engine::getPossibleMoves(Color color)
{
    return QList<Position>();
}
