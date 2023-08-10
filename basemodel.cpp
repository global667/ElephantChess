#include "basemodel.h"

bool Board::isLegalMove(int fromRow, int fromCol, int toRow, int toCol)
{
    return isValidMove({fromRow, fromCol}, {toRow, toCol}, getColor({fromRow, fromCol}));
}

// Check if the position is within the board boundaries
bool Board::isValidPosition(int row, int col)
{
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Check if the position is empty or occupied by an opponent's piece
bool Board::isVacantOrOpponent(int row, int col, Color color)
{
    if (!isValidPosition(row, col))
        return false;
    return (pieces[row][col].type == PieceType::Empty || pieces[row][col].color != color);
}
// TODO: Boeser BLick
// Check if the General can move to the target position
bool Board::isValidGeneralMove(Position from, Position to, Color color)
{
    // Implement the move validation logic for the General here...
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // Check if the target position is within the palace
    if (color == Color::Red) {
        if (to.row < 0 || to.row > 2 || to.col < 3 || to.col > 5) {
            return false;
        }
    } else {
        if (to.row < 7 || to.row > 9 || to.col < 3 || to.col > 5) {
            return false;
        }
    }

    // Check if the General moves one step horizontally or vertically
    if ((dx == 0 && std::abs(dy) == 1) || (std::abs(dx) == 1 && dy == 0)) {
        return true;
    }

    return false;
}

// Check if the Chariot can move to the target position
bool Board::isValidChariotMove(Position from, Position to, Color color)
{
    // Check if the Chariot moves horizontally or vertically
    if (from.row == to.row) {
        int step = (to.col > from.col) ? 1 : -1;
        for (int col = from.col + step; col != to.col; col += step) {
            if (pieces[from.row][col].type != PieceType::Empty) {
                return false; // There is an obstacle in the path
            }
        }
    } else if (from.col == to.col) {
        int step = (to.row > from.row) ? 1 : -1;
        for (int row = from.row + step; row != to.row; row += step) {
            if (pieces[row][from.col].type != PieceType::Empty) {
                return false; // There is an obstacle in the path
            }
        }
    } else {
        return false; // Invalid move for the Chariot
    }
    return true;
}

// Check if the Horse can move to the target position
bool Board::isValidHorseMove(Position from, Position to, Color color)
{
    int dx = to.row - from.row;
    int dy = to.col - from.col;
    qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);

    if ((dx * dx + dy * dy) == 5) {
        // Check if the position between from and to is vacant
        int checkRow = from.row + dx / 2;
        int checkCol = from.col + dy / 2;
        if (isValidPosition(checkRow, checkCol)
            && pieces[checkRow][checkCol].type == PieceType::Empty) {
            return true;
        }
    }
    return false;
}
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
bool Board::isValidElephantMove(Position from, Position to, Color color)
{
    //qDebug() << "Elephant: from to: " << from.row << from.col << to.row << to.col;
    int dx = to.row - from.row;
    int dy = to.col - from.col;
    //qDebug() << "(dx * dx + dy * dy) = " << (dx * dx + dy * dy);
    if (color == Color::Black) {
        return (to.row >= 5 && to.row <= 9 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));
    } else {
        return (to.row >= 0 && to.row <= 4 && to.col >= 0 && to.col <= 8 && (dx * dx + dy * dy) == 8
                && isVacantOrOpponent(from.row + dx / 2, from.col + dy / 2, color));
    }
}

// Check if the Advisor can move to the target position
bool Board::isValidAdvisorMove(Position from, Position to, Color color)
{
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    if (color == Color::Red) {
        return (to.row >= 0 && to.row <= 2 && to.col >= 3 && to.col <= 5
                && (dx * dx + dy * dy) == 2);
    } else {
        return (to.row >= 7 && to.row <= 9 && to.col >= 3 && to.col <= 5
                && (dx * dx + dy * dy) == 2);
    }
}

// Check if the Cannon can move to the target position
bool Board::isValidCannonMove(Position from, Position to, Color color)
{
    // Implement the move validation logic for the Cannon here...
    // Check if the Cannon moves horizontally or vertically
    if (from.row == to.row) {
        int step = (to.col > from.col) ? 1 : -1;
        int obstacleCount = 0;
        for (int col = from.col + step; (col != to.col) && (isValidPosition(from.row, col));
             col += step) {
            if (pieces[from.row][col].type != PieceType::Empty) {
                obstacleCount++;
            }
        }
        if (obstacleCount == 1 && pieces[to.row][to.col].type != PieceType::Empty) {
            return true;
        }
    } else if (from.col == to.col) {
        int step = (to.row > from.row) ? 1 : -1;
        int obstacleCount = 0;
        for (int row = from.row + step; row != to.row && (isValidPosition(row, from.col));
             row += step) {
            if (pieces[row][from.col].type != PieceType::Empty) {
                obstacleCount++;
            }
        }
        if (obstacleCount == 1 && pieces[to.row][to.col].type != PieceType::Empty) {
            return true;
        }
    }
    return false;
}

