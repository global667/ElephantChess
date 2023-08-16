#ifndef GENMOVE_H
#define GENMOVE_H

#include <QDebug>
#include <QObject>
#include "basemodel.h"
#include "types.h"

class GenMove : public QObject
{
    Q_OBJECT
public:
    explicit GenMove(QObject *parent = nullptr);
    ~GenMove(){};
    GenMove(Position from, Position to, Board board)
    {
        from = from;
        to = to;
        board = board;
        copyBoard(pieces, board.pieces);
        qDebug() << this->pieces[0][0].img << Qt::endl;
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
        from = other.from;
        to = other.to;
        copyBoard(pieces, other.pieces);
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

private:
    Board board;
    Piece pieces[ROWS][COLS];
    Position from;
    Position to;
};

#endif // GENMOVE_H
