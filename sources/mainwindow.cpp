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
#include "sources/game.h"
#undef THREE_D_VIEW
#undef ENGINE
// #ifdef TEST
// #endif
#include <QDesktopServices>

//#include "game.h"
//#include <QtQuick/QQuickView>
//#include <QtQuick3D/qquick3d.h>
//#include <QQmlApplicationEngine>

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
                                     QString("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1\n")
                                     +
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

    game->start();
}

void MainWindow::InitEngine() {
    if (QFile::exists(QDir::currentPath() + "/pikafish.exe")) {
        /*    qDebug() << "Starting built-in engine";
                engine = new Engine();
                basemodel.mode = BaseModel::Mode::engine;
                connect(engine, SIGNAL(updateView(Point,Point,BaseModel::Mode)),
                        SLOT(PlayNextTwoMoves(Point,Point,BaseModel::Mode)));
            } else { */

        basemodel.engineData.engineName = "pikafish.exe";
        qDebug() << "Starting uci engine (" + basemodel.engineData.engineName +
                " in extra thread";
        basemodel.mode = BaseModel::Mode::uci;
        uci.reset(new UCI());
        //connect(uci.get(), SIGNAL(updateView(Point,Point,BaseModel::Mode)),
        //        SLOT(PlayNextTwoMoves(Point,Point,BaseModel::Mode)));
        UCI::start();
        uci->engine.waitForReadyRead();
    } else {
        if (!QDir::currentPath().contains("pikafish.zip")) {
            QMessageBox::information(this, "Information",
                                 "Download the engine from the releases page on \nhttps://github.com/global667/Pikafish\nand put it in the program folder. Then restart the Program.\n"
                                 + QDir::currentPath() + "\n");
            QProcess *wget = new QProcess();
            if (!wget->startDetached("wget",QStringList("https://github.com/global667/Pikafish/releases/download/latest/pikafish.zip"), "./")) {
                qDebug() << "wget not found";
                delete wget;
            } else
                while (wget->waitForFinished(1000)) {
                    qDebug() << "wget finished";
                    delete wget;
                    break;
                }
        }
        QProcess *powershell = new QProcess();
        if (powershell->startDetached("powershell", QStringList({"-command", "Expand-Archive", "-Force", "./pikafish.zip", "./"})))
        {
            while (powershell->waitForFinished(1000)) {
                qDebug() << "powershell finished";
                break;
            }
            qDebug() << "pikafish.zip downloaded and extracted";
            delete powershell;
            exit(0);
        }
        else
        {
            qDebug() << "powershell not found";
            delete powershell;
        }
    }
}

