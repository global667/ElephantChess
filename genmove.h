#ifndef GENMOVE_H
#define GENMOVE_H

#include <QDebug>
#include <vector>
//#include <QObject>
#include "types.h"

class GenMove //: public QObject
{
    //Q_OBJECT
public:
    explicit GenMove(); //QObject *parent = nullptr);
    ~GenMove(){};
    GenMove(Position from, Position to, const Piece p[ROWS][COLS], Color onMove)
    {
        from = from;
        to = to;
        copyBoard(pieces, p);
        onMove = onMove;
    }

    // Array der Spielsteine kopieren
    void copyBoard(Piece pieces[ROWS][COLS], const Piece other[ROWS][COLS])
    {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = other[row][col]; // Tiefenkopie der Piece-Objekte
            }
        }
    }

    // Copy-Konstruktor
    GenMove(const GenMove &other)
    {
        copyBoard(pieces, other.pieces);
        onMove = other.onMove;
    }

    // Gibt zurück, ob ein Zug gülig ist
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
    std::vector<std::pair<Position, Position>> isValidPieceMove(const Position from);

    Piece pieces[ROWS][COLS];
    Color onMove;
};

#endif // GENMOVE_H
