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

#include "board.h"
#include "basemodel.h"

extern BaseModel basemodel;

Board::Board(QObject *parent)
    : QObject{parent}
{
    //Q_INIT_RESOURCE(res);

    initBoard();
}

// Kopierkonstruktor
Board::Board(const Board &other)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            pieces[row][col] = other.pieces[row][col]; // Tiefenkopie der Piece-Objekte
        }
    }
}

Board &Board::operator=(const Board &other)
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

void Board::placePiece(Piece piece)
{
    //Piece p = Piece(piece.color, piece.type, piece.pos, piece.img);
    pieces[piece.pos.row][piece.pos.col] = piece;
};

// Macht einen Zug auf dem Brett
void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    Piece piece = pieces[fromRow][fromCol];
    pieces[fromRow][fromCol] = Piece(GetColor({fromRow, fromCol}),
                                     pieceType::Empty,
                                     {fromRow, fromCol},
                                     QImage());
    piece.pos.row = toRow;
    piece.pos.col = toCol;

    // basemodel.fromHuman = {fromRow, fromCol};
    // basemodel.toHuman = {toRow, toCol};

    basemodel.fromUCI = {fromRow, fromCol};
    basemodel.toUCI = {toRow, toCol};

    pieces[toRow][toCol] = piece;

    basemodel.moves.append(basemodel.board.posToken(fromCol, fromRow, toCol, toRow));

    //qDebug() << "Moved piece from " << fromRow << "," << fromCol << " to " << toRow << "," << toCol;
};

// Gibt das Brett in der Konsole aus
void Board::printBoard()
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
void Board::toggleOnMove()
{
    if (onMove == color::Red) {
        onMove = color::Black;
    } else {
        onMove = color::Red;
    }
};

void Board::initBoard()
{
    if (viewStyleMode::traditional_native == viewStyleModeVar) {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = Piece(color::Red, pieceType::Empty, {row, col}, "");
            }
        }
        placePiece(Piece(color::Red, pieceType::Chariot, {0, 0}, "\u8ECA"));        //"車"));
        placePiece(Piece(color::Red, pieceType::Horse, {0, 1}, QString("\u99AC"))); //"馬"));
        placePiece(Piece(color::Red, pieceType::Elephant, {0, 2}, "\u76f8"));       //"相"));
        placePiece(Piece(color::Red, pieceType::Advisor, {0, 3}, "\u4ed5"));        //"仕"));
        placePiece(Piece(color::Red, pieceType::General, {0, 4}, "\u5e25"));        //"帥"));
        placePiece(Piece(color::Red, pieceType::Advisor, {0, 5}, "\u4ed5"));        // "仕"));
        placePiece(Piece(color::Red, pieceType::Elephant, {0, 6}, "\u76f8"));       // "相"));
        placePiece(Piece(color::Red, pieceType::Horse, {0, 7}, "\u99AC"));          // "馬"));
        placePiece(Piece(color::Red, pieceType::Chariot, {0, 8}, "\u8ECA"));        // "車"));
        placePiece(Piece(color::Red, pieceType::Cannon, {2, 1}, "\u70ae"));         //"炮"));
        placePiece(Piece(color::Red, pieceType::Cannon, {2, 7}, "\u70ae"));         //"炮"));
        placePiece(Piece(color::Red, pieceType::Soldier, {3, 0}, "\u5175"));        //"兵"));
        placePiece(Piece(color::Red, pieceType::Soldier, {3, 2}, "\u5175"));        //"兵"));
        placePiece(Piece(color::Red, pieceType::Soldier, {3, 4}, "\u5175"));        //"兵"));
        placePiece(Piece(color::Red, pieceType::Soldier, {3, 6}, "\u5175"));        //"兵"));
        placePiece(Piece(color::Red, pieceType::Soldier, {3, 8}, "\u5175"));        //"兵"));

        placePiece(Piece(color::Black, pieceType::Chariot, {9, 0}, "\u8ECA"));        //"車"));
        placePiece(Piece(color::Black, pieceType::Horse, {9, 1}, QString("\u99AC"))); // "馬"));
        placePiece(Piece(color::Black, pieceType::Elephant, {9, 2}, "\u8c61"));       //"象"));
        placePiece(Piece(color::Black, pieceType::Advisor, {9, 3}, "\u58eb"));        // "士"));
        placePiece(Piece(color::Black, pieceType::General, {9, 4}, "\u5c07"));        // "將"));
        placePiece(Piece(color::Black, pieceType::Advisor, {9, 5}, "\u58eb"));        //"士"));
        placePiece(Piece(color::Black, pieceType::Elephant, {9, 6}, "\u8c61"));       //"象"));
        placePiece(Piece(color::Black, pieceType::Horse, {9, 7}, QString("\u99AC"))); //"馬"));
        placePiece(Piece(color::Black, pieceType::Chariot, {9, 8}, "\u8ECA"));        //"車"));
        placePiece(Piece(color::Black, pieceType::Cannon, {7, 1}, "\u7832"));         //"砲"));
        placePiece(Piece(color::Black, pieceType::Cannon, {7, 7}, "\u7832"));         //"砲"));
        placePiece(Piece(color::Black, pieceType::Soldier, {6, 0}, "\u5352"));        //"卒"));
        placePiece(Piece(color::Black, pieceType::Soldier, {6, 2}, "\u5352"));        //"卒"));
        placePiece(Piece(color::Black, pieceType::Soldier, {6, 4}, "\u5352"));        //"卒"));
        placePiece(Piece(color::Black, pieceType::Soldier, {6, 6}, "\u5352"));        //"卒"));
        placePiece(Piece(color::Black, pieceType::Soldier, {6, 8}, "\u5352"));        //"卒"));
    } else {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = Piece(color::Red, pieceType::Empty, {row, col}, QImage());
            }
        }

        placePiece(Piece(color::Red,
                         pieceType::Chariot,
                         {0, 0},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(color::Red, pieceType::Horse, {0, 1}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Elephant,
                         {0, 2},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Advisor,
                         {0, 3},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::General,
                         {0, 4},
                         QImage(":/res/Xiangqi_General_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Advisor,
                         {0, 5},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Elephant,
                         {0, 6},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(
            Piece(color::Red, pieceType::Horse, {0, 7}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Chariot,
                         {0, 8},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(color::Red, pieceType::Cannon, {2, 1}, QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(
            Piece(color::Red, pieceType::Cannon, {2, 7}, QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Soldier,
                         {3, 0},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Soldier,
                         {3, 2},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Soldier,
                         {3, 4},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Soldier,
                         {3, 6},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Red,
                         pieceType::Soldier,
                         {3, 8},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));

        placePiece(Piece(color::Black,
                         pieceType::Chariot,
                         {9, 0},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(color::Black, pieceType::Horse, {9, 1}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Elephant,
                         {9, 2},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Advisor,
                         {9, 3},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::General,
                         {9, 4},
                         QImage(":/res/Xiangqi_General_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Advisor,
                         {9, 5},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Elephant,
                         {9, 6},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(
            Piece(color::Black, pieceType::Horse, {9, 7}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Chariot,
                         {9, 8},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Cannon,
                         {7, 1},
                         QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Cannon,
                         {7, 7},
                         QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Soldier,
                         {6, 0},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Soldier,
                         {6, 2},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Soldier,
                         {6, 4},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Soldier,
                         {6, 6},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(color::Black,
                         pieceType::Soldier,
                         {6, 8},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
    }
};
