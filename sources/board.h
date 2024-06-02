#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QDebug>
#include <QObject>
#include <QString>

#include <vector>

//#include "basemodel.h"
//#include "sources/cchess_rules.h"

#include "basetypes.h"
#include "pieces_interface.h"


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

#endif // BOARD_H
