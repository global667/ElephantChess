#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>

#include "types.h"

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

    // Initialisiert das Spielbrett
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

    // Plaziert ein Piece auf dem Board
    void placePiece(Piece piece)
    {
        //Piece p = Piece(piece.color, piece.type, piece.pos, piece.img);
        pieces[piece.pos.row][piece.pos.col] = piece;
    };

    // Macht einen Zug auf dem Brett
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
                 << toCol;
        qDebug() << "From image: " << piece.img;
        //printBoard();
    };

    // Gibt das Brett in der Konsole aus
    void printBoard()
    {
        for (int i = 0; i < ROWS; i++) {
            QString line = "";
            for (int j = 0; j < COLS; j++) {
                line += QString::number((int) pieces[i][j].type);
                line += " ";
            }
            qDebug() << line;
        }
    };

    // Wechselt die Farbe des Spielers, der am Zug ist
    void toggleOnMove()
    {
        if (onMove == Color::Red) {
            onMove = Color::Black;
        } else {
            onMove = Color::Red;
        }
    };

    // Gibt die Farbe des Spielers, der am Zug ist, zurück
    Color onMove = Color::Red;

    // Brett des Spiels
    Piece pieces[ROWS][COLS];
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
