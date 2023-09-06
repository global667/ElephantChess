#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QObject>

#include "board.h"
#include "types.h"

class BaseModel : public QObject
{
    Q_OBJECT
public:
    explicit BaseModel(QObject *parent = nullptr){};

    static const short BoardRows = ROWS;
    static const short BoardCols = COLS;
    static const short BoardRowPoints = BoardRows - 1;
    static const short BoardColPoints = BoardCols - 1;

    // a class that holds all board data
    Board board;
    Board board_copy;

    int currentMove = 0;
    QList<Board> moveHistory;

    Position fromHuman = {-1, -1};
    Position toHuman = {-1, -1};
    Position fromUCI = {-1, -1};
    Position toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    Color gameView = Color::Red;

    // legt fest welche Farbe der Mensch spielt
    Color humanColor = Color::Red;

    QString engine = "chameleon";
};

#endif // BASEMODEL_H
