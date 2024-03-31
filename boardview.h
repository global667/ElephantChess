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

#include "basemodel.h"
#include <cchess_rules.h>

// This class is the 2D-view of the board. It is a QWidget and draws the board and the pieces.
// Handles the mouse input and the context menu.
class BoardView : public QWidget
{
    Q_OBJECT

public:
    explicit BoardView(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void PaintBoard(QPainter *p) const;    
    void PaintMarker(QPainter *p);
    void PrepareNativePiece(QPicture *p, int row, int col, int h, int w);
    void PaintSelectedPieces(QPainter *p) const;

    QPoint CalcBoardCoords(QPoint r);
    void SetEditorPieces();

    // controls the mouse input
    bool pressed = false;
    bool secondclick = false;

    // Cutting points of the board
    const int cutpWidth = BaseModel::BoardColPoints;  //8;
    const int cutpHeight = BaseModel::BoardRowPoints; //9;

    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den roten Figuren gehört.
    QString redRiver = QString::fromUtf8("\u695a\u6cb3"); //"楚 河"; // (Chǔ hé) - Dieser Schriftzug bedeutet "Fluss von Chu".

    //Es befindet sich auf der Seite des Brettes, die dem Spieler mit den schwarzen Figuren gehört.
    QString blackRiver = QString::fromUtf8("\u6f22\u754c"); //"漢 界"; //  (Hàn jiè) - Dieser Schriftzug bedeutet "Grenze von Han".

    // Kontexmenue mit Figuren und Marker
    ContexMenu *contextMenu;  

signals:
    // signal to the controller
    void updateView(QPoint from, QPoint to, QString kind);
};

#endif // BOARDVIEW_H
