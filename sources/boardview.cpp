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
#include "qpicture.h"

#include <QFontDatabase>
#include <QImageReader>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QRadialGradient>
#include <QTextItem>

#include <cchess_rules.h>

extern BaseModel basemodel;

BoardView::BoardView(QWidget *parent) : QWidget{parent} {
  // qDebug() << __PRETTY_FUNCTION__;
  setMouseTracking(false);
  // setFocusPolicy(Qt::StrongFocus);
  contextMenu = new ContexMenu(this);
  pix = new QPicture();

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

  // SetEditorPieces();

  if (basemodel.gameView == color::Black) {
    painter.rotate(180);
    painter.translate(-width(), -height());
  }
  painter.setRenderHint(QPainter::Antialiasing, true);
  PaintBoard(&painter);
  //PaintPieces(&painter);
  // int i = 8;
  // int j = 0;
  auto w = painter.viewport().width();  // p->viewport().width();
  auto h = painter.viewport().height(); // p->viewport().height();

  for (int j = 0; j < 10; j++) {
      for (int i = 0; i < 9; i++) {
        PrepareNativePiece(pix,j,i, h, w);

        painter.drawPicture(/*QRect(*/(50 + ((i) * (w - 2 * 50) / cutpWidth)) -
                     w / cutpWidth / 2 / 1.5,
                 (50 + (9-j) * (h - 50 - 100) / cutpHeight) -
                     h / cutpWidth / 2 / 1.5/*,
                            w / (cutpWidth) / 1.5, h / cutpWidth / 1.5)*/,
                      *pix);  // PaintMarker(&painter);

      }
  }
  PaintSelectedPieces(&painter);
}

