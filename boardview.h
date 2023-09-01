#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <vector>

#include "basemodel.h"
#include "types.h"

// This class is the view of the board. It is a QWidget and draws the board and the pieces.

class BoardView : public QWidget
{
    Q_OBJECT
public:
    explicit BoardView(QWidget *parent = nullptr);

    void paintMarker(QPainter *p);
    void paintBoard(QPainter *p);
    void paintPieces(QPainter *p);
    void paintPiecesRaw(QPainter *p);
    void MovePiece(Position from, Position to); // override;
    std::vector<std::pair<Position, Position>> legalPieceMovesVar;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    // controls the mouse input
    bool pressed = false;
    // Board coordinates from to for the 2-dim array
    int fromCol;
    int fromRow;
    int toCol;
    int toRow;

    int rowOffset = 0;
    int colOffset = 0;

    // Cutting points of the board
    const int cutp_width = BaseModel::BoardColPoints;  //8;
    const int cutp_height = BaseModel::BoardRowPoints; //9;

    QString red_river = "楚 河"; // (Chǔ hé) - Dieser Schriftzug bedeutet "Fluss von Chu".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den roten Figuren gehört.

    QString black_river = "漢 界"; //  (Hàn jiè) - Dieser Schriftzug bedeutet "Grenze von Han".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den schwarzen Figuren gehört.

    QMenu *contextMenu;
    enum class MarkerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };
    QList<QPair<QPoint, MarkerType>> markers;

    QPoint calcBoardcoords(QPoint r);

public slots:
    void Kreuz();
    void Kreis();
    void Dreieck();
    void Linie();
    void Linienende();
    void Viereck();
    void clearMarkers();

signals:
    // signals to the controller
    void updateView(Position from, Position to);
};

#endif // BOARDVIEW_H
