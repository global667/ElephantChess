#ifndef TYPES_H
#define TYPES_H

#include <QImage>

// A chinese chess board in ascii
//    a b c d e f g h i
//    -------------------
//    9|r h e a g a e h r|9
//    8| | | | | | | | | |8
//    7| | c | | | | c | |7
//    6|s| |s| |s| |s| |s|6
//    5| | | | | | | | | |5
//    4| | | | | | | | | |4
//    3|s| |s| |s| |s| |s|3
//    2| | c | | | | c | |2
//    1| | | | | | | | | |1
//    -------------------
//    a b c d e f g h i

#define ROWS 10
#define COLS 9

enum class ViewStyleMode { traditional_native, traditional_png, western_png };

enum class PieceType { Empty, General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier };

enum class Color { Red, Black };

struct Position
{
    int row;
    int col;
};

//extern Position glfrom, glto;

// class Piece
class Piece
{
public:
    Piece() {}
    Piece(Color color, PieceType type, Position pos, QImage img)
        : color(color)
        , type(type)
        , pos(pos)
    {
        //if (img.isNull())
        //    qDebug() << "Piece(): img is null";
        this->img = QImage(img);
    }

    Piece(Color color, PieceType type, Position pos, QString name)
        : color(color)
        , type(type)
        , pos(pos)
    {
        //if (img.isNull())
        //    qDebug() << "Piece(): img is null";
        this->name = name;
    }

    virtual ~Piece() {}

    QImage img;
    Color color;
    PieceType type;
    Position pos;
    QString name;
};

enum class MarkerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };

enum class CompletePieceType {
    GeneralRot,
    AdvisorRot,
    ElephantRot,
    HorseRot,
    ChariotRot,
    CannonRot,
    SoldierRot,
    GeneralSchwarz,
    AdvisorSchwarz,
    ElephantSchwarz,
    HorseSchwarz,
    ChariotSchwarz,
    CannonSchwarz,
    SoldierSchwarz
};

#endif // TYPES_H
