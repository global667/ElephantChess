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
#include <QVBoxLayout>
#include <QWidget>

#include "basemodel.h"
#include "boardview.h"
#include "settingsview.h"
#include "uci.h"

//extern BaseModel basemodel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // The engine, as exe and its thread
    UCI uci;
    QThread uciThread;

private:
    BoardView *boardview;
    QTabWidget *tabview;
    QWidget *tabwidget1;
    QWidget *tabwidget2;
    QWidget *menu;
    QTableView *table;
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

public slots:
    void open();
    void save();
    void settings();
    void toggleEngineStatus();
    void newgame();
    void exit();
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

    void boardStyleChanged();
public slots:
    void game(int fromX, int fromY, int toX, int toY, int sender);
};
#endif // MAINWINDOW_H
