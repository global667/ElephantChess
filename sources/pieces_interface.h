#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include "basetypes.h"

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


#endif // PIECE_H
