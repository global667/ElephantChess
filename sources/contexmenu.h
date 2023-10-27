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

#ifndef CONTEXMENU_H
#define CONTEXMENU_H

#include <QMenu>

#include "basemodel.h"
#include "types.h"

class ContexMenu : public QMenu
{
    Q_OBJECT
public:
    ContexMenu(QWidget *parent = 0);

    QList<QPair<QPoint, markerType>> markers;
    QList<QPair<QPoint, completePieceType>> pieces;
    int contextMenuX;
    int contextMenuY;

    // TODO: Use Pattern
public slots:
    void Kreuz();
    void Kreis();
    void Dreieck();
    void Linie();
    void Linienende();
    void Viereck();
    void clearMarkers();

    void GeneralRot();
    void GeneralSchwarz();
    void BeraterRot();
    void BeraterSchwarz();
    void PferdRot();
    void PferdSchwarz();
    void ElefantRot();
    void ElefantSchwarz();
    void KanoneRot();
    void KanoneSchwarz();
    void SoldatRot();
    void SoldatSchwarz();
    void TurmRot();
    void TurmSchwarz();
    void clearBoard();
};

#endif // CONTEXMENU_H
