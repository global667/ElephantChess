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
//#define THREE_D_VIEW
//#ifdef TEST
//#endif
#include <QDesktopServices>

extern BaseModel basemodel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef THREE_D_VIEW
    renderView = new RenderView();
    renderView->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    view = QWidget::createWindowContainer(renderView);
    setCentralWidget(view);
    renderView->show();
#else
    boardview = new BoardView(this);
    setCentralWidget(boardview);
#endif

    InitWidgets();
    InitEngine();
    InitConnections();
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::InitEngine()
{
    engine = new Engine();
    basemodel.kind = "engine";
}
void MainWindow::InitConnections()
{
    // engine moves
    connect(engine,
            SIGNAL(updateView(QPoint, QPoint, QString)),
            SLOT(ToMove(QPoint, QPoint, QString)));
    // mouse clicked moves (human)
#ifdef THREE_D_VIEW
#else
    connect(boardview,
            SIGNAL(updateView(QPoint, QPoint, QString)),
            SLOT(ToMove(QPoint, QPoint, QString)));
#endif
    connect(settings, SIGNAL(boardStyleChanged()), SLOT(Newgame()));

    connect(
        table,
        &QTreeWidget::itemClicked,
        this,
        [=](QTreeWidgetItem *item, int col) {
            int row = table->indexFromItem(item).row();
            basemodel.board = basemodel.moveHistory[row];
            isTableClicked = row;
            repaint();
        },
        Qt::AutoConnection);
    //SLOT(ItemClicked(QTreeWidgetItem *, int)));

    connect(
        lleft,
        &QPushButton::pressed,
        this,
        [=]() {
            basemodel.currentMove = 0;
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        left,
        &QPushButton::pressed,
        this,
        [=]() {
            basemodel.currentMove--;
            if (basemodel.currentMove <= 0) {
                basemodel.currentMove = 0;
            }
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        right,
        &QPushButton::pressed,
        this,
        [=]() {
            basemodel.currentMove++;
            if (basemodel.currentMove >= basemodel.moveHistory.size() - 1) {
                basemodel.currentMove = basemodel.moveHistory.size() - 1;
            }
            ResetToHistory();
        },
        Qt::AutoConnection);
    connect(
        rright,
        &QPushButton::pressed,
        this,
        [=]() {
            basemodel.currentMove = basemodel.moveHistory.size() - 1;
            ResetToHistory();
        },
        Qt::AutoConnection);
}

void MainWindow::InitWidgets()
{
    settings = new SettingsView();

    tabview = new QTabWidget(this);
    tabwidget1 = new QWidget(tabview);
    tabwidget2 = new QWidget(tabview);
    menubar = new QMenuBar(this);
    menu1 = new QMenu(menubar);
    //table = new QTableView(tabwidget1);
    navigationwidget = new QWidget(this);
    navigationview = new QWidget(navigationwidget);

    // navigation buttons
    right = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSeekForward)), "Forward");
    rright = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)), "End");
    left = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward)), "Back");
    lleft = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward)), "Begin");
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
    QHBoxLayout *tabwidget2layout = new QHBoxLayout;
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
    opp1->setPlaceholderText("Player One");
    opponents->addWidget(opp1);
    opp2 = new QLineEdit();
    opp2->setPlaceholderText(basemodel.engineName);
    opponents->addWidget(opp2);    
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

    // hole Dock
    dockWidget = new QDockWidget(tr("Dock"), this);
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
                    tr("New game"),
                    this,
                    SLOT(Newgame()))
        ->setToolTip("Starts a new game with the given engine");
    toolbar
        ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)),
                    tr("Open"),
                    this,
                    SLOT(Open()))
        ->setToolTip("Open a PGN-file and load it");
    ;
    toolbar
        ->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)),
                    tr("Save"),
                    this,
                    SLOT(Save()))
        ->setToolTip("Save a PGN-file");
    toolbar->addSeparator();

    toolbar->addAction(QIcon(":res/play-now.png"), tr("Play now!"), this, SLOT(PlayNow()))
        ->setToolTip("Let's the engine makes a move now (changes the color you play)");

    toolbar->addSeparator();

    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
                    tr("Toggle view"),
                    this,
                    SLOT(ToggleGameView()))
        ->setToolTip("Set the color that's above to the bottom");
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxInformation))),
                       tr("Tipp"),
                       this,
                       SLOT(GiveTipp()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxCritical))),
                    tr("Give up"),
                    this,
                    SLOT(GiveUpGame()))
        ->setToolTip("Give the game up. You will loose.");
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":res/settings.png"), tr("Settings"), this, SLOT(OpenSettings()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogHelpButton))),
                    tr("Help"),
                    this,
                    SLOT(Help()))
        ->setToolTip("Starts the help in a browser");
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_TitleBarMenuButton))),
                       tr("About"),
                       this,
                       SLOT(About()));
    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                    tr("Exit"),
                    QCoreApplication::instance(),
                    &QCoreApplication::quit)
        ->setToolTip("Exit the application");
    toolbar->addSeparator();

    toolbar
        ->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                    tr("EvalDebug"),
                    this,
                    SLOT(Debug()))
        ->setToolTip("Evaluate and comment a position");
}

void MainWindow::Debug()
{
    loggingTextView->insertPlainText("Test,test,test");
}

