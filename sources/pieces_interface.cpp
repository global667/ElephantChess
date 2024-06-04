#include "pieces_interface.h"
#include "board.h"

Piece::Piece(const Color color, QString name) : color(color), name(std::move(name)) {}

Piece::~Piece() { id = -1; }

bool Piece::isValidMove(const Point &from, const Point &to, const std::vector<std::vector<std::shared_ptr<Piece> > > &board)  { return false;}

auto Piece::generateValidMoves(const Point &position,
                               const std::vector<std::vector<std::shared_ptr<Piece> > > &board) -> std::vector<std::pair<
    Point, Point> > {
    std::vector<std::pair<Point, Point>> moves;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (Point move = {i, j}; move.x >= 0 && move.x < 10 && move.y >= 0 && move.y < 9) {
                if (isValidMove(position, move, board)) {
                    moves.emplace_back(position, move);
                }
            }
        }
    }

    std::vector<std::vector<std::shared_ptr<Piece>>> local_board;
    local_board.clear();
    local_board = std::move(board);
    std::vector<std::pair<Point, Point>> moves_valid = moves;
    moves.clear();
    Color color = local_board[position.x][position.y]->getColor();
    for (auto &[position, move] : moves_valid)    {

        std::shared_ptr<Piece> p = local_board[move.x][move.y];

        Board::movePiece(position, move, local_board);

        if (!Board::isCheck(Board::toggleColor(color),local_board) &&
            !Board::isEvilGlare(color,local_board)) {

            moves.emplace_back(position, move);
        }
        Board::undoMove(position,move,p ,local_board);
    }

    return moves;
}

Color Piece::getColor() const { return color; }

QString Piece::getName() const { return name; }

QString Piece::getEuroName() const { return euroName; }

QString Piece::getEuroNameDesc() const { return euroNameDesc; }

int Piece::getId() const { return id; }

Piece Piece::operator=(const Piece &other) {
    color = other.color;
    name = other.name;
    return *this;
}
