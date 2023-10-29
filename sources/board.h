#ifndef BOARD_H
#define BOARD_H
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
    Board &operator=(const Board &other);

    virtual ~Board() {}// Q_CLEANUP_RESOURCE(res); }

    color GetColor(position p) { return pieces[p.row][p.col].colr; }

    Piece GetPiece(position p) { return pieces[p.row][p.col]; }

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
    color onMove = color::Red;
    // Brett des Spiels
    Piece pieces[ROWS][COLS];
    viewStyleMode viewStyleModeVar = viewStyleMode::traditional_native;

    QByteArray posToken(QByteArray token)
    {
        QByteArray t = token.split(' ').at(1);
        return t;
    }

    QByteArray posToken(int fromX, int fromY, int toX, int toY)
    {
        QByteArray m;
        char c1 = fromX + 'a';
        char c2 = (fromY) + '0';
        char c3 = toX + 'a';
        char c4 = (toY) + '0';
        m.clear();
        m.append(c1);
        m.append(c2);
        m.append(c3);
        m.append(c4);
        return m;
    }
};

#endif // BOARD_H
