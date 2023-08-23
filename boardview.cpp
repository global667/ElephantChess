#include "boardview.h"
#include "basemodel.h"
#include "genmove.h"
#include "mainwindow.h"

#include <QImageReader>
#include <QPainter>

extern BaseModel basemodel;
extern Position glfrom;
extern Position glto;

BoardView::BoardView(QWidget *parent)
    : QWidget{parent}
{

}

void BoardView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    paintBoard(&painter);
    paintPieces(&painter);
}

void BoardView::paintBoard(QPainter *p)
{
    const QColor background(252, 175, 62);
    const QColor sides(206, 92, 0);
    const QColor river("#3A438F");

    p->fillRect(p->viewport(), background);

    QPen pn(Qt::black);
    pn.setWidth(2);
    p->setPen(pn);

    // Palaeste
    p->drawLine(50 + (3 * (width() - 2 * 50) / cutp_width),
                50 + 0 * (height() - 50 - 100) / cutp_height,
                50 + (5 * (width() - 2 * 50) / cutp_width),
                50 + 2 * (height() - 50 - 100) / cutp_height);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutp_width),
                50 + 2 * (height() - 50 - 100) / cutp_height,
                50 + (5 * (width() - 2 * 50) / cutp_width),
                50 + 0 * (height() - 50 - 100) / cutp_height);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutp_width),
                50 + 9 * (height() - 50 - 100) / cutp_height,
                50 + (5 * (width() - 2 * 50) / cutp_width),
                50 + 7 * (height() - 50 - 100) / cutp_height);

    p->drawLine(50 + (3 * (width() - 2 * 50) / cutp_width),
                50 + 7 * (height() - 50 - 100) / cutp_height,
                50 + (5 * (width() - 2 * 50) / cutp_width),
                50 + 9 * (height() - 50 - 100) / cutp_height);

    // Vertikale Linien
    for (int i = 0; i <= cutp_width; i++) {
        p->drawLine(50 + (i * (width() - 2 * 50) / cutp_width),
                    50,
                    50 + (i * (width() - 2 * 50) / cutp_width),
                    height() - 100);
    }

    // Horizontale Linien
    for (int i = 0; i <= cutp_height; i++) {
        p->drawLine(50,
                    50 + i * (height() - 50 - 100) / cutp_height,
                    width() - 50,
                    50 + i * (height() - 50 - 100) / cutp_height);
    }

    // Oberer Rand
    p->fillRect(0, 0, width(), 50, sides);

    // Fluss
    p->fillRect(0,
                50 + 4 * (height() - 50 - 100) / cutp_height,
                width(),
                (height() - 50 - 100) / cutp_height,
                river); //background);

    // Seitenraender
    // Linker Rand
    p->fillRect(0, 0, 50, height(), sides);
    // Unterer Rand
    p->fillRect(0, height() - 2 * 50, width(), 2 * 50, sides);
    // Rechter Rand
    p->fillRect(width() - 50, 0, 50, height(), sides);

    p->setPen(QColor(0, 0, 0));
    // Horizontaler Text
    for (int i = 0; i <= cutp_height; i++) {
        QString c = QString("%1").arg(QChar(('9' - 1) - i + 1));
        p->drawText(50 / 2,
                    50 + i * (height() - 50 - 100) / cutp_height,
                    width() - 50,
                    50 + i * (height() - 50 - 100) / cutp_height,
                    0,
                    c);
    }
    // Vertikale Linien
    for (int i = 0; i <= cutp_width; i++) {
        p->drawText(50 + (i * (width() - 2 * 50) / cutp_width),
                    height() - (2 * 50 / 2),
                    50 + (i * (width() - 2 * 50) / cutp_width),
                    height() - 100,
                    0,
                    QString("%1").arg(QChar('a' + i)));
    }
}

