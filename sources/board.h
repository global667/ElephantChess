#ifndef BOARD_H
#define BOARD_H

#include <QObject>

#include "types.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);

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

    virtual ~Board() {}// Q_CLEANUP_RESOURCE(res); }

    // Initialisiert das Spielbrett
    void initBoard();

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
    ViewStyleMode viewStyleMode = ViewStyleMode::traditional_native;
};

#endif // BOARD_H
