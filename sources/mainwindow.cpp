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

#include "mainwindow.h"

//#ifdef TEST
//#include "config.h"
//#endif
#include "genmove.h"

extern BaseModel basemodel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 900);
    setWindowIcon(QIcon(":/res/generalRed.png"));
//#ifdef TEST
//    setWindowTitle("ElephantChess v"
//                   + QString("%1.%2").arg(ElephantChess_VERSION_MAJOR).arg(ElephantChess_VERSION_MINOR));
//#else
    setWindowTitle("ElephantChess");
    //#endif

    // mainwidgets
    boardview = new BoardView(this);
    setCentralWidget(boardview);

    tabview = new QTabWidget(this);
    tabwidget1 = new QWidget(tabview);
    tabwidget2 = new QWidget(tabview);
    menubar = new QMenuBar(this);
    menu1 = new QMenu(menubar);
    //table = new QTableView(tabwidget1);
    navigationwidget = new QWidget(this);
    navigationview = new QWidget(navigationwidget);

    // navigation buttons
    right = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), "");
    rright = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), "");
    left = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward)), "");
    lleft = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward)), "");
    connect(lleft, SIGNAL(pressed()), SLOT(lleftPressed()));
    connect(left, SIGNAL(pressed()), SLOT(leftPressed()));
    connect(right, SIGNAL(pressed()), SLOT(rightPressed()));
    connect(rright, SIGNAL(pressed()), SLOT(rrightPressed()));

    // move listing
    model = new QStandardItemModel(0, 2);
    model->setHeaderData(0, Qt::Horizontal, tr("Red"));
    model->setHeaderData(1, Qt::Horizontal, tr("Black"));
    headerview = new QHeaderView(Qt::Horizontal);
    headerview->resizeSections(QHeaderView::Stretch);
    headerview->setStretchLastSection(true);
    headerview->setDefaultAlignment(Qt::AlignJustify | Qt::AlignVCenter);
    table = new QTreeWidget();
    table->setColumnCount(1);
    table->setHeaderLabels(QStringList() << "Moves");

    /*   // menu buttons
    openbutton = new QAction(QIcon(":/res/icons/open.png"), tr("Laden"), menubar);
    //openbutton->setIcon();
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(openbutton, &QAction::triggered, this, &MainWindow::open);

    savebutton = new QAction(tr("Speichern"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(savebutton, &QAction::triggered, this, &MainWindow::save);

    enginestartsbutton = new QAction(tr("Enginestatus wechseln"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(enginestartsbutton, &QAction::triggered, this, &MainWindow::toggleEngineStatus);

    settingsbutton = new QAction(tr("Optionen"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(settingsbutton, &QAction::triggered, this, &MainWindow::settings);

    newgamebutton = new QAction(tr("Neues Spiel"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(newgamebutton, &QAction::triggered, this, &MainWindow::newgame);

    exitbutton = new QAction(tr("Exit"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(exitbutton, &QAction::triggered, this, &MainWindow::close);

    setMenuBar(menubar);
    menu1 = menuBar()->addMenu("Datei");
    menu1->addAction(openbutton);
    menu1->addAction(savebutton);
    menu1->addAction(settingsbutton);
    menu1->addAction(enginestartsbutton);
    menu1->addAction(newgamebutton);
    menu1->addAction(exitbutton);
*/
    // tabwidget1 layout
    QHBoxLayout *tab1layout = new QHBoxLayout;
    tab1layout->addWidget(table);
    tabwidget1->setLayout(tab1layout);

    // tabwidget2 layout

    // tabview
    tabview->addTab(tabwidget1, "Move list");
    tabview->addTab(tabwidget2, "Info");

    // navgationview
    QHBoxLayout *navibuttonslayout = new QHBoxLayout;
    navibuttonslayout->addWidget(lleft);
    navibuttonslayout->addWidget(left);
    navibuttonslayout->addWidget(right);
    navibuttonslayout->addWidget(rright);
    navigationview->setLayout(navibuttonslayout);

    gameinfoswidget = new QWidget();
    gameinfosh = new QVBoxLayout;
    opponents = new QHBoxLayout;
    opp1 = new QLineEdit();
    opp1->setPlaceholderText("Player One");
    opponents->addWidget(opp1);
    opp2 = new QLineEdit();
    opp2->setPlaceholderText(basemodel.engineName);
    opponents->addWidget(opp2);

    // TODO: set the placeholder text with setPlaceholderText(...); [ok]
    location = new QHBoxLayout;
    loca = new QLineEdit();
    loca->setPlaceholderText("beijin masters");
    location->addWidget(loca);
    location->addWidget(new QLabel(", round"));
    round = new QLineEdit();
    round->setPlaceholderText("8");
    round->setMaximumWidth(30);
    location->addWidget(round);
    location->addWidget(new QLabel(", the"));
    date = new QLineEdit();
    date->setPlaceholderText("20.02.2000");
    date->setMaximumWidth(60);
    location->addWidget(date);

    gameinfosh->addLayout(opponents);
    gameinfosh->addLayout(location);

    gameinfoswidget->setLayout(gameinfosh);

    QVBoxLayout *naviwidlayout = new QVBoxLayout;
    naviwidlayout->addWidget(navigationview);
    naviwidlayout->addWidget(gameinfoswidget);
    naviwidlayout->addWidget(tabview);

    navigationwidget->setLayout(naviwidlayout);

    dockWidget = new QDockWidget(tr("Dock"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setWidget(navigationwidget);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    // Add toolbar
    toolbar = new QToolBar(this);
    toolbar->setObjectName("toolbar");
    toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setBackgroundRole(QPalette::Light);
    toolbar->setForegroundRole(QPalette::Light);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(Qt::TopToolBarArea, toolbar);
    //QIcon icon = style()->standardIcon(QStyle::SP_ArrowBack);

    //QAction *new_action = new QAction();
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogApplyButton)),
                       tr("New game"),
                       this,
                       SLOT(newgame()));
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)),
                       tr("Open"),
                       this,
                       SLOT(open()));
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)),
                       tr("Save"),
                       this,
                       SLOT(save()));
    toolbar->addSeparator();

    //    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
    //                       tr("Toggle player"),
    //                       this,
    //                       SLOT(togglePlayer()));

    toolbar->addAction(QIcon(":res/play-now.png"), tr("Play now!"), this, SLOT(playNow()));

    toolbar->addSeparator();

    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
                       tr("Toggle view"),
                       this,
                       SLOT(toggleGameView()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxInformation))),
                       tr("Tipp"),
                       this,
                       SLOT(giveTipp()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxCritical))),
                       tr("Give up"),
                       this,
                       SLOT(giveUpGame()));
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":res/settings.png"), //style()->standardIcon(QStyle::SP_ComputerIcon)),
                       tr("Options"),
                       this,
                       SLOT(settings()));
    toolbar->addAction(QIcon(style()->standardIcon(
                           (QStyle::SP_DialogHelpButton))), //SP_TitleBarContextHelpButton))),
                       tr("Help"),
                       this,
                       SLOT(Help()));
    QAction *about_action = new QAction(QIcon(
                                            style()->standardIcon((QStyle::SP_TitleBarMenuButton))),
                                        tr("About"),
                                        this);
    toolbar->addAction(about_action);
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                       tr("Close"),
                       QCoreApplication::instance(),
                       &QCoreApplication::quit);

    dialog = new SettingsView(this);
    dialog->SetModel(&basemodel);

    statusBar()->showMessage(tr("Ready"));
    //Q_ASSERT(&uci);
    //Q_ASSERT(&uciThread);
    //uci.moveToThread(&uciThread);
    //qDebug() << "Starting uci engine in extra thread";
    //uci.start();
    //uciThread.start();

    connect(&engine,
            SIGNAL(updateView(position, position, QString)),
            SLOT(ToMove(position, position, QString)));
    connect(boardview,
            SIGNAL(updateView(position, position, QString)),
            SLOT(ToMove(position, position, QString)));

    //connect(&engine, SIGNAL(updateView(Players)), SLOT(redToMove(Players)));
    //connect(boardview, SIGNAL(updateView(Players)), SLOT(redToMove(Players)));

    connect(dialog, SIGNAL(boardStyleChanged()), this, SLOT(newgame()));

    connect(table,
            SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            SLOT(itemClicked(QTreeWidgetItem *, int)));
}

