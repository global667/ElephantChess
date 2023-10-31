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

#include "boardview.h"
#include "basemodel.h"
#include "genmove.h"

#include <QImageReader>
#include <QPainter>
#include <QPoint>
#include <QRadialGradient>
#include <QTextItem>
#include <QFontDatabase>

extern BaseModel basemodel;

BoardView::BoardView(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(false);
    setFocusPolicy(Qt::StrongFocus);
    contextMenu = new ContexMenu(this);
}

void BoardView::contextMenuEvent(QContextMenuEvent *event)
{
    //Q_UNUSED(event);
    //qDebug() << "contextMenuEvent";
    contextMenu->contextMenuX = event->pos().x();
    contextMenu->contextMenuY = event->pos().y();
    contextMenu->exec(QCursor::pos());
}

void BoardView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    SetEditorPieces();

    if (basemodel.gameView == color::Black) {
        painter.rotate(180);
        painter.translate(-width(), -height());
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    PaintBoard(&painter);
    PaintPieces(&painter);
    PaintSelectedPieces(&painter);
    PaintMarker(&painter);
}

void BoardView::PaintMarker(QPainter *p)
{
    Q_ASSERT(p);

    auto w = p->viewport().width();  //p->viewport().width();
    auto h = p->viewport().height(); //p->viewport().height();

    int lcol = 0;
    int lrow = 0;

    for (auto marker : contextMenu->markers) {
        auto x = marker.first.x();
        auto y = marker.first.y();

        qDebug() << x << y << "x und y";

        QPoint coords = CalcBoardCoords(marker.first);
        int col = coords.x();
        int row = coords.y();

        switch (marker.second) {
        case markerType::Kreuz:

            break;
        case markerType::Kreis:
            p->setPen(QPen(Qt::green, 8));
            p->drawChord(QRect((50 + (((col - 1)) * (w - 2 * 50) / cutpWidth))
                                   - w / cutpWidth / 2 / 1.5,
                               (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight)
                                   - h / cutpWidth / 2 / 1.5,
                               w / (cutpWidth) / 1.5,
                               h / cutpWidth / 1.5),
                         0,
                         5760);

            break;
        case markerType::Dreieck:

            break;
        case markerType::Linie:
            p->setPen(QPen(Qt::blue, 8));
            p->drawPoint((50
                          + (((col - 1)) * (w - 2 * 50) / cutpWidth)), // - w / cutp_width / 2 / 1.5,
                         (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight));
            //- h / cutp_width / 2 / 1.5);
            lrow = row;
            lcol = col;
            break;
        case markerType::Linienende:
            p->setPen(QPen(Qt::blue, 8));

            p->drawLine(50 + (((lcol - 1)) * (w - 2 * 50) / cutpWidth), // - w / cutp_width / 2 / 1.5,
                        (50 + ((lrow - 1)) * (h - 50 - 100) / cutpHeight),
                        50 + (((col - 1)) * (w - 2 * 50) / cutpWidth), // - w / cutp_width / 2 / 1.5,
                        (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight));

            break;
        case markerType::Viereck:
            p->setPen(QPen(Qt::yellow, 8));

            p->drawRect(
                QRect((50 + (((col - 1)) * (w - 2 * 50) / cutpWidth)) - w / cutpWidth / 2 / 1.5,
                      (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight) - h / cutpWidth / 2 / 1.5,
                      w / (cutpWidth) / 1.5,
                      h / cutpWidth / 1.5));
            break;
        default:
            break;
        }
    }
}

