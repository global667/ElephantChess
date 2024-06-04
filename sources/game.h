//
// Created by wolfk on 05.05.2024.
//

#ifndef GAME_H
#define GAME_H

#include <QMutex>
#include <QThread>
#include <QTreeWidget>

class MainWindow;
struct Point;

class Game final : public QThread {

    Q_OBJECT;

public:
    explicit Game(QTreeWidget *table, MainWindow *parent = nullptr);

    void run() override;
    void AddMoveToHistory();
    void AddMoveToList(std::pair<Point, Point> move);
    void ResetToHistory();

private:
    bool isMouseClicked = false;

    QMutex mutex;

    void YouLose();

    void YouWin();

    QTreeWidget *table;
    MainWindow *parent;

    QTimer *timer{}, *timer2{};
};

#endif // GAME_H
