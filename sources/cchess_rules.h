#ifndef CCHESS_RULES_H
#define CCHESS_RULES_H

#include "qdebug.h"
#include <QObject>
#include <QString>

#include <vector>
#include <memory>

enum class PieceType {
    General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier, Empty
};

enum class Color {
    Red, Black, None
};

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return x != other.x || y != other.y;
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    bool operator>(const Point& other) const {
        return x > other.x || (x == other.x && y > other.y);
    }

    bool operator<=(const Point& other) const {
        return x <= other.x || (x == other.x && y <= other.y);
    }

    bool operator>=(const Point& other) const {
        return x >= other.x || (x == other.x && y >= other.y);
    }

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }

    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }

    bool operator*(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Piece {
public:
    Color color;
    QString name;
    QString euroName;
    QString euroNameDesc;
    int id;
public:
    Piece(Color color, QString name);
    virtual ~Piece();

    virtual bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const;;
    std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const;;

    Color getColor() const;
    QString getName() const;
    QString getEuroName() const;
    QString getEuroNameDesc() const;
    int getId() const;

    Piece operator=(const Piece& other);
};

class General : public Piece {
public:
    General(Color color, QString name) : Piece(color, name) {euroName = "General";
        euroNameDesc = "Moves and captures within the palace (3x3 area) one square\n"
                       "orthogonally and may not face the opposing General directly\n"
                       "along the same file without an intervening piece.";
        id = 0;
    }
    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Bewegungslogik für den General
        int dx = std::abs(to.x - from.x);
        int dy = std::abs(to.y - from.y);

        Piece piece = *board[to.x][to.y];
        if (to.x >= 0 && to.x < 3 && to.y >= 3 && to.y < 6) {
            if ((dx == 1 && dy == 0) || (dy == 1 && dx == 0)) {
                if (piece.getColor() != Color::Red) {
                    return true;
                }
            }
        }

        if (to.x >= 7 && to.x < 10 && to.y >= 3 && to.y < 6) {
            if ((dx == 1 && dy == 0) || (dy == 1 && dx == 0)) {
                if (piece.getColor() != Color::Black) {
                    return true;
                }
            }
        }
        return false;

    }
};

class Advisor : public Piece {
public:
    Advisor(Color color, QString name) : Piece(color, name) {euroName = "Advisor"; euroNameDesc = "Moves and captures one point diagonally and must stay within the palace.";
        id = 1;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Implementieren Sie die Bewegungslogik für den Berater

        // Bewegungslogik für den General
        int dx = std::abs(to.x - from.x);
        int dy = std::abs(to.y - from.y);

        Piece piece = *board[to.x][to.y];
        if (to.x >= 0 && to.x < 3 && to.y >= 3 && to.y < 6) {
            if ((dx == 1 && dy == 1)) {
                if (piece.getColor() != Color::Red) {
                    return true;
                }
            }
        }

        if (to.x >= 7 && to.x < 10 && to.y >= 3 && to.y < 6) {
            if ((dx == 1 && dy == 1)) {
                if (piece.getColor() != Color::Black) {
                    return true;
                }
            }
        }
        return false;
    }

    // std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
    //     return {};
    // }
};

class Elephant : public Piece {
public:
    Elephant(Color color, QString name) : Piece(color, name) {euroName = "Elephant";
        euroNameDesc ="Moves exactly two points diagonally and cannot cross the river, serving as a defensive piece.";
        id = 2;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Implementieren Sie die Bewegungslogik für den Elephant
        if ((to.x >= 0 && to.x < 6 && to.y >= 0 && to.y < 5) || (to.x >= 0 && to.x < 6 && to.y >= 4 && to.y < 9)) {
            if (std::abs(to.x - from.x) == 2 && std::abs(to.y - from.y) == 2) {
                if (board[(to.x + from.x) / 2][(to.y + from.y) / 2]->getColor() == Color::None
                    && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                    return true;
                }
            }
        }

        return false;
    }

    // std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
    //     return {};
    // }
};

class Horse : public Piece {
public:
    Horse(Color color, QString name) : Piece(color, name) {euroName = "Horse";
        euroNameDesc = "Moves one point orthogonally and then one point diagonally outward, similar to the knight in international chess, but its movement can be blocked by an intervening piece.";
        id = 3;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Implementieren Sie die Bewegungslogik für den Berater
        if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
            if (std::abs(to.x - from.x) == 2 && std::abs(to.y - from.y) == 1) {
                if (board[(to.x + from.x) / 2][from.y]->getColor() == Color::None
                    && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                    return true;
                }
            }

            if (std::abs(to.x - from.x) == 1 && std::abs(to.y - from.y) == 2) {
                if (board[from.x][(to.y + from.y) / 2]->getColor() == Color::None
                    && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                    return true;
                }
            }
        }
        return false;
    }

    // std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
    //     return {};
    // }
};