// Draws the pieces on the board (native)
QPixmap *BoardView::PrepareNativePiece(QPainter *p, int row, int col)
{
    //QPainter *p;
    Q_ASSERT(p);

    auto w = p->viewport().width();  //p->viewport().width();
    auto h = p->viewport().height(); //p->viewport().height();

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    p->setPen(pen);

    QFont font;
    font.setPointSize(30);
    font.setWeight(QFont::DemiBold);
    p->setFont(font);
    // Draws all pieces
    if (basemodel.board.pieces[row][8 - col].name != "") {
        if (basemodel.board.pieces[row][8 - col].colr == color::Red) {
            // Draw red
            pen.setColor(Qt::red);
            p->setPen(pen);
        } else {
            // Draw black
            pen.setColor(Qt::black);
            p->setPen(pen);
        }
        QRadialGradient gradient1(QPointF(50.0 + (((8 - col)) * (w - 2.0 * 50.0) / cutpWidth),
                                          50 + (9 - row) * (h - 2 * 50) / cutpHeight),
                                  50);
        gradient1.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
        gradient1.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

        QBrush brush1(gradient1);
        p->setBrush(brush1);

        p->drawChord(QRect((50 + (((8 - col)) * (w - 2 * 50) / cutpWidth)) - w / cutpWidth / 2 / 1.5,
                           (50 + (9 - row) * (h - 50 - 100) / cutpHeight) - h / cutpWidth / 2 / 1.5,
                           w / (cutpWidth) / 1.5,
                           h / cutpWidth / 1.5),
                     0,
                     5760);

        p->drawText(QRect((50 + (((8 - col)) * (w - 2 * 50) / cutpWidth)) - w / cutpWidth / 2 / 1.8,
                          (50 + (9 - row) * (h - 50 - 100) / cutpHeight) - h / cutpWidth / 2 / 2,
                          w / (cutpWidth),
                          h / cutpWidth),
                    basemodel.board.pieces[row][8 - col].name);
        p->drawPixmap(0, 0, pix);
    }
    return &pix;
}

// Painted from upper left!
void BoardView::PaintPieces(QPainter *p)
{
    Q_ASSERT(p);

    auto w = p->window().width();  //p->viewport().width();
    auto h = p->window().height(); //p->viewport().height();

    // Draws all pieces

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            QPixmap pixm;
            QPixmap pixm2;
            if (basemodel.board.pieces[j][8 - i].colr == color::Red) {
                if (basemodel.viewStyleModeVar == viewStyleMode::western_png) {
                    pixm = QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img);
                    pixm2 = pixm.copy(100, 0, 100, 100);
                } else if (basemodel.viewStyleModeVar == viewStyleMode::traditional_png) {
                    pixm = QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img,
                                              Qt::PreferDither);
                    pixm2 = pixm.copy(0, 0, 100, 100);
                } else if (basemodel.viewStyleModeVar == viewStyleMode::traditional_native) {
                    pixm2 = *PrepareNativePiece(p, j, i);
                }
            } else {
                if (basemodel.viewStyleModeVar == viewStyleMode::western_png) {
                    pixm = QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img);
                    pixm2 = pixm.copy(300, 0, 100, 100);
                } else if (basemodel.viewStyleModeVar == viewStyleMode::traditional_png) {
                    pixm = QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img,
                                              Qt::PreferDither);
                    pixm2 = pixm.copy(200, 0, 100, 100);
                } else if (basemodel.viewStyleModeVar == viewStyleMode::traditional_native) {
                    pixm2 = *PrepareNativePiece(p, j, i);
                }
            }
            p->drawPixmap(QRect((50 + ((8 - i) * (w - 2 * 50) / cutpWidth))
                                    - w / cutpWidth / 2 / 1.5,
                                (50 + (9 - j) * (h - 50 - 100) / cutpHeight)
                                    - h / cutpWidth / 2 / 1.5,
                                w / (cutpWidth) / 1.5,
                                h / cutpWidth / 1.5),
                          pixm2);
        }
    }
}

