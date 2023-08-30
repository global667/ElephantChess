#include "boardview.h"
#include "basemodel.h"
#include "genmove.h"

#include <QImageReader>
#include <QPainter>
#include <QRadialGradient>
#include <QTextItem>

extern BaseModel basemodel;

BoardView::BoardView(QWidget *parent)
    : QWidget{parent}
{

}

void BoardView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing, true);
    paintBoard(&painter);

    if (basemodel.board.viewStyleMode != ViewStyleMode::traditional_native)
        paintPieces(&painter);
    else
        paintPiecesRaw(&painter);
}

// For the future: make a function that draws a single piece
void BoardView::paintPiecesRaw(QPainter *p)
{
    Q_ASSERT(p);

    auto w = p->window().width();  //p->viewport().width();
    auto h = p->window().height(); //p->viewport().height();

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    p->setPen(pen);
    QFont font("YaHei Consolas Hybrid", 30, 75);
    font.setStretch(150);

    p->setFont(font);

    // Draws all pieces
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            //            p->drawPixmap(QRect((50 + ((8 - i) * (w - 2 * 50) / cutp_width))
            //                                    - w / cutp_width / 2 / 1.5,
            //                                (50 + (9 - j) * (h - 50 - 100) / cutp_height)
            //                                    - h / cutp_width / 2 / 1.5,
            //                                w / (cutp_width) / 1.5,
            //                                h / cutp_width / 1.5),
            //                          QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img));

            if (basemodel.board.pieces[j][8 - i].name != "") {
                // Draw red piece
                if (basemodel.board.pieces[j][8 - i].color == Color::Red) {
                    pen.setColor(Qt::red);
                    p->setPen(pen);

                    QRadialGradient gradient(QPointF(50.0
                                                         + (((8 - i)) * (w - 2.0 * 50.0)
                                                            / cutp_width),
                                                     50 + (9 - j) * (h - 2 * 50) / cutp_height),
                                             50);
                    gradient.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
                    gradient.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

                    QBrush brush(gradient); //Qt::SolidPattern);
                    //brush.setColor(QColor::fromRgb());
                    //brush

                    p->setBrush(brush);

                    p->drawChord(QRect((50 + (((8 - i)) * (w - 2 * 50) / cutp_width))
                                           - w / cutp_width / 2 / 1.5,
                                       (50 + ((9 - j)) * (h - 50 - 100) / cutp_height)
                                           - h / cutp_width / 2 / 1.5,
                                       w / (cutp_width) / 1.5,
                                       h / cutp_width / 1.5),
                                 0,
                                 5760);

                    p->drawText(QRect((50 + (((8 - i)) * (w - 2 * 50) / cutp_width))
                                          - w / cutp_width / 2 / 1.8,
                                      (50 + ((9 - j)) * (h - 50 - 100) / cutp_height)
                                          - h / cutp_width / 2 / 2,
                                      w / (cutp_width),
                                      h / cutp_width),
                                basemodel.board.pieces[j][8 - i].name);

                } else {
                    // Draw black piece
                    pen.setColor(Qt::black);
                    p->setPen(pen);

                    QRadialGradient gradient1(QPointF(50.0
                                                          + (((8 - i)) * (w - 2.0 * 50.0)
                                                             / cutp_width),
                                                      50 + (9 - j) * (h - 2 * 50) / cutp_height),
                                              50);
                    gradient1.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
                    gradient1.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

                    QBrush brush1(gradient1); //Qt::SolidPattern);
                    p->setBrush(brush1);

                    p->drawChord(QRect((50 + (((8 - i)) * (w - 2 * 50) / cutp_width))
                                           - w / cutp_width / 2 / 1.5,
                                       (50 + (9 - j) * (h - 50 - 100) / cutp_height)
                                           - h / cutp_width / 2 / 1.5,
                                       w / (cutp_width) / 1.5,
                                       h / cutp_width / 1.5),
                                 0,
                                 5760);

                    p->drawText(QRect((50 + (((8 - i)) * (w - 2 * 50) / cutp_width))
                                          - w / cutp_width / 2 / 1.8,
                                      (50 + (9 - j) * (h - 50 - 100) / cutp_height)
                                          - h / cutp_width / 2 / 2,
                                      w / (cutp_width),
                                      h / cutp_width),
                                basemodel.board.pieces[j][8 - i].name);
                }
            }
        }
    }

    // Draws selected piece
    p->setBrush(Qt::transparent);
    if (pressed) {
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(QRect((50 + (((basemodel.fromHuman.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.fromHuman.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));
    } else if (basemodel.fromUCI.col != -1 && basemodel.fromUCI.row != -1) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(QRect((50 + (((basemodel.toUCI.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.toUCI.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));

        p->drawEllipse(QRect((50 + (((basemodel.fromUCI.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.fromUCI.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));
    }

    // draws legal moves as dots
    //QPen pen;
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

void BoardView::paintBoard(QPainter *p)
{
    Q_ASSERT(p);

    const QColor background(252, 175, 62);
    const QColor sides(206, 92, 0);
    const QColor river("#3A438F");

    p->fillRect(p->viewport(), background);

    QPen pn(Qt::black);
    pn.setWidth(2);
    p->setPen(pn);

    /*QFont font;
    font.setBold(true);
    font.setPointSize(10);
    p->setFont(font);
*/
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

    // Flussufer
    QFont tmp = QFont(p->font());
    QFont font = QFont("YaHei", 40);
    font.setPointSize(40);
    font.setBold(false);
    font.setItalic(true);
    p->setFont(font);
    p->drawText(QRect(100,
                      5 * (height() - 50 - 100) / cutp_height + 10,
                      150, //width(),
                      (height() - 50 - 100) / cutp_height / 2),
                Qt::AlignCenter,
                red_river);
    p->drawText(QRect(600,
                      5 * (height() - 50 - 100) / cutp_height - 20,
                      150, //width(),
                      (height() - 50 - 100) / cutp_height / 2),
                Qt::AlignCenter,
                black_river);

    //    font.setPointSize(10);
    //font.setBold(false);
    p->setFont(tmp);

    // Seitenraender
    // Linker Rand
    p->fillRect(0, 0, 50, height(), sides);
    // Unterer Rand
    p->fillRect(0, height() - 2 * 50, width(), 2 * 50, sides);
    // Rechter Rand
    p->fillRect(width() - 50, 0, 50, height(), sides);

    p->setPen(QColor(0, 0, 0));

    if (0 == 1) {
        // Westliche, an Schach angelehnte Notation

        // Vertikaler Text
        for (int i = 0; i <= cutp_height; i++) {
            p->drawText(50 / 2,
                        50 + i * (height() - 50 - 100) / cutp_height,
                        //width() - 50,
                        //50 + i * (height() - 50 - 100) / cutp_height,
                        //0,
                        QString("%1").arg(QChar('9' - i)));
        }
        // Horizontaler Text
        for (int i = 0; i <= cutp_width; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutp_width),
                        height() - (2 * 50 / 2),
                        //50 + (i * (width() - 2 * 50) / cutp_width),
                        //height() - 100,
                        //0,
                        QString("%1").arg(QChar('a' + i)));
        }
    } else {
        // Traditionelle Notation, wird in späteren Versionen implementiert

        // Horizontaler Text ,unten
        QStringList list;
        list << "一"
             << "二"
             << "三"
             << "四"
             << "五"
             << "六"
             << "七"
             << "八"
             << "九";

        for (int i = 0; i <= cutp_width; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutp_width),
                        height() - (2 * 50 / 2),
                        QString("%1").arg(list.at(8 - i)));
        }

        // Horizontaler Text, oben
        for (int i = 0; i <= cutp_width; i++) {
            p->drawText(50 + (i * (width() - 2 * 50) / cutp_width),
                        10,
                        QString("%1").arg(QChar('1' + i)));
        }
    }
}

// Painted from upper left!
void BoardView::paintPieces(QPainter *p)
{
    Q_ASSERT(p);

    auto w = p->window().width();  //p->viewport().width();
    auto h = p->window().height(); //p->viewport().height();

    // Draws all pieces

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            QPixmap pixm = QPixmap::fromImage(basemodel.board.pieces[j][8 - i].img);
            QPixmap pixm2;
            if (basemodel.board.pieces[j][8 - i].color == Color::Red) {
                if (basemodel.board.viewStyleMode == ViewStyleMode::western_png) {
                    pixm2 = pixm.copy(100, 0, 100, 100);
                } else {
                    pixm2 = pixm.copy(0, 0, 100, 100);
                }
            } else {
                if (basemodel.board.viewStyleMode == ViewStyleMode::western_png) {
                    pixm2 = pixm.copy(300, 0, 100, 100);
                } else {
                    pixm2 = pixm.copy(200, 0, 100, 100);
                }
            }

            p->drawPixmap(QRect((50 + ((8 - i) * (w - 2 * 50) / cutp_width))
                                    - w / cutp_width / 2 / 1.5,
                                (50 + (9 - j) * (h - 50 - 100) / cutp_height)
                                    - h / cutp_width / 2 / 1.5,
                                w / (cutp_width) / 1.5,
                                h / cutp_width / 1.5),
                          pixm2);
        }
    }

    // Draws selected piece
    if (pressed) {
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(QRect((50 + (((basemodel.fromHuman.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.fromHuman.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));
    } else if (basemodel.fromUCI.col != -1 && basemodel.fromUCI.row != -1) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(5);
        p->setPen(pen);
        p->drawEllipse(QRect((50 + (((basemodel.toUCI.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.toUCI.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));

        p->drawEllipse(QRect((50 + (((basemodel.fromUCI.col)) * (w - 2 * 50) / cutp_width))
                                 - w / cutp_width / 2 / 1.5,
                             (50 + (9 - (basemodel.fromUCI.row)) * (h - 50 - 100) / cutp_height)
                                 - h / cutp_width / 2 / 1.5,
                             w / (cutp_width) / 1.5,
                             h / cutp_width / 1.5));
    }

    // draws legal moves as dots
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

        basemodel.fromHuman.col = fromCol - 1;
        basemodel.fromHuman.row = 10 - fromRow;

        GenMove legalMoves(basemodel.board.pieces, basemodel.board.onMove);
        legalPieceMovesVar = legalMoves.isValidPieceMove(basemodel.fromHuman);

        /*
        if (legalPieceMovesVar.size() == 0) {
            pressed = false;
            glfrom.col = -1;
            glfrom.row = -1;
            return;
        }
*/

    } else if (pressed) {
        toCol = static_cast<int>((((50 + boardCursorCol) / squareCol)));
        toRow = static_cast<int>((((50 + boardCursorRow) / squareRow)));

        GenMove mate(basemodel.board.pieces, basemodel.board.onMove);

        // Is in Checkmate
        if (mate.isCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
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
