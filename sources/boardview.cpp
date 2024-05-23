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

#include "QPicture"
#include "uci.h"
#include <QFontDatabase>
#include <QImageReader>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QRadialGradient>
#include <QTextItem>

extern BaseModel basemodel;

constexpr int FONT_POINT_SIZE = 30;
constexpr int PEN_WIDTH = 3;
constexpr qreal PIECE_SCALE_FACTOR = 1.3; // Adjust based on your scaling needs

BoardView::BoardView(QWidget *parent) : QWidget{parent} {
    // qDebug() << __PRETTY_FUNCTION__;
    setMouseTracking(false);
    contextMenu = new ContexMenu(this);
}

void BoardView::PrepareEuroPiece(QPicture *pix, int row, int col, int h, int w) {

    const auto piece = basemodel.position.board[row][col];
    // Check for null pointer and empty piece
    if (!piece || piece->getName() == "")
        return;

    const QString path = ":/res/" + piece->euroName + "_" + (piece->getColor()== Color::Red ? "r" : "b" )+ "_z.png";

    const QImage image(path);
    if (image.isNull()) {
        qDebug() << "Failed to load image: " << path;
        return;
    }
    QPainter painter;
    painter.begin(pix);
    pix->setBoundingRect(QRect(0, 0, w, h));

    // Gradient center and dimensions adjustment
    QPointF center(w / (2.0 * PIECE_SCALE_FACTOR * cutpWidth), h / (2.0 * PIECE_SCALE_FACTOR * cutpHeight));
    qreal radius = qMin(w, h) / (2.0 * PIECE_SCALE_FACTOR * qMax(cutpWidth, cutpHeight));

    QRadialGradient gradient(center, radius);
    gradient.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));
    gradient.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));

    painter.setBrush(gradient);

    // Draw piece circle
    const auto width = pix->boundingRect().width();
    const auto height = pix->boundingRect().height();
    const auto wdth = static_cast<double>(width) / static_cast<double>(cutpWidth) / PIECE_SCALE_FACTOR;
    const auto hght = (double) height / (double) cutpHeight / PIECE_SCALE_FACTOR;
    const auto drawRect = QRect(0, 0, round(wdth), round(hght));
    painter.drawEllipse(drawRect);

    painter.drawImage(drawRect, image);
    painter.end();
}

void BoardView::contextMenuEvent(QContextMenuEvent *event) {
    // qDebug() << __PRETTY_FUNCTION__;
    // Q_UNUSED(event);
    // qDebug() << "contextMenuEvent";
    contextMenu->contextMenuX = event->pos().x();
    contextMenu->contextMenuY = event->pos().y();
    contextMenu->exec(QCursor::pos());
}


void BoardView::paintEvent(QPaintEvent *event) {
    // qDebug() << __PRETTY_FUNCTION__;
    QPainter painter(this);

    //SetEditorPieces();

    if (basemodel.gameView == Color::Black) {
        painter.rotate(180);
        painter.translate(-width(), -height());
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    PaintBoard(&painter);

    auto width = this->width();
    auto height = this->height();
    auto marginLeft = width - 2 * 50;
    auto marginRight = height - 50. - 100;

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            if (!basemodel.position.board[j][i]->getName().isEmpty()) {
                auto *piecePicture = new QPicture();
                if (basemodel.nativePieces == false)
                    PrepareEuroPiece(piecePicture, j, i, height, width);
                else
                    PrepareNativePiece(piecePicture, j, i, height, width);
                auto x = (50. + ((i) * marginLeft / cutpWidth)) - width / cutpWidth / 2 / PIECE_SCALE_FACTOR;
                auto y = (50. + (9 - j) * marginRight / cutpHeight) - height / cutpWidth / 2. / PIECE_SCALE_FACTOR;
                painter.drawPicture(x, y, *piecePicture);
                delete piecePicture;
            }
        }
    }
    PaintSelectedPieces(&painter);
   // if (basemodel.fromUCI.x != -1)
        DrawEngineMoves(&painter);
}

