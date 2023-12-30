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

#include "genmove.h"

//GenMove::GenMove(QObject *parent)
//    : QObject{parent}
//{

//}

GenMove::GenMove(const Piece p[ROWS][COLS], color onMove)
{
    //qDebug() << __PRETTY_FUNCTION__;
    copyBoard(pieces, p);
    this->playerOnMove = onMove;
}

// Array der Spielsteine kopieren
void GenMove::copyBoard(Piece pieces[ROWS][COLS], const Piece other[ROWS][COLS])
{
    //qDebug() << __PRETTY_FUNCTION__;
    for (int rows = 0; rows < ROWS; ++rows) {
        for (int col = 0; col < COLS; ++col) {
            pieces[rows][col] = other[rows][col]; // Tiefenkopie der Piece-Objekte
        }
    }
}

// Copy-Konstruktor
GenMove::GenMove(const GenMove &other)
{
    //qDebug() << __PRETTY_FUNCTION__;
    copyBoard(pieces, other.pieces);
    playerOnMove = other.playerOnMove;
}

// Check if the QPoint is within the board boundaries
bool GenMove::IsValidPosition(int row, int col)
{
    //qDebug() << __PRETTY_FUNCTION__;
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Check if the QPoint is empty or occupied by an opponent's piece
bool GenMove::IsVacantOrOpponent(int row, int col, color colr)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (!IsValidPosition(row, col))
        return false;
    return (pieces[row][col].type == pieceType::Empty || pieces[row][col].colr != colr);
}
// TODO: Boeser BLick
// Check if the General can move to the target QPoint
bool GenMove::IsValidGeneralMove(QPoint from, QPoint to, color colr)
{
    //qDebug() << __PRETTY_FUNCTION__;
    // Implement the move validation logic for the General here...
    int dx = to.x() - from.x();
    int dy = to.y() - from.y();

    // Check if the target QPoint is within the palace
    if (colr == color::Red) {
        if (to.x() < 0 || to.x() > 2 || to.y() < 3 || to.y() > 5) {
            return false;
        }
    } else {
        if (to.x() < 7 || to.x() > 9 || to.y() < 3 || to.y() > 5) {
            return false;
        }
    }
    if (pieces[to.y()][to.x()].colr == colr) {
        // return false;
    }
    // Check if the General moves one step horizontally or vertically
    if (((dx == 0 && std::abs(dy) == 1) || (std::abs(dx) == 1 && dy == 0))
        && IsVacantOrOpponent(to.x(), to.y(), colr)) {
        return true;
    }

    return false;
}