void BoardView::PaintMarker(QPainter *p) {
  // qDebug() << __PRETTY_FUNCTION__;
  Q_ASSERT(p);

  auto w = p->viewport().width();  // p->viewport().width();
  auto h = p->viewport().height(); // p->viewport().height();

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

void BoardView::PrepareNativePiece(QPicture *pix, int row, int col, int h, int w) {

    QPen pen;
    pen.setWidth(3);
    QFont font;
    QPainter p;
    //pix = new QImage(100,100,QImage::Format_ARGB32_Premultiplied);
    p.begin(pix);
    font.setPointSize(30);
    font.setWeight(QFont::DemiBold);
    p.setFont(font);
    int i_row = row;
    int i_col = col;//8- col;
    // Draws all pieces
    if (basemodel.position.board[i_row][i_col].piece != nullptr) {
        if (basemodel.position.board[i_row][i_col].piece->color == Color::Red) {
            // Draw red
            pen.setColor(Qt::red);
            p.setPen(pen);
        } else {
            // Draw black
            pen.setColor(Qt::black);
            p.setPen(pen);
        }
        QRadialGradient gradient1(
            QPointF(50.0 + (((i_col)) * (100 - 2.0 * 50.0) / cutpWidth),
                    50 + (i_row) * (100 - 2 * 50) / cutpHeight),
            50);
        gradient1.setColorAt(1, QColor::fromRgb(222, 91, 16, 255));
        gradient1.setColorAt(0, QColor::fromRgb(255, 255, 255, 255));

        QBrush brush1(gradient1);
        p.setBrush(brush1);

        p.drawChord(QRect(0,0,
                            w / (cutpWidth) / 1.5, h / cutpWidth / 1.5),
                     0, 5760);

        p.drawText(QRect(0,0, w / (cutpWidth) / 1.5, h / cutpWidth / 1.5),
                    basemodel.position.board[i_row][i_col].piece->name);
        //p.drawPixmap(0, 0,100,100, *pix);
    }
    p.end();

}

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
        p->drawPicture(/*QRect(*//*(50 + ((8-i) * (w - 2 * 50) / cutpWidth)) -
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
void BoardView::PaintBoard(QPainter *p) {
  // qDebug() << __PRETTY_FUNCTION__;
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
    p->drawLine(50 + (i * (width() - 2 * 50) / cutpWidth), 50,
                50 + (i * (width() - 2 * 50) / cutpWidth), height() - 100);
  }

  // Horizontale Linien
  for (int i = 0; i <= cutpHeight; i++) {
    p->drawLine(50, 50 + i * (height() - 50 - 100) / cutpHeight, width() - 50,
                50 + i * (height() - 50 - 100) / cutpHeight);
  }

  // TODO: Hervorgehoben Punkte von Soldaten und Kanonen implementieren

  // Oberer Rand
  p->fillRect(0, 0, width(), 50, sides);

  // Fluss
  p->fillRect(0, 50 + 4 * (height() - 50 - 100) / cutpHeight, width(),
              (height() - 50 - 100) / cutpHeight,
              river); // background);

  // Flussufer
  QFont tmp = QFont(p->font());
  QFont font = QFont();
  font.setPointSize(30);
  font.setBold(false);
  font.setItalic(true);
  p->setFont(font);
  p->drawText(QRect(100, 5 * (height() - 50 - 100) / cutpHeight + 10,
                    150, // width(),
                    (height() - 50 - 100) / cutpHeight / 2),
              Qt::AlignCenter, redRiver);
  p->drawText(QRect(600, 5 * (height() - 50 - 100) / cutpHeight - 20,
                    150, // width(),
                    (height() - 50 - 100) / cutpHeight / 2),
              Qt::AlignCenter, blackRiver);

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
      p->drawText(50 / 2, 50 + i * (height() - 50 - 100) / cutpHeight,
                  // width() - 50,
                  // 50 + i * (height() - 50 - 100) / cutp_height,
                  // 0,
                  QString("%1").arg(QChar('9' - i)));
    }
    // Horizontaler Text
    for (int i = 0; i <= cutpWidth; i++) {
      p->drawText(50 + (i * (width() - 2 * 50) / cutpWidth),
                  height() - (2 * 50 / 2),
                  // 50 + (i * (width() - 2 * 50) / cutp_width),
                  // height() - 100,
                  // 0,
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
                  height() - (2 * 50 / 2), QString("%1").arg(list.at(8 - i)));
    }

    // Horizontaler Text, oben
    for (int i = 0; i <= cutpWidth; i++) {
      p->drawText(50 + (i * (width() - 2 * 50) / cutpWidth), 10,
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

void BoardView::PaintSelectedPieces(QPainter *p) {
  // qDebug() << __PRETTY_FUNCTION__;
  Q_ASSERT(p);

  auto w = p->viewport().width();  // p->viewport().width();
  auto h = p->viewport().height(); // p->viewport().height();
  QPen pen;
  p->setBrush(Qt::transparent);
  pen.setColor(Qt::green);
  pen.setWidth(5);
  p->setPen(pen);

  if (fromHuman.y() != -1) {

    if (secondclick == false) {
      // Draws selected piece
      p->setBrush(Qt::transparent);
      pen.setColor(Qt::green);
      pen.setWidth(5);
      p->setPen(pen);
      p->drawEllipse(
          QRect((50 + (((fromHuman.y())) * (w - 2 * 50) / cutpWidth)) -
                    w / cutpWidth / 2 / 1.5,
                (50 + (9 - (fromHuman.x())) * (h - 50 - 100) / cutpHeight) -
                    h / cutpWidth / 2 / 1.5,
                w / (cutpWidth) / 1.5, h / cutpWidth / 1.5));

      pen.setColor(Qt::red);
      pen.setWidth(5);
      p->setPen(pen);

      // draws legal moves as circles
      // Position pos(basemodel.position.board,
      // basemodel.position.players_color);

      for (const auto &move : basemodel.position.generate_piece_moves(
               basemodel.position.board[fromHuman.x()][fromHuman.y()]
                   .piece->piece_type,
               fromHuman.y(), fromHuman.x())) {
        /*if ((!basemodel.position.is_evil_glare(
                QPoint(fromHuman.x(), fromHuman.y()),
                QPoint(move.second, move.first),
                basemodel.position.board[fromHuman.x()][fromHuman.y()]
                    .piece->color))) {*/
          p->drawEllipse(
              QRect((50 + ((move.first) * (w - 2 * 50) / cutpWidth)) -
                        w / cutpWidth / 2 / 1.5,
                    (50 + (9 - move.second) * (h - 50 - 100) / cutpHeight) -
                        h / cutpWidth / 2 / 1.5,
                    w / (cutpWidth) / 1.5, h / cutpWidth / 1.5));
        //}
      }
    }
  }
  //}

  if (basemodel.fromUCI.y() != -1) {
    // draws the last moved line
    pen.setColor(Qt::black);

    pen.setWidth(4);
    p->setPen(pen);
    p->setOpacity(0.7);

    // qDebug() << basemodel.toUCI.y();
    p->drawLine(
        (50 + (((basemodel.fromUCI.x())) * (w - 2 * 50) / cutpWidth)),
        (50 + ((9 - basemodel.fromUCI.y())) * (h - 50 - 100) / cutpHeight),

        (50 + (((basemodel.toUCI.x())) * (w - 2 * 50) / cutpWidth)),
        (50 + ((9 - basemodel.toUCI.y())) * (h - 50 - 100) / cutpHeight));

    pen.setWidth(25);
    p->setPen(pen);

    p->drawPoint(
        (50 + (((basemodel.fromUCI.x())) * (w - 2 * 50) / cutpWidth)),
        (50 + ((9 - basemodel.fromUCI.y())) * (h - 50 - 100) / cutpHeight));

    pen.setWidth(18);
    p->setPen(pen);
    p->drawPoint(
        (50 + (((basemodel.toUCI.x())) * (w - 2 * 50) / cutpWidth)),
        (50 + ((9 - basemodel.toUCI.y())) * (h - 50 - 100) / cutpHeight));

    pen.setWidth(5);
    p->setPen(pen);
    p->setOpacity(1);

    pen.setColor(Qt::black);
    pen.setWidth(5);
    p->setPen(pen);
    p->drawEllipse(
        QRect((50 + (((basemodel.toUCI.x())) * (w - 2 * 50) / cutpWidth)) -
                  w / cutpWidth / 2 / 1.5,
              (50 + (9 - (basemodel.toUCI.y())) * (h - 50 - 100) / cutpHeight) -
                  h / cutpWidth / 2 / 1.5,
              w / (cutpWidth) / 1.5, h / cutpWidth / 1.5));

    p->drawEllipse(QRect(
        (50 + (((basemodel.fromUCI.x())) * (w - 2 * 50) / cutpWidth)) -
            w / cutpWidth / 2 / 1.5,
        (50 + (9 - (basemodel.fromUCI.y())) * (h - 50 - 100) / cutpHeight) -
            h / cutpWidth / 2 / 1.5,
        w / (cutpWidth) / 1.5, h / cutpWidth / 1.5));
  }
}