void MainWindow::itemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "item clicked";
    int row = table->indexFromItem(item).row();
    basemodel.board = basemodel.moveHistory[row];
    isTableClicked = row;
    repaint();
}

void MainWindow::giveUpGame()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::toggleGameView()
{
    if (basemodel.gameView == color::Red) {
        basemodel.gameView = color::Black;
        statusBar()->showMessage("Black on bottom now");
    } else {
        basemodel.gameView = color::Red;
        statusBar()->showMessage("Red on bottom now");
    }
    repaint();
}

//void MainWindow::togglePlayer()
//{
//    //disconnect(&uci, SIGNAL(updateView(Position, Position)), nullptr, nullptr);
//    //disconnect(boardview, SIGNAL(updateView(Position, Position)), nullptr, nullptr);

//    if (basemodel.humanColor == color::Red) {
//        basemodel.humanColor = color::Black;

//        /* connect(&uci, SIGNAL(updateView(Position, Position)), SLOT(redToMove(Position, Position)));
//        connect(boardview,
//                SIGNAL(updateView(Position, Position)),
//                SLOT(blackToMove(Position, Position)));
//*/
//        engine.engineGo();
//    }

//    else {
//        basemodel.humanColor = color::Red;
//        /*
//        connect(&uci, SIGNAL(updateView(Position, Position)), SLOT(blackToMove(Position, Position)));
//        connect(boardview,
//                SIGNAL(updateView(Position, Position)),
//                SLOT(redToMove(Position, Position)));