QFile *MainWindow::LoadPGNFile()
{
    auto openFile = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());

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
void MainWindow::Open()
{
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

void MainWindow::ReadPGNData(QString data)
{
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
}

void MainWindow::PutPGNOnBoard()
{
    int c = 0;
    basemodel.board.initBoard();

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
    repaint();
}

void MainWindow::Save()
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

void MainWindow::GiveUpGame()
{
    QMessageBox::information(this, "Information", "You have decided to give up, you lose...");
}

void MainWindow::ToggleGameView()
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

void MainWindow::GiveTipp()
{
    std::pair<QPoint, QPoint> move = engine->GetBestMove(basemodel.board.onMove);
    QPoint from = move.first;
    QPoint to = move.second;
    QString c;
    if (basemodel.board.onMove == color::Red)
        c = "Red";
    else
        c = "Black";
    QString token = basemodel.posToken(from.y(), from.x(), to.y(), to.x());
    QString bestMoveWouldBe = QString("The best move for %1 would be %2").arg(c).arg(token);
    QMessageBox::information(this, "Engine says:", bestMoveWouldBe);
}

void MainWindow::About()
{
    about = new AboutView();
    about->show();
}

void MainWindow::Help()
{
    QDesktopServices::openUrl(
        QUrl("https://github.com/global667/ElephantChess/blob/main/README.md"));
    statusBar()->showMessage(tr("Have open URL in browser"));
}

void MainWindow::PlayNow()
{
    if (basemodel.kind == "engine")
        engine->engineGo();
    else
        uci->engineGo();
}

void MainWindow::OpenSettings()
{
    connect(settings, SIGNAL(finished()), this, SLOT(UpdateSettings()));
    qDebug() << "OpenSettings()";
    settings->show();
}

// called after OpenSettings()
void MainWindow::UpdateSettings()
{
    qDebug() << basemodel.engineName;
    if (basemodel.engineName == "built-in") {
        if (!engine) {
            engine = new Engine();
            basemodel.kind = "engine";
            disconnect(uci, SIGNAL(updateView(position, position, QString)), nullptr, nullptr);
            connect(engine,
                    SIGNAL(updateView(position, position, QString)),
                    SLOT(ToMove(position, position, QString)));
            opp2->setPlaceholderText(basemodel.engineName);
            if (!uci)
                delete uci;
        }
    } else {
        basemodel.kind = "uci";
        if (!uci) {
            qDebug() << "!uci";
            uci = new UCI();
            disconnect(engine, SIGNAL(updateView(QPoint, QPoint, QString)), nullptr, nullptr);
            connect(uci,
                    SIGNAL(updateView(QPoint, QPoint, QString)),
                    SLOT(ToMove(QPoint, QPoint, QString)));
        } else {
            qDebug() << "uci";
            uciThread.quit();
            uci = new UCI();
            connect(uci,
                    SIGNAL(updateView(QPoint, QPoint, QString)),
                    SLOT(ToMove(QPoint, QPoint, QString)));
        }
        opp2->setPlaceholderText(basemodel.engineName);
        Q_ASSERT(&uci);
        Q_ASSERT(&uciThread);
        uci->moveToThread(&uciThread);
        qDebug() << "Starting uci engine (" + basemodel.engineName + " in extra thread";
        uci->start();
        uciThread.start();
    }
}

// Startet ein neues Spiel
void MainWindow::Newgame()
{
    basemodel.board.initBoard();
    basemodel.moveHistory.clear();
    model->clear();
    //row = 0,
    column = 0;
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

// End Toolbar slots


void MainWindow::ResetToHistory()
{
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    basemodel.fromUCI = {-1, -1};
    basemodel.toUCI = {-1, -1};
    boardview->fromHuman = {-1, -1};
    boardview->toHuman = {-1, -1};
    repaint();
}

void MainWindow::ToMove(QPoint from, QPoint to, QString kind)
{
    // not used at the moment
    //basemodel.currentMoves.push_back({from, to});

    QString name = basemodel.board.pieces[from.x()][from.y()].name;
    QString beaten;
    if (basemodel.board.GetPiece({to.x(), to.y()}).type != pieceType::Empty)
        beaten = "x";
    else
        beaten = "-";

    basemodel.board.movePiece(from.x(), from.y(), to.x(), to.y());
    if (basemodel.board.onMove == color::Black) {
        basemodel.fromUCI = from;
        basemodel.toUCI = to;
    } else {
        basemodel.fromHuman = from;
        basemodel.toHuman = to;
    }
    AddMoveToHistory();

    QStringList mv;
    mv << name << QString(basemodel.moves.last().at(0)) << QString(basemodel.moves.last().at(1))
       << beaten << QString(basemodel.moves.last().at(2)) << QString(basemodel.moves.last().at(3));
    AddMoveToList(mv.join(""));

    basemodel.board.toggleOnMove();

    if (kind.contains("human")) {
        engine->engineGo();
    } else if (kind.contains("uci")) {
        uci->engineGo();
    } else if (kind.contains("engine")) {
    } else {
        qDebug() << "Error in game loop ToMove()";
    }
    repaint();
}

void MainWindow::AddMoveToHistory()
{
    basemodel.currentMove++;
    basemodel.moveHistory.append(basemodel.board);
}

void MainWindow::AddMoveToList(QString move)
{
    qDebug() << move;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, move);
    if (isTableClicked) {
            if (table->topLevelItemCount() == isTableClicked + 1) {
            table->addTopLevelItem(item);
            } else {
            table->currentItem()->addChild(item);
            }
    } else {
            table->addTopLevelItem(item);
    }
    item = nullptr;
    //column++;
}

void MainWindow::ItemClicked(QTreeWidgetItem *item, int column)
{
    //qDebug() << "item clicked";
    //    int row = table->indexFromItem(item).row();
    //    basemodel.board = basemodel.moveHistory[row];
    //    isTableClicked = row;
    //    repaint();
}

void MainWindow::ToggleEngineStatus()
{
    if (uci)
            return;
    if (uciThread.isRunning()) {
            uciThread.quit();
    } else {
            uciThread.start();
    }
}

MainWindow::~MainWindow()
{
    uciThread.quit();
    uciThread.wait();
}
