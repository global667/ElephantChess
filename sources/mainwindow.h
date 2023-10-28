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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIODevice>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QStringList>
#include <QTabWidget>
#include <QTableView>
#include <QTextStream>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "basemodel.h"
#include "boardview.h"
#include "engine.h"
#include "settingsview.h"
//#include "uci.h"

//extern BaseModel basemodel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // The engine, as exe and its thread
    //UCI uci;
    //QThread uciThread;
    Engine engine;

private:
    BoardView *boardview;
    QTabWidget *tabview;
    QWidget *tabwidget1;
    QWidget *tabwidget2;
    QWidget *menu;

    //QTableView *table;
    QTreeWidget *table;

    QWidget *toolbar;
    QAction *openbutton, *savebutton, *settingsbutton, *enginestartsbutton, *exitbutton,
        *newgamebutton;
    QMenuBar *menubar;
    QMenu *menu1;
    QDockWidget *dockWidget;
    QStandardItemModel *model;
    QHeaderView *headerview;
    QWidget *navigationwidget;
    QWidget *navigationview;
    QPushButton *lleft, *left, *right, *rright;

    QWidget *gameinfoswidget;
    QVBoxLayout *gameinfosh;
    QHBoxLayout *opponents;
    QLineEdit *opp1, *opp2, *loca, *round, *date;
    QHBoxLayout *location;

    SettingsView *dialog;

    int row = 0, column = 0;
    void addMoveToList();
    void addMoveToHistory();

    QPushButton button;

    int isTableClicked = 0;

public slots:
    void open();
    void save();
    void settings();
    void toggleEngineStatus();
    void newgame();
    //void exit();
    void toggleGameView();
    void togglePlayer();
    void giveTipp();
    void About();
    void Help();
    void giveUpGame();

    void lleftPressed();
    void leftPressed();
    void rightPressed();
    void rrightPressed();

    void updateSettings();
    void itemClicked(QTreeWidgetItem *, int);

public slots:
    void redToMove(position from, position to);
    void blackToMove(position from, position to);
};
#endif // MAINWINDOW_H
