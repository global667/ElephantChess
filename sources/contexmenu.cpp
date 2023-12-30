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

#include "contexmenu.h"

#include <QPoint>

extern BaseModel basemodel;

ContexMenu::ContexMenu(QWidget *parent)
    : QMenu(parent)
    , parent(parent)
{
    //qDebug() << __PRETTY_FUNCTION__;

    QAction *a0 = addAction("Clear board");
    QMenu *redPiecesMenu = new QMenu("Pieces (Red)");
    QAction *a11 = redPiecesMenu->addAction("General");
    QAction *a12 = redPiecesMenu->addAction("Advisor");
    QAction *a13 = redPiecesMenu->addAction("Elephant");
    QAction *a14 = redPiecesMenu->addAction("Horse");
    QAction *a15 = redPiecesMenu->addAction("Cannon");
    QAction *a16 = redPiecesMenu->addAction("Soldier");
    QAction *a17 = redPiecesMenu->addAction("Chariot");
    connect(a11, &QAction::triggered, this, &ContexMenu::GeneralRot);
    connect(a12, &QAction::triggered, this, &ContexMenu::BeraterRot);
    connect(a13, &QAction::triggered, this, &ContexMenu::ElefantRot);
    connect(a14, &QAction::triggered, this, &ContexMenu::PferdRot);
    connect(a15, &QAction::triggered, this, &ContexMenu::KanoneRot);
    connect(a16, &QAction::triggered, this, &ContexMenu::SoldatRot);
    connect(a17, &QAction::triggered, this, &ContexMenu::TurmRot);
    QMenu *blackPiecesMenu = new QMenu("Pieces (Black)");
    QAction *a21 = blackPiecesMenu->addAction("General");
    QAction *a22 = blackPiecesMenu->addAction("Advisor");
    QAction *a23 = blackPiecesMenu->addAction("Elephant");
    QAction *a24 = blackPiecesMenu->addAction("Horse");
    QAction *a25 = blackPiecesMenu->addAction("Cannon");
    QAction *a26 = blackPiecesMenu->addAction("Soldier");
    QAction *a27 = blackPiecesMenu->addAction("Chariot");
    connect(a21, &QAction::triggered, this, &ContexMenu::GeneralSchwarz);
    connect(a22, &QAction::triggered, this, &ContexMenu::BeraterSchwarz);
    connect(a23, &QAction::triggered, this, &ContexMenu::ElefantSchwarz);
    connect(a24, &QAction::triggered, this, &ContexMenu::PferdSchwarz);
    connect(a25, &QAction::triggered, this, &ContexMenu::KanoneSchwarz);
    connect(a26, &QAction::triggered, this, &ContexMenu::SoldatSchwarz);
    connect(a27, &QAction::triggered, this, &ContexMenu::TurmSchwarz);
    addMenu(blackPiecesMenu);
    addMenu(redPiecesMenu);
    //QAction *action = contextMenu->addAction("Kreuz");
    QAction *action2 = addAction("Circle");
    //QAction *action3 = contextMenu->addAction("Dreieck");
    QAction *action4 = addAction("Quad");
    QAction *action5 = addAction("Line");
    QAction *action6 = addAction("Line end");
    QAction *action7 = addAction("Clear symbols");

    connect(a0, &QAction::triggered, this, &ContexMenu::clearBoard);
    //connect(action, &QAction::triggered, this, &BoardView::Kreuz);
    connect(action5, &QAction::triggered, this, &ContexMenu::Linie);
    connect(action2, &QAction::triggered, this, &ContexMenu::Kreis);
    //connect(action3, &QAction::triggered, this, &BoardView::Dreieck);
    connect(action4, &QAction::triggered, this, &ContexMenu::Viereck);
    connect(action6, &QAction::triggered, this, &ContexMenu::Linienende);
    connect(action7, &QAction::triggered, this, &ContexMenu::clearMarkers);
}

void ContexMenu::clearBoard()
{
    //qDebug() << __PRETTY_FUNCTION__;
    //if (!&pieces)
    pieces.clear();

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            basemodel.board.pieces[row][col] = Piece(color::Red, pieceType::Empty, {row, col}, "");
        }
    }

    //if (&basemodel.moveHistory)
    basemodel.moveHistory.clear();
    //model->clear();
    //column = 0;
    basemodel.currentMove = 0;
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    basemodel.board.onMove = color::Red;

    //if (!&basemodel.moves)
    basemodel.moves.clear();
    //basemodel.moveHistory.append(basemodel.board);
    basemodel.currentMove++;

    parent->repaint();
}

void ContexMenu::GeneralRot()
{
    //qDebug() << __PRETTY_FUNCTION__; //QPoint cur = QCursor::pos();
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::GeneralRot));
    repaint();
}

void ContexMenu::GeneralSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::GeneralSchwarz));
}

void ContexMenu::BeraterRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::AdvisorRot));
}

void ContexMenu::BeraterSchwarz()
{
   //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::AdvisorSchwarz));
}

void ContexMenu::PferdRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::HorseRot));
}

void ContexMenu::PferdSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::HorseSchwarz));
}

void ContexMenu::ElefantRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ElephantRot));
}

void ContexMenu::ElefantSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ElephantSchwarz));
}

void ContexMenu::KanoneRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::CannonRot));
}

void ContexMenu::KanoneSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::CannonSchwarz));
}

void ContexMenu::SoldatRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::SoldierRot));
}

void ContexMenu::SoldatSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::SoldierSchwarz));
}

void ContexMenu::TurmRot()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ChariotRot));
}

void ContexMenu::TurmSchwarz()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ChariotSchwarz));
}

void ContexMenu::clearMarkers()
{
    //qDebug() << __PRETTY_FUNCTION__;
    markers.clear();
}

void ContexMenu::Viereck()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Viereck));
}

void ContexMenu::Dreieck()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Dreieck));
}

void ContexMenu::Kreis()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Kreis));
    repaint();
}

void ContexMenu::Linie()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Linie));
}

void ContexMenu::Linienende()
{
    //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Linienende));
}

void ContexMenu::Kreuz()
{
   //qDebug() << __PRETTY_FUNCTION__;
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Kreuz));
}
