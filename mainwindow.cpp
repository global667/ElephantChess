#include "mainwindow.h"

#include "Config.h"
#include "genmove.h"

//extern Position glfrom, glto;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 900);
    setWindowIcon(QIcon(":/res/generalRed.png"));
    setWindowTitle("XiangQi v"
                   + QString("%1.%2").arg(XiangQi_VERSION_MAJOR).arg(XiangQI_VERSION_MINOR));
    // widgets
    boardview = new BoardView(this);
    Q_ASSERT(boardview);
    tabview = new QTabWidget(this);
    tabwidget1 = new QWidget(tabview);
    tabwidget2 = new QWidget(tabview);
    menubar = new QMenuBar(this);
    menu1 = new QMenu(menubar);
    table = new QTableView(tabwidget1);
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

    model = new QStandardItemModel(0, 2);
    model->setHeaderData(0, Qt::Horizontal, tr("Red"));
    model->setHeaderData(1, Qt::Horizontal, tr("Black"));

    headerview = new QHeaderView(Qt::Horizontal);
    headerview->resizeSections(QHeaderView::Stretch);
    headerview->setStretchLastSection(true);
    headerview->setDefaultAlignment(Qt::AlignJustify | Qt::AlignVCenter);

    table->setHorizontalHeader(headerview);
    table->setModel((QAbstractItemModel *) model);

    setCentralWidget(boardview);

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
    tabview->addTab(tabwidget1, "Zugliste");
    tabview->addTab(tabwidget2, "Info");

    // navgationview
    QHBoxLayout *navibuttonslayout = new QHBoxLayout;
    navibuttonslayout->addWidget(lleft);
    navibuttonslayout->addWidget(left);
    navibuttonslayout->addWidget(right);
    navibuttonslayout->addWidget(rright);
    navigationview->setLayout(navibuttonslayout);

    gameinfoswidget = new QWidget();
    QVBoxLayout *gameinfosh = new QVBoxLayout;
    QHBoxLayout *opponents = new QHBoxLayout;
    opp1 = new QLineEdit("opp1");
    opponents->addWidget(opp1);
    opp2 = new QLineEdit("opp2");
    opponents->addWidget(opp2);

    // TODO: set the placeholder text with setPlaceholderText(...);
    QHBoxLayout *location = new QHBoxLayout;
    loca = new QLineEdit("beijin mastertournement");
    location->addWidget(loca);
    location->addWidget(new QLabel(", Runde"));
    round = new QLineEdit();
    round->setText("8");
    round->setMaximumWidth(30);
    location->addWidget(round);
    location->addWidget(new QLabel(", den"));
    date = new QLineEdit("20.02.2000");
    date->setMaximumWidth(60);
    location->addWidget(date);

    gameinfosh->addLayout(opponents);
    gameinfosh->addLayout(location);

    gameinfoswidget->setLayout(gameinfosh);
    //QLineEdit("none");
    //ICS unrated blitz match, 2000.06.05</center>\n"
    //"<center>topogigio (ELO -) vs. Wolf S. Kappesser (ELO 1200)</center>");
    //gameinfoslabel->setTextFormat(Qt::RichText);
    // navigationwidget
    QVBoxLayout *naviwidlayout = new QVBoxLayout;
    naviwidlayout->addWidget(navigationview);
    naviwidlayout->addWidget(gameinfoswidget);
    naviwidlayout->addWidget(tabview);

    navigationwidget->setLayout(naviwidlayout);

    QDockWidget *dockWidget = new QDockWidget(tr("Dock"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setWidget(navigationwidget);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    // Add toolbar
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setObjectName("toolbar");
    toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setBackgroundRole(QPalette::Light);
    toolbar->setForegroundRole(QPalette::Light);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(Qt::TopToolBarArea, toolbar);
    QIcon icon = style()->standardIcon(QStyle::SP_ArrowBack);
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogApplyButton)),
                       tr("Neues Spiel"),
                       this,
                       SLOT(newgame()));
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)),
                       tr("Öffnen"),
                       this,
                       SLOT(open()));
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_DialogSaveButton)),
                       tr("Speichern"),
                       this,
                       SLOT(save()));
    toolbar->addSeparator();

    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
                       tr("Spieler"),
                       this,
                       SLOT(togglePlayer()));
    toolbar->addSeparator();

    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_BrowserReload))),
                       tr("Sicht"),
                       this,
                       SLOT(toggleGameView()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxInformation))),
                       tr("Tipp"),
                       this,
                       SLOT(giveTipp()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_MessageBoxCritical))),
                       tr("Aufgeben"),
                       this,
                       SLOT(giveUpGame()));
    toolbar->addSeparator();
    toolbar->addAction(QIcon(style()->standardIcon(QStyle::SP_ComputerIcon)),
                       tr("Einstellungen"),
                       this,
                       SLOT(settings()));
    toolbar->addAction(QIcon(style()->standardIcon(
                           (QStyle::SP_DialogHelpButton))), //SP_TitleBarContextHelpButton))),
                       tr("Hilfe"),
                       this,
                       SLOT(Help()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_TitleBarMenuButton))),
                       tr("Über"),
                       this,
                       SLOT(About()));
    toolbar->addAction(QIcon(style()->standardIcon((QStyle::SP_DialogCloseButton))),
                       tr("Beenden"),
                       this,
                       SLOT(exit()));

    statusBar()->showMessage(tr("Ready"));
    Q_ASSERT(&uci);
    Q_ASSERT(&uciThread);
    uci.moveToThread(&uciThread);
    qDebug() << "Starting uci engine in extra thread";
    uci.start();
    uciThread.start();

    connect(&uci, SIGNAL(updateView(int, int, int, int, int)), SLOT(game(int, int, int, int, int)));
    connect(boardview,
            SIGNAL(updateView(int, int, int, int, int)),
            SLOT(game(int, int, int, int, int)));
}

