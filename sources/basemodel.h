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

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDebug>
#include <QObject>

#include "board.h"
#include "types.h"

class BaseModel : public QObject
{
    Q_OBJECT
public:
    explicit BaseModel(QObject *parent = nullptr)
    {
        engineName = "built-in";
        moveHistory.append(board);
        currentMove++;
    };

    static const short BoardRows = ROWS;
    static const short BoardCols = COLS;
    static const short BoardRowPoints = BoardRows - 1;
    static const short BoardColPoints = BoardCols - 1;

    // a class that holds all board data
    Board board;
    Board boardCopy;

    viewStyleMode viewStyleModeVar = viewStyleMode::traditional_native;

    int currentMove = 0;
    QStringList moves;
    QString engineName = "built-in";
    QList<Board> moveHistory;
    QString kind = "uci";

    QList<QPair<position, position>> currentMoves;
    position fromHuman = {-1, -1};
    position toHuman = {-1, -1};
    position fromUCI = {-1, -1};
    position toUCI = {-1, -1};

    // legt fest welche Farbe unten ist und welche oben,
    // die genannte ist unten
    color gameView = color::Red;

    // legt fest welche Farbe der Mensch spielt
    color humanColor = color::Red;

    // .:Helper functions:.
    // Gibt das Brett in der Konsole aus
    void printBoard()
    {
        for (int i = 0; i < ROWS; i++) {
            QString line = "";
            for (int j = 0; j < COLS; j++) {
                line += QString::number((int) board.pieces[i][j].type);
                line += " ";
            }
            qDebug() << line;
        }
    };

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

    QByteArray ToUnicodeSym(Piece p) { return QByteArray(p.name.toUtf8()); }
};

#endif // BASEMODEL_H