class Chariot : public Piece {
public:
    Chariot(Color color, QString name) : Piece(color, name) {euroName = "Chariot";
        euroNameDesc = "Moves and captures any number of points along a row or column across the board without leaping.";
        id = 4;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Implementieren Sie die Bewegungslogik für den Chariot
        if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
            if (board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                if (from.x == to.x) {
                    int start = std::min(from.y, to.y);
                    int end = std::max(from.y, to.y);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[from.x][i]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }

                if (from.y == to.y) {
                    int start = std::min(from.x, to.x);
                    int end = std::max(from.x, to.x);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[i][from.y]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }
            }
        }

        return false;
    }

    // std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
    //     return {};
    // }
};

class Cannon : public Piece {
public:
    Cannon(Color color, QString name) : Piece(color, name) {euroName = "Cannon";
    euroNameDesc = "Moves like the chariot but captures by leaping exactly one piece, friend or foe, along its path to its target.";
    id = 5;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Implementieren Sie die Bewegungslogik für den Kanone
        if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
            if (board[to.x][to.y]->getColor() == Color::None) {
                if (from.x == to.x) {
                    int start = std::min(from.y, to.y);
                    int end = std::max(from.y, to.y);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[from.x][i]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }

                if (from.y == to.y) {
                    int start = std::min(from.x, to.x);
                    int end = std::max(from.x, to.x);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[i][from.y]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }
            } else if (board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                if (from.x == to.x) {
                    int start = std::min(from.y, to.y);
                    int end = std::max(from.y, to.y);
                    int count = 0;
                    for (int i = start + 1; i < end; ++i) {
                        if (board[from.x][i]->getColor() != Color::None){
                            count++;
                        }
                    }
                    if (count == 1) {
                        return true;
                    }
                }

                if (from.y == to.y) {
                    int start = std::min(from.x, to.x);
                    int end = std::max(from.x, to.x);
                    int count = 0;
                    for (int i = start + 1; i < end; ++i) {
                        if (board[i][from.y]->getColor() != Color::None){
                            count++;
                        }
                    }
                    if (count == 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
    //     return {};
    // }
};

class Soldier : public Piece {
public:
    Soldier(Color color, QString name) : Piece(color, name) {euroName = "Soldier";
    euroNameDesc = "Moves and captures by advancing one point forward; once across the river, it may also move and capture one point horizontally.";
        id = 6;
    }
    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override {
        // Ensure move is within board bounds
        if (to.x < 0 || to.x >= 10 || to.y < 0 || to.y >= 9) {
            return false;
        }

        // Movement vector
        int dx = to.x - from.x;
        int dy = to.y - from.y;

        // Check for moving onto a piece of the same color
        if (board[to.x][to.y] && board[to.x][to.y]->getColor() == getColor()) {
            return false;
        }

        // Movement rules before and after crossing the river
        if (getColor() == Color::Black) { // Red moves upwards
            if (from.x >= 5) { // Before crossing the river
                return (dx == -1 && dy == 0); // Only forward
            } else { // After crossing the river
                return (dx == -1 && dy == 0) || (dx == 0 && std::abs(dy) == 1); // Forward or sideways
            }
        } else { // Black moves downwards
            if (from.x < 5) { // Before crossing the river
                return (dx == 1 && dy == 0); // Only forward
            } else { // After crossing the river
                return (dx == 1 && dy == 0) || (dx == 0 && std::abs(dy) == 1); // Forward or sideways
            }
        }
    }
};

class Board {
public:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    Color players_color;

public:
    Board();

    std::shared_ptr<Piece> getPieceAt(const Point& position) const;

    std::vector<std::pair<Point, Point>> getAllValidMoves(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    std::vector<std::pair<Point, Point>> getValidMovesForPiece(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    bool movePiece(const Point& from, const Point& to, std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    // undo a move on specific board
    void undoMove(const Point& from, const Point& to, std::shared_ptr<Piece> piece, std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    void setupInitialPositions();

    void toggleColor();

    Color toggleColor(Color color);

    // isCheck checks if the player is in check
    bool isCheck(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    // look for evil glare booth generals and check if there is a piece in between
    bool isEvilGlare(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    bool isCheckmate(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);
};


#endif // CCHESS_RULES_H
