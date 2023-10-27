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
#include <vector>
//#include <QObject>
#include "types.h"

class GenMove //: public QObject
{
    //Q_OBJECT
public:
    explicit GenMove(); //QObject *parent = nullptr);
    ~GenMove(){};
    GenMove(const Piece p[ROWS][COLS], color onMove);
    // Array der Spielsteine kopieren
    void copyBoard(Piece pieces[ROWS][COLS], const Piece other[ROWS][COLS]);

    // Copy-Konstruktor
    GenMove(const GenMove &other);

    // Gibt zurück, ob ein Zug gülig ist
    bool IsLegalMove(int fromRow, int fromCol, int toRow, int toCol);

    bool IsValidPosition(int row, int col);
    bool IsVacantOrOpponent(int row, int col, color color);
    bool IsValidSoldierMove(position from, position to, color color);
    bool IsValidCannonMove(position from, position to, color color);
    bool IsValidHorseMove(position from, position to, color color);
    bool IsValidElephantMove(position from, position to, color color);
    bool IsValidAdvisorMove(position from, position to, color color);
    bool IsValidGeneralMove(position from, position to, color color);
    bool IsValidChariotMove(position from, position to, color color);
    std::vector<std::pair<position, position>> GenerateLegalMoves(color currentPlayerColor);
    bool IsCheck(color currentPlayerColor);
    bool IsCheckmate(color currentPlayerColor);
    //bool isStaleMate(Color currentPlayerColor);
    bool PerformMove(position from, position to, color currentPlayerColor);
    color GetColor(position p);
    Piece GetPiece(position p);
    position FindGeneralPosition(color currentPlayerColor);
    void PlacePiece(int row, int col, pieceType type, color color);
    bool IsValidMove(position from, position to, color currentPlayerColor);
    std::vector<std::pair<position, position>> IsValidPieceMove(const position from);

    Piece pieces[ROWS][COLS];
    color onMove;
};

#endif // GENMOVE_H