void MainWindow::InitConnections() {
#ifndef THREE_D_VIEW
    //connect(boardview, SIGNAL(updateView(Point,Point,BaseModel::Mode)),
   //         SLOT(PlayNextTwoMoves(Point,Point,BaseModel::Mode)));
#endif
    //connect(settings, SIGNAL(boardStyleChanged()), SLOT(Newgame()));
    connect(uci.get(), SIGNAL(giveTipp(Point,Point)), SLOT(EngineTipp(Point,Point)));

    /*connect(
        table, &QTreeWidget::itemClicked, this,
        [=](const QTreeWidgetItem *item, int col) {
            const int row = table->indexFromItem(item).row();
            basemodel.position = basemodel.moveHistory[row];
            isTableClicked = row;
            repaint();
        },
        Qt::AutoConnection); */
    // SLOT(ItemClicked(QTreeWidgetItem *, int)));
    connect(
        lleft, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove = 0;
            ResetToHistory();
            //basemodel.position.players_color = Color::Red;

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
            //basemodel.position.toggleColor();

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
            //basemodel.position.toggleColor();

        },
        Qt::AutoConnection);
    connect(
        rright, &QPushButton::pressed, this,
        [=]() {
            basemodel.currentMove = static_cast<int>(basemodel.moveHistory.size()) - 1;
            ResetToHistory();
            //basemodel.position.players_color = basemodel.moveHistory.back().players_color;

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

    // move listing
    // model = new QStandardItemModel(0, 2);
    // model->setHeaderData(0, Qt::Horizontal, tr("Red"));
    // model->setHeaderData(1, Qt::Horizontal, tr("Black"));
    //headerview = new QHeaderView(Qt::Horizontal);
    // headerview->resizeSections(QHeaderView::Stretch);
    // headerview->setStretchLastSection(true);
    // headerview->setDefaultAlignment(Qt::AlignJustify | Qt::AlignVCenter);
    table = new QTreeWidget(navigationwidget);
    table->setColumnCount(1);
    table->setHeaderLabels(QStringList() << "Moves");

    // tabwidget1 layout
    tab1layout = new QHBoxLayout(tabwidget1);
    tab1layout->addWidget(table);
    tabwidget1->setLayout(tab1layout);

    tabwidget2layout = new QVBoxLayout(tabwidget2);
    // tabwidget2 layout
    loggingTextView = new QTextEdit(tabwidget2layout->widget());
    nps = new QLineEdit(tabwidget2layout->widget());
    nps->setText("0 nodes/s");
    nps->setReadOnly(true);
    eval = new QLineEdit(tabwidget2layout->widget());
    eval->setText("Eval: 0");

    tabwidget2layout->addWidget(eval);
    tabwidget2layout->addWidget(nps);
    tabwidget2layout->addWidget(loggingTextView);
    tabwidget2->setLayout(tabwidget2layout);

    // tabview
    tabview->addTab(tabwidget1, "Move list");
    tabview->addTab(tabwidget2, "Info");

    navibuttonslayout = new QHBoxLayout(navigationview);

    // navigation buttons
    right = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), "Forward", navibuttonslayout->widget());
    rright = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), "End", navibuttonslayout->widget());
    left = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward)), "Back", navibuttonslayout->widget());
    lleft = new QPushButton(
        QIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward)), "Begin", navibuttonslayout->widget());
    right->setToolTip("Goes on move for");
    rright->setToolTip("Goes to the end of the game");
    left->setToolTip("Goes one move back");
    lleft->setToolTip("Goes to the begining of the game");

    // navgationview
    navibuttonslayout->addWidget(lleft);
    navibuttonslayout->addWidget(left);
    navibuttonslayout->addWidget(right);
    navibuttonslayout->addWidget(rright);
    navigationview->setLayout(navibuttonslayout);

    // gameinfos
    gameinfoswidget = new QWidget(this);
    gameinfosh = new QVBoxLayout(gameinfoswidget);
    opponents = new QHBoxLayout(gameinfoswidget);
    opp1 = new QLineEdit(opponents->widget());
    opp1->setPlaceholderText("Me");
    opponents->addWidget(opp1);
    opp2 = new QLineEdit(opponents->widget());
    opp2->setText(basemodel.engineData.engineName);
    opponents->addWidget(opp2);
    location = new QHBoxLayout;
    loca = new QLineEdit(gameinfosh->widget());
    loca->setPlaceholderText("home masters");
    location->addWidget(loca);
    location->addWidget(new QLabel(", round"));
    round = new QLineEdit(gameinfosh->widget());
    round->setPlaceholderText("1");
    round->setMaximumWidth(30);
    location->addWidget(round);
    location->addWidget(new QLabel(", the"));
    date = new QLineEdit(gameinfosh->widget());
    date->setPlaceholderText("20.02.2000");
    date->setMaximumWidth(60);
    location->addWidget(date);

    gameinfosh->addLayout(opponents);
    gameinfosh->addLayout(location);

    gameinfoswidget->setLayout(gameinfosh);

    naviwidlayout = new QVBoxLayout(navigationwidget);
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
            ->setToolTip("Open a PGN-file and load it");;
    toolbar
            ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)),
                        tr("Save"), this, SLOT(Save()))
            ->setToolTip("Save a PGN-file");
    toolbar->addSeparator();

   /* toolbar
            ->addAction(QIcon(":res/play-now.png"), tr("Play now!"), this,
                        SLOT(PlayNow()))
            ->setToolTip(
                "Let's the engine makes a move now (changes the color you play)");
*/
    toolbar->addSeparator();
    toolbar
                  ->addAction(QIcon(":res/toggle-lang.jpg"),
                              tr("Toggle Pieces"), this, SLOT(togglePiecesView()))
                  ->setToolTip("Evaluate and comment a position");

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
    //toolbar->addAction(QIcon(":res/settings.png"), tr("Load engine"), this,
    //                  SLOT(OpenSettings()));
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
    game = new Game(table, this);
}

void MainWindow::Debug() const { loggingTextView->insertPlainText("Test,test,test"); }

QFile *MainWindow::LoadPGNFile() {
    const auto openFile =
            QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());

    statusBar()->showMessage(tr("Try to load file ") + openFile);

    if (openFile.isNull()) {
        statusBar()->showMessage(tr("Open file error"));
        return nullptr;
    }
    auto opfile = new QFile(openFile);
    if (!opfile->open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Open file error"));
        return nullptr;
    }
    statusBar()->showMessage(tr("Open file: ") + openFile);
    return opfile;
}

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

