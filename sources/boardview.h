/*
  ElephantChess, a UCI chinese chess playing GUI with builtin engine
  Copyright (C) 2022-2023 Wolf S. Kappesser

  ElephantChess is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ElephantChess is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

    void MovePiece(position from, position to);

private:
    void PaintMarker(QPainter *p);
    void PaintBoard(QPainter *p);
    void PaintPieces(QPainter *p);
    QPixmap *PaintPiecesRaw(QPainter *p, int row, int col);
    void DrawSelectedPieces(QPainter *p);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPoint CalcBoardCoords(QPoint r);
    QPixmap pix;
    void SetEditorPieces();

    // controls the mouse input
    bool pressed = false;
    // Board coordinates from to for the 2-dim array
    int fromCol;
    int fromRow;
    int toCol;
    int toRow;
    std::vector<std::pair<position, position>> allPreviewMoves;

    // Cutting points of the board
    const int cutpWidth = BaseModel::BoardColPoints;  //8;
    const int cutpHeight = BaseModel::BoardRowPoints; //9;

    QString redRiver = "楚 河"; // (Chǔ hé) - Dieser Schriftzug bedeutet "Fluss von Chu".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den roten Figuren gehört.

    QString blackRiver = "漢 界"; //  (Hàn jiè) - Dieser Schriftzug bedeutet "Grenze von Han".
    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den schwarzen Figuren gehört.

    // Kontexmenue mit Figuren und Marker
    ContexMenu *contextMenu;

signals:
    // signals to the controller
    void updateView(position from, position to);
};

#endif // BOARDVIEW_H
