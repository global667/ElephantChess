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
// #define THREE_D_VIEW
// #ifdef TEST
// #endif
#include <QDesktopServices>
#include <QtQuick/QQuickView>
#include <QtQuick3D/qquick3d.h>
#include <QQmlApplicationEngine>

extern BaseModel basemodel;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
#ifdef THREE_D_VIEW
    //renderView = new RenderView();
    //renderView->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/3dBoard.qml")));
    //if (engine.rootObjects().isEmpty())
    //    return -1;

    viewQml = new QQuickView;
    viewQml->setSource(QUrl::fromLocalFile(":/3dBoard.qml"));
    view = QWidget::createWindowContainer(viewQml);//(renderView);
    setCentralWidget(view);
    //renderView->show();

#else
    boardview = new BoardView(this);
    setCentralWidget(boardview);
#endif
    InitEngine();
    InitWidgets();
    InitConnections();
    statusBar()->showMessage(tr("Ready"));

    loggingTextView->insertPlainText(QString("\nperft test\n") +
                                     QString("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1\n") +
                                     QString("depth       nodes    checks    captures\n") +
                                     QString("1          44         0           2\n") +
                                     QString("2        1920         6          72\n") +
                                     QString("3       79666       384        3159\n") +
                                     QString("4     3290240     19380      115365\n"));
    // 5   133312995    953251     4917734
    // 6  5392831844  39150745   200568035"

    //loggingTextView->insertPlainText(basemodel.position.perftTest(1));
    //loggingTextView->insertPlainText(basemodel.position.perftTest(2));
    //loggingTextView->insertPlainText(basemodel.position.perftTest(3));
    //loggingTextView->insertPlainText(basemodel.position.perftTest(4));
    //loggingTextView->insertPlainText(basemodel.position.perftTest(5));


}
#define ENGINE
void MainWindow::InitEngine() {
#ifdef ENGINE
    engine = new Engine();
    basemodel.kind = "engine";
    connect(engine, SIGNAL(updateView(Point, Point, QString)),
            SLOT(PlayNextTwoMoves(Point, Point, QString)));
#else
    basemodel.kind = "uci";
    uci = new UCI();
    connect(uci, SIGNAL(updateView(Point, Point, QString)),
            SLOT(PlayNextTwoMoves(Point, Point, QString)));
    uci->start();
    uci->engine.waitForReadyRead();
#endif
}
void MainWindow::InitConnections() {
#ifndef THREE_D_VIEW
    connect(boardview, SIGNAL(updateView(Point, Point, QString)),
            SLOT(PlayNextTwoMoves(Point, Point, QString)));
#endif
    // TODO: let board unchanged (at the moment it will reseted)
    //connect(settings, SIGNAL(boardStyleChanged()), SLOT(Newgame()));

    connect(
        table, &QTreeWidget::itemClicked, this,
        [=](QTreeWidgetItem *item, int col) {
            int row = table->indexFromItem(item).row();
            basemodel.position = basemodel.moveHistory[row];
            isTableClicked = row;
            repaint();
        },
        Qt::AutoConnection);
    // SLOT(ItemClicked(QTreeWidgetItem *, int)));

    connect(
        lleft, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove = 0;
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        left, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove--;
            if (basemodel.currentMove <= 0) {
                basemodel.currentMove = 0;
            }
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        right, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove++;
            if (basemodel.currentMove >= basemodel.moveHistory.size() - 1) {
                basemodel.currentMove = basemodel.moveHistory.size() - 1;
            }
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        rright, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove = basemodel.moveHistory.size() - 1;
            ResetToHistory();
        },
        Qt::AutoConnection);
}