// TODO: Refactor this function  to use without row/col
void BoardView::PrepareNativePiece(QPicture *pix, const int row, const int col, const int h, const int w) const {
    const auto piece = basemodel.position.board[row][col];
    // Check for null pointer and empty piece
    // TODO: Check if piece is empty with a better method
    if (!piece || piece->getName() == "")
        return;

    pix->setBoundingRect(QRect(0, 0, w, h));
    QPainter painter;
    painter.begin(pix);

    // Font setup
    QFont font;
    font.setPointSize(FONT_POINT_SIZE);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);

    // Pen setup
    QPen pen(piece->color == Color::Red ? Qt::red : Qt::black, PEN_WIDTH);
    painter.setPen(pen);

    // Gradient center and dimensions adjustment
    QPointF center(w / (2.0 * PIECE_SCALE_FACTOR * cutpWidth), h / (2.0 * PIECE_SCALE_FACTOR * cutpHeight));
    qreal radius = qMin(w, h) / (2.0 * PIECE_SCALE_FACTOR * qMax(cutpWidth, cutpHeight));

    QRadialGradient gradient(center, radius);
    gradient.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));
    gradient.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));

    painter.setBrush(gradient);

    // Draw piece circle
    const auto width = pix->boundingRect().width();
    const auto height = pix->boundingRect().height();
    const auto wdth = static_cast<double>(width) / static_cast<double>(cutpWidth) / PIECE_SCALE_FACTOR;
    const auto hght = (double) height / (double) cutpHeight / PIECE_SCALE_FACTOR;
    const auto drawRect = QRect(0, 0, round(wdth), round(hght));
    painter.drawEllipse(drawRect);

    //painter.drawEllipse(center, radius, radius);

    // Draw piece name
    //QRectF textRect(center - QPointF(radius, radius / 2), QSizeF(2 * radius, radius));
    //QRectF textRect(x, y, wght, hght);
    painter.drawText(drawRect, Qt::AlignCenter, piece->name);

    painter.end();
}

// Split in more smaller functions
void BoardView::PaintBoard(QPainter *p) const {
    // qDebug() << __PRETTY_FUNCTION__;
    Q_ASSERT(p);

    const auto width = this->width();
    const auto height = this->height();

    const auto squareWidth = (width - 2. * 50.) / cutpWidth;
    const auto squareHeight = (height - 50. - 100.) / cutpHeight;

    constexpr QColor background(252, 175, 62);
    constexpr QColor sides(206, 92, 0);
    constexpr QColor river(63, 67, 143); //"#3A438F");

    p->fillRect(p->viewport(), background);

    QPen pn(Qt::black);
    pn.setWidth(2);
    p->setPen(pn);

    // Palaeste
    p->drawLine(50 + (3 * squareWidth),
                50 + 0 * squareHeight,
                50 + (5 * squareWidth),
                50 + 2 * squareHeight);

    p->drawLine(50 + (3 * squareWidth),
                50 + 2 * squareHeight,
                50 + (5 * squareWidth),
                50 + 0 * squareHeight);

    p->drawLine(50 + (3 * squareWidth),
                50 + 9 * squareHeight,
                50 + (5 * squareWidth),
                50 + 7 * squareHeight);

    p->drawLine(50 + (3 * squareWidth),
                50 + 7 * squareHeight,
                50 + (5 * squareWidth),
                50 + 9 * squareHeight);

    // Vertikale Linien
    for (int i = 0; i <= cutpWidth; i++) {
        p->drawLine(50 + (i * squareWidth), 50,
                    50 + (i * squareWidth), height - 100);
    }

    // Horizontale Linien
    for (int i = 0; i <= cutpHeight; i++) {
        p->drawLine(50, 50 + i * squareHeight, width - 50,
                    50 + i * squareHeight);
    }

    // TODO: Hervorgehoben Punkte von Soldaten und Kanonen implementieren

    // Oberer Rand
    p->fillRect(0, 0, width, 50, sides);

    // Fluss
    p->fillRect(0, 50 + 4 * squareHeight, width, squareHeight, river);

    // Flussufer
    auto tmp = QFont(p->font());
    auto font = QFont();
    font.setPointSize(30);
    font.setBold(true);
    font.setItalic(true);
    p->setFont(font);
    p->drawText(QRect(50, 5 * squareHeight + 10,
                      150, // width(),
                      squareHeight / 2),
                Qt::AlignCenter, redRiver);
    p->drawText(QRect(600, 5 * squareHeight - 20,
                      150, // width(),
                      squareHeight / 2),
                Qt::AlignCenter, blackRiver);

    p->setFont(tmp);

    // Seitenraender
    // Linker Rand
    p->fillRect(0, 0, 50, height, sides);
    // Unterer Rand
    p->fillRect(0, height - 2 * 50, width, 2 * 50, sides);
    // Rechter Rand
    p->fillRect(width - 50, 0, 50, height, sides);

    p->setPen(QColor(0, 0, 0));

    // Notation
    if (true) {
        // Westliche, an Schach angelehnte Notation

        // Vertikaler Text
        for (int i = 0; i <= cutpHeight; i++) {
            p->drawText(50 / 2, 50 + i * squareHeight,
                        QString("%1").arg(QChar('9' - i)));
        }
        // Horizontaler Text
        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * squareWidth),
                        height - (2 * 50 / 2),
                        QString("%1").arg(QChar('a' + i)));
        }
    } else {
        // TODO: Traditionelle Notation, wird in späteren Versionen implementiert

        // Horizontaler Text ,unten
        QStringList list;
        list << "\u4e00" //"一"
                << "\u4e8c" //"二"
                << "\u4e09" //"三"
                << "\u56db" //"四"
                << "\u4e94" //"五"
                << "\u516d" //"六"
                << "\u4e03" //"七"
                << "\u516b" //"八"
                << "\u4e5d"; //"九";

        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * squareWidth),
                        height - (2 * 50 / 2), QString("%1").arg(list.at(8 - i)));
        }

        // Horizontaler Text, oben
        for (int i = 0; i <= cutpWidth; i++) {
            p->drawText(50 + (i * squareWidth), 10,
                        QString("%1").arg(QChar('1' + i)));
        }
    }

    // Aussenraender
    pn.setWidth(3);
    p->setPen(pn);
    p->drawRect(46, 46, width - 93, height - 143);

    pn.setWidth(2);
    p->setPen(pn);
    p->drawRect(50, 50, width - 100, height - 150);

    pn.setWidth(2);
    p->setPen(pn);
}

