#include "board.h"
#include "basemodel.h"

extern BaseModel basemodel;

Board::Board(QObject *parent)
    : QObject{parent}
{
    Q_INIT_RESOURCE(res);

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

void Board::placePiece(Piece piece)
{
    //Piece p = Piece(piece.color, piece.type, piece.pos, piece.img);
    pieces[piece.pos.row][piece.pos.col] = piece;
};

// Macht einen Zug auf dem Brett
void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    Piece piece = pieces[fromRow][fromCol];
    pieces[fromRow][fromCol] = Piece(Color::Red, PieceType::Empty, {fromRow, fromCol}, QImage());
    piece.pos.row = toRow;
    piece.pos.col = toCol;

    basemodel.fromHuman = {fromRow, fromCol};
    basemodel.toHuman = {toRow, toCol};

    pieces[toRow][toCol] = piece;

    qDebug() << "Moved piece from " << fromRow << "," << fromCol << " to " << toRow << "," << toCol;
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
    if (onMove == Color::Red) {
        onMove = Color::Black;
    } else {
        onMove = Color::Red;
    }
};

void Board::initBoard()
{
    if (ViewStyleMode::traditional_native == viewStyleMode) {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = Piece(Color::Red, PieceType::Empty, {row, col}, "");
            }
        }
        placePiece(Piece(Color::Red, PieceType::Chariot, {0, 0}, "車"));
        placePiece(Piece(Color::Red, PieceType::Horse, {0, 1}, "馬"));
        placePiece(Piece(Color::Red, PieceType::Elephant, {0, 2}, "相"));
        placePiece(Piece(Color::Red, PieceType::Advisor, {0, 3}, "仕"));
        placePiece(Piece(Color::Red, PieceType::General, {0, 4}, "帥"));
        placePiece(Piece(Color::Red, PieceType::Advisor, {0, 5}, "仕"));
        placePiece(Piece(Color::Red, PieceType::Elephant, {0, 6}, "相"));
        placePiece(Piece(Color::Red, PieceType::Horse, {0, 7}, "馬"));
        placePiece(Piece(Color::Red, PieceType::Chariot, {0, 8}, "車"));
        placePiece(Piece(Color::Red, PieceType::Cannon, {2, 1}, "炮"));
        placePiece(Piece(Color::Red, PieceType::Cannon, {2, 7}, "炮"));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 0}, "兵"));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 2}, "兵"));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 4}, "兵"));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 6}, "兵"));
        placePiece(Piece(Color::Red, PieceType::Soldier, {3, 8}, "兵"));

        placePiece(Piece(Color::Black, PieceType::Chariot, {9, 0}, "車"));
        placePiece(Piece(Color::Black, PieceType::Horse, {9, 1}, "馬"));
        placePiece(Piece(Color::Black, PieceType::Elephant, {9, 2}, "象"));
        placePiece(Piece(Color::Black, PieceType::Advisor, {9, 3}, "士"));
        placePiece(Piece(Color::Black, PieceType::General, {9, 4}, "將"));
        placePiece(Piece(Color::Black, PieceType::Advisor, {9, 5}, "士"));
        placePiece(Piece(Color::Black, PieceType::Elephant, {9, 6}, "象"));
        placePiece(Piece(Color::Black, PieceType::Horse, {9, 7}, "馬"));
        placePiece(Piece(Color::Black, PieceType::Chariot, {9, 8}, "車"));
        placePiece(Piece(Color::Black, PieceType::Cannon, {7, 1}, "砲"));
        placePiece(Piece(Color::Black, PieceType::Cannon, {7, 7}, "砲"));
        placePiece(Piece(Color::Black, PieceType::Soldier, {6, 0}, "卒"));
        placePiece(Piece(Color::Black, PieceType::Soldier, {6, 2}, "卒"));
        placePiece(Piece(Color::Black, PieceType::Soldier, {6, 4}, "卒"));
        placePiece(Piece(Color::Black, PieceType::Soldier, {6, 6}, "卒"));
        placePiece(Piece(Color::Black, PieceType::Soldier, {6, 8}, "卒"));
    } else {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                pieces[row][col] = Piece(Color::Red, PieceType::Empty, {row, col}, QImage());
            }
        }

        placePiece(Piece(Color::Red,
                         PieceType::Chariot,
                         {0, 0},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(Color::Red, PieceType::Horse, {0, 1}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Elephant,
                         {0, 2},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Advisor,
                         {0, 3},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::General,
                         {0, 4},
                         QImage(":/res/Xiangqi_General_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Advisor,
                         {0, 5},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Elephant,
                         {0, 6},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(
            Piece(Color::Red, PieceType::Horse, {0, 7}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Chariot,
                         {0, 8},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(Color::Red, PieceType::Cannon, {2, 1}, QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(
            Piece(Color::Red, PieceType::Cannon, {2, 7}, QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Soldier,
                         {3, 0},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Soldier,
                         {3, 2},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Soldier,
                         {3, 4},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Soldier,
                         {3, 6},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Red,
                         PieceType::Soldier,
                         {3, 8},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));

        placePiece(Piece(Color::Black,
                         PieceType::Chariot,
                         {9, 0},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(
            Piece(Color::Black, PieceType::Horse, {9, 1}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Elephant,
                         {9, 2},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Advisor,
                         {9, 3},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::General,
                         {9, 4},
                         QImage(":/res/Xiangqi_General_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Advisor,
                         {9, 5},
                         QImage(":/res/Xiangqi_Advisor_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Elephant,
                         {9, 6},
                         QImage(":/res/Xiangqi_Elephant_(Trad).png")));
        placePiece(
            Piece(Color::Black, PieceType::Horse, {9, 7}, QImage(":/res/Xiangqi_Horse_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Chariot,
                         {9, 8},
                         QImage(":/res/Xiangqi_Chariot_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Cannon,
                         {7, 1},
                         QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Cannon,
                         {7, 7},
                         QImage(":/res/Xiangqi_Cannon_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Soldier,
                         {6, 0},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Soldier,
                         {6, 2},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Soldier,
                         {6, 4},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Soldier,
                         {6, 6},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
        placePiece(Piece(Color::Black,
                         PieceType::Soldier,
                         {6, 8},
                         QImage(":/res/Xiangqi_Soldier_(Trad).png")));
    }
};