void MainWindow::InitWidgets() {
    //settings = new SettingsView();

    tabview = new QTabWidget(this);
    tabwidget1 = new QWidget(tabview);
    tabwidget2 = new QWidget(tabview);
    menubar = new QMenuBar(this);
    menu1 = new QMenu(menubar);
    // table = new QTableView(tabwidget1);
    navigationwidget = new QWidget(this);
    navigationview = new QWidget(navigationwidget);

    // navigation buttons
    right = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), "Forward");
    rright = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), "End");
    left = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward)), "Back");
    lleft = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward)), "Begin");
    right->setToolTip("Goes on move for");
    rright->setToolTip("Goes to the end of the game");
    left->setToolTip("Goes one move back");
    lleft->setToolTip("Goes to the begining of the game");

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

    // tabwidget1 layout
    QHBoxLayout *tab1layout = new QHBoxLayout;
    tab1layout->addWidget(table);
    tabwidget1->setLayout(tab1layout);

    // tabwidget2 layout
    loggingTextView = new QTextEdit();
    nps = new QLineEdit();
    nps->setText("0 nodes/s");
    nps->setReadOnly(true);
    eval = new QLineEdit();
    eval->setText("Eval: 0");

    QVBoxLayout *tabwidget2layout = new QVBoxLayout;
    tabwidget2layout->addWidget(eval);
    tabwidget2layout->addWidget(nps);
    tabwidget2layout->addWidget(loggingTextView);
    tabwidget2->setLayout(tabwidget2layout);

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

    // gameinfos
    gameinfoswidget = new QWidget();
    gameinfosh = new QVBoxLayout;
    opponents = new QHBoxLayout;
    opp1 = new QLineEdit();
    opp1->setPlaceholderText("Me");
    opponents->addWidget(opp1);
    opp2 = new QLineEdit();
    opp2->setText(basemodel.engineName);
    opponents->addWidget(opp2);
    location = new QHBoxLayout;
    loca = new QLineEdit();
    loca->setPlaceholderText("hometown masters");
    location->addWidget(loca);
    location->addWidget(new QLabel(", round"));
    round = new QLineEdit();
    round->setPlaceholderText("1");
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

    // hole Dock
    dockWidget = new QDockWidget(tr("Informations"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setWidget(navigationwidget);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    // Toolbar
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

    // TODO: Add shortcuts
    toolbar
        ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogApplyButton)),
                    tr("New game"), this, SLOT(Newgame()))
        ->setToolTip("Starts a new game with the given engine");
    toolbar
        ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)),
                    tr("Open"), this, SLOT(Open()))
        ->setToolTip("Open a PGN-file and load it");
    ;
    toolbar
        ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)),
                    tr("Save"), this, SLOT(Save()))
        ->setToolTip("Save a PGN-file");
    toolbar->addSeparator();

    toolbar
        ->addAction(QIcon(":res/play-now.png"), tr("Play now!"), this,
                    SLOT(PlayNow()))
        ->setToolTip(
            "Let's the engine makes a move now (changes the color you play)");

    toolbar->addSeparator();

    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
                    tr("Toggle view"), this, SLOT(ToggleGameView()))
        ->setToolTip("Set the color that's above to the bottom");
    toolbar->addAction(
        QIcon(style()->standardIcon((QStyle::SP_MessageBoxInformation))),
        tr("Tipp"), this, SLOT(GiveTipp()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxCritical))),
                    tr("Give up"), this, SLOT(GiveUpGame()))
        ->setToolTip("Give the game up. You will loose.");
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":res/settings.png"), tr("Load engine"), this,
                       SLOT(OpenSettings()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogHelpButton))),
                    tr("Help"), this, SLOT(Help()))
        ->setToolTip("Starts the help in a browser");
    toolbar->addAction(
        QIcon(style()->standardIcon((QStyle::SP_TitleBarMenuButton))),
        tr("About"), this, SLOT(About()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                    tr("Exit"), QCoreApplication::instance(),
                    &QCoreApplication::quit)
        ->setToolTip("Exit the application");
    toolbar->addSeparator();

    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                    tr("EvalDebug"), this, SLOT(Debug()))
        ->setToolTip("Evaluate and comment a position");
}

void MainWindow::Debug() { loggingTextView->insertPlainText("Test,test,test"); }

QFile *MainWindow::LoadPGNFile() {
    auto openFile =
        QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());

    statusBar()->showMessage(tr("Try to load file ") + openFile);

    if (openFile.isNull()) {
        statusBar()->showMessage(tr("Open file error"));
        return nullptr;
    }
    QFile *opfile = new QFile(openFile);
    if (!opfile->open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Open file error"));
        return nullptr;
    }
    statusBar()->showMessage(tr("Open file: ") + openFile);
    return opfile;
}

// TODO: Add data to history
// Toolbar slots
void MainWindow::Open() {
    QFile *opfile = LoadPGNFile();
    if (opfile == nullptr) {
        QMessageBox::critical(this, "Error", "Error while open file");
        return;
    }
    QTextStream textstream(opfile);

    ReadPGNData(textstream.readAll());

    qDebug() << basemodel.moves;

    opfile->close();

    PutPGNOnBoard();
}

