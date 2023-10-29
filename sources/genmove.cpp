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

GenMove::GenMove() //QObject *parent)
    //: QObject{parent}
{

}

GenMove::GenMove(const Piece p[ROWS][COLS], color onMove)
{
    copyBoard(pieces, p);
    this->playerOnMove = onMove;
}

// Array der Spielsteine kopieren
void GenMove::copyBoard(Piece pieces[ROWS][COLS], const Piece other[ROWS][COLS])
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            pieces[row][col] = other[row][col]; // Tiefenkopie der Piece-Objekte
        }
    }
}

// Copy-Konstruktor
GenMove::GenMove(const GenMove &other)
{
    copyBoard(pieces, other.pieces);
    playerOnMove = other.playerOnMove;
}

bool GenMove::IsLegalMove(int fromRow, int fromCol, int toRow, int toCol)
{
    return IsValidMove({fromRow, fromCol}, {toRow, toCol}, GetColor({fromRow, fromCol}));
}

// Check if the position is within the board boundaries
bool GenMove::IsValidPosition(int row, int col)
{
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Check if the position is empty or occupied by an opponent's piece
bool GenMove::IsVacantOrOpponent(int row, int col, color colr)
{
    if (!IsValidPosition(row, col))
        return false;
    return (pieces[row][col].type == pieceType::Empty || pieces[row][col].colr != colr);
}
// TODO: Boeser BLick
// Check if the General can move to the target position
bool GenMove::IsValidGeneralMove(position from, position to, color colr)
{
    // Implement the move validation logic for the General here...
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // Check if the target position is within the palace
    if (colr == color::Red) {
        if (to.row < 0 || to.row > 2 || to.col < 3 || to.col > 5) {
            return false;
        }
    } else {
        if (to.row < 7 || to.row > 9 || to.col < 3 || to.col > 5) {
            return false;
        }
    }
    if (pieces[to.col][to.row].colr == colr) {
        // return false;
    }
    // Check if the General moves one step horizontally or vertically
    if (((dx == 0 && std::abs(dy) == 1) || (std::abs(dx) == 1 && dy == 0))
        && IsVacantOrOpponent(to.row, to.col, colr)) {
        return true;
    }

    return false;
}

// Check if the Chariot can move to the target position
bool GenMove::IsValidChariotMove(position from, position to, color color)
{
    if (to.row == from.row && to.col == from.col) {
        return false;
    }
    bool isOwnPiece = true;
    // Check if the Chariot moves horizontally or vertically
    if (from.row == to.row) {
        int step = (to.col > from.col) ? 1 : -1;
        if (step == 1) {
            for (int col = from.col + step; col <= to.col; col += step) {
                if (isOwnPiece == false && pieces[from.row][col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[from.row][col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }

                if (isOwnPiece == false && pieces[from.row][col].colr == color) {
                    return false;
                }
            }
        } else {
            for (int col = from.col + step; col >= to.col; col += step) {
                if (isOwnPiece == false && pieces[from.row][col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[from.row][col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[from.row][col].colr == color) {
                    return false;
                }
            }
        }
    } else if (from.col == to.col) {
        int step = (to.row > from.row) ? 1 : -1;
        if (step == 1) {
            for (int row = from.row + step; row <= to.row; row += step) {
                if (isOwnPiece == false && pieces[row][from.col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[row][from.col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[row][from.col].colr == color) {
                    return false;
                }
            }
        } else {
            for (int row = from.row + step; row >= to.row; row += step) {
                if (isOwnPiece == false && pieces[row][from.col].colr != color) {
                    //isOwnPiece = true;
                    return false; // There is an obstacle in the path
                }
                if (pieces[row][from.col].type != pieceType::Empty) {
                    isOwnPiece = false; // There is an obstacle in the path
                }
                if (isOwnPiece == false && pieces[row][from.col].colr == color) {
                    return false;
                }
            }
        }
    } else {
        return false; // Invalid move for the Chariot
    }
    return true;
}

// Check if the Horse can move to the target position
bool GenMove::IsValidHorseMove(position from, position to, color color)
{
    int dx = to.row - from.row;
    int dy = to.col - from.col;
    // qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);

    if ((dx * dx + dy * dy) == 5) {
        // Check if the position between from and to is vacant
        int checkRow = from.row + dx / 2;
        int checkCol = from.col + dy / 2;
        if (IsValidPosition(checkRow, checkCol)
            && pieces[checkRow][checkCol].type == pieceType::Empty) {
            return true;
        }
    }
    return false;
}

// TODO: Elephant func is buggy
//(to.row == 0 && to.col == 2) || (to.row == 2 && to.col == 4)
//    || (to.row == 2 && to.col == 0) || (to.row == 4 && to.col == 2);

// Check if the Elephant can move to the target position
/*
bool Board::isValidElephantMove(Position from, Position to, Color color)
{
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    if (color == Color::Red) {
        return (to.row >= 5 && to.row <= 9 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));

    } else {
        return (to.row >= 5 && to.row <= 9 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));
    }
}
*/
bool GenMove::IsValidElephantMove(position from, position to, color color)
{
    //qDebug() << "Elephant: from to: " << from.row << from.col << to.row << to.col;

    int dx = to.row - from.row;
    int dy = to.col - from.col;
    //qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);
    if (color == color::Black) {
        return (to.row >= 5 && to.row <= 9 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && IsVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));
        //&& isVacantOrOpponent(to.row, to.col, color));

    } else {
        return (to.row >= 0 && to.row <= 4 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && IsVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));
        //&& isVacantOrOpponent(to.row, to.col, color));
    }
}

// Check if the Advisor can move to the target position
bool GenMove::IsValidAdvisorMove(position from, position to, color color)
{
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    if (color == color::Red) {
        return (to.row >= 0 && to.row <= 2 && to.col >= 3 && to.col <= 5 && (dx * dx + dy * dy) == 2
                && IsVacantOrOpponent(to.row, to.col, color));
    } else {
        return (to.row >= 7 && to.row <= 9 && to.col >= 3 && to.col <= 5 && (dx * dx + dy * dy) == 2
                && IsVacantOrOpponent(to.row, to.col, color));
    }
}

// Check if the Cannon can move to the target position
bool GenMove::IsValidCannonMove(position from, position to, color color)
{
    // Implement the move validation logic for the Cannon here...
    // Check if the Cannon moves horizontally or vertically
    if (from.row == to.row) {
        int step = (to.col > from.col) ? 1 : -1;
        int obstacleCount = 0;
        for (int col = from.col + step; (col != to.col) && (IsValidPosition(from.row, col));
             col += step) {
            if (pieces[from.row][col].type != pieceType::Empty) {
                obstacleCount++;
            }
        }
        if (obstacleCount == 1 && pieces[to.row][to.col].colr == color::Black) {
            return true;
        }

        if (obstacleCount == 0 && pieces[to.row][to.col].type == pieceType::Empty) {
            return true;
        }
    } else if (from.col == to.col) {
        int step = (to.row > from.row) ? 1 : -1;
        int obstacleCount = 0;
        for (int row = from.row + step; row != to.row && (IsValidPosition(row, from.col));
             row += step) {
            if (pieces[row][from.col].type != pieceType::Empty) {
                obstacleCount++;
            }
        }
        if ((obstacleCount == 1) && (pieces[to.row][to.col].colr == color::Black)) {
            return true;
        }
        if (obstacleCount == 0 && pieces[to.row][to.col].type == pieceType::Empty) {
            return true;
        }
    }
    return false;
}

// TODO: Walk before the river is buggy, can do a sidewalk shouldn't so.
// Check if the Soldier can move to the target position
bool GenMove::IsValidSoldierMove(position from, position to, color color)
{
    // Implement the move validation logic for the Soldier here...
    int dx = to.row - from.row;
    int dy = to.col - from.col;
    //    qDebug() << "Soldier:";
    //    qDebug() << "dx:" << dx << "dy:" << dy;

    if (color == color::Black) {
        if (from.row >= 5 && dx == -1 && dy == 0) {
            return true;
        }
        if (from.row < 5 && ((dx == -1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    } else {
        if (from.row <= 4 && dx == 1 && dy == 0) {
            return true;
        }
        if (from.row > 4 && ((dx == 1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    }
    return false;
};
// TODO: Boeser Blick
// Generate all legal moves for the current player from the given board position
std::vector<std::pair<position, position>> GenMove::GenerateLegalMoves(color currentPlayerColor)
{
    std::vector<std::pair<position, position>> legalMoves;

    // Iterate over all squares on the board
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].colr == currentPlayerColor) {
                position from = {row, col};

                // Check all possible moves for the piece on this square
                for (int toRow = 0; toRow < ROWS; ++toRow) {
                    for (int toCol = 0; toCol < COLS; ++toCol) {
                        position to = {toRow, toCol};
                        //qDebug() << "generateLegalMoves: " << from.col << from.row << to.col
                        //         << to.row << isValidMove(from, to, currentPlayerColor);
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
    color c;
    if (currentPlayerColor == color::Red) {
        c = color::Black;
    } else {
        c = color::Red;
    }

    // Find the position of the General of the currentPlayerColor
    position generalPos = FindGeneralPosition(currentPlayerColor);
    //qDebug() << "General: " << generalPos.col << generalPos.row;

    // Check if the General is under attack from the opponent's pieces
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].colr == c) {
                position from = {row, col};
                //qDebug() << "from: " << from.col << from.row << "to: " << generalPos.col
                //         << generalPos.row;
                if (IsValidMove(from, generalPos, c)) {
                    qDebug() << "Check";
                    qDebug() << "Check on " << from.col << from.row << "to: " << generalPos.col
                             << generalPos.row;
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
    // Generate all legal moves for the currentPlayerColor
    std::vector<std::pair<position, position>> legalMoves = GenerateLegalMoves(currentPlayerColor);

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
bool GenMove::PerformMove(position from, position to, color currentPlayerColor)
{
    // Check if the move is valid
    if (!IsValidMove(from, to, currentPlayerColor)) {
        return false;
    }

    // Update the board state
    pieces[to.row][to.col] = pieces[from.row][from.col];
    pieces[from.row][from.col] = Piece(color::Red,
                                       pieceType::Empty,
                                       {from.row, from.col},
                                       QImage(nullptr));

    // Set the previous position to empty

    return true;
}

color GenMove::GetColor(position p)
{
    return pieces[p.row][p.col].colr;
}

Piece GenMove::GetPiece(position p)
{
    return pieces[p.row][p.col];
}

// ...

// Helper function to find the position of the General of the currentPlayerColor
position GenMove::FindGeneralPosition(color currentPlayerColor)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].type == pieceType::General
                && pieces[row][col].colr == currentPlayerColor) {
                return {row, col};
            }
        }
    }
    // Return an invalid position if the General is not found (shouldn't happen in a valid game)
    return {-1, -1};
}

bool GenMove::IsValidMove(position from, position to, color currentPlayerColor)
{
    pieceType pieceType = pieces[from.row][from.col].type;

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
        //qDebug() << "From to cannon" << from.row << from.col << to.row << to.col;
        return IsValidCannonMove(from, to, currentPlayerColor);
    case pieceType::Soldier:
        return IsValidSoldierMove(from, to, currentPlayerColor);
    default:
        return false;
    }

    //return false;
}

std::vector<std::pair<position, position>> GenMove::IsValidPieceMove(const position fromPos)
{
    pieceType pieceType = pieces[fromPos.row][fromPos.col].type;
    color currentPlayerColor = pieces[fromPos.row][fromPos.col].colr;
    std::vector<std::pair<position, position>> legalMoves = GenerateLegalMoves(currentPlayerColor);
    std::vector<std::pair<position, position>> legalPieceMoves, legalPieceMovestmp;

    if (pieceType == pieceType::Empty) {
        //legalPieceMoves.push_back({{-1, -1}, {-1, -1}});
        legalPieceMoves.clear();
        return legalPieceMoves;
    }

    GenMove copy(*this);
    GenMove copy2(*this);

    // Check if the general is in check and if so, only allow moves that get it out of check
    for (auto move : legalMoves) {
        position to = move.second;
        position from = move.first;
        if (fromPos.col == from.col && from.row == fromPos.row) {
            copy = copy2;
            copy.PerformMove(from, to, currentPlayerColor);
            if (!copy.IsCheck(currentPlayerColor) && IsValidMove(from, to, currentPlayerColor)) {
                legalPieceMoves.push_back({fromPos, {to.row, to.col}});
            }
        }
        /*for (auto l : legalPieceMoves) {
            qDebug() << "Legal moves" << l.first.row << l.first.col << l.second.row << l.second.col;
        }*/
    }
    return legalPieceMoves;
}