void BoardView::PaintBoard(QPainter *p)
{
    Q_ASSERT(p);

    const QColor background(252, 175, 62);
    const QColor sides(206, 92, 0);
    const QColor river("#3A438F");

    p->fillRect(p->viewport(), background);

    QPen pn(Qt::black);
    pn.setWidth(2);
    p->setPen(pn);

    // Palaeste
    p->drawLine(50 + (3 * (width() - 2 * 50) / cutpWidth),
                50 + 0 * (height() - 50 - 100) / cutpHeight,
                50 + (5 * (width() - 2 * 50) / cutpWidth),
                50 + 2 * (height() - 50 - 100) / cutpHeight);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutpWidth),
                50 + 2 * (height() - 50 - 100) / cutpHeight,
                50 + (5 * (width() - 2 * 50) / cutpWidth),
                50 + 0 * (height() - 50 - 100) / cutpHeight);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutpWidth),
                50 + 9 * (height() - 50 - 100) / cutpHeight,
                50 + (5 * (width() - 2 * 50) / cutpWidth),
                50 + 7 * (height() - 50 - 100) / cutpHeight);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutpWidth),
                50 + 7 * (height() - 50 - 100) / cutpHeight,
                50 + (5 * (width() - 2 * 50) / cutpWidth),
                50 + 9 * (height() - 50 - 100) / cutpHeight);

    // Vertikale Linien
    for (int i = 0; i <= cutpWidth; i++) {
        p->drawLine(50 + (i * (width() - 2 * 50) / cutpWidth),
                    50,
                    50 + (i * (width() - 2 * 50) / cutpWidth),
                    height() - 100);
    }

    // Horizontale Linien
    for (int i = 0; i <= cutpHeight; i++) {
        p->drawLine(50,
                    50 + i * (height() - 50 - 100) / cutpHeight,
                    width() - 50,
                    50 + i * (height() - 50 - 100) / cutpHeight);
    }

    // TODO: Hervorgehoben Punkte von Soldaten und Kanonen implementieren

    // Oberer Rand
    p->fillRect(0, 0, width(), 50, sides);

    // Fluss
    p->fillRect(0,
                50 + 4 * (height() - 50 - 100) / cutpHeight,
                width(),
                (height() - 50 - 100) / cutpHeight,
                river); //background);

    // Flussufer
    QFont tmp = QFont(p->font());
    QFont font = QFont();
    font.setPointSize(30);
    font.setBold(false);
    font.setItalic(true);
    p->setFont(font);
    p->drawText(QRect(100,
                      5 * (height() - 50 - 100) / cutpHeight + 10,
                      150, //width(),
                      (height() - 50 - 100) / cutpHeight / 2),
                Qt::AlignCenter,
                redRiver);
    p->drawText(QRect(600,
                      5 * (height() - 50 - 100) / cutpHeight - 20,
                      150, //width(),
                      (height() - 50 - 100) / cutpHeight / 2),
                Qt::AlignCenter,
                blackRiver);

    p->setFont(tmp);

    // Seitenraender
    // Linker Rand
    p->fillRect(0, 0, 50, height(), sides);
    // Unterer Rand
    p->fillRect(0, height() - 2 * 50, width(), 2 * 50, sides);
    // Rechter Rand
    p->fillRect(width() - 50, 0, 50, height(), sides);

    p->setPen(QColor(0, 0, 0));

    // Notation
    if (0 == 0) {
        // Westliche, an Schach angelehnte Notation

        // Vertikaler Text
        for (int i = 0; i <= cutpHeight; i++) {
            p->drawText(50 / 2,
                        50 + i * (height() - 50 - 100) / cutpHeight,
                        //width() - 50,
                        //50 + i * (height() - 50 - 100) / cutp_height,
                        //0,
                        QString("%1").arg(QChar('9' - i)));
        }
        // Horizontaler Text
        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutpWidth),
                        height() - (2 * 50 / 2),
                        //50 + (i * (width() - 2 * 50) / cutp_width),
                        //height() - 100,
                        //0,
                        QString("%1").arg(QChar('a' + i)));
        }
    } else {
        // TODO: Traditionelle Notation, wird in späteren Versionen implementiert

        // Horizontaler Text ,unten
        QStringList list;
        list << "\u4e00"  //"一"
             << "\u4e8c"  //"二"
             << "\u4e09"  //"三"
             << "\u56db"  //"四"
             << "\u4e94"  //"五"
             << "\u516d"  //"六"
             << "\u4e03"  //"七"
             << "\u516b"  //"八"
             << "\u4e5d"; //"九";

        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutpWidth),
                        height() - (2 * 50 / 2),
                        QString("%1").arg(list.at(8 - i)));
        }

        // Horizontaler Text, oben
        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutpWidth),
                        10,
                        QString("%1").arg(QChar('1' + i)));
        }
    }

    // Aussenraender
    pn.setWidth(3);
    p->setPen(pn);
    p->drawRect(46, 46, width() - 93, height() - 143);

    pn.setWidth(2);
    p->setPen(pn);
    p->drawRect(50, 50, width() - 100, height() - 150);

    pn.setWidth(2);
    p->setPen(pn);
}