void MainWindow::ReadPGNData(QString data) {
    data.shrink_to_fit();
    for (auto &s : data.split('\n')) {
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
                            if (s.contains("1.")) {  
                                s.shrink_to_fit();
                                
                                //s.remove("[\\d].");
                                QRegularExpression re(R"(\d?\d+\.+)");
                                for (auto &d : s.split(re)) {
                                    //d.remove(" ");
                                    for (auto &m : d.split(" "))
                                    {//(R"(\w\d\w\d)");
                                        m.shrink_to_fit();
                                        if (!m.isEmpty()) {
                                            basemodel.moves << m;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::PutPGNOnBoard() {
    int c = 0;
    basemodel.position.setupInitialPositions();
    basemodel.currentMove = 0;
    basemodel.moveHistory.clear();
    model->clear();
    // row = 0,
    column = 0;

    QStandardItem *item;
    for (QString &item1 : basemodel.moves) {
        item = new QStandardItem(item1);
        if (c % 2 == 0) {
            model->setItem(c / 2, 0, item);
        }
        if (c % 2 == 1)
            model->setItem(c / 2, 1, item);
        c++;

        auto fx = ((char)item1.at(0).toLatin1()) - 'a';
        auto fy = 9 - (item1.at(1).digitValue());
        auto tx = ((char)item1.at(2).toLatin1()) - 'a';
        auto ty = 9 - (item1.at(3).digitValue());
        basemodel.position.movePiece(Point(9 - fy, 8 - fx),Point(9 - ty, 8 - tx), basemodel.position.board);

        AddMoveToHistory();

        /*      QStringList mv;

        QString name = basemodel.position.board[8-fx][9-fy].piece->name;
        QString beaten;
        if (basemodel.position.board[8-tx][9-ty].piece->piece_type != PieceType::Empty)
            beaten = "x";
        else
            beaten = "-";

        mv << name << QString(basemodel.moves.last().at(0))
            << QString(basemodel.moves.last().at(1)) << beaten
            << QString(basemodel.moves.last().at(2))
            << QString(basemodel.moves.last().at(3))
            << (!basemodel.position.is_check(basemodel.position.players_color) ? QString("") : QString("+"));
*/
        AddMoveToList(std::make_pair(Point(9 - fy, 8 - fx),Point(9 - ty, 8 - tx)));
    }
    column = c;
    repaint();
}

void MainWindow::AddMoveToHistory() {


    basemodel.currentMove++;
    basemodel.moveHistory.append(basemodel.position);
}
void MainWindow::AddMoveToList(std::pair<Point, Point> move) {
    if (move.first.x == -1)
        return;
    QString name =
        basemodel.position.board[move.second.x][move.second.y]->getName();
    QString beaten;
    if (!basemodel.position.board[move.first.x][move.first.y]->getName().isEmpty())
        beaten = "x";
    else
        beaten = "-";


    QByteArray moveAsString = basemodel.posToken(
        move.first.x, move.first.y, move.second.x, move.second.y);
    basemodel.moves.append(moveAsString);

    QStringList mv;
    mv << name << QString(basemodel.moves.last().at(0))
       << QString(basemodel.moves.last().at(1)) << beaten
       << QString(basemodel.moves.last().at(2))
       << QString(basemodel.moves.last().at(3))
       << (!basemodel.position.isCheck(basemodel.position.players_color, basemodel.position.board) ? QString("") : QString("+"));

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, mv.join(""));
    if (isTableClicked) {
        if (table->topLevelItemCount() == isTableClicked + 1) {
            table->addTopLevelItem(item);
        }
        else {
            table->currentItem()->addChild(item);
        }
    }
    else {
        table->addTopLevelItem(item);
    }
    item = nullptr;
    // column++;
}

void MainWindow::Save() {
    auto saveFile =
        QFileDialog::getSaveFileName(this, tr("Save file "), "~/untitled",
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

void MainWindow::GiveUpGame() {
    QMessageBox::information(this, "Information",
                             "You have decided to give up, you lose...");
}

void MainWindow::ToggleGameView() {
    if (basemodel.gameView == Color::Red) {
        basemodel.gameView = Color::Black;
        statusBar()->showMessage("Black on bottom now");
    } else {
        basemodel.gameView = Color::Red;
        statusBar()->showMessage("Red on bottom now");
    }
    repaint();
}

void MainWindow::GiveTipp() {
    std::pair<Point, Point> move =
        engine->GetBestMove(basemodel.position.players_color);
    Point from = move.first;
    Point to = move.second;
    QString c;
    if (basemodel.position.players_color == Color::Red)
        c = "Red";
    else
        c = "Black";
    QString token = basemodel.posToken(from.x, from.y, to.x, to.y);
    QString bestMoveWouldBe =
        QString("The best move for %1 would be %2").arg(c).arg(token);
    QMessageBox::information(this, "Engine says:", bestMoveWouldBe);
}

void MainWindow::About() {
    about = new AboutView();
    about->show();
}

void MainWindow::Help() const {
    QDesktopServices::openUrl(
        QUrl("https://github.com/global667/ElephantChess/blob/main/README.md"));
    statusBar()->showMessage(tr("Have open URL in browser"));
}

void MainWindow::PlayNow() {
    if (basemodel.kind == "engine")
    {
        std::pair<Point, Point> move = engine->engineGo();//= std::make_pair(QPoint(1,1), QPoint(1,1));//

        basemodel.fromUCI = move.first;
        basemodel.toUCI = move.second;
        //basemodel.fromHuman = from;
        //basemodel.toHuman = to;

        if (move.first.x == -1) {
            YouWin();
        }

        basemodel.position.movePiece({move.first.x, move.first.y},
                                     {move.second.x, move.second.y}, basemodel.position.board);
        AddMoveToHistory();
        AddMoveToList(move);
        basemodel.position.toggleColor();

    }
    else
        uci->engineGo();

    repaint();
}

void MainWindow::OpenSettings() {
    // Bind new engine
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Choose engine",
                                                    QDir::homePath(),
                                                    nullptr);
    if (filename.isEmpty()) {
        filename = "built-in";
        basemodel.engineName = filename;
        return;
    }
    basemodel.engineName = filename;

    if (basemodel.engineName == "built-in") {
        if (!engine) {
            engine = new Engine();
            basemodel.kind = "engine";
            disconnect(uci, SIGNAL(updateView(position, position, QString)), nullptr,
                       nullptr);
            connect(engine, SIGNAL(updateView(position, position, QString)),
                    SLOT(PlayNextTwoMoves(position, position, QString)));
            opp2->setPlaceholderText(basemodel.engineName);
            if (!uci)
                delete uci;
        }
    } else {
        basemodel.kind = "uci";
        if (!uci) {
            qDebug() << "!uci";
            uci = new UCI();
            disconnect(engine, SIGNAL(updateView(Point, Point, QString)), nullptr,
                       nullptr);
            connect(uci, SIGNAL(updateView(Point, Point, QString)),
                    SLOT(PlayNextTwoMoves(Point, Point, QString)));
        } else {
            qDebug() << "uci";
            uciThread.quit();
            uci = new UCI();
            connect(uci, SIGNAL(updateView(Point, Point, QString)),
                    SLOT(PlayNextTwoMoves(Point, Point, QString)));
        }
        //opp2->setText(basemodel.engineName);
        Q_ASSERT(&uci);
        Q_ASSERT(&uciThread);
        // uci->moveToThread(&uciThread);
        qDebug() << "Starting uci engine (" + basemodel.engineName +
                        " in extra thread";
        // uciThread.start();
        uci->start();
        uci->engine.waitForReadyRead();
    }
}

// Startet ein neues Spiel
void MainWindow::Newgame() {
    basemodel.position.setupInitialPositions();
    basemodel.moveHistory.clear();
    model->clear();
    // row = 0,
    column = 0;
    basemodel.currentMove = 0;
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    basemodel.position.players_color = Color::Red;
    for (int i = basemodel.moves.size(); i >= 0; i--) {
        table->takeTopLevelItem(i);
    }
    basemodel.moves.clear();
    basemodel.moveHistory.append(basemodel.position);
    basemodel.currentMove++;
    repaint();
}

// End Toolbar slots

void MainWindow::ResetToHistory() {
    //basemodel.position = basemodel.moveHistory[basemodel.currentMove];
    basemodel.position = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};

    repaint();
}

void MainWindow::paintFromThreadSlot() {
    repaint();
    timer->stop();
    timer2->stop();
    nps->setText("0 nodes/s");

    if (basemodel.fromUCI.x == -1) {
        YouWin();
        return;
    }
}

void MainWindow::updateFromThreadSlot() {
    nps->setText(QString::number(basemodel.engineData.nodes) + " nodes/s\n with a depth of " + QString::number(basemodel.engineData.searchDepth));
    eval->setText(QString::number(basemodel.engineData.evaluation) + " eval\n for move " + basemodel.engineData.bestMove);
    opp2->setText(basemodel.engineData.engineName);
}

// Play the next two moves begin with red as human and black as engine
void MainWindow::PlayNextTwoMoves(Point from, Point to, QString kind) {
    // not used at the moment
    // basemodel.currentMoves.push_back({from, to});
    std::pair<Point, Point> move = std::make_pair(from, to);

    if (from.x == -1 && from.y == -1 && to.x == -1 && to.y == -1) {
        YouLose();
        return;
    }    
    if (!basemodel.position.board[from.x][from.y]) {
        qDebug() << "Error in ToMove" << from.x << " " << from.y;
        return;
    }
      
    basemodel.position.movePiece({move.first.x, move.first.y},
                                 {move.second.x, move.second.y}, basemodel.position.board);
    AddMoveToHistory();
    AddMoveToList(move);
    basemodel.position.toggleColor();
    repaint();

    if (kind.contains("engine")) {

        timer = new QTimer(this);
        timer2 = new QTimer(this);
        connect(timer, &QTimer::timeout, engine, &Engine::nodesPerSecond);
        connect(timer2, &QTimer::timeout, this, &MainWindow::nodesPerSecond);

        timer->start(1000);
        timer2->start(100);

        QObject::connect(this, SIGNAL(paintFromThread()), this, SLOT(paintFromThreadSlot()));
        QObject::connect(engine, SIGNAL(updateFromThread()), this, SLOT(updateFromThreadSlot()));


        // Putting in a separeted thread
        auto future = QtConcurrent::run([this]() {
            std::pair<Point, Point> move = engine->engineGo();

            basemodel.fromUCI = move.first;
            basemodel.toUCI = move.second;

            basemodel.position.movePiece({move.first.x, move.first.y},
                                         {move.second.x, move.second.y}, basemodel.position.board);
            AddMoveToHistory();
            AddMoveToList(move);
            basemodel.position.toggleColor();

            emit paintFromThread();
        });
    } else if (kind.contains("uci")) {
        uci->engineGo();
    } else {
        qDebug() << "Error in game loop ToMove()";
    }        
    repaint();
}

// You win message
void MainWindow::YouWin() {
    QMessageBox msgBox;
    msgBox.setText("The game has finished: Red won.");
    msgBox.setInformativeText("Do you want to save game");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        // Save was clicked
        Save();
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        Newgame();
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
void MainWindow::YouLose() {
    QMessageBox msgBox;
    msgBox.setText("The game has finished: Red lost.");
    msgBox.setInformativeText("Do you want to save game");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        // Save was clicked
        Save();
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        Newgame();
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

void MainWindow::ItemClicked(QTreeWidgetItem *item, int column) {
    // qDebug() << "item clicked";
    //     int row = table->indexFromItem(item).row();
    //     basemodel.board = basemodel.moveHistory[row];
    //     isTableClicked = row;
    //     repaint();
}

void MainWindow::nodesPerSecond()
{
    nps->setText(QString::number(basemodel.engineData.nodes) + " nodes/s\n with a depth of " + QString::number(basemodel.engineData.searchDepth));
    eval->setText(QString::number(basemodel.engineData.evaluation) + " eval\n for move " + basemodel.engineData.bestMove);
    opp2->setText(basemodel.engineData.engineName);
}

void MainWindow::ToggleEngineStatus() {
    if (uci)
        return;
    if (uciThread.isRunning()) {
        uciThread.quit();
    } else {
        uciThread.start();
    }
}

MainWindow::~MainWindow() {
    if (uci)
        delete uci;
    if (view)
        delete view;
    if (renderView)
        delete renderView;
    if (boardview)
        delete boardview;

    if (opp1)
        delete opp1;
    if (opp2)
        delete opp2;
    if (opponents)
        delete opponents;

    if (loca)
        delete loca;
    if (round)
        delete round;
    if (date)
        delete date;
    if (location)
        delete location;

    if (gameinfosh)
        delete gameinfosh;
    if (gameinfoswidget)
        delete gameinfoswidget;

    if (lleft)
        delete lleft;
    if (left)
        delete left;
    if (right)
        delete right;
    if (rright)
        delete rright;

    if (headerview)
        delete headerview;
    if (model)
        delete model;

    // QTableView *table;
    if (table)
        delete table;
    if (tabwidget2)
        delete tabwidget2;
    if (tabwidget1)
        delete tabwidget1;
    if (tabview)
        delete tabview;

    if (menu)
        delete menu;

    if (navigationview)
        delete navigationview;

    if (navigationwidget)
        delete navigationwidget;

    if (dockWidget)
        delete dockWidget;

    //if (settings)
    //    delete settings;
    if (about)
        delete about;
    if (openbutton)
        delete openbutton;
    if (savebutton)
        delete savebutton;
    if (settingsbutton)
        delete settingsbutton;
    if (enginestartsbutton)
        delete enginestartsbutton;
    if (exitbutton)
        delete exitbutton;
    if (exitbutton)
        delete newgamebutton;

    if (menu1)
        delete menu1;
    if (menubar)
        delete menubar;

    if (toolbar)
        delete toolbar;
}