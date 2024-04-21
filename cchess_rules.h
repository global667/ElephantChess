#ifndef CCHESS_RULES_H
#define CCHESS_RULES_H

#include <QObject>
#include <QString>

#include <vector>
#include <memory>

class cchess_rules : public QObject
{
    Q_OBJECT
public:
    explicit cchess_rules(QObject *parent = nullptr);

signals:
};

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

public:
    Piece(Color color, QString name) : color(color), name(name) {}
    virtual ~Piece() {}

    virtual bool isValidMove(const Point& from, const Point& to, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const { return false;};
    std::vector<std::pair<Point, Point>> generateValidMoves(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
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
    };

    Color getColor() const { return color; }
    QString getName() const { return name; }

    Piece operator=(const Piece& other) {
        color = other.color;
        name = other.name;
        return *this;
    }
};

class General : public Piece {
public:
    General(Color color, QString name) : Piece(color, name) {}

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
    Advisor(Color color, QString name) : Piece(color, name) {}

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
    Elephant(Color color, QString name) : Piece(color, name) {}

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
    Horse(Color color, QString name) : Piece(color, name) {}

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
    Chariot(Color color, QString name) : Piece(color, name) {}

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
    Cannon(Color color, QString name) : Piece(color, name) {}

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
    Soldier(Color color, QString name) : Piece(color, name) {}

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
    Board() : board(10, std::vector<std::shared_ptr<Piece>>(9, nullptr)) {
        // Initialisiere das Brett mit Null-Pointern oder spezifischen Startpositionen
        setupInitialPositions();
    }

    std::shared_ptr<Piece> getPieceAt(const Point& position) const {
        return board[position.x][position.y];
    }

     std::vector<std::pair<Point, Point>> getAllValidMoves(Color color, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
        std::vector<std::pair<Point, Point>> moves;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] && board[i][j]->getColor() == color) {
                    auto validMoves = getValidMovesForPiece({i, j}, board);
                    //moves.insert(moves.end(), validMoves.begin(), validMoves.end());
                    for (const auto& move : validMoves) {
                        moves.push_back(move);
                    }
                }
            }
        }
        return moves;
     }

    std::vector<std::pair<Point, Point>> getValidMovesForPiece(const Point& position, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
        if (board[position.x][position.y]) {
            return board[position.x][position.y]->generateValidMoves(position, board);
        }
        return {};
    }

    bool movePiece(const Point& from, const Point& to, std::vector<std::vector<std::shared_ptr<Piece>>>& board) {
        if (board[from.x][from.y] && board[from.x][from.y]->isValidMove(from, to, board)) {
            // Bewegung ist gültig
            board[to.x][to.y] = std::move(board[from.x][from.y]);
            board[from.x][from.y] = std::make_shared<Piece>(Color::None, "");;
            return true;
        }
        return false;
    }

    void setupInitialPositions() {

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

        // Restliche Felder mit Null-Pointern initialisieren
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (!board[i][j]) {
                    board[i][j] = std::make_shared<Piece>(Color::None, "");
                }
            }
        }
    }

    void toggleColor() {
        players_color = (players_color == Color::Red) ? Color::Black : Color::Red;
    }
};


#endif // CCHESS_RULES_H