bool BoardView::event(QEvent *event) {
    const auto width = this->width();
    const auto height = this->height();
    const auto marginLeft = width - 2 * 50;
    const auto marginRight = height - 50. - 100;

    if (event->type() == QEvent::ToolTip) {
        auto *helpEvent = dynamic_cast<QHelpEvent *>(event);
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 9; i++) {
                auto x = 50. + i * marginLeft / cutpWidth - width / cutpWidth / 2 / PIECE_SCALE_FACTOR;
                auto y = (50. + (9 - j) * marginRight / cutpHeight) - height / cutpWidth / 2. / PIECE_SCALE_FACTOR;

                if (helpEvent->pos().x() > x && helpEvent->pos().x() < x + width / cutpWidth / PIECE_SCALE_FACTOR &&
                    helpEvent->pos().y() > y && helpEvent->pos().y() < y + height / cutpHeight / PIECE_SCALE_FACTOR &&
                    !basemodel.position.board[j][i]->getName().isEmpty()) {
                    QToolTip::showText(helpEvent->globalPos(),
                                       QString("%1:\n%2").arg(basemodel.position.board[j][i]->getEuroName()).arg(
                                           basemodel.position.board[j][i]->getEuroNameDesc()), this, rect());
                    return true;
                }
            }
        }
        //if (helpEvent->pos().x() > 50 && helpEvent->pos().x() < width() - 50) {
        //   QToolTip::showText(helpEvent->globalPos(), QString("%1, %2").arg(helpEvent->pos().x()).arg(helpEvent->pos().y()), this, rect());
        //}
        return true;
    } else {
        QToolTip::hideText();
        event->ignore();
    }
    return QWidget::event(event);
}

void BoardView::PaintSelectedPieces(QPainter *painter) const {
    // qDebug() << __PRETTY_FUNCTION__;
    Q_ASSERT(painter);

    if (basemodel.fromHuman.x == -1 || basemodel.fromHuman.y == -1)
        return;

    const auto width = this->width();
    const auto height = this->height();
    if (pressed == true && secondclick == false) {
        QPen pen;
        // Draw selected piece
        pen.setColor(Qt::green);
        pen.setWidth(5);

        painter->setPen(pen);
        painter->setBrush(Qt::transparent);

        const auto x = (50 + (((basemodel.fromHuman.y)) * (width - 2 * 50) / cutpWidth)) -
                       width / cutpWidth / 2 / PIECE_SCALE_FACTOR;
        const auto y = (50 + (9 - (basemodel.fromHuman.x)) * (height - 50 - 100) / cutpHeight) -
                       height / cutpWidth / 2 / PIECE_SCALE_FACTOR;
        const auto wght = width / (cutpWidth) / PIECE_SCALE_FACTOR;
        const auto hght = height / cutpHeight / PIECE_SCALE_FACTOR;

        painter->drawEllipse(QRect(x, y, wght, hght));

        // Draw legal moves
        pen.setColor(Qt::red);
        pen.setWidth(5);
        painter->setPen(pen);

        const auto fromX = basemodel.fromHuman.x;
        const auto fromY = basemodel.fromHuman.y;

        auto all_moves = basemodel.position.board[fromX][fromY]->
                generateValidMoves(Point(fromX, fromY), basemodel.position.board);
        for (const auto &move: all_moves) {
            auto x = (50 + ((move.second.y) * (width - 2 * 50) / cutpWidth)) -
                     width / cutpWidth / 2 / PIECE_SCALE_FACTOR;
            auto y = (50 + (9 - move.second.x) * (height - 50 - 100) / cutpHeight) -
                     height / cutpWidth / 2 / PIECE_SCALE_FACTOR;

            painter->drawEllipse(QRect(x, y, wght, hght));
        }
    }
}

void BoardView::DrawEngineMoves(QPainter *painter) const {
    // draws the last moved line
    QPen pen;
    const auto width = this->width();
    const auto height = this->height();

    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter->setPen(pen);

    painter->setOpacity(0.7);

    const auto x1 = (50 + (basemodel.fromUCI.x) * (width - 2 * 50) / cutpWidth);
    const auto y1 = (50 + (9 - basemodel.fromUCI.y) * (height - 50 - 100) / cutpHeight);
    const auto x2 = (50 + (basemodel.toUCI.x) * (width - 2 * 50) / cutpWidth);
    const auto y2 = (50 + (9 - basemodel.toUCI.y) * (height - 50 - 100) / cutpHeight);

    painter->drawLine(x1, y1, x2, y2);

    pen.setWidth(25);
    painter->setPen(pen);
    painter->drawPoint(x1, y1);

    pen.setWidth(18);
    painter->setPen(pen);
    painter->drawPoint(x2, y2);

    pen.setWidth(5);
    painter->setPen(pen);
    painter->setOpacity(1);

    pen.setColor(Qt::black);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawEllipse(QRect(x1 - width / cutpWidth / 2 / 1.5,
                               y1 - height / cutpWidth / 2 / 1.5,
                               width / (cutpWidth) / 1.5,
                               height / cutpHeight / 1.5));

    painter->drawEllipse(QRect(
        x2 - width / cutpWidth / 2 / 1.5,
        y2 - height / cutpWidth / 2 / 1.5,
        width / (cutpWidth) / 1.5, height / cutpHeight / 1.5));

    painter->setOpacity(1.0);
}

void BoardView::mousePressEvent(QMouseEvent *event) {
    const int x = event->pos().x();
    const int y = event->pos().y();
    const Point boardPos = CalcBoardCoords({x, y});
    const Point coord{10 - boardPos.y, boardPos.x - 1}; // Simplified coordinate conversion
    if (!pressed) {
        pressed = true;
        basemodel.fromHuman = coord;

        if (const Piece piece = *basemodel.position.board[coord.x][coord.y];
            piece.getName().isEmpty() || piece.getColor() != basemodel.position.players_color) {
            pressed = false; // Reset if invalid piece or color
            return;
        }
        update(); // Refresh to show selected piece
    } else {
        basemodel.toHuman = coord;
        if (basemodel.toHuman.x == basemodel.fromHuman.x && basemodel.toHuman.y == basemodel.fromHuman.y) {
            pressed = false;
            update(); // Refresh to show changes or revert view
            return;
        }
        if (basemodel.position.board[basemodel.toHuman.x][basemodel.toHuman.y]->getColor() == basemodel.position.
            players_color) {
            basemodel.fromHuman = basemodel.toHuman; // Move the selected piece to the new square
            update(); // Refresh to show changes or revert view
            return;
        }
        pressed = false;
        auto all_moves = Board::getAllValidMoves(basemodel.position.players_color, basemodel.position.board);
        for (const auto &[from, to]: all_moves) {
            if (from == basemodel.fromHuman && to == basemodel.toHuman) {
                //emit updateView(basemodel.fromHuman, basemodel.toHuman, basemodel.mode);
                basemodel.fromHuman = from;
                basemodel.toHuman = to;
                update();
                return;
            }
        }
        // If no valid move was found, reset the state but do not emit the move
        update(); // Refresh to show changes or revert view
    }
}