void BoardView::mousePressEvent(QMouseEvent *event) {
  // qDebug() << __PRETTY_FUNCTION__;
  // qDebug() << "mousePressEvent";
  QPoint p = CalcBoardCoords({event->pos().x(), event->pos().y()});
  if (!pressed) {
      from = p;
  fromHuman = p;
    fromHuman.setY(from.x() - 1);
    fromHuman.setX(10 - from.y());
    pressed = true;
    secondclick = false;
/*
        if (basemodel.position.board[fromHuman.x()][fromHuman.y()].piece !=
       nullptr) { pressed = false; return;
        }
        if (basemodel.position.board[fromHuman.x()][fromHuman.y()].piece->color
       != basemodel.position.players_color) { pressed = false; return;
        }
*/
  } else if (pressed) {
    to = p;
    toHuman = p;
    toHuman.setX(to.x()-1);
    toHuman.setY(10-to.y());

    // GenMove mate(basemodel.board.pieces, basemodel.board.onMove);

    // Is in Checkmate
    // if (mate.IsCheckmate(basemodel.board.onMove)) {
    //    qDebug() << "Checkmate";
    //    return;
    //}
    pressed = false;
    secondclick = true;

    // Position p(basemodel.board.pieces, basemodel.board.onMove);

    // std::vector<std::pair<QPoint, QPoint>> moves;
    // std::vector<std::pair<int, int>> all_moves_to;
    // std::vector<std::pair<int, int>> all_moves_from;

    // find all possible moves
    // moves = p.generate_all_moves();
    // from.setX( 9 -from.x() );
    // from.setY( 8 -from.y() );

    //std::cout << from.x() << from.y();
    //std::cout << to.x() << to.y() << std::endl;
    int i = 0;
    // draws legal moves as circles
    // Position pos(basemodel.position.board, basemodel.position.players_color);

    //for (const auto &move : basemodel.position.generate_piece_moves(
    //         basemodel.position.board[fromHuman.x()][fromHuman.y()]
    //             .piece->piece_type,
    //         fromHuman.y(), fromHuman.x())) {
      /*if ((!pos.is_evil_glare(
              QPoint(fromHuman.x(), fromHuman.y()),
              QPoint(move.second, move.first),
              pos.board[fromHuman.x()][fromHuman.y()].piece->color))) {*/
      //std::cout << "before" << move.first << move.second;
      //std::cout << fromHuman.x() << fromHuman.y();
      //std::cout << toHuman.x() << toHuman.y() << std::endl;

      //if ((move.first == toHuman.x()) && (move.second == toHuman.y())) {
        //std::cout << move.first << move.second << std::endl;
        //if (basemodel.kind.contains("uci")) {
        //  emit updateView({fromHuman.x(), fromHuman.y()},
        //                  {toHuman.x(), toHuman.y()}, "uci");
        //} else {
          emit updateView({fromHuman.x(), fromHuman.y()},
                          {toHuman.x(), toHuman.y()}, "human");
        //}
        //repaint();
        //return;
     // } else {
     //   fromHuman = {-1, -1};
     //   break;
     // }
    //  i++;
    //}
    //fromHuman = {-1, -1};
 // }

  }
  repaint();
}

// TODO: exchange hanzi with unicode [ok]
//  Sets the selected pieces on the (clean) board
void BoardView::SetEditorPieces() {
  // qDebug() << __PRETTY_FUNCTION__;
  QPoint coords;
  for (auto piece : contextMenu->pieces) {
    coords = CalcBoardCoords(piece.first);
    QPoint tmp = coords;
    coords.setX(10 - tmp.y());
    coords.setY(tmp.x() - 1);
    switch (piece.second) {
      /*       case completePieceType::GeneralRot:
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
           break;*/
    default:
      qDebug() << "Error in boarview::SetEditorPieces";
      break;
    }
  }
}

QPoint BoardView::CalcBoardCoords(QPoint r) {
  // qDebug() << __PRETTY_FUNCTION__;
  float w = width();
  float h = height();
  QPoint p = r;

  float x = p.x();
  float y = p.y();

  // qDebug() << x << y << "x und y";

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
