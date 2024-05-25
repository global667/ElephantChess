//
// Created by wolfk on 05.05.2024.
//

#ifndef GAME_H
#define GAME_H

#include <QThread>
#include <QTreeWidget>

#include "basemodel.h"
#include "mainwindow.h"
#include "engine.h"
#include "uci.h"

class Game final : public QThread {

    Q_OBJECT;
public:
    explicit Game(QTreeWidget *table,  MainWindow *parent = nullptr);

    void run() override;

private:
    bool isMouseClicked = false;
    QMutex mutex;

    void YouLose();

    void YouWin();

    void AddMoveToList(std::pair<Point, Point> move);

    void AddMoveToHistory();

    void ResetToHistory();

    QTreeWidget *table;
    MainWindow *parent;

    QTimer *timer{}, *timer2{};
};



#endif //GAME_H
