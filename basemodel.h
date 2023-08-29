#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QObject>

#include "types.h"

class Board : public QObject
{
    Q_OBJECT
public:
    Board();

    // Kopierkonstruktor
    Board(const Board &other);

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

    virtual ~Board() { Q_CLEANUP_RESOURCE(res); }

    // Initialisiert das Spielbrett
    void initBoard(bool native = true);

    // Plaziert ein Piece auf dem Board
    void placePiece(Piece piece);

    // Macht einen Zug auf dem Brett
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // Gibt das Brett in der Konsole aus
    void printBoard();

    // Wechselt die Farbe des Spielers, der am Zug ist
    void toggleOnMove();

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
};

#endif // BASEMODEL_H
