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
{
    QAction *a0 = addAction("Kläre Brett");
    QMenu *redPiecesMenu = new QMenu("Figuren (Rot)");
    QAction *a11 = redPiecesMenu->addAction("General");
    QAction *a12 = redPiecesMenu->addAction("Berater");
    QAction *a13 = redPiecesMenu->addAction("Elefant");
    QAction *a14 = redPiecesMenu->addAction("Pferd");
    QAction *a15 = redPiecesMenu->addAction("Kanone");
    QAction *a16 = redPiecesMenu->addAction("Soldat");
    QAction *a17 = redPiecesMenu->addAction("Turm");
    connect(a11, &QAction::triggered, this, &ContexMenu::GeneralRot);
    connect(a12, &QAction::triggered, this, &ContexMenu::BeraterRot);
    connect(a13, &QAction::triggered, this, &ContexMenu::ElefantRot);
    connect(a14, &QAction::triggered, this, &ContexMenu::PferdRot);
    connect(a15, &QAction::triggered, this, &ContexMenu::KanoneRot);
    connect(a16, &QAction::triggered, this, &ContexMenu::SoldatRot);
    connect(a17, &QAction::triggered, this, &ContexMenu::TurmRot);
    QMenu *blackPiecesMenu = new QMenu("Figuren (Schwarz)");
    QAction *a21 = blackPiecesMenu->addAction("General");
    QAction *a22 = blackPiecesMenu->addAction("Berater");
    QAction *a23 = blackPiecesMenu->addAction("Elefant");
    QAction *a24 = blackPiecesMenu->addAction("Pferd");
    QAction *a25 = blackPiecesMenu->addAction("Kanone");
    QAction *a26 = blackPiecesMenu->addAction("Soldat");
    QAction *a27 = blackPiecesMenu->addAction("Turm");
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
    QAction *action2 = addAction("Kreis");
    //QAction *action3 = contextMenu->addAction("Dreieck");
    QAction *action4 = addAction("Viereck");
    QAction *action5 = addAction("Linie");
    QAction *action6 = addAction("Linienende");
    QAction *action7 = addAction("Kläre Symbole");

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
    qDebug() << "clearBoard";
    pieces.clear();
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            basemodel.board.pieces[row][col] = Piece(color::Red, pieceType::Empty, {row, col}, "");
        }
    }

    basemodel.moveHistory.clear();
    //model->clear();
    //column = 0;
    basemodel.currentMove = 0;
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    basemodel.board.onMove = color::Red;

    basemodel.moves.clear();
    //basemodel.moveHistory.append(basemodel.board);
    basemodel.currentMove++;

    repaint();
}

void ContexMenu::GeneralRot()
{
    qDebug() << "GeneralRot";
    //QPoint cur = QCursor::pos();
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::GeneralRot));
    repaint();
}

void ContexMenu::GeneralSchwarz()
{
    qDebug() << "GeneralSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::GeneralSchwarz));
}

void ContexMenu::BeraterRot()
{
    qDebug() << "BeraterRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::AdvisorRot));
}

void ContexMenu::BeraterSchwarz()
{
    qDebug() << "BeraterSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::AdvisorSchwarz));
}

void ContexMenu::PferdRot()
{
    qDebug() << "PferdRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::HorseRot));
}

void ContexMenu::PferdSchwarz()
{
    qDebug() << "PferdSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::HorseSchwarz));
}

void ContexMenu::ElefantRot()
{
    qDebug() << "ElefantRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ElephantRot));
}

void ContexMenu::ElefantSchwarz()
{
    qDebug() << "ElefantSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ElephantSchwarz));
}

void ContexMenu::KanoneRot()
{
    qDebug() << "KanoneRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::CannonRot));
}

void ContexMenu::KanoneSchwarz()
{
    qDebug() << "KanoneSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::CannonSchwarz));
}

void ContexMenu::SoldatRot()
{
    qDebug() << "SoldatRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::SoldierRot));
}

void ContexMenu::SoldatSchwarz()
{
    qDebug() << "SoldatSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::SoldierSchwarz));
}

void ContexMenu::TurmRot()
{
    qDebug() << "TurmRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ChariotRot));
}

void ContexMenu::TurmSchwarz()
{
    qDebug() << "TurmSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, completePieceType::ChariotSchwarz));
}

void ContexMenu::clearMarkers()
{
    markers.clear();
}

void ContexMenu::Viereck()
{
    qDebug() << "Viereck";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Viereck));
}

void ContexMenu::Dreieck()
{
    qDebug() << "Dreieck";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Dreieck));
}

void ContexMenu::Kreis()
{
    qDebug() << "Kreis";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Kreis));
    repaint();
}

void ContexMenu::Linie()
{
    qDebug() << "Linie";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Linie));
}

void ContexMenu::Linienende()
{
    qDebug() << "Linienende";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Linienende));
}

void ContexMenu::Kreuz()
{
    qDebug() << "Kreuz";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, markerType::Kreuz));
}