void MainWindow::giveUpGame()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::exit()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}
void MainWindow::toggleGameView()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}
void MainWindow::togglePlayer()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}
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

void MainWindow::open()
{
    auto openFile = QFileDialog::getOpenFileName(this, tr("Datei öffnen"));
    statusBar()->showMessage(tr("Öffne Datei: ") + openFile);
    //QMessageBox::information(this, "Information", "Noch nicht implementiert");

    if (openFile.isNull()) {
        statusBar()->showMessage(tr("Lade Datei: fehlgeschlagen"));
        return;
    }
    QFile opfile(openFile);
    if (!opfile.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Lade Datei: Öffnen fehlgeschlagen"));
        return;
    }

    statusBar()->showMessage(tr("Lade Datei: ") + openFile);

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
                                        uci.moves << s.split(".").last();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug() << uci.moves;

    opfile.close();
    int c = 0;
    basemodel.board.initBoard();
    update();

    QStandardItem *item;
    for (QString &item1 : uci.moves) {
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
    auto saveFile = QFileDialog::getSaveFileName(this, tr("Datei speichern"));

    if (saveFile.isNull()) {
        statusBar()->showMessage(tr("Speichere Datei: fehlgeschlagen"));
        return;
    }
    QFile svfile(saveFile);
    if (!svfile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Speichere Datei: Öffnen fehlgeschlagen"));
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
    for (const auto &m : uci.moves) {
        if (++i % 2 == 1)
            textstream << QString("%1.").arg((i / 2) + 1);
        textstream << m << " ";
    }

    textstream << "0-1\n\n";

    svfile.close();

    statusBar()->showMessage(tr("Speichere Datei: ") + saveFile);
}

void MainWindow::settings()
{
    SettingsView dialog(this);
    dialog.setModel(&basemodel);
    dialog.exec();
}

void MainWindow::toggleEngineStatus()
{
    if (uciThread.isRunning()) {
        uciThread.quit();
    } else {
        uciThread.start();
    }
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
    //basemodel.board().onMove = Color::Red;
    uci.moves.clear();
    repaint();
}

void MainWindow::lleftPressed()
{
    basemodel.currentMove = 0;
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    if (uciThread.isRunning()) {
        uciThread.quit();
    }
    repaint();
}

void MainWindow::leftPressed()
{
    basemodel.currentMove--;
    if (basemodel.currentMove < 0) {
        basemodel.currentMove = 0;
    }
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    if (uciThread.isRunning()) {
        uciThread.quit();
    }
    repaint();
}

void MainWindow::rightPressed()
{
    basemodel.currentMove++;
    if (basemodel.currentMove > basemodel.moveHistory.size() - 1) {
        basemodel.currentMove = basemodel.moveHistory.size() - 1;
    }
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    if (uciThread.isRunning()) {
        uciThread.quit();
    }

    repaint();
}

void MainWindow::rrightPressed()
{
    basemodel.currentMove = basemodel.moveHistory.size() - 1;
    basemodel.board = basemodel.moveHistory[basemodel.currentMove];
    basemodel.fromHuman = {-1, -1};
    basemodel.toHuman = {-1, -1};
    if (uciThread.isRunning()) {
        uciThread.quit();
    }
    repaint();
}

// sender = -1 -> model
// sender = 0 -> human
// sender = 1 -> uci
void MainWindow::game(int fromX, int fromY, int toX, int toY, int sender)
{
    qDebug() << "sender:" << sender;
    GenMove isMate(basemodel.board.pieces, basemodel.board.onMove);

    switch (sender) {
    case -1:
        break;
    case 0:
        //Is the game in observe mode?
        if (basemodel.moveHistory.size() != basemodel.currentMove) {
            //If so, delete all moves after current move and starts the engine
            toggleEngineStatus();
            for (auto i = 0; i < basemodel.moveHistory.size(); ++i) {
                if (i > basemodel.currentMove) {
                    basemodel.moveHistory.pop_back();
                }
                //uci.moves.pop_back();
            }
        }

        // Is in Checkmate?
        if (isMate.isCheckmate(basemodel.board.onMove)) {
            qDebug() << "Checkmate";
            return;
        }

        // Give move to engine
        uci.MovePiece({fromX, fromY}, {toX, toY});

        addMoveToList();
        addMoveToHistory();

        basemodel.board.toggleOnMove();
        repaint();
        break;
    case 1:
        boardview->MovePiece({fromX, fromY}, {toX, toY});
        basemodel.fromUCI = {fromX, fromY};
        basemodel.toUCI = {toX, toY};
        addMoveToList();
        addMoveToHistory();

        basemodel.board.toggleOnMove();
        repaint();

        break;
    default:
        qDebug() << "Error in game";
        statusBar()->showMessage("Error in game");
        break;
    }

    //row++;

    repaint();
}

void MainWindow::addMoveToHistory()
{
    basemodel.moveHistory.append(basemodel.board);
    basemodel.currentMove++;
}

void MainWindow::addMoveToList()
{
    QString mv = QString(uci.moves.last());

    QStandardItem *item = new QStandardItem(mv);

    if (column % 2 == 0) {
        model->setItem(column / 2, 0, item);
    }
    if (column % 2 == 1) {
        model->setItem(column / 2, 1, item);
    }
    column++;
    //delete item;
}

MainWindow::~MainWindow()
{
    uciThread.quit();
    uciThread.wait();
}
