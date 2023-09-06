#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include "contexmenu.h"
#include <vector>

#include "basemodel.h"
#include "types.h"

// This class is the view of the board. It is a QWidget and draws the board and the pieces.
// Handles the mouse input and the context menu.

class BoardView : public QWidget
{
    Q_OBJECT
public:
    explicit BoardView(QWidget *parent = nullptr);

    void MovePiece(Position from, Position to);

private:
    void paintMarker(QPainter *p);
    void paintBoard(QPainter *p);
    void paintPieces(QPainter *p);
    QPixmap *paintPiecesRaw(QPainter *p, int row, int col);
    void drawSelectedPieces(QPainter *p);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPoint calcBoardcoords(QPoint r);
    QPixmap pix;
    void setEditorPieces();

    // controls the mouse input
    bool pressed = false;
    // Board coordinates from to for the 2-dim array
    int fromCol;
    int fromRow;
    int toCol;
    int toRow;
    std::vector<std::pair<Position, Position>> allPreviewMoves;

    // Cutting points of the board
    const int cutp_width = BaseModel::BoardColPoints;  //8;
    const int cutp_height = BaseModel::BoardRowPoints; //9;

    QString red_river = "楚 河"; // (Chǔ hé) - Dieser Schriftzug bedeutet "Fluss von Chu".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den roten Figuren gehört.

    QString black_river = "漢 界"; //  (Hàn jiè) - Dieser Schriftzug bedeutet "Grenze von Han".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den schwarzen Figuren gehört.

    // Kontexmenue mit Figuren und Marker
    contexMenu *contextMenu;

signals:
    // signals to the controller
    void updateView(Position from, Position to);
};

#endif // BOARDVIEW_H