void BoardView::PaintSelectedPieces(QPainter *p)
{
    Q_ASSERT(p);

    auto w = p->viewport().width();  //p->viewport().width();
    auto h = p->viewport().height(); //p->viewport().height();
    QPen pen;
    p->setBrush(Qt::transparent);
    pen.setColor(Qt::green);
    pen.setWidth(5);
    p->setPen(pen);

    if (fromHuman.col != -1) {
        // Draws selected piece
        p->setBrush(Qt::transparent);
        pen.setColor(Qt::green);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(
            QRect((50 + (((fromHuman.col)) * (w - 2 * 50) / cutpWidth)) - w / cutpWidth / 2 / 1.5,
                  (50 + (9 - (fromHuman.row)) * (h - 50 - 100) / cutpHeight)
                      - h / cutpWidth / 2 / 1.5,
                  w / (cutpWidth) / 1.5,
                  h / cutpWidth / 1.5));

        // draws legal moves as dots
        pen.setColor(Qt::red);
        pen.setWidth(5);
        p->setPen(pen);

        GenMove legalMoves(basemodel.board.pieces, basemodel.board.onMove);
        std::vector<std::pair<position, position>> allPreviewMoves;
        allPreviewMoves = legalMoves.IsValidPieceMove(fromHuman);
        for (auto move : allPreviewMoves) {
            for (int j = 0; j < 10; j++) {
                for (int i = 0; i < 9; i++) {
                    if (move.second.col == i && move.second.row == j) {
                        p->drawEllipse(
                            QRect((50 + ((move.second.col) * (w - 2 * 50) / cutpWidth))
                                      - w / cutpWidth / 2 / 1.5,
                                  (50 + (9 - move.second.row) * (h - 50 - 100) / cutpHeight)
                                      - h / cutpWidth / 2 / 1.5,
                                  w / (cutpWidth) / 1.5,
                                  h / cutpWidth / 1.5));
                    }
                }
            }
        }
    }

    if (basemodel.fromUCI.col != -1) {
        // draws the last moved line
        pen.setColor(Qt::black);

        pen.setWidth(4);
        p->setPen(pen);
        p->setOpacity(0.7);

        //qDebug() << basemodel.toUCI.col;
        p->drawLine((50 + (((basemodel.fromUCI.col)) * (w - 2 * 50) / cutpWidth)),
                    (50 + ((9 - basemodel.fromUCI.row)) * (h - 50 - 100) / cutpHeight),

                    (50 + (((basemodel.toUCI.col)) * (w - 2 * 50) / cutpWidth)),
                    (50 + ((9 - basemodel.toUCI.row)) * (h - 50 - 100) / cutpHeight));

        pen.setWidth(25);
        p->setPen(pen);

        p->drawPoint((50 + (((basemodel.fromUCI.col)) * (w - 2 * 50) / cutpWidth)),
                     (50 + ((9 - basemodel.fromUCI.row)) * (h - 50 - 100) / cutpHeight));

        pen.setWidth(18);
        p->setPen(pen);
        p->drawPoint((50 + (((basemodel.toUCI.col)) * (w - 2 * 50) / cutpWidth)),
                     (50 + ((9 - basemodel.toUCI.row)) * (h - 50 - 100) / cutpHeight));

        pen.setWidth(5);
        p->setPen(pen);
        p->setOpacity(1);

        pen.setColor(Qt::black);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(QRect((50 + (((basemodel.toUCI.col)) * (w - 2 * 50) / cutpWidth))
                                 - w / cutpWidth / 2 / 1.5,
                             (50 + (9 - (basemodel.toUCI.row)) * (h - 50 - 100) / cutpHeight)
                                 - h / cutpWidth / 2 / 1.5,
                             w / (cutpWidth) / 1.5,
                             h / cutpWidth / 1.5));

        p->drawEllipse(QRect((50 + (((basemodel.fromUCI.col)) * (w - 2 * 50) / cutpWidth))
                                 - w / cutpWidth / 2 / 1.5,
                             (50 + (9 - (basemodel.fromUCI.row)) * (h - 50 - 100) / cutpHeight)
                                 - h / cutpWidth / 2 / 1.5,
                             w / (cutpWidth) / 1.5,
                             h / cutpWidth / 1.5));
    }
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "mousePressEvent";
    QPoint p = CalcBoardCoords({event->pos().x(), event->pos().y()});
    if (!pressed) {
        fromHuman.col = p.x() - 1;
        fromHuman.row = 10 - p.y();
        pressed = true;

        if (basemodel.board.pieces[fromHuman.row][fromHuman.col].type == pieceType::Empty) {
            pressed = false;
            return;
        }
        if (basemodel.board.pieces[fromHuman.row][fromHuman.col].colr != basemodel.board.onMove) {
            pressed = false;
            return;
        }
        //qDebug() << basemodel.fromHuman.col << basemodel.fromHuman.row;
    } else if (pressed) {
        toHuman.col = p.x() - 1;
        toHuman.row = 10 - p.y();

        GenMove mate(basemodel.board.pieces, basemodel.board.onMove);

        // Is in Checkmate
        if (mate.IsCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
            return;
        }
        pressed = false;
        GenMove legalMoves(basemodel.board.pieces, basemodel.board.onMove);
        std::vector<std::pair<position, position>> allPreviewMoves;
        allPreviewMoves = legalMoves.IsValidPieceMove(fromHuman);
        for (auto move : allPreviewMoves) {
            if ((move.first.row == fromHuman.row) && (move.first.col == fromHuman.col)
                && (move.second.row == toHuman.row) && (move.second.col == toHuman.col)) {
                if (basemodel.kind.contains("uci")) {
                    emit updateView({fromHuman.row, fromHuman.col},
                                    {toHuman.row, toHuman.col},
                                    "uci");
                } else {
                    emit updateView({fromHuman.row, fromHuman.col},
                                    {toHuman.row, toHuman.col},
                                    "human");
                }
                break;
            }
        }
        fromHuman = {-1, -1};
    }

    repaint();
}

