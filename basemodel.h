#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPair>

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

enum class PieceType { Empty, General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier };

enum class Color { Red, Black };

struct Position
{
    int row;
    int col;
};

extern Position glfrom, glto;

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
        this->img = img; //new QImage(":/res/rookRed.png");
    }
    virtual ~Piece() {}

    QImage img;

    Color color;
    PieceType type;
    Position pos;
};

// class Board
class Board : public QObject
{
    Q_OBJECT
public:
    Board() { initBoard(); }

    // Kopierkonstruktor
    Board(const Board &other)
    {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = other.pieces[row][col]; // Tiefenkopie der Piece-Objekte
            }
        }
    }

    // Zuweisungsoperator
    Board &operator=(const Board &other)
    {
        if (this != &other) { // Vermeiden Sie Selbstzuweisung
            for (int row = 0; row < ROWS; ++row) {
                for (int col = 0; col < COLS; ++col) {
                    pieces[row][col] = other.pieces[row][col]; // Tiefenkopie der Piece-Objekte
                }
            }
        }
        return *this;
    }

    virtual ~Board() {}

    void initBoard()
    {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = Piece(Color::Red, PieceType::Empty, {row, col}, QImage(nullptr));
            }
        }
        placePiece(Piece(Color::Red, PieceType::Chariot, {0, 0}, QImage(":/res/rookRed.png")));
        placePiece(Piece(Color::Red, PieceType::Horse, {0, 1}, QImage(":/res/horseRed.png")));
        placePiece(Piece(Color::Red, PieceType::Elephant, {0, 2}, QImage(":/res/elephantRed.png")));
        placePiece(Piece(Color::Red, PieceType::Advisor, {0, 3}, QImage(":/res/advisorRed.png")));
        placePiece(Piece(Color::Red, PieceType::General, {0, 4}, QImage(":/res/generalRed.png")));
        placePiece(Piece(Color::Red, PieceType::Advisor, {0, 5}, QImage(":/res/advisorRed.png")));
        placePiece(Piece(Color::Red, PieceType::Elephant, {0, 6}, QImage(":/res/elephantRed.png")));
        placePiece(Piece(Color::Red, PieceType::Horse, {0, 7}, QImage(":/res/horseRed.png")));
        placePiece(Piece(Color::Red, PieceType::Chariot, {0, 8}, QImage(":/res/rookRed.png")));
        placePiece(Piece(Color::Red, PieceType::Cannon, {2, 1}, QImage(":/res/cannonRed.png")));
        placePiece(Piece(Color::Red, PieceType::Cannon, {2, 7}, QImage(":/res/cannonRed.png")));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 0}, QImage(":/res/soldierRed.png")));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 2}, QImage(":/res/soldierRed.png")));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 4}, QImage(":/res/soldierRed.png")));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 6}, QImage(":/res/soldierRed.png")));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 8}, QImage(":/res/soldierRed.png")));

        placePiece(Piece(Color::Black, PieceType::Chariot, {9, 0}, QImage(":/res/rookBlack.png")));
        placePiece(Piece(Color::Black, PieceType::Horse, {9, 1}, QImage(":/res/horseBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Elephant, {9, 2}, QImage(":/res/elephantBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Advisor, {9, 3}, QImage(":/res/advisorBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::General, {9, 4}, QImage(":/res/generalBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Advisor, {9, 5}, QImage(":/res/advisorBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Elephant, {9, 6}, QImage(":/res/elephantBlack.png")));
        placePiece(Piece(Color::Black, PieceType::Horse, {9, 7}, QImage(":/res/horseBlack.png")));
        placePiece(Piece(Color::Black, PieceType::Chariot, {9, 8}, QImage(":/res/rookBlack.png")));
        placePiece(Piece(Color::Black, PieceType::Cannon, {7, 1}, QImage(":/res/cannonBlack.png")));
        placePiece(Piece(Color::Black, PieceType::Cannon, {7, 7}, QImage(":/res/cannonBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Soldier, {6, 0}, QImage(":/res/soldierBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Soldier, {6, 2}, QImage(":/res/soldierBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Soldier, {6, 4}, QImage(":/res/soldierBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Soldier, {6, 6}, QImage(":/res/soldierBlack.png")));
        placePiece(
            Piece(Color::Black, PieceType::Soldier, {6, 8}, QImage(":/res/soldierBlack.png")));
    };
    void placePiece(Piece piece)
    {
        //Piece p = Piece(piece.color, piece.type, piece.pos, piece.img);
        pieces[piece.pos.row][piece.pos.col] = piece;
    };

    void movePiece(int fromRow, int fromCol, int toRow, int toCol)
    {
        Piece piece = pieces[fromRow][fromCol];
        pieces[fromRow][fromCol] = Piece(Color::Red, PieceType::Empty, {fromRow, fromCol}, QImage());
        piece.pos.row = toRow;
        piece.pos.col = toCol;

        glfrom = {fromRow, fromCol};
        glto = {toRow, toCol};

        pieces[toRow][toCol] = piece;

        qDebug() << "Moved piece from " << fromRow << "," << fromCol << " to " << toRow << ","
                 << toCol << Qt::endl;
        qDebug() << "From image: " << piece.img << Qt::endl;
    };

    void toggleOnMove()
    {
        if (onMove == Color::Red) {
            onMove = Color::Black;
        } else {
            onMove = Color::Red;
        }
    };
    Color onMove = Color::Red;

    Piece pieces[ROWS][COLS];

    bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isValidPosition(int row, int col);
    bool isVacantOrOpponent(int row, int col, Color color);
    bool isValidSoldierMove(Position from, Position to, Color color);
    bool isValidCannonMove(Position from, Position to, Color color);
    bool isValidHorseMove(Position from, Position to, Color color);
    bool isValidElephantMove(Position from, Position to, Color color);
    bool isValidAdvisorMove(Position from, Position to, Color color);
    bool isValidGeneralMove(Position from, Position to, Color color);
    bool isValidChariotMove(Position from, Position to, Color color);

    std::vector<std::pair<Position, Position>> generateLegalMoves(Color currentPlayerColor);
    bool isCheck(Color currentPlayerColor);
    bool isCheckmate(Color currentPlayerColor);
    //bool isStaleMate(Color currentPlayerColor);
    bool performMove(Position from, Position to, Color currentPlayerColor);
    Color getColor(Position p);
    Piece getPiece(Position p);
    Position findGeneralPosition(Color currentPlayerColor);
    void placePiece(int row, int col, PieceType type, Color color);
    bool isValidMove(Position from, Position to, Color currentPlayerColor);
};

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

signals:
};

#endif // BASEMODEL_H
