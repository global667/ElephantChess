#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "boardview.h"
//#include "boardmodel.h"
//#include "movegen.h"

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
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTabWidget>
#include <QTableView>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "basemodel.h"

static BaseModel basemodel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

public slots:
    void open();
    void save();
    void settings();
    void enginestarts();
    void newgame();

    void lleftPressed();
    void leftPressed();
    void rightPressed();
    void rrightPressed();
};
#endif // MAINWINDOW_H
