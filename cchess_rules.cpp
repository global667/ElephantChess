#include "cchess_rules.h"



Piece::Piece(Color color, QString name) : color(color), name(name) {}

Piece::~Piece() {}

bool Piece::isValidMove(const Point &from, const Point &to, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) const { return false;}

std::vector<std::pair<Point, Point> > Piece::generateValidMoves(const Point &position, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) const {
    std::vector<std::pair<Point, Point>> moves;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            Point move = {i, j};
            if (move.x >= 0 && move.x < 10 && move.y >= 0 && move.y < 9) {
                if (isValidMove(position, move, board)) {
                    moves.push_back({position, move});
                }
            }
        }
    }
    return moves;
}

Color Piece::getColor() const { return color; }

QString Piece::getName() const { return name; }

QString Piece::getEuroName() const { return euroName; }

Piece Piece::operator=(const Piece &other) {
    color = other.color;
    name = other.name;
    return *this;
}

Board::Board() : board(10, std::vector<std::shared_ptr<Piece>>(9, nullptr)) {
    // Initialisiere das Brett mit Null-Pointern oder spezifischen Startpositionen
    setupInitialPositions();
}

std::shared_ptr<Piece> Board::getPieceAt(const Point &position) const {
    return board[position.x][position.y];
}

std::vector<std::pair<Point, Point> > Board::getAllValidMoves(Color color, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    std::vector<std::pair<Point, Point>> moves;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] && board[i][j]->getColor() == color) {
                auto validMoves = getValidMovesForPiece({i, j}, board);
                //moves.insert(moves.end(), validMoves.begin(), validMoves.end());
                for (const auto& move : validMoves) {
                    // check if board is in check or evil glare after move
                    std::vector<std::vector<std::shared_ptr<Piece>>> new_board = board;
                    movePiece(move.first, move.second, new_board);
                    if (!isCheck(toggleColor(color), new_board) && !isEvilGlare(color, new_board))
                        moves.push_back(move);
                    undoMove(move.first, move.second, board[move.second.x][move.second.y], new_board);
                }
            }
        }
    }
    return moves;
}

std::vector<std::pair<Point, Point> > Board::getValidMovesForPiece(const Point &position, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    if (board[position.x][position.y]) {
        auto moves = board[position.x][position.y]->generateValidMoves(position, board);

        // check if moves is in check or evil glare
        for (const auto& move : moves) {
            std::vector<std::vector<std::shared_ptr<Piece>>> new_board = board;
            movePiece(move.first, move.second, new_board);
            if (isCheck(toggleColor(board[position.x][position.y]->getColor()), new_board) || isEvilGlare(board[position.x][position.y]->getColor(), new_board)) {
                moves.erase(std::remove(moves.begin(), moves.end(), move), moves.end());
            }
            undoMove(move.first, move.second, board[move.second.x][move.second.y], new_board);
        }
        return moves;
    }
    return {};
}

bool Board::movePiece(const Point &from, const Point &to, std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    if (board[from.x][from.y] && board[from.x][from.y]->isValidMove(from, to, board)) {
        // Bewegung ist gültig
        board[to.x][to.y] = std::move(board[from.x][from.y]);
        board[from.x][from.y] = std::make_shared<Piece>(Color::None, "");;
        return true;
    }
    return false;
}

void Board::undoMove(const Point &from, const Point &to, std::shared_ptr<Piece> piece, std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    board[from.x][from.y] = std::move(board[to.x][to.y]);
    board[to.x][to.y] = piece;
}