// Calculates from mouse to board coordinates
constexpr float LEFT_MARGIN = 50.0f;
constexpr float RIGHT_MARGIN = 50.0f;
constexpr float TOP_MARGIN = 50.0f;
constexpr float BOTTOM_MARGIN = 100.0f;
constexpr float BOARD_COL_POINTS = BaseModel::BoardColPoints; // Assuming this is a valid static constant
constexpr float BOARD_ROW_POINTS = BaseModel::BoardRowPoints; // Assuming this is a valid static constant

Point BoardView::CalcBoardCoords(Point r) const {
    const float w = this->width();
    const float h = this->height();

    // Calculate dimensions of each square on the board
    const float squareColWidth = (w - LEFT_MARGIN - RIGHT_MARGIN) / BOARD_COL_POINTS;
    const float squareRowHeight = (h - TOP_MARGIN - BOTTOM_MARGIN) / BOARD_ROW_POINTS;

    // Transform cursor position based on game view
    const float boardCursorX = basemodel.gameView == Color::Red ? r.x : w - r.x;
    const float boardCursorY = basemodel.gameView == Color::Red ? r.y : h - r.y;

    // Calculate board coordinates from cursor position
    int col = static_cast<int>(std::floor(boardCursorX / squareColWidth)) + 1;
    int row = static_cast<int>(std::floor(boardCursorY / squareRowHeight)) + 1;

    return {col, row};
}

void BoardView::PaintMarker(QPainter *p) const {
    // qDebug() << __PRETTY_FUNCTION__;
    Q_ASSERT(p);

    auto w = this->width();
    auto h = this->height();

    int lcol = 0;
    int lrow = 0;

    for (auto &marker: contextMenu->markers) {
        auto x = marker.first.x;
        auto y = marker.first.y;

        qDebug() << x << y << "x und y";

        const Point coords = CalcBoardCoords(marker.first);
        int col = coords.x;
        int row = coords.y;

        switch (marker.second) {
            case markerType::Kreuz:

                break;
            case markerType::Kreis:
                p->setPen(QPen(Qt::green, 8));
                p->drawChord(QRect((50 + (((col - 1)) * (w - 2 * 50) / cutpWidth)) -
                                   w / cutpWidth / 2 / 1.5,
                                   (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight) -
                                   h / cutpWidth / 2 / 1.5,
                                   w / (cutpWidth) / 1.5, h / cutpWidth / 1.5),
                             0, 5760);

                break;
            case markerType::Dreieck:

                break;
            case markerType::Linie:
                p->setPen(QPen(Qt::blue, 8));
                p->drawPoint((50 + (((col - 1)) * (w - 2 * 50) /
                                    cutpWidth)), // - w / cutp_width / 2 / 1.5,
                             (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight));
            //- h / cutp_width / 2 / 1.5);
                lrow = row;
                lcol = col;
                break;
            case markerType::Linienende:
                p->setPen(QPen(Qt::blue, 8));

                p->drawLine(50 + (((lcol - 1)) * (w - 2 * 50) /
                                  cutpWidth), // - w / cutp_width / 2 / 1.5,
                            (50 + ((lrow - 1)) * (h - 50 - 100) / cutpHeight),
                            50 + (((col - 1)) * (w - 2 * 50) /
                                  cutpWidth), // - w / cutp_width / 2 / 1.5,
                            (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight));

                break;
            case markerType::Viereck:
                p->setPen(QPen(Qt::yellow, 8));

                p->drawRect(QRect((50 + (((col - 1)) * (w - 2 * 50) / cutpWidth)) -
                                  w / cutpWidth / 2 / 1.5,
                                  (50 + ((row - 1)) * (h - 50 - 100) / cutpHeight) -
                                  h / cutpWidth / 2 / 1.5,
                                  w / (cutpWidth) / 1.5, h / cutpWidth / 1.5));
                break;
            default:
                break;
        }
    }
}