// Painted from upper left!
void BoardView::paintPieces(QPainter *p)
{
    auto w = p->window().width();  //p->viewport().width();
    auto h = p->window().height(); //p->viewport().height();

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            p->drawPixmap(QRect((50 + ((8 - i) * (w - 2 * 50) / cutp_width))
                                    - w / cutp_width / 2 / 1.5,
                                (50 + (9 - j) * (h - 50 - 100) / cutp_height)
                                    - h / cutp_width / 2 / 1.5,
                                w / (cutp_width) / 1.5,
                                h / cutp_width / 1.5),
                          QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img));
        }
    }

    qDebug() << "glfrom, glto" << glfrom.col << glfrom.row << glto.col << glto.row;

    // Draw selected piece
    if (pressed) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(
            QRect((50 + (((glfrom.col)) * (w - 2 * 50) / cutp_width)) - w / cutp_width / 2 / 1.5,
                  (50 + (9 - (glfrom.row)) * (h - 50 - 100) / cutp_height)
                      - h / cutp_width / 2 / 1.5,
                  w / (cutp_width) / 1.5,
                  h / cutp_width / 1.5));
    } else if (glfrom.col != -1 && glfrom.row != -1) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(
            QRect((50 + (((glto.col)) * (w - 2 * 50) / cutp_width)) - w / cutp_width / 2 / 1.5,
                  (50 + (9 - (glto.row)) * (h - 50 - 100) / cutp_height) - h / cutp_width / 2 / 1.5,
                  w / (cutp_width) / 1.5,
                  h / cutp_width / 1.5));
    }

    // draw legal moves as dots
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(5);
    p->setPen(pen);
    qDebug() << "legalPieceMovesVar.size()" << legalPieceMovesVar.size();
    for (auto move : legalPieceMovesVar) {
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 9; i++) {
                if (move.second.col == i && move.second.row == j) {
                    p->drawEllipse(QRect((50 + ((move.second.col) * (w - 2 * 50) / cutp_width))
                                             - w / cutp_width / 2 / 1.5,
                                         (50 + (9 - move.second.row) * (h - 50 - 100) / cutp_height)
                                             - h / cutp_width / 2 / 1.5,
                                         w / (cutp_width) / 1.5,
                                         h / cutp_width / 1.5));
                }
            }
        }
    }
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    auto squareCol = (width() - 2 * 50) / BaseModel::BoardColPoints;
    auto squareRow = (height() - 50 - 100) / BaseModel::BoardRowPoints;
    auto boardCursorCol = event->pos().x();
    auto boardCursorRow = event->pos().y();

    if (!pressed) {
        fromCol = static_cast<int>((50 + boardCursorCol) / squareCol);
        fromRow = static_cast<int>((((50 + boardCursorRow) / squareRow)));
        pressed = true;
        if (basemodel.board.pieces[10 - fromRow][fromCol - 1].type == PieceType::Empty) {
            pressed = false;
            return;
        }
        if (basemodel.board.pieces[10 - fromRow][fromCol - 1].color != basemodel.board.onMove) {
            pressed = false;
            return;
        }

        glfrom.col = fromCol - 1;
        glfrom.row = 10 - fromRow;

        GenMove legalMoves({glfrom.row, glfrom.col},
                           {-1, -1},
                           basemodel.board.pieces,
                           basemodel.board.onMove);
        legalPieceMovesVar = legalMoves.isValidPieceMove({glfrom.row, glfrom.col});

        qDebug() << "legalPieceMovesVar" << legalPieceMovesVar.size();

        if (legalPieceMovesVar.size() == 0) {
            pressed = false;
            glfrom.col = -1;
            glfrom.row = -1;
            return;
        }

    } else if (pressed) {
        toCol = static_cast<int>((((50 + boardCursorCol) / squareCol)));
        toRow = static_cast<int>((((50 + boardCursorRow) / squareRow)));

        GenMove mate({glfrom.row, glfrom.col},
                     {toRow - 1, toCol - 1},
                     basemodel.board.pieces,
                     basemodel.board.onMove);

        // Is in Checkmate
        if (mate.isCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
            return;
        }

        if (toCol == fromCol && toRow == fromRow) {
            return;
        }
        pressed = false;

        for (auto move : legalPieceMovesVar) {
            if ((move.first.row == 10 - fromRow) && (move.first.col == fromCol - 1)
                && (move.second.row == 10 - toRow) && (move.second.col == toCol - 1)) {
                basemodel.board.movePiece(10 - fromRow, fromCol - 1, 10 - toRow, toCol - 1);
                emit updateView(10 - fromRow, fromCol - 1, 10 - toRow, toCol - 1, 0);
                break;
            }
        }
        legalPieceMovesVar.clear();
    }

    repaint();
}

void BoardView::MovePiece(Position from, Position to)
{
    basemodel.board.movePiece(from.row, from.col, to.row, to.col);
}