// TODO: Walk before the river is buggy, can do a sidewalk shouldn't so.
// Check if the Soldier can move to the target position
bool Board::isValidSoldierMove(Position from, Position to, Color color)
{
    // Implement the move validation logic for the Soldier here...
    int dx = to.row - from.row;
    int dy = to.col - from.col;
    //    qDebug() << "Soldier:";
    //    qDebug() << "dx:" << dx << "dy:" << dy;

    if (color == Color::Red) {
        if (from.row >= 5 && dx == 1 && dy == 0) {
            return true;
        }
        if (from.row < 5 && ((dx == 1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    } else {
        if (from.row <= 4 && dx == -1 && dy == 0) {
            return true;
        }
        if (from.row > 4 && ((dx == -1 && dy == 0) || (dx == 0 && std::abs(dy) == 1))) {
            return true;
        }
    }
    return false;
};
// TODO: Boeser Blick
// Generate all legal moves for the current player from the given board position
std::vector<std::pair<Position, Position>> Board::generateLegalMoves(Color currentPlayerColor)
{
    std::vector<std::pair<Position, Position>> legalMoves;

    // Iterate over all squares on the board
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].color == currentPlayerColor) {
                Position from = {row, col};

                // Check all possible moves for the piece on this square
                for (int toRow = 0; toRow < ROWS; ++toRow) {
                    for (int toCol = 0; toCol < COLS; ++toCol) {
                        Position to = {toRow, toCol};
                        //qDebug() << "generateLegalMoves: " << from.col << from.row << to.col
                        //         << to.row << isValidMove(from, to, currentPlayerColor);
                        // Check if the move is valid for the piece
                        if (isValidMove(from, to, currentPlayerColor)) {
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
bool Board::isCheck(Color currentPlayerColor)
{
    // Find the position of the General of the currentPlayerColor
    Position generalPos = findGeneralPosition(currentPlayerColor);

    // Check if the General is under attack from the opponent's pieces
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].color != currentPlayerColor) {
                Position from = {row, col};
                if (isValidMove(from, generalPos, pieces[row][col].color)) {
                    return true;
                }
            }
        }
    }

    return false;
}

// Check if a player is in checkmate
bool Board::isCheckmate(Color currentPlayerColor)
{
    // Generate all legal moves for the currentPlayerColor
    std::vector<std::pair<Position, Position>> legalMoves = generateLegalMoves(currentPlayerColor);

    // Check if any move can resolve the check
    for (const auto &move : legalMoves) {
        Board copy(*this); // Make a copy of the current board state
        if (copy.performMove(move.first, move.second, currentPlayerColor)) {
            // If the move is valid and resolves the check, then the player is not in checkmate
            return false;
        }
    }

    return true; // No move resolves the check, so the player is in checkmate
}

// Print the board on console

// ...
// Perform a move on the board and update the board state
bool Board::performMove(Position from, Position to, Color currentPlayerColor)
{
    // Check if the move is valid
    if (!isValidMove(from, to, currentPlayerColor)) {
        return false;
    }

    // Update the board state
    pieces[to.row][to.col] = pieces[from.row][from.col];
    pieces[from.row][from.col] = Piece(Color::Red,
                                       PieceType::Empty,
                                       {from.row, from.col},
                                       QImage(nullptr));

    // Set the previous position to empty

    return true;
}

Color Board::getColor(Position p)
{
    return pieces[p.row][p.col].color;
}

Piece Board::getPiece(Position p)
{
    return pieces[p.row][p.col];
}

// ...

// Helper function to find the position of the General of the currentPlayerColor
Position Board::findGeneralPosition(Color currentPlayerColor)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (pieces[row][col].type == PieceType::General
                && pieces[row][col].color == currentPlayerColor) {
                return {row, col};
            }
        }
    }
    // Return an invalid position if the General is not found (shouldn't happen in a valid game)
    return {-1, -1};
}

bool Board::isValidMove(Position from, Position to, Color currentPlayerColor)
{
    PieceType pieceType = pieces[from.row][from.col].type;

    switch (pieceType) {
    case PieceType::General:
        return isValidGeneralMove(from, to, currentPlayerColor);
    case PieceType::Advisor:
        return isValidAdvisorMove(from, to, currentPlayerColor);
    case PieceType::Elephant:
        return isValidElephantMove(from, to, currentPlayerColor);
    case PieceType::Horse:
        return isValidHorseMove(from, to, currentPlayerColor);
    case PieceType::Chariot:
        return isValidChariotMove(from, to, currentPlayerColor);
    case PieceType::Cannon:
        return isValidCannonMove(from, to, currentPlayerColor);
    case PieceType::Soldier:
        return isValidSoldierMove(from, to, currentPlayerColor);
    default:
        return false;
    }

    //return false;
}
//Q_GLOBAL_STATIC(BaseModel, basemodel)
BaseModel basemodel;
Position glfrom = {-1, -1}, glto = {-1, -1};