//  Sets the selected pieces on the (clean) board
void BoardView::SetEditorPieces() {
    // qDebug() << __PRETTY_FUNCTION__;
    QPoint coords;
    /* for (auto& piece : contextMenu->pieces) {
        coords = CalcBoardCoords(piece.first);
        QPoint tmp = coords;
        coords.setX(10 - tmp.y());
        coords.setY(tmp.x() - 1);
        switch (piece.second) {
                  case completePieceType::GeneralRot:
             basemodel.position.add_piece(new
      PPiece{PieceType::General,Color::Red, QImage(), QPoint(coords.x(),
      coords.y()), "\u5e25"},coords.x(), coords.y()); //"帥")); break; case
      completePieceType::GeneralSchwarz: basemodel.position.add_piece(new
      PPiece{ PieceType::General,Color::Black, QImage(), QPoint(coords.x(),
      coords.y()),
                                                     "\u5c07"},coords.x(),
      coords.y()); //"將")); break; case completePieceType::AdvisorRot:
           basemodel.position.add_piece(new PPiece{
      PieceType::Advisor,Color::Red,QImage(), QPoint(coords.x(), coords.y()),
                                           "\u4ed5"},coords.x(), coords.y());
      //"仕")); break; case completePieceType::AdvisorSchwarz:
           basemodel.position.add_piece(new
      PPiece{PieceType::Advisor,Color::Black,QImage(), QPoint(coords.x(),
      coords.y()),
                                           "\u58eb"},coords.x(), coords.y()); //
      "士")); break; case completePieceType::ElephantRot:
           basemodel.position.InitPiece(Piece(color::Red, pieceType::Elephant,
                                           {coords.x(), coords.y()},
                                           "\u76f8")); //"相"));
           break;
         case completePieceType::ElephantSchwarz:
           basemodel.position.InitPiece(Piece(color::Black, pieceType::Elephant,
                                           {coords.x(), coords.y()},
                                           "\u8c61")); //"象"));
           break;
         case completePieceType::HorseRot:
           basemodel.position.InitPiece(Piece(color::Red, pieceType::Horse,
                                           {coords.x(), coords.y()},
                                           "\u508c")); //"傌"));
           break;
         case completePieceType::HorseSchwarz:
           basemodel.position.InitPiece(Piece(color::Black, pieceType::Horse,
                                           {coords.x(), coords.y()},
                                           "\u99ac")); //"馬"));
           break;
         case completePieceType::ChariotRot:
           basemodel.position.InitPiece(Piece(color::Red, pieceType::Chariot,
                                           {coords.x(), coords.y()},
                                           "\u4fe5")); //"俥"));
           break;
         case completePieceType::ChariotSchwarz:
           basemodel.position.InitPiece(Piece(color::Black, pieceType::Chariot,
                                           {coords.x(), coords.y()},
                                           "\u8eca")); //"車"));
           break;
         case completePieceType::CannonRot:
           basemodel.position.InitPiece(Piece(color::Red, pieceType::Cannon,
                                           {coords.x(), coords.y()},
                                           "\u70ae")); // "炮"));
           break;
         case completePieceType::CannonSchwarz:
           basemodel.position.InitPiece(Piece(color::Black, pieceType::Cannon,
                                           {coords.x(), coords.y()},
                                           "\u7832")); // "砲"));
           break;
         case completePieceType::SoldierRot:
           basemodel.position.InitPiece(Piece(color::Red, pieceType::Soldier,
                                           {coords.x(), coords.y()},
                                           "\u5175")); //"兵"));
           break;
         case completePieceType::SoldierSchwarz:
           basemodel.position.InitPiece(Piece(color::Black, pieceType::Soldier,
                                           {coords.x(), coords.y()},
                                           "\u5352")); //"卒"));
           break;
        default:
            qDebug() << "Error in boarview::SetEditorPieces";
            break;
        }
    }*/
}


