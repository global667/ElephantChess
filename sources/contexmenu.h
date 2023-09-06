#ifndef CONTEXMENU_H
#define CONTEXMENU_H

#include <QMenu>

#include "basemodel.h"
#include "types.h"

class contexMenu : public QMenu
{
    Q_OBJECT
public:
    contexMenu(QWidget *parent = 0);

    QList<QPair<QPoint, MarkerType>> markers;

    QList<QPair<QPoint, CompletePieceType>> pieces;
    int contextMenuX;
    int contextMenuY;

    //protected:
    //void mousePressEvent(QMouseEvent *event);

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
