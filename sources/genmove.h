/*
  ElephantChess, a UCI chinese chess playing GUI with builtin engine
  Copyright (C) 2022-2023 Wolf S. Kappesser

  ElephantChess is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ElephantChess is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GENMOVE_H
#define GENMOVE_H

#include <QDebug>
#include <QObject>
#include "types.h"
#include <vector>

class GenMove : public QObject
{
    Q_OBJECT
public:
    GenMove() {}
    GenMove(QObject *parent = nullptr) {}
    // Copy-Konstruktor
    GenMove(const GenMove &other);
    GenMove(const Piece p[ROWS][COLS], color onMove);
    GenMove &operator=(const GenMove &other)
    {
        copyBoard(this->pieces, other.pieces);
        this->playerOnMove = other.playerOnMove;
        return *this;
    }

    ~GenMove() {}
    // Array der Spielsteine kopieren
    void copyBoard(Piece pieces[ROWS][COLS], const Piece other[ROWS][COLS]);

    bool IsValidPosition(int row, int col);
    bool IsVacantOrOpponent(int row, int col, color color);
    bool IsValidSoldierMove(QPoint from, QPoint to, color color);
    bool IsValidCannonMove(QPoint from, QPoint to, color color);
    bool IsValidHorseMove(QPoint from, QPoint to, color color);
    bool IsValidElephantMove(QPoint from, QPoint to, color color);
    bool IsValidAdvisorMove(QPoint from, QPoint to, color color);
    bool IsValidGeneralMove(QPoint from, QPoint to, color color);
    bool IsValidChariotMove(QPoint from, QPoint to, color color);

    std::vector<std::pair<QPoint, QPoint>> GenerateLegalMoves(color currentPlayerColor);
    bool IsCheck(color currentPlayerColor);
    bool IsCheckmate(color currentPlayerColor);
    bool IsEvilGlare(color color);
    //bool isStaleMate(Color currentPlayerColor);
    bool PerformMove(QPoint from, QPoint to, color currentPlayerColor);
    color GetColor(QPoint p);
    Piece GetPiece(QPoint p);
    QPoint FindGeneralPosition(color currentPlayerColor);
    void PlacePiece(int row, int col, pieceType type, color color);

    // Gibt zurück, ob ein Zug gülig ist
    bool IsValidMove(QPoint from, QPoint to, color currentPlayerColor);
    std::vector<std::pair<QPoint, QPoint>> AllValidMoves(const QPoint from);

    Piece pieces[ROWS][COLS];
    color playerOnMove;
};

#endif // GENMOVE_H
