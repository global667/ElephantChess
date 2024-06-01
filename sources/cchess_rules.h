#ifndef CCHESS_RULES_H
#define CCHESS_RULES_H

#include <QDebug>
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
    Point(const int x = 0, const int y = 0) : x(x), y(y) {}
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

    [[nodiscard]] bool isValid() const {
        return x >= 0 && x < 9 && y >= 0 && y < 10;
    }
};

class Piece {
public:
    Color color;
    QString name;
    QString euroName;
    QString euroNameDesc;
    int id{};
public:
    Piece(Color color, QString name);
    virtual ~Piece();

    virtual bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);
    virtual std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>> &board);

    [[nodiscard]] Color getColor() const;
    [[nodiscard]] QString getName() const;
    [[nodiscard]] QString getEuroName() const;
    [[nodiscard]] QString getEuroNameDesc() const;
    [[nodiscard]] int getId() const;

    auto operator=(const Piece &other) -> Piece;
};

class General final : public Piece {
public:
    General(const Color color, const QString &name) : Piece(color, name) {euroName = "General";
        euroNameDesc = "Moves and captures within the palace (3x3 area) one square\n"
                       "orthogonally and may not face the opposing General directly\n"
                       "along the same file without an intervening piece.";
        id = 0;
    }
    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board)  override {
        // Bewegungslogik für den General
        const int dx = std::abs(to.x - from.x);
        const int dy = std::abs(to.y - from.y);

        const Piece piece = *board[to.x][to.y];
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

class Advisor final : public Piece {
public:
    Advisor(const Color color, const QString &name) : Piece(color, name) {euroName = "Advisor"; euroNameDesc = "Moves and captures one point diagonally and must stay within the palace.";
        id = 1;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
        // Implementieren Sie die Bewegungslogik für den Berater

        // Bewegungslogik für den General
        const int dx = std::abs(to.x - from.x);
        const int dy = std::abs(to.y - from.y);

        const Piece piece = *board[to.x][to.y];
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

class Elephant final : public Piece {
public:
    Elephant(const Color color, const QString &name) : Piece(color, name) {euroName = "Elephant";
        euroNameDesc ="Moves exactly two points diagonally and cannot cross the river, serving as a defensive piece.";
        id = 2;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
        // Implementieren Sie die Bewegungslogik für den Elephant
        if (((to.x >= 0 && to.x < 6 && to.y >= 0 && to.y < 5) || (to.x >= 0 && to.x < 6 && to.y >= 4 && to.y < 9)) && board[from.x][from.y]->getColor() == Color::Red) {
            if (std::abs(to.x - from.x) == 2 && std::abs(to.y - from.y) == 2) {
                if (board[(to.x + from.x) / 2][(to.y + from.y) / 2]->getColor() == Color::None
                    && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                    return true;
                }
            }
        }

        if (((to.x >= 5 && to.x < 10 && to.y >= 0 && to.y < 5) || (to.x >= 5 && to.x < 10 && to.y >= 4 && to.y < 9)) && board[from.x][from.y]->getColor() == Color::Black) {
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

class Horse final : public Piece {
public:
    Horse(const Color color, const QString &name) : Piece(color, name) {euroName = "Knight";
        euroNameDesc = "Moves one point orthogonally and then one point diagonally outward, similar to the knight in international chess, but its movement can be blocked by an intervening piece.";
        id = 3;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
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

class Chariot final : public Piece {
public:
    Chariot(const Color color, const QString &name) : Piece(color, name) {euroName = "Chariot";
        euroNameDesc = "Moves and captures any number of points along a row or column across the board without leaping.";
        id = 4;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
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
                    const int start = std::min(from.x, to.x);
                    const int end = std::max(from.x, to.x);
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

    std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position,
                                                            const std::vector<std::vector<std::shared_ptr<Piece>>> &board) override {
        // Implementieren Sie die Bewegungslogik für den Chariot
        std::vector<std::pair<Point, Point>> validMoves;
        Point from = position;
        const std::vector<Point> chariotMoves = {Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1)};

        for (const Point& move : chariotMoves) {
            Point to = from + move;
            while (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                if (board[to.x][to.y]->getColor() == Color::None) {
                    validMoves.emplace_back(from, to);
                } else {
                    if (board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                        validMoves.emplace_back(from, to);
                    }
                    break;
                }
                to = to + move;
            }
        }
 /*       for (auto m : validMoves) {
            if(!Board::isCheck(m.first, m.second, board, board[m.first.x][m.first.y]->getColor())) {
                validMoves.erase(m);
            }
        }
 */
        return validMoves;
    }

    /*      if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
             if (board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                 if (from.x == to.x) {
                     int start = std::min(from.y, to.y);
                     int end = std::max(from.y, to.y);
                     for (int i = start + 1; i < end; ++i) {
                         if (board[from.x][i]->getColor() != Color::None) {
                             return false;
                         }
                     }
                     return true;
                 }

                 if (from.y == to.y) {
                     int start = std::min(from.x, to.x);
                     int end = std::max(from.x, to.x);
                     for (int i = start + 1; i < end; ++i) {
                         if (board[i][from.y]->getColor() != Color::None) {
                             return false;
                         }
                     }
                     return true;
                 }
             }
         }

         return false;
     }*/
};

class Cannon final : public Piece {
public:
    Cannon(const Color color, const QString &name) : Piece(color, name) {euroName = "Cannon";
        euroNameDesc = "Moves like the chariot but captures by leaping exactly one piece, friend or foe, along its path to its target.";
        id = 5;
    }

    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
        // Implementieren Sie die Bewegungslogik für den Kanone
        if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
            if (board[to.x][to.y]->getColor() == Color::None) {
                if (from.x == to.x) {
                    const int start = std::min(from.y, to.y);
                    const int end = std::max(from.y, to.y);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[from.x][i]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }

                if (from.y == to.y) {
                    const int start = std::min(from.x, to.x);
                    const int end = std::max(from.x, to.x);
                    for (int i = start + 1; i < end; ++i) {
                        if (board[i][from.y]->getColor() != Color::None){
                            return false;
                        }
                    }
                    return true;
                }
            } else if (board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                if (from.x == to.x) {
                    const int start = std::min(from.y, to.y);
                    const int end = std::max(from.y, to.y);
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
                    const int start = std::min(from.x, to.x);
                    const int end = std::max(from.x, to.x);
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
/*
    std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>> board) override {
        // Implementieren Sie die Bewegungslogik für den Kanone
        if (board != this->board) {
            initializeCannonMoveTables(board);
            this->board = board;
        }
        std::vector<Point> moves;
        std::vector<std::pair<Point, Point>> allMoves;
        int x = position.x;
        int y = position.y;
        for (int d = 0; d < 4; ++d) {
            moves.insert(moves.end(), cannonMoves[x][y][d].begin(), cannonMoves[x][y][d].end());
        }
        for(const auto& move : moves) {
            allMoves.push_back({position, move});
        }

        return allMoves;
    }
*/
private:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;

    enum Direction {
        North, South, East, West
    };

    const std::array<Point, 4> directions = {
        { {0, -1}, {0, 1}, {1, 0}, {-1, 0} } // Norden, Süden, Osten, Westen
    };

    // Tabellen für die Zugmöglichkeiten der Kanone für jede Richtung und Position
    std::vector<Point> cannonMoves[9][10][4];

    void initializeCannonMoveTables(const std::vector<std::vector<std::shared_ptr<Piece>>>& board)  {
        for (int x = 0; x < 9; ++x) {
            for (int y = 0; y < 10; ++y) {
                for (int d = 0; d < 4; ++d) {
                    Point current = {x, y};
                    bool springboardEncountered = false;

                    while (true) {
                        current = current + directions[d];
                        if (!current.isValid()) break;

                        if (board[current.y][current.x]->color != Color::None) { // ' ' bedeutet leeres Feld
                            if (!springboardEncountered) {
                                springboardEncountered = true; // Erstes Hindernis gefunden
                            } else {
                                // Zweitens Hindernis beendet die Bewegung in dieser Richtung
                                //if (board[current.y][current.x]->color != board[x][y]->color) {
                                    cannonMoves[x][y][d].push_back(current);
                                    break;
                                //} else {
                                    break;
                               // }
                            }
                        } else {
                            if (!springboardEncountered) {
                                // Freies Feld, ohne dass wir über ein Hindernis gesprungen sind
                                cannonMoves[x][y][d].push_back(current);
                           // } else {
                                // Nach dem ersten Hindernis können wir hier schlagen
                              //  cannonMoves[x][y][d].push_back(current);

                           }
                        }
                    }
                }
            }
        }
    }

};

class Soldier final : public Piece {
public:
    Soldier(const Color color, const QString &name) : Piece(color, name) {euroName = "Soldier";
        euroNameDesc = "Moves and captures by advancing one point forward; once across the river, it may also move and capture one point horizontally.";
        id = 6;
    }
    bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) override {
        // Ensure move is within board bounds
        if (to.x < 0 || to.x >= 10 || to.y < 0 || to.y >= 9) {
            return false;
        }

        // Movement vector
        const int dx = to.x - from.x;
        const int dy = to.y - from.y;

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
    std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) override {
        // Implementieren Sie die Bewegungslogik für den Soldat
        std::vector<std::pair<Point, Point>> validMoves;
        // Get the color of the piece
        Color color = board[position.x][position.y]->getColor();
        if (color == Color::Red) {
            Point from = position;
            Point to = from + Point(1, 0);
            if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                    validMoves.emplace_back(from, to);
                }
            }
            if (from.x >= 5) {
                to = from + Point(0, 1);
                if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                    if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                        validMoves.emplace_back(from, to);
                    }
                }
                to = from + Point(0, -1);
                if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                    if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                        validMoves.emplace_back(from, to);
                    }
                }
            }
        } else {
            Point from = position;
            Point to = from + Point(-1, 0);
            if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                    validMoves.emplace_back(from, to);
                }
            }
            if (from.x < 5) {
                to = from + Point(0, 1);
                if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                    if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
                        validMoves.emplace_back(from, to);
                    }
                }
                to = from + Point(0, -1);
                if (to.x >= 0 && to.x < 10 && to.y >= 0 && to.y < 9) {
                    if (board[to.x][to.y]->getColor() == Color::None || board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()){
                        validMoves.emplace_back(from, to);
                    }
                }
            }
        }
        return validMoves;
    }
};

class Board {
public:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    Color players_color;

public:
    Board();

    [[nodiscard]] std::shared_ptr<Piece> getPieceAt(const Point& position) const;

    static std::vector<std::pair<Point, Point>> getAllValidMoves(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    //std::vector<std::pair<Point, Point>> getValidMovesForPiece(const Point& position, const //std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    static bool movePiece(const Point& from, const Point& to, std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    // undo a move on specific board
    static void undoMove(const Point& from, const Point& to, const std::shared_ptr<Piece> &piece, std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    void setupInitialPositions();

    void toggleColor();

    static Color toggleColor(Color color);

    // isCheck checks if the player is in check
    static bool isCheck(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    // look for evil glare booth generals and check if there is a piece in between
    static bool isEvilGlare(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);

    static bool isCheckmate(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board);
};


#endif // CCHESS_RULES_H
