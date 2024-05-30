//
// Created by wolfk on 05.05.2024.
//

#include <QByteArray>
#include <QList>
#include <QMessageBox>
#include <QStringList>
#include <QTreeWidgetItem>

#include "game.h"
#include "engine.h"
#include "basemodel.h"
#include "uci.h"

#include "mainwindow.h"

extern BaseModel basemodel;

Game::Game(QTreeWidget *table, MainWindow *parent)
    : table(table), parent(parent) {}

// Play the next two moves begin with red as human and black as engine
void Game::run() {
    // not used at the moment
    // basemodel.currentMoves.push_back({from, to});
    do {
        /*    Point from; Point to; const BaseModel::Mode mode = basemodel.mode;
        std::pair<Point, Point> move = std::make_pair(from, to);

        if (from.x == -1 && from.y == -1 && to.x == -1 && to.y == -1) {
            YouLose();
            return;
        }
        if (!basemodel.position.board[from.x][from.y]) {
            qDebug() << "Error in ToMove" << from.x << " " << from.y;
            return;
        }

        //TODO: Fix PlayNow bug

        int j=basemodel.currentMove;
        int l = basemodel.moveHistory.size();
        bool isBackMove = (j < l-1);
        if (isBackMove) {
            table->clear();
            basemodel.moves.clear();

            for (int i = j; i < l-1 ; i++) {
                //table->takeTopLevelItem(i);
                basemodel.moveHistory.removeLast();
            }
            //basemodel.currentMove = j;
            basemodel.position = basemodel.moveHistory.last();
            parent->update();
        } */

    if (basemodel.position.players_color == Color::Red &&
            basemodel.fromHuman.x != -1 && basemodel.fromHuman.y != -1 &&
            basemodel.toHuman.x != -1 && basemodel.toHuman.y != -1) {
            mutex.lock();
        auto move = std::make_pair(basemodel.fromHuman, basemodel.toHuman);
            Board::movePiece({move.first.x, move.first.y},
                         {move.second.x, move.second.y},
                         basemodel.position.board);
        AddMoveToList(move);
            // AddMoveToHistory();

        // if (!isBackMove)
        basemodel.fromHuman = {-1, -1};
        basemodel.toHuman = {-1, -1};
        // isMouseClicked = false;
        basemodel.position.toggleColor();
        // parent->repaint();
        parent->uci->engineGo(false);
        parent->update();
        mutex.unlock();
    }

    if (basemodel.position.players_color == Color::Black &&
        basemodel.fromUCI.x != -1 && basemodel.fromUCI.y != -1 &&
        basemodel.toUCI.x != -1 && basemodel.toUCI.y != -1) {
        //mutex.lock();
        auto move = std::make_pair(basemodel.fromUCI, basemodel.toUCI);
        Board::movePiece({move.first.x, move.first.y},
                         {move.second.x, move.second.y},
                         basemodel.position.board);
        AddMoveToList(move);
        // AddMoveToHistory();

        // if (!isBackMove)
        basemodel.fromUCI = {-1, -1};
        basemodel.toUCI = {-1, -1};
        // isMouseClicked = false;
        basemodel.position.toggleColor();
        parent->update();
        //mutex.unlock();
    }

    /*     if (mode == BaseModel::Mode::engine) {
             /*   timer = new QTimer(this);
                timer2 = new QTimer(this);
                connect(timer, &QTimer::timeout, engine.get(),
       &Engine::nodesPerSecond); connect(timer2, &QTimer::timeout, this,
       &MainWindow::NodesPerSecond);

                timer->start(1000);
                timer2->start(100);

                QObject::connect(this, SIGNAL(paintFromThread()), this,
       SLOT(paintFromThreadSlot())); QObject::connect(engine.get(),
       SIGNAL(updateFromThread()), this, SLOT(updateFromThreadSlot()));


                // Putting in a separeted thread
                auto future = QtConcurrent::run([this]() {
                    std::pair<Point, Point> pair = engine->engineGo();

                    basemodel.fromUCI = pair.first;
                    basemodel.toUCI = pair.second;


                    Board::movePiece({pair.first.x, pair.first.y},
                                     {pair.second.x, pair.second.y},
       basemodel.position.board); AddMoveToList(pair); AddMoveToHistory();

                    emit PaintFromThread();

                    basemodel.position.toggleColor();
                });
         } else if (mode == BaseModel::Mode::uci) {
             //uci->engineGo(false);
         } else if (mode == BaseModel::Mode::human) {
             basemodel.position.players_color = Color::Red;
             qDebug() << "Waiting for human...";
         } else {
             qDebug() << "Error in game loop ToMove()";
         }
         parent->repaint();*/
    } while (true);
}

void Game::AddMoveToHistory() {
    basemodel.currentMove++;
    basemodel.moveHistory.append(basemodel.position);
}

void Game::AddMoveToList(const std::pair<Point, Point> move) {
    if (move.first.x == -1 || basemodel.moveHistory.isEmpty())
        return;
      const QString name =
            basemodel.moveHistory.last().board[move.first.x][move.first.y]->getName();
    const QString beaten =
    basemodel.moveHistory.last().board[move.second.x][move.second.y]->getName().isEmpty()
                               ? "-"
                               : "x"; // If there is no piece on the target field, then it is not a beaten move

    const QByteArray moveAsString = BaseModel::posToken(
        move.first.x, move.first.y, move.second.x, move.second.y);
    basemodel.moves.append(moveAsString);

      QStringList mv;
    mv << name << QString(basemodel.moves.last().at(0))
            << QString(basemodel.moves.last().at(1)) << beaten
            << QString(basemodel.moves.last().at(2))
            << QString(basemodel.moves.last().at(3))
            << (!Board::isCheck(basemodel.position.players_color,
    basemodel.position.board) ? QString("") : QString("+"));

    auto *item = new QTreeWidgetItem(table);
    item->setText(0, QString::number(basemodel.currentMove+1) + ". " + mv.join(" "));
    table->addTopLevelItem(item);

    AddMoveToHistory();
    // auto *item = new QTreeWidgetItem(table);
    // const int ply = (basemodel.moves.size() - 1) % 2;
    // item->setText(0, QString::number(basemodel.currentMove) + ". " + mv.join("
    // "));

    /* if (isTableClicked) {
       if (0 == ply) {
           table->addTopLevelItem(item);
       } else {
           table->setCurrentItem(item);
           //table->addTopLevelItem(item); // Insert the item at the top
       }
   } else { */
    // table->addTopLevelItem(item);
    // }
}
// You win message
void Game::YouWin() {
    QMessageBox msgBox;
    msgBox.setText("The game has finished: Red won.");
    msgBox.setInformativeText("Do you want to save game");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    switch (msgBox.exec()) {
    case QMessageBox::Save:
        // Save was clicked
        parent->Save();
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        // Newgame();
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

// You lose message
void Game::YouLose() {
    QMessageBox msgBox;
    msgBox.setText("The game has finished: Red lost.");
    msgBox.setInformativeText("Do you want to save game");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    switch (const int ret = msgBox.exec()) {
    case QMessageBox::Save:
        // Save was clicked
        parent->Save();
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        parent->Newgame();
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

// void MainWindow::ItemClicked(QTreeWidgetItem *item, int column) {
//  qDebug() << "item clicked";
//      int row = table->indexFromItem(item).row();
//      basemodel.board = basemodel.moveHistory[row];
//      isTableClicked = row;
//      repaint();
// }