void Board::setupInitialPositions() {

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            board[i][j] = std::make_shared<Piece>(Color::None, "");
        }
    }

    // Hier Figuren auf dem Brett platzieren
    board[0][0] = std::make_shared<Chariot>(Color::Red,"\u8ECA" );
    board[0][1] = std::make_shared<Horse>(Color::Red, "\u99AC");
    board[0][2] = std::make_shared<Elephant>(Color::Red, "\u76f8");
    board[0][3] = std::make_shared<Advisor>(Color::Red, "\u4ed5");
    board[0][4] = std::make_shared<General>(Color::Red, "\u5e25");
    board[0][5] = std::make_shared<Advisor>(Color::Red, "\u4ed5");
    board[0][6] = std::make_shared<Elephant>(Color::Red, "\u76f8");
    board[0][7] = std::make_shared<Horse>(Color::Red, "\u99AC");
    board[0][8] = std::make_shared<Chariot>(Color::Red,"\u8ECA");

    board[2][1] = std::make_shared<Cannon>(Color::Red, "\u70ae");
    board[2][7] = std::make_shared<Cannon>(Color::Red, "\u70ae");

    board[3][0] = std::make_shared<Soldier>(Color::Red,"\u5175");
    board[3][2] = std::make_shared<Soldier>(Color::Red,"\u5175");
    board[3][4] = std::make_shared<Soldier>(Color::Red,"\u5175");
    board[3][6] = std::make_shared<Soldier>(Color::Red,"\u5175");
    board[3][8] = std::make_shared<Soldier>(Color::Red,"\u5175");

    board[9][0] = std::make_shared<Chariot>(Color::Black,"\u8ECA");
    board[9][1] = std::make_shared<Horse>(Color::Black, "\u99AC");
    board[9][2] = std::make_shared<Elephant>(Color::Black, "\u8c61");
    board[9][3] = std::make_shared<Advisor>(Color::Black, "\u58eb");
    board[9][4] = std::make_shared<General>(Color::Black, "\u5c07");
    board[9][5] = std::make_shared<Advisor>(Color::Black, "\u58eb");
    board[9][6] = std::make_shared<Elephant>(Color::Black, "\u8c61");
    board[9][7] = std::make_shared<Horse>(Color::Black, "\u99AC");
    board[9][8] = std::make_shared<Chariot>(Color::Black,"\u8ECA");

    board[7][1] = std::make_shared<Cannon>(Color::Black, "\u7832");
    board[7][7] = std::make_shared<Cannon>(Color::Black, "\u7832");

    board[6][0] = std::make_shared<Soldier>(Color::Black,	"\u5352");
    board[6][2] = std::make_shared<Soldier>(Color::Black,	"\u5352");
    board[6][4] = std::make_shared<Soldier>(Color::Black,	"\u5352");
    board[6][6] = std::make_shared<Soldier>(Color::Black,	"\u5352");
    board[6][8] = std::make_shared<Soldier>(Color::Black,	"\u5352");
}

void Board::toggleColor() {
    players_color = (players_color == Color::Red) ? Color::Black : Color::Red;
}

Color Board::toggleColor(Color color) {
    return (color == Color::Red) ? Color::Black : Color::Red;
}

bool Board::isCheck(Color color, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    Point generalPosition;
    if (color == Color::Black) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j]->getName() == "\u5e25" && board[i][j]->getColor() != color) {
                    generalPosition = {i, j};
                }
            }
        }
    } else {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j]->getName() == "\u5c07" && board[i][j]->getColor() != color) {
                    generalPosition = {i, j};
                }
            }
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j]->getColor() == color) {
                if (board[i][j]->isValidMove({i, j}, generalPosition, board)) {
                    return true;
                }
            }
        }
    }
    /*       for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j]->getName() == "\u5c07" && board[i][j]->getColor() != color) {
                    generalPosition = {i, j};
                }
            }
        }
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j]->getColor() == color) {
                    if (board[i][j]->isValidMove({i, j}, generalPosition, board)) {
                        return true;
                    }
                }
            }
        }*/
    return false;
}

bool Board::isEvilGlare(Color color, const std::vector<std::vector<std::shared_ptr<Piece> > > &board)
{
    Point generalPosition;
    Point generalPosition2;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j]->getName() == "\u5c07") {
                generalPosition = {i, j};
            }
            if (board[i][j]->getName() == "\u5e25") {
                generalPosition2 = {i, j};
            }
        }
    }
    bool isEvil = false;
    if (generalPosition.y == generalPosition2.y)
    {
        isEvil = true;
        for (int i = generalPosition2.x + 1; i < generalPosition.x; i++)
        {
            if (!board[i][generalPosition.y]->getName().isEmpty())
            {
                isEvil = false;
            }
        }
    }
    return isEvil;
}

bool Board::isCheckmate(Color color, const std::vector<std::vector<std::shared_ptr<Piece> > > &board) {
    if (isCheck(color, board)) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j]->getColor() == color) {
                    auto moves = getValidMovesForPiece({i, j}, board);
                    if (!moves.empty()) {
                        return false;
                    }
                }
            }
        }
    } else {
        return false;
    }
    return true;
}
