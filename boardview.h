#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

#include "basemodel.h"
#include "types.h"
//#include "player.h"

// This class is the view of the board. It is a QWidget and draws the board and the pieces.

class BoardView : public QWidget //, public Player
{
    Q_OBJECT
public:
    explicit BoardView(QWidget *parent = nullptr);

    void paintBoard(QPainter *p);
    void paintPieces(QPainter *p);
    void MovePiece(Position from, Position to); // override;
    std::vector<std::pair<Position, Position>> legalPieceMovesVar;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // Mousecoordinates
    //double m_mx;
    //double m_my;
    // controls the mouse input
    bool pressed = false;
    // Board coordinates from to for the 2-dim array
    int fromCol;
    int fromRow;
    int toCol;
    int toRow;
    // Cutting points of the board
    const int cutp_width = BaseModel::BoardColPoints;  //8;
    const int cutp_height = BaseModel::BoardRowPoints; //9;

signals:
    void updateView(int, int, int, int, int);
};

#endif // BOARDVIEW_H
