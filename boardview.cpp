#include "boardview.h"
#include "mainwindow.h"

#include <QPainter>

extern BaseModel basemodel;

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
    //const QColor river("#b0d7db");

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
                background);

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

void BoardView::paintPieces(QPainter *p)
{
    auto w = p->viewport().width();
    auto h = p->viewport().height();
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            p->drawImage(QRect((50 + ((8 - i) * (w - 2 * 50) / cutp_width))
                                   - w / cutp_width / 2 / 1.5,
                               (50 + (9 - j) * (h - 50 - 100) / cutp_height)
                                   - h / cutp_width / 2 / 1.5,
                               w / (cutp_width) / 1.5,
                               h / cutp_width / 1.5),
                         basemodel.board.pieces[j][8 - i].img);
            //qDebug() << j << j / 9 << j % 9;
            //qDebug() << basemodel.board.pieces[0][0].img;
            //qDebug() << basemodel.board.pieces[j % 9][j / 9].pos.col;
        }
    }
    /*
    p->drawImage(QRect((50 + ((fromCol - 1) * (w - 2 * 50) / cutp_width)) - w / cutp_width / 2 / 1.5,
                       (50 + (fromRow - 1) * (h - 50 - 100) / cutp_height)
                           - h / cutp_width / 2 / 1.5,
                       w / (cutp_width) / 1.5,
                       h / cutp_width / 1.5),
                 QImage(":res/rookRed.png"));
*/
    //paint Dots
    /*    qDebug() << "count of moves to dot:" << piece_moves.size();
    for (auto move : piece_moves) {
        p->drawRoundedRect(QRect((((move.second.col) * (w - 2 * 50) / cutp_width))
                                     - w / cutp_width / 2 / 1.5,
                                 ((move.second.row) * (h - 50 - 100) / cutp_height)
                                     - h / cutp_width / 2 / 1.5,
                                 w / (cutp_width) / 1.5,
                                 h / cutp_width / 1.5),
                           1,
                           1);
        qDebug() << "move to dot" << move.second.col << move.second.row;
    }
*/
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
        qDebug() << "from: " << fromCol << fromRow;
        /*
        auto curr_color = ((Widget *) parentWidget())
                              ->boardmodel.movGen.getColor(
                                  {static_cast<int>(m_fromX), static_cast<int>(m_fromY)});
        qDebug() << "color" << &curr_color;

        auto moves = ((Widget *) parentWidget())->boardmodel.movGen.generateLegalMoves(curr_color);

        for (const auto &move : moves) {
            if (move.first.col == (int) m_fromX && move.first.row == (int) m_fromY) {
                piece_moves.push_back(move);
            }
            qDebug() << move.first.col << move.first.row << m_fromX << m_fromY;
        }
*/
        // basemodel.board.placePiece(
        //    Piece(Color::Red, PieceType::Chariot, {fromRow, fromCol}, QImage(":/res/rookRed.png")));
        // qDebug() << fromCol << fromRow;

        pressed = true;

    } else if (pressed) {
        toCol = static_cast<int>((((50 + boardCursorCol) / squareCol)));
        toRow = static_cast<int>((((50 + boardCursorRow) / squareRow)));
        qDebug() << "to: " << toCol << toRow;
        pressed = false;
        //qDebug() << "mousePressEvent: " << fromRow - 2 << 10 - fromCol << toRow - 2 << 10 - toCol;
        if (basemodel.board.isLegalMove(fromRow - 1, fromCol - 1, toRow - 1, toCol - 1)) {
            basemodel.board.movePiece(10 - fromRow, fromCol - 1, 10 - toRow, toCol - 1);
            emit updateView(10 - fromRow, fromCol - 1, 10 - toRow, toCol - 1, 0);
        } else {
            qDebug() << "Illegal move";
        }
        // coordinates needed?
    }
    repaint();
}