//*/
//    }
//}

void MainWindow::giveTipp()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}
void MainWindow::About()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}
void MainWindow::Help()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::playNow()
{
    /*if (disconnect(&engine, SIGNAL(updateView(position, position)), nullptr, nullptr)
        && disconnect(boardview, SIGNAL(updateView(position, position)), nullptr, nullptr)) {
        if (basemodel.board.onMove == color::Black) {
            connect(boardview,
                    SIGNAL(updateView(position, position)),
                    SLOT(blackToMove(position, position)));
            connect(&engine,
                    SIGNAL(updateView(position, position)),
                    SLOT(redToMove(position, position)));
        } else {
            connect(&engine,
                    SIGNAL(updateView(position, position)),
                    SLOT(blackToMove(position, position)));
            connect(boardview,
                    SIGNAL(updateView(position, position)),
                    SLOT(redToMove(position, position)));
        }

        engine.engineGo();
        //basemodel.board.toggleOnMove();
    } else {
        qDebug() << "Error in play Now !";
    }*/
    engine.engineGo();
}

void MainWindow::open()
{
    auto openFile = QFileDialog::getOpenFileName(this, tr("Open file"));
    statusBar()->showMessage(tr("Open file: ") + openFile);
    //QMessageBox::information(this, "Information", "Noch nicht implementiert");

    if (openFile.isNull()) {
        statusBar()->showMessage(tr("Open file error"));
        return;
    }
    QFile opfile(openFile);
    if (!opfile.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Open file error"));
        return;
    }

    statusBar()->showMessage(tr("Load file ") + openFile);

    QTextStream textstream(&opfile);
    auto str = textstream.readAll();
    for (auto &s : str.split('\n')) {
        if (s.contains("[Event ")) {
            loca->setText(s.split("\"").at(1));
        } else {
            if (s.contains("Round")) {
                round->setText(s.split("\"").at(1));
            } else {
                if (s.contains("White")) {
                    opp1->setText(s.split("\"").at(1));
                } else {
                    if (s.contains("Black")) {
                        opp2->setText(s.split("\"").at(1));
                    } else {
                        if (s.contains("Date")) {
                            date->setText(s.split("\"").at(1));
                        } else {
                            if (s.split(" ").first().contains("1.")) {
                                for (auto &s : s.split(" ")) {
                                    if (s.contains(".")) {
                                        basemodel.moves << s.split(".").last();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug() << basemodel.moves;

    opfile.close();
    int c = 0;
    basemodel.board.initBoard();
    update();

    QStandardItem *item;
    for (QString &item1 : basemodel.moves) {
        item = new QStandardItem(item1);
        if (c % 2 == 0)
            model->setItem(c / 2, 0, item);
        if (c % 2 == 1)
            model->setItem(c / 2, 1, item);
        c++;

        auto fx = ((char) item1.at(0).toLatin1()) - 'a';
        auto fy = 9 - (item1.at(1).digitValue());
        auto tx = ((char) item1.at(2).toLatin1()) - 'a';
        auto ty = 9 - (item1.at(3).digitValue());
        basemodel.board.movePiece(9 - fy, 8 - fx, 9 - ty, 8 - tx);
    }
    column = c;
    update();
}

void MainWindow::save()
{
    auto saveFile = QFileDialog::getSaveFileName(this,
                                                 tr("Save file "),
                                                 "~/untitled",
                                                 tr("Portable Game Notation ( *.pgn)"));

    if (saveFile.isNull()) {
        statusBar()->showMessage(tr("Speichere Datei: fehlgeschlagen"));
        return;
    }
    if (!saveFile.contains(".pgn"))
        saveFile = saveFile + ".pgn";
    QFile svfile(saveFile);
    if (!svfile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Save file: opening error"));
        return;
    }

    QTextStream textstream(&svfile);

    textstream << "[Event \"" << loca->text() << "\"]\n";
    textstream << "[Date \"" << date->text() << "\"]\n";
    textstream << "[Round \"" << round->text() << "\"]\n";
    textstream << "[White \"" << opp1->text() << "\"]\n";
    textstream << "[Black \"" << opp2->text() << "\"]\n";
    textstream << "[Result \""
               << "0-1"
               << "\"]\n\n";

    int i = 0;
    for (const auto &m : basemodel.moves) {
        if (++i % 2 == 1)
            textstream << QString("%1.").arg((i / 2) + 1);
        textstream << m << " ";
    }

    textstream << "0-1\n\n";

    svfile.close();

    statusBar()->showMessage(tr("Save file: ") + saveFile);
}

void MainWindow::settings()
{
    dialog->setModal(true);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(updateSettings()));
    dialog->open();
}

void MainWindow::updateSettings()
{
    //uciThread.quit();
    //uciThread.wait();
    //uci.engineName =
    //basemodel.engine = dialog->engineName;
    //Q_ASSERT(&uci);
    //Q_ASSERT(&uciThread);
    //uci.moveToThread(&uciThread);
    qDebug() << "Starting uci engine (" + basemodel.engineName + ") in extra thread";
    //uci.start();
    //uciThread.start();
    opp2->setPlaceholderText(basemodel.engineName);
}

void MainWindow::toggleEngineStatus()
{
    //    if (uciThread.isRunning()) {
    //        uciThread.quit();
    //    } else {
    //        uciThread.start();
    //    }
}

// Startet ein neues Spiel
void MainWindow::newgame()
{
    basemodel.board.initBoard();
    basemodel.moveHistory.clear();
    model->clear();
    row = 0, column = 0;
    basemodel.currentMove = 0;
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    basemodel.board.onMove = color::Red;
    for (int i = basemodel.moves.size(); i >= 0; i--) {
        table->takeTopLevelItem(i);
    }
    basemodel.moves.clear();
    basemodel.moveHistory.append(basemodel.board);
    basemodel.currentMove++;
    repaint();
}

void MainWindow::lleftPressed()
{
    basemodel.currentMove = 0;
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    //if (uciThread.isRunning()) {
    //    uciThread.quit();
    //}
    repaint();
}

void MainWindow::leftPressed()
{
    basemodel.currentMove--;
    if (basemodel.currentMove <= 0) {
        basemodel.currentMove = 0;
    }
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    //if (uciThread.isRunning()) {
    //    uciThread.quit();
    //}
    repaint();
}

void MainWindow::rightPressed()
{
    basemodel.currentMove++;
    if (basemodel.currentMove >= basemodel.moveHistory.size() - 1) {
        basemodel.currentMove = basemodel.moveHistory.size() - 1;
    }
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    //if (uciThread.isRunning()) {
    //    uciThread.quit();
    //}

    repaint();
}

void MainWindow::rrightPressed()
{
    basemodel.currentMove = basemodel.moveHistory.size() - 1;
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    //if (uciThread.isRunning()) {
    //    uciThread.quit();
    //}
    repaint();
}

void MainWindow::ToMove(position from, position to, QString kind)
{
    qDebug() << "ToMove";
    basemodel.currentMoves.push_back({from, to});
    //if (basemodel.board.onMove == color::Red) {
    // qDebug() << "onMove == Red";
    if (kind.contains("human")) {
        GenMove isMate(basemodel.board.pieces, basemodel.board.onMove);

        // Is in Check?
        if (isMate.IsCheck(basemodel.board.onMove)) {
            qDebug() << "Check";
            statusBar()->showMessage("Check");
            //return;
        }

        // Is in Checkmate?
        if (isMate.IsCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
            statusBar()->showMessage("Checkmate");
            //return;
        }

        // Give move to engine
        engine.MovePiece(from, to);
        addMoveToList();
        addMoveToHistory();
        basemodel.board.toggleOnMove();
        //basemodel.board.onMove = color::Black;
        engine.engineGo();
    } else if (kind.contains("engine")) {
        //qDebug() << "onMove == Black";
        basemodel.board.movePiece(from.row, from.col, to.row, to.col);
        //boardview->MovePiece(from, to);
        basemodel.fromUCI = from;
        basemodel.toUCI = to;
        basemodel.board.toggleOnMove();
        addMoveToHistory();
        addMoveToList();
    } else {
        qDebug() << "Error in game loop ToMove()";
    }
        repaint();
}
/*
void MainWindow::blackToMove(position from, position to)
{
    qDebug() << "blackToMove";

    //if (basemodel.board.onMove == color::Black) {
    //qDebug() << "onMove == Black";

    basemodel.board.movePiece(from.row, from.col, to.row, to.col);
    //boardview->MovePiece(from, to);
    basemodel.fromHuman = from;
    basemodel.toHuman = to;
    basemodel.fromUCI = from;
    basemodel.toUCI = to;
    //basemodel.board.toggleOnMove();
    basemodel.board.onMove = color::Red;
    addMoveToHistory();
    addMoveToList();
    /*} else {
        qDebug() << "onMove == Red";

        GenMove isMate(basemodel.board.pieces, basemodel.board.onMove);

        // Is in Check?
        if (isMate.IsCheck(basemodel.board.onMove)) {
            qDebug() << "Check";
            statusBar()->showMessage("Check");
            return;
        }

        // Is in Checkmate?
        if (isMate.IsCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
            statusBar()->showMessage("Checkmate");
            return;
        }

        // Give move to engine
        engine.MovePiece(from, to);
        addMoveToList();
        addMoveToHistory();
        basemodel.board.toggleOnMove();
        engine.engineGo();
    }
    repaint();
}
*/
void MainWindow::addMoveToHistory()
{
    basemodel.moveHistory.append(basemodel.board);
    basemodel.currentMove++;
}

void MainWindow::addMoveToList()
{
    QString mv = QString(basemodel.moves.last());

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, mv);
    if (isTableClicked) {
            //qDebug() << isTableClicked << "isTableClicked";
            //qDebug() << table->topLevelItemCount() << "table->topLevelItemCount()";
            if (table->topLevelItemCount() == isTableClicked + 1) {
            table->addTopLevelItem(item);
            } else {
            table->currentItem()->addChild(item);
            }
    } else {
            table->addTopLevelItem(item);
    }
    /*    if (column % 2 == 0) {
        model->setItem(column / 2, 0, item);
    }
    if (column % 2 == 1) {
        model->setItem(column / 2, 1, item);
    }
*/
    item = nullptr;
    mv = nullptr;
    column++;
    //delete item;
}

MainWindow::~MainWindow()
{
    //uciThread.quit();
    //uciThread.wait();
}