// Check if the Chariot can move to the target QPoint
bool GenMove::IsValidChariotMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (to.x() == from.x() && to.y() == from.y()) {
        return false;
    }
    bool isOwnPiece = true;
    // Check if the Chariot moves horizontally or vertically
    if (from.x() == to.x()) {
        int step = (to.y() > from.y()) ? 1 : -1;
        if (step == 1) {
            for (int col = from.y() + step; col <= to.y(); col += step) {
                if (isOwnPiece == false && pieces[from.x()][col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[from.x()][col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }

                if (isOwnPiece == false && pieces[from.x()][col].colr == color) {
                    return false;
                }
            }
        } else {
            for (int col = from.y() + step; col >= to.y(); col += step) {
                if (isOwnPiece == false && pieces[from.x()][col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[from.x()][col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[from.x()][col].colr == color) {
                    return false;
                }
            }
        }
    } else if (from.y() == to.y()) {
        int step = (to.x() > from.x()) ? 1 : -1;
        if (step == 1) {
            for (int row = from.x() + step; row <= to.x(); row += step) {
                if (isOwnPiece == false && pieces[row][from.y()].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[row][from.y()].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[row][from.y()].colr == color) {
                    return false;
                }
            }
        } else {
            for (int row = from.x() + step; row >= to.x(); row += step) {
                if (isOwnPiece == false && pieces[row][from.y()].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[row][from.y()].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[row][from.y()].colr == color) {
                    return false;
                }
            }
        }
    } else {
        return false; // Invalid move for the Chariot
    }
    return true;
}

// Check if the Horse can move to the target QPoint
bool GenMove::IsValidHorseMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    int dx = to.x() - from.x();
    int dy = to.y() - from.y();
    // qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);

    if ((dx * dx + dy * dy) == 5) {
        // Check if the QPoint between from and to is vacant
        int checkRow = from.x() + dx / 2;
        int checkCol = from.y() + dy / 2;
        if (IsValidPosition(checkRow, checkCol)
            && pieces[checkRow][checkCol].type == pieceType::Empty) {
            return true;
        }
    }
    return false;
}

// TODO: Elephant func is buggy
//(to.x() == 0 && toy()== 2) || (to.x() == 2 && toy()== 4)
//    || (to.x() == 2 && toy()== 0) || (to.x() == 4 && toy()== 2);

// Check if the Elephant can move to the target QPoint
/*
bool Board::isValidElephantMove(QPoint from, QPoint to, Color color)
{
    int dx = to.x() - from.x();
    int dy = toy()- from.col;

    if (color == Color::Red) {
        return (to.x() >= 5 && to.x() <= 9 && toy()>= 0 && toy()<= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.x() + dx / 2, fromy()+ dy / 2, color));

    } else {
        return (to.x() >= 5 && to.x() <= 9 && toy()>= 0 && toy()<= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.x() + dx / 2, fromy()+ dy / 2, color));
    }
}
*/
bool GenMove::IsValidElephantMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << "Elephant: from to: " << from.x() << fromy()<< to.x() << to.col;

    int dx = to.x() - from.x();
    int dy = to.y() - from.y();
    //qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);
    if (color == color::Black) {
        return (to.x() >= 5 && to.x() <= 9 && to.y() >= 0 && to.y() <= 8 && (dx * dx + dy * dy) == 8
                && IsVacantOrOpponent(from.x() + dx / 2, from.y() + dy / 2, color));
        //&& isVacantOrOpponent(to.x(), to.col, color));

    } else {
        return (to.x() >= 0 && to.x() <= 4 && to.y() >= 0 && to.y() <= 8 && (dx * dx + dy * dy) == 8
                && IsVacantOrOpponent(from.x() + dx / 2, from.y() + dy / 2, color));
        //&& isVacantOrOpponent(to.x(), to.col, color));
    }
}

// Check if the Advisor can move to the target QPoint
bool GenMove::IsValidAdvisorMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    int dx = to.x() - from.x();
    int dy = to.y() - from.y();

    if (color == color::Red) {
        return (to.x() >= 0 && to.x() <= 2 && to.y() >= 3 && to.y() <= 5 && (dx * dx + dy * dy) == 2
                && IsVacantOrOpponent(to.x(), to.y(), color));
    } else {
        return (to.x() >= 7 && to.x() <= 9 && to.y() >= 3 && to.y() <= 5 && (dx * dx + dy * dy) == 2
                && IsVacantOrOpponent(to.x(), to.y(), color));
    }
}

// Check if the Cannon can move to the target QPoint
bool GenMove::IsValidCannonMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    // Implement the move validation logic for the Cannon here...
    // Check if the Cannon moves horizontally or vertically
    if (from.x() == to.x()) {
        int step = (to.y() > from.y()) ? 1 : -1;
        int obstacleCount = 0;
        for (int col = from.y() + step; (col != to.y()) && (IsValidPosition(from.x(), col));
             col += step) {
            if (pieces[from.x()][col].type != pieceType::Empty) {
                obstacleCount++;
            }
        }
        if (obstacleCount == 1 && pieces[to.x()][to.y()].colr == color::Black) {
            return true;
        }

        if (obstacleCount == 0 && pieces[to.x()][to.y()].type == pieceType::Empty) {
            return true;
        }
    } else if (from.y() == to.y()) {
        int step = (to.x() > from.x()) ? 1 : -1;
        int obstacleCount = 0;
        for (int row = from.x() + step; row != to.x() && (IsValidPosition(row, from.y()));
             row += step) {
            if (pieces[row][from.y()].type != pieceType::Empty) {
                obstacleCount++;
            }
        }
        if ((obstacleCount == 1) && (pieces[to.x()][to.y()].colr == color::Black)) {
            return true;
        }
        if (obstacleCount == 0 && pieces[to.x()][to.y()].type == pieceType::Empty) {
            return true;
        }
    }
    return false;
}