/*
// Draws the pieces on the board (native)
QPixmap *BoardView::PrepareNativePiece(QPainter *p, int row, int col) {
  // qDebug() << __PRETTY_FUNCTION__;
  // QPainter *p;
  Q_ASSERT(p);
  // if (basemodel.pieces[row][col].type == pieceType::Empty)
  //     return

  auto w = p->viewport().width();  // p->viewport().width();
  auto h = p->viewport().height(); // p->viewport().height();

  QPen pen;
  pen.setColor(Qt::red);
  pen.setWidth(3);
  p->setPen(pen);

  QFont font;
  font.setPointSize(30);
  font.setWeight(QFont::DemiBold);
  p->setFont(font);
  int i_row = 9-row;//9-row
  int i_col = col;//8- col;
  // Draws all pieces
  if (basemodel.position.board[i_row][i_col].piece != nullptr) {
	if (basemodel.position.board[i_row][i_col].piece->color == Color::Red) {
	  // Draw red
	  pen.setColor(Qt::red);
	  p->setPen(pen);
	} else {
	  // Draw black
	  pen.setColor(Qt::black);
	  p->setPen(pen);
	}
	QRadialGradient gradient1(
		QPointF(50.0 + (((i_col)) * (w - 2.0 * 50.0) / cutpWidth),
				50 + (i_row) * (h - 2 * 50) / cutpHeight),
		50);
	gradient1.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
	gradient1.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

	QBrush brush1(gradient1);
	p->setBrush(brush1);

	p->drawChord(QRect((50 + (((i_col)) * (w - 2 * 50) / cutpWidth)) -
						   w / cutpWidth / 2 / 1.5,
					   (50 + (i_row) * (h - 50 - 100) / cutpHeight) -
						   h / cutpWidth / 2 / 1.5,
					   w / (cutpWidth) / 1.5, h / cutpWidth / 1.5),
				 0, 5760);

	p->drawText(QRect((50 + (((i_col)) * (w - 2 * 50) / cutpWidth)) -
						  w / cutpWidth / 2 / 1.8,
					  (50 + (i_row) * (h - 50 - 100) / cutpHeight) -
						  h / cutpWidth / 2 / 2,
					  w / (cutpWidth), h / cutpWidth),
				basemodel.position.board[i_row][i_col].piece->name);
	p->drawPixmap(0, 0, pix);
  }
  return &pix;
}
*/

/*
// Painted from upper left!
void BoardView::PaintPieces(QPainter *p) {
  // qDebug() << __PRETTY_FUNCTION__;
  Q_ASSERT(p);

  auto w = p->window().width();  // p->viewport().width();
  auto h = p->window().height(); // p->viewport().height();

  // Draws all pieces

  for (int j = 0; j < 10; j++) {
	for (int i = 0; i < 9; i++) {
	  QPixmap pixm;
	  QPixmap pixm2;
	  QPicture img;
	  //PPiece *piece = basemodel.position.board[j][i].piece;
	  if (basemodel.position.board[j][i].piece != nullptr) {
		if (basemodel.position.board[j][i].piece->color == Color::Red) {
		  if (basemodel.viewStyleModeVar == viewStyleMode::western_png) {
			pixm =
				QPixmap::fromImage(basemodel.position.board[j][i].piece->img);
			pixm2 = pixm.copy(100, 0, 100, 100);
		  } else if (basemodel.viewStyleModeVar ==
					 viewStyleMode::traditional_png) {
			pixm = QPixmap::fromImage(basemodel.position.board[j][i].piece->img,
									  Qt::PreferDither);
			pixm2 = pixm.copy(0, 0, 100, 100);
		  } else if (basemodel.viewStyleModeVar ==
					 viewStyleMode::traditional_native) {
			//pixm2 = *PrepareNativePiece(p, j, i);
			  PrepareNativePiece(&img, j, i);

		  }
		} else {
		  if (basemodel.viewStyleModeVar == viewStyleMode::western_png) {
			pixm =
				QPixmap::fromImage(basemodel.position.board[j][i].piece->img);
			pixm2 = pixm.copy(300, 0, 100, 100);
		  } else if (basemodel.viewStyleModeVar ==
					 viewStyleMode::traditional_png) {
			pixm = QPixmap::fromImage(basemodel.position.board[j][i].piece->img,
									  Qt::PreferDither);
			pixm2 = pixm.copy(200, 0, 100, 100);
		  } else if (basemodel.viewStyleModeVar ==
					 viewStyleMode::traditional_native) {
			//pixm2 = *PrepareNativePiece(p, j, i);
			  PrepareNativePiece(&img, j, i);
		  }
		}
		p->drawPicture(/*QRect(*/ /*(50 + ((8-i) * (w - 2 * 50) / cutpWidth)) -
								w / cutpWidth / 2 / 1.5,
							(50 + (9-j) * (h - 50 - 100) / cutpHeight) -
								h / cutpWidth / 2 / 1.5/*,
							w / (cutpWidth) / 1.5, h / cutpWidth / 1.5),*/
/* img);
      }
    }
  }
}
*/
