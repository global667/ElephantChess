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
//#include <QtQuick/QQuickView>
//#include <QtQuick3D/qquick3d.h>
//#include <QQmlApplicationEngine>
#include <QtConcurrent>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>

#include "aboutview.h"
#include "basemodel.h"
#include "boardview.h"
#include "engine.h"
//#include "renderview.h"
//#include "settingsview.h"
#include "uci.h"

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // The engine, and its thread
    QScopedPointer<UCI> uci;
    QThread uciThread;
    QScopedPointer<Engine> engine;
    void YouLose();
    void YouWin();
    QTimer *timer{}, *timer2{};

private:
    void InitWidgets();
    void InitConnections();
    void InitEngine();

    BoardView *boardview = nullptr;

    //QQuickView *viewQml = nullptr;

    QWidget *view = nullptr;
    //Qt3DExtras::Qt3DWindow *renderView = nullptr;

    QTabWidget *tabview = nullptr;
    QWidget *tabwidget1 = nullptr;
    QWidget *tabwidget2 = nullptr;
    QWidget *menu = nullptr;

    //QTableView *table;
    QTreeWidget *table = nullptr;

    QToolBar *toolbar = nullptr;
    QAction *openbutton = nullptr, *savebutton = nullptr, *settingsbutton = nullptr, *enginestartsbutton = nullptr, *exitbutton = nullptr,
        *newgamebutton = nullptr;
    QMenuBar *menubar = nullptr;
    QMenu *menu1 = nullptr;
    QDockWidget *dockWidget = nullptr;
    QStandardItemModel *model = nullptr;
    QHeaderView *headerview = nullptr;
    QWidget *navigationwidget = nullptr;
    QWidget *navigationview = nullptr;
    QPushButton *lleft = nullptr, *left = nullptr, *right = nullptr, *rright = nullptr;


    QWidget *gameinfoswidget = nullptr;
    QVBoxLayout *gameinfosh = nullptr;
    QHBoxLayout *opponents = nullptr;
    QLineEdit *opp1 = nullptr, *opp2 = nullptr, *loca = nullptr, *round = nullptr, *date = nullptr;
    QHBoxLayout *location = nullptr;

    //SettingsView *settings = nullptr;

    AboutView *about = nullptr;

    //int row = 0,
    int column = 0;
    void AddMoveToList(std::pair<Point, Point> move) const;

    static void AddMoveToHistory();

    QPushButton button;

    int isTableClicked = 0;
    bool tipp = false;

    void ResetToHistory();
    QFile *LoadPGNFile();
    void PutPGNOnBoard();
    void ReadPGNData(QString data) const;
    QTextEdit *loggingTextView = nullptr;
    QLineEdit *nps = nullptr, *eval = nullptr;
    QHBoxLayout *tab1layout = nullptr, *navibuttonslayout = nullptr;
    QVBoxLayout *tabwidget2layout = nullptr, *naviwidlayout = nullptr;

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
    void engineTipp(Point from,Point to);
    void About();
    void Help() const;
    void GiveUpGame();

    //void UpdateSettings();
    static void ItemClicked(QTreeWidgetItem *, int);

    void nodesPerSecond() const;

public slots:
    void PlayNextTwoMoves(Point from, Point to, const BaseModel::Mode mode);
    void paintFromThreadSlot();
    void updateFromThreadSlot() const;
private slots:
    void onDownloaded(const QString& filename = {});
    void Debug() const;
public: signals:
    void paintFromThread();

};
#endif // MAINWINDOW_H
