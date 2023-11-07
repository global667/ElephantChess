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
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QTableView>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "aboutview.h"
#include "basemodel.h"
#include "boardview.h"
#include "engine.h"
#include "genmove.h"
#include "renderview.h"
#include "settingsview.h"
#include "types.h"
#include "uci.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // The engine, and its thread
    UCI *uci;
    QThread uciThread;
    Engine *engine;

private:
    void InitWidgets();
    void InitConnections();
    void InitEngine();

    BoardView *boardview;

    QWidget *view;
    Qt3DExtras::Qt3DWindow *renderView;

    QTabWidget *tabview;
    QWidget *tabwidget1;
    QWidget *tabwidget2;
    QWidget *menu;

    //QTableView *table;
    QTreeWidget *table;

    QToolBar *toolbar;
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

    SettingsView *settings;

    AboutView *about;

    //int row = 0,
    int column = 0;
    void AddMoveToList(QString p);
    void AddMoveToHistory();

    QPushButton button;

    int isTableClicked = 0;

    void ResetToHistory();
    QFile *LoadPGNFile();
    void PutPGNOnBoard();
    void ReadPGNData(QString data);
    QTextEdit *loggingTextView;
public slots:
    void Open();
    void Save();
    void OpenSettings();
    void ToggleEngineStatus();
    void Newgame();
    void PlayNow();
    void ToggleGameView();
    //void togglePlayer();
    void GiveTipp();
    void About();
    void Help();
    void GiveUpGame();

    void LleftPressed();
    void LeftPressed();
    void RightPressed();
    void RrightPressed();

    void UpdateSettings();
    void ItemClicked(QTreeWidgetItem *, int);

public slots:
    void ToMove(QPoint from, QPoint to, QString kind);
private slots:
    void Debug();
};
#endif // MAINWINDOW_H