// TODO: Walk before the river is buggy, can do a sidewalk shouldn't so.
// Check if the Soldier can move to the target QPoint
bool GenMove::IsValidSoldierMove(QPoint from, QPoint to, color color)
{
    //qDebug() << __PRETTY_FUNCTION__;
    // Implement the move validation logic for the Soldier here...
    int dx = to.x() - from.x();
    int dy = to.y() - from.y();
    //    qDebug() << "Soldier:";
    //    qDebug() << "dx:" << dx << "dy:" << dy;

    if (color == color::Black) {
        if (from.x() >= 5 && dx == -1 && dy == 0) {
            return true;
        }
        if (from.x() < 5 && ((dx == -1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    } else {
        if (from.x() <= 4 && dx == 1 && dy == 0) {
            return true;
        }
        if (from.x() > 4 && ((dx == 1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    }
    return false;
}
// TODO: Boeser Blick
// Generate all legal moves for the current player from the given board QPoint
std::vector<std::pair<QPoint, QPoint>> GenMove::GenerateLegalMoves(color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    std::vector<std::pair<QPoint, QPoint>> legalMoves;

    // Iterate over all squares on the board
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].colr == currentPlayerColor) {
                QPoint from = {row, col};

                // Check all possible moves for the piece on this square
                for (int torow = 0; torow < ROWS; ++torow) {
                    for (int tocol = 0; tocol < COLS; ++tocol) {
                        QPoint to = {torow, tocol};
                        //qDebug() << "generateLegalMoves: " << fromy()<< from.x() << to.col
                        //         << to.x() << isValidMove(from, to, currentPlayerColor);
                        // Check if the move is valid for the piece
                        if (IsValidMove(from, to, currentPlayerColor)) {
                            legalMoves.push_back({from, to});
                        }
                    }
                }
            }
        }
    }
    //qDebug() << "legalMoves" << legalMoves.size();
    return legalMoves;
}

// Check if a player is in check
bool GenMove::IsCheck(color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    color c;
    if (currentPlayerColor == color::Red) {
        c = color::Black;
    } else {
        c = color::Red;
    }

    // Find the QPoint of the General of the currentPlayerColor
    QPoint generalPos = FindGeneralPosition(currentPlayerColor);
    //qDebug() << "General: " << generalPosy()<< generalPos.x();

    // Check if the General is under attack from the opponent's pieces
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].colr == c) {
                QPoint from = {row, col};
                //qDebug() << "from: " << fromy()<< from.x() << "to: " << generalPos.col
                //         << generalPos.x();
                if (IsValidMove(from, generalPos, c)) {
                    qDebug() << "Check";
                    qDebug() << "Check on " << from.y() << from.x() << "to: " << generalPos.y()
                             << generalPos.x();
                    ;
                    return true;
                }
            }
        }
    }

    return false;
}

// Check if a player is in checkmate
bool GenMove::IsCheckmate(color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    // Generate all legal moves for the currentPlayerColor
    std::vector<std::pair<QPoint, QPoint>> legalMoves = GenerateLegalMoves(currentPlayerColor);

    // Check if any move can resolve the check
    for (const auto &move : legalMoves) {
        GenMove copy(*this); // Make a copy of the current board state
        if (copy.PerformMove(move.first, move.second, currentPlayerColor)) {
            // If the move is valid and resolves the check, then the player is not in checkmate
            return false;
        }
    }

    return true; // No move resolves the check, so the player is in checkmate
}

// Print the board on console

// ...
// Perform a move on the board and update the board state
bool GenMove::PerformMove(QPoint from, QPoint to, color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    // Check if the move is valid
    if (!IsValidMove(from, to, currentPlayerColor)) {
        return false;
    }

    // Update the board state
    pieces[to.x()][to.y()] = pieces[from.x()][from.y()];
    pieces[from.x()][from.y()] = Piece(color::Red,
                                       pieceType::Empty,
                                       {from.x(), from.y()},
                                       QImage(nullptr),
                                       "");

    // Set the previous QPoint to empty

    return true;
}