//TODO: exchange hanzi with unicode
// Sets the selected pieces on the (clean) board
void BoardView::SetEditorPieces()
{
    QPoint coords;
    for (auto piece : contextMenu->pieces) {
        coords = CalcBoardCoords(piece.first);
        QPoint tmp = coords;
        coords.setX(10 - tmp.y());
        coords.setY(tmp.x() - 1);
        switch (piece.second) {
        case completePieceType::GeneralRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::General, {coords.x(), coords.y()}, "帥"));
            break;
        case completePieceType::GeneralSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::General, {coords.x(), coords.y()}, "將"));
            break;
        case completePieceType::AdvisorRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Advisor, {coords.x(), coords.y()}, "仕"));
            break;
        case completePieceType::AdvisorSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Advisor, {coords.x(), coords.y()}, "士"));
            break;
        case completePieceType::ElephantRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Elephant, {coords.x(), coords.y()}, "相"));
            break;
        case completePieceType::ElephantSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Elephant, {coords.x(), coords.y()}, "象"));
            break;
        case completePieceType::HorseRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Horse, {coords.x(), coords.y()}, "傌"));
            break;
        case completePieceType::HorseSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Horse, {coords.x(), coords.y()}, "馬"));
            break;
        case completePieceType::ChariotRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Chariot, {coords.x(), coords.y()}, "俥"));
            break;
        case completePieceType::ChariotSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Chariot, {coords.x(), coords.y()}, "車"));
            break;
        case completePieceType::CannonRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Cannon, {coords.x(), coords.y()}, "炮"));
            break;
        case completePieceType::CannonSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Cannon, {coords.x(), coords.y()}, "砲"));
            break;
        case completePieceType::SoldierRot:
            basemodel.board.InitPiece(
                Piece(color::Red, pieceType::Soldier, {coords.x(), coords.y()}, "兵"));
            break;
        case completePieceType::SoldierSchwarz:
            basemodel.board.InitPiece(
                Piece(color::Black, pieceType::Soldier, {coords.x(), coords.y()}, "卒"));
            break;
        default:
            qDebug() << "Error in boarview";
            break;
        }
    }
}

QPoint BoardView::CalcBoardCoords(QPoint r)
{
    float w = width();
    float h = height();
    QPoint p = r;

    float x = p.x();
    float y = p.y();

    //qDebug() << x << y << "x und y";

    float squareCol = (w - 2.0 * 50.0) / BaseModel::BoardColPoints;
    float squareRow = (h - 50.0 - 100.0) / BaseModel::BoardRowPoints;
    float boardCursorCol;
    float boardCursorRow;
    if (basemodel.gameView == color::Red) {
        boardCursorCol = x;
        boardCursorRow = y;
    } else {
        boardCursorCol = w - x;
        boardCursorRow = h - y;
    }

    int col, row;

    col = static_cast<int>(floor((boardCursorCol) / squareCol) + 1);
    row = static_cast<int>(floor((boardCursorRow) / squareRow) + 1);
    return QPoint(col, row);
}