void MainWindow::ReadPGNData(QString data) const {
    data.shrink_to_fit();
    basemodel.moves.clear();
    //model->clear();
    for (auto &s: data.split('\n')) {
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
                                for (auto &d: s.split(re)) {
                                    //d.remove(" ");
                                    for (auto &m: d.split(" ")) {
                                        //(R"(\w\d\w\d)");
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
 /*   basemodel.position.setupInitialPositions();
    basemodel.moveHistory.clear();
    //for (const auto & m : model->findItems("*", Qt::MatchWildcard)) {
    //     model->removeRow(m->row());
    // }
    //table->clear();
    // model->clear();

    basemodel.moves.clear();
    table->clear();
    //AddMoveToHistory();
    basemodel.currentMove = 0;
    int c = 0;
*/
    auto moves = basemodel.moves;
    Newgame();
    for (auto &item1: moves) {
        //auto *item = new QStandardItem(item1);
        //if (c % 2 == 0) {
        //     model->setItem(c / 2, 0, item);
        // }
        //if (c % 2 == 1)
        //table->addTopLevelItem(new QTreeWidgetItem(QStringList(item1)));

        const int fx = ((char) item1.at(0).toLatin1() - 'a');
        const int fy = static_cast<char>(9 - item1[1].digitValue());
        const int tx = ((char) item1.at(2).toLatin1() - 'a');
        const int ty = static_cast<char>(9 - item1[3].digitValue());

        //game->AddMoveToHistory();
        game->AddMoveToList(std::make_pair(Point(9 - fy, fx), Point(9 - ty, tx)));
        Board::movePiece(Point(9 - fy, fx), Point(9 - ty, tx), basemodel.position.board);

        //c++;
        basemodel.position.toggleColor();
    }
   // ResetToHistory();
    basemodel.currentMove++;
    //column = c;
    repaint();
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
    for (const auto &m: basemodel.moves) {
        if (++i % 2 == 1)
            textstream << QString("%1.").arg((i / 2) + 1);
        textstream << m << " ";
    }

    textstream << "\n0-1\n\n";

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

void MainWindow::EngineTipp(Point from, Point to) {
    QString c;
    if (basemodel.position.players_color == Color::Red)
        c = "Red";
    else
        c = "Black";
    const QString token = BaseModel::posToken(from.x, from.y, to.x, to.y);
    const QString bestMoveWouldBe =
            QString("The best move for %1 would be %2").arg(c, token);
    QMessageBox::information(this, "Engine says:", bestMoveWouldBe);
}

void MainWindow::GiveTipp() const {
    ///std::pair<Point, Point> move =
    //engine->GetBestMove(basemodel.position.players_color);
    uci->engineGo(true);;
}

void MainWindow::About() {
    about = new AboutView();
    about->show();
}

using q_url = QUrl;

void MainWindow::Help() const {
    QDesktopServices::openUrl(
        q_url("https://github.com/global667/ElephantChess/blob/main/README.md"));
    statusBar()->showMessage(tr("Have open URL in browser"));
}

void MainWindow::PlayNow() {

    /*std::pair<Point, Point> move = uci->engineGo(false); //= std::make_pair(QPoint(1,1), QPoint(1,1));//

    if (basemodel.mode == BaseModel::Mode::engine || basemodel.mode == BaseModel::Mode::uci) {


        basemodel.fromUCI = move.first;
        basemodel.toUCI = move.second;
        //basemodel.fromHuman = from;
        //basemodel.toHuman = to;

    } else {
        basemodel.fromHuman = move.first;
        basemodel.toHuman = move.second;
    }

    if (move.first.x == -1) {
        QMessageBox::information(this, "Information", "Engine has no legal moves");
        return;
    }

    Board::movePiece({move.first.x, move.first.y},
                     {move.second.x, move.second.y}, basemodel.position.board);
    //AddMoveToList(move);
    //AddMoveToHistory();

    basemodel.position.toggleColor();
    repaint();*/
}


void MainWindow::OpenSettings() {
    // Bind new engine
    QDir dir;
    if (auto current = QDir::currentPath() + "/engines"; QDir(current).exists() == false) {
        dir.mkdir(QDir::currentPath() + "/engines");
    }
    /*
        QString filename = QFileDialog::getOpenFileName(this,
                                                        "Choose engine",
                                                        QDir::currentPath() + "/engines",
                                                        nullptr);
    */
    QMessageBox::information(this, "Information",
                             "Download the engine from the releases page on github and put it in the engines folder");

    auto *download = new DownloadView(this);
}

void MainWindow::OnDownloaded(const QString &filename) {
    QMessageBox::information(this, "Information", "Downloaded " + filename);

    basemodel.engineData.engineName = filename;
    basemodel.mode = BaseModel::Mode::uci;
    if (!uci) {
        qDebug() << "!uci";
        uci.reset(new UCI());
        disconnect(engine.get(), SIGNAL(updateView(Point,Point,BaseModel::Mode)), nullptr,
                   nullptr);
        connect(uci.get(), SIGNAL(updateView(Point,Point,BaseModel::Mode)),
                SLOT(PlayNextTwoMoves(Point,Point,BaseModel::Mode)));
    } else {
        qDebug() << "uci";
        //uciThread.quit();
        uci.reset(new UCI());
        connect(uci.get(), SIGNAL(updateView(Point,Point,BaseModel::Mode)),
                SLOT(PlayNextTwoMoves(Point,Point,BaseModel::Mode)));
    }
    //opp2->setText(basemodel.engineName);
    Q_ASSERT(&uci);
    //Q_ASSERT(&uciThread);
    // uci->moveToThread(&uciThread);
    qDebug() << "Starting uci engine (" + basemodel.engineData.engineName +
            " in extra thread";
    // uciThread.start();
    UCI::start();
    uci->engine.waitForReadyRead();


    /*   if (filename.isEmpty()) {
           filename = "ElephantChessEngine";
           basemodel.engineName = filename;
           return;
       } else {
           //filename = filename.split("/").last();
       }
       basemodel.engineName = filename;
   */
    /*  if (basemodel.engineName == "ElephantChessEngine") {
          if (!engine) {
              engine = new Engine();
              basemodel.mode = BaseModel::Mode::engine;
              disconnect(uci, SIGNAL(updateView(position,position,QString)), nullptr,
                         nullptr);
              connect(engine, SIGNAL(updateView(position,position,BaseModel::Mode)),
                      SLOT(PlayNextTwoMoves(position,position,BaseModel::Mode)));
              opp2->setPlaceholderText(basemodel.engineName);
              if (!uci)
                  delete uci;
          }
      } else {*/

    //}
}

// Startet ein neues Spiel
void MainWindow::Newgame() {
    basemodel.position.setupInitialPositions();
    basemodel.moveHistory.clear();

    basemodel.currentMoves.clear();
    //model->clear();
    // row = 0,
    //column = 0;
    basemodel.currentMove = 0;
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    basemodel.position.players_color = Color::Red;
    for (int i = static_cast<int>(basemodel.moves.size()); i >= 0; i--) {
        table->takeTopLevelItem(i);
    }   
    basemodel.moves.clear();
    basemodel.moveHistory.append(basemodel.position);
    //basemodel.currentMove++;
    repaint();
}


// End Toolbar slots

void MainWindow::ResetToHistory() {

    basemodel.position = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};

    if (basemodel.currentMove % 2 == 1)
        basemodel.position.players_color = Color::Black;
    else
        basemodel.position.players_color = Color::Red;

    repaint();
}

void MainWindow::PaintFromThreadSlot() {
    repaint();
    //timer->stop();
   //timer2->stop();
    //nps->setText("0 nodes/s");

    if (basemodel.fromUCI.x == -1) {
       //YouWin();
        return;
    }
}

void MainWindow::UpdateFromThreadSlot() const {
    nps->setText(
        QString::number(basemodel.engineData.nodes) + " nodes/s\n with a depth of " + QString::number(
            basemodel.engineData.searchDepth));
    eval->setText(
        QString::number(basemodel.engineData.evaluation) + " eval\n for move " + basemodel.engineData.bestMove);
    opp2->setText(basemodel.engineData.engineName);
}

void MainWindow::togglePiecesView() {
    basemodel.nativePieces = !basemodel.nativePieces;
repaint();
}

void MainWindow::NodesPerSecond() const {
    nps->setText(
        QString::number(basemodel.engineData.nodes) + " nodes\n with a depth of " + QString::number(
            basemodel.engineData.searchDepth));
    eval->setText(
        QString::number(basemodel.engineData.evaluation) + " eval\n for move " + basemodel.engineData.bestMove);
    opp2->setText(basemodel.engineData.engineName);
}