color GenMove::GetColor(QPoint p)
{
    //qDebug() << __PRETTY_FUNCTION__;
    return pieces[p.x()][p.y()].colr;
}

Piece GenMove::GetPiece(QPoint p)
{
    return pieces[p.x()][p.y()];
}

// ...

// Helper function to find the QPoint of the General of the currentPlayerColor
QPoint GenMove::FindGeneralPosition(color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].type == pieceType::General
                && pieces[row][col].colr == currentPlayerColor) {
                return {row, col};
            }
        }
    }
    // Return an invalid QPoint if the General is not found (shouldn't happen in a valid game)
    return {-1, -1};
}

bool GenMove::IsValidMove(QPoint from, QPoint to, color currentPlayerColor)
{
    //qDebug() << __PRETTY_FUNCTION__;
    pieceType pieceType = pieces[from.x()][from.y()].type;

    switch (pieceType) {
    case pieceType::General:
        return IsValidGeneralMove(from, to, currentPlayerColor);
    case pieceType::Advisor:
        return IsValidAdvisorMove(from, to, currentPlayerColor);
    case pieceType::Elephant:
        return IsValidElephantMove(from, to, currentPlayerColor);
    case pieceType::Horse:
        return IsValidHorseMove(from, to, currentPlayerColor);
    case pieceType::Chariot:
        return IsValidChariotMove(from, to, currentPlayerColor);
    case pieceType::Cannon:
        //qDebug() << "From to cannon" << from.x() << fromy()<< to.x() << to.col;
        return IsValidCannonMove(from, to, currentPlayerColor);
    case pieceType::Soldier:
        return IsValidSoldierMove(from, to, currentPlayerColor);
    default:
        return false;
    }

    //return false;
}

std::vector<std::pair<QPoint, QPoint>> GenMove::AllValidMoves(const QPoint fromPos)
{
    //qDebug() << __PRETTY_FUNCTION__;
    pieceType pieceType = pieces[fromPos.x()][fromPos.y()].type;
    color currentPlayerColor = pieces[fromPos.x()][fromPos.y()].colr;
    std::vector<std::pair<QPoint, QPoint>> legalMoves = GenerateLegalMoves(currentPlayerColor);
    std::vector<std::pair<QPoint, QPoint>> legalPieceMoves, legalPieceMovestmp;

    if (pieceType == pieceType::Empty) {
        //legalPieceMoves.push_back({{-1, -1}, {-1, -1}});
        legalPieceMoves.clear();
        return legalPieceMoves;
    }

    GenMove copy(*this);
    GenMove copy2(*this);

    // Check if the general is in check and if so, only allow moves that get it out of check
    for (auto move : legalMoves) {
        QPoint to = move.second;
        QPoint from = move.first;
        if (fromPos.y() == from.y() && from.x() == fromPos.x()) {
            copy = copy2;
            copy.PerformMove(from, to, currentPlayerColor);
            if (!copy.IsCheck(currentPlayerColor)  && !copy.IsEvilGlare(currentPlayerColor)/*&& IsValidMove(from, to, currentPlayerColor)*/) {
                legalPieceMoves.push_back({fromPos, {to.x(), to.y()}});
            }
        }
        /*for (auto l : legalPieceMoves) {
            qDebug() << "Legal moves" << l.first.x() << l.firsty()<< l.second.x() << l.second.col;
        }*/
    }
    return legalPieceMoves;
}

bool GenMove::IsEvilGlare(color col)
{

    QPoint general = FindGeneralPosition(col);
    color opp_col;

    if (col == color::Red)
        opp_col = color::Black;
    else
        opp_col = color::Red;

    QPoint opp_general = FindGeneralPosition(opp_col);

    if (opp_general.y() == general.y()) {
        if (opp_general.x() < general.x()) {
            for (size_t i = opp_general.x() + 1; i < general.x(); i++) {
                //qDebug() << i << general.x();
                if (pieces[i][general.y()].type != pieceType::Empty) {
                    return false;
                }
            }
        } else {
            for (size_t i = general.x() + 1; i < opp_general.x(); i++) {
                //qDebug() << i << general.x();
                if (pieces[i][general.y()].type != pieceType::Empty) {
                    return false;
                }
            }
        }
    }

    return true;
}
