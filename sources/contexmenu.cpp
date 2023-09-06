#include "contexmenu.h"

#include <QPoint>

extern BaseModel basemodel;

contexMenu::contexMenu(QWidget *parent)
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
    connect(a11, &QAction::triggered, this, &contexMenu::GeneralRot);
    connect(a12, &QAction::triggered, this, &contexMenu::BeraterRot);
    connect(a13, &QAction::triggered, this, &contexMenu::ElefantRot);
    connect(a14, &QAction::triggered, this, &contexMenu::PferdRot);
    connect(a15, &QAction::triggered, this, &contexMenu::KanoneRot);
    connect(a16, &QAction::triggered, this, &contexMenu::SoldatRot);
    connect(a17, &QAction::triggered, this, &contexMenu::TurmRot);
    QMenu *blackPiecesMenu = new QMenu("Figuren (Schwarz)");
    QAction *a21 = blackPiecesMenu->addAction("General");
    QAction *a22 = blackPiecesMenu->addAction("Berater");
    QAction *a23 = blackPiecesMenu->addAction("Elefant");
    QAction *a24 = blackPiecesMenu->addAction("Pferd");
    QAction *a25 = blackPiecesMenu->addAction("Kanone");
    QAction *a26 = blackPiecesMenu->addAction("Soldat");
    QAction *a27 = blackPiecesMenu->addAction("Turm");
    connect(a21, &QAction::triggered, this, &contexMenu::GeneralSchwarz);
    connect(a22, &QAction::triggered, this, &contexMenu::BeraterSchwarz);
    connect(a23, &QAction::triggered, this, &contexMenu::ElefantSchwarz);
    connect(a24, &QAction::triggered, this, &contexMenu::PferdSchwarz);
    connect(a25, &QAction::triggered, this, &contexMenu::KanoneSchwarz);
    connect(a26, &QAction::triggered, this, &contexMenu::SoldatSchwarz);
    connect(a27, &QAction::triggered, this, &contexMenu::TurmSchwarz);
    addMenu(blackPiecesMenu);
    addMenu(redPiecesMenu);
    //QAction *action = contextMenu->addAction("Kreuz");
    QAction *action2 = addAction("Kreis");
    //QAction *action3 = contextMenu->addAction("Dreieck");
    QAction *action4 = addAction("Viereck");
    QAction *action5 = addAction("Linie");
    QAction *action6 = addAction("Linienende");
    QAction *action7 = addAction("Kläre Symbole");

    connect(a0, &QAction::triggered, this, &contexMenu::clearBoard);
    //connect(action, &QAction::triggered, this, &BoardView::Kreuz);
    connect(action5, &QAction::triggered, this, &contexMenu::Linie);
    connect(action2, &QAction::triggered, this, &contexMenu::Kreis);
    //connect(action3, &QAction::triggered, this, &BoardView::Dreieck);
    connect(action4, &QAction::triggered, this, &contexMenu::Viereck);
    connect(action6, &QAction::triggered, this, &contexMenu::Linienende);
    connect(action7, &QAction::triggered, this, &contexMenu::clearMarkers);
}

//void contexMenu::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "contexMenu::mousePressEvent";
//    if (event->button() == Qt::RightButton) {
//        contextMenuX = event->position().x();
//        contextMenuY = event->position().y();
//        //popup(event->globalPosition().toPoint());
//        event->accept();
//        return;
//    }
//    event->ignore();
//}

void contexMenu::clearBoard()
{
    qDebug() << "clearBoard";
    pieces.clear();
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            basemodel.board.pieces[row][col] = Piece(Color::Red, PieceType::Empty, {row, col}, "");
        }
    }
    repaint();
}

void contexMenu::GeneralRot()
{
    qDebug() << "GeneralRot";
    QPoint cur = mapFromGlobal(QCursor::pos());
    pieces.append(std::make_pair(cur, CompletePieceType::GeneralRot));
    repaint();
}

void contexMenu::GeneralSchwarz()
{
    qDebug() << "GeneralSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::GeneralSchwarz));
}

void contexMenu::BeraterRot()
{
    qDebug() << "BeraterRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::AdvisorRot));
}

void contexMenu::BeraterSchwarz()
{
    qDebug() << "BeraterSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::AdvisorSchwarz));
}

void contexMenu::PferdRot()
{
    qDebug() << "PferdRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::HorseRot));
}

void contexMenu::PferdSchwarz()
{
    qDebug() << "PferdSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::HorseSchwarz));
}

void contexMenu::ElefantRot()
{
    qDebug() << "ElefantRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::ElephantRot));
}

void contexMenu::ElefantSchwarz()
{
    qDebug() << "ElefantSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::ElephantSchwarz));
}

void contexMenu::KanoneRot()
{
    qDebug() << "KanoneRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::CannonRot));
}

void contexMenu::KanoneSchwarz()
{
    qDebug() << "KanoneSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::CannonSchwarz));
}

void contexMenu::SoldatRot()
{
    qDebug() << "SoldatRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::SoldierRot));
}

void contexMenu::SoldatSchwarz()
{
    qDebug() << "SoldatSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::SoldierSchwarz));
}

void contexMenu::TurmRot()
{
    qDebug() << "TurmRot";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::ChariotRot));
}

void contexMenu::TurmSchwarz()
{
    qDebug() << "TurmSchwarz";
    QPoint cur = {contextMenuX, contextMenuY};
    pieces.append(std::make_pair(cur, CompletePieceType::ChariotSchwarz));
}

void contexMenu::clearMarkers()
{
    markers.clear();
}

void contexMenu::Viereck()
{
    qDebug() << "Viereck";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Viereck));
}

void contexMenu::Dreieck()
{
    qDebug() << "Dreieck";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Dreieck));
}

void contexMenu::Kreis()
{
    qDebug() << "Kreis";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Kreis));
    repaint();
}

void contexMenu::Linie()
{
    qDebug() << "Linie";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Linie));
}

void contexMenu::Linienende()
{
    qDebug() << "Linienende";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Linienende));
}

void contexMenu::Kreuz()
{
    qDebug() << "Kreuz";
    QPoint cur = {contextMenuX, contextMenuY};
    markers.append(std::make_pair(cur, MarkerType::Kreuz));
}
