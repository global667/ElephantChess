#include "mainwindow.h"

//#include "Config.h"
#include <PGNGame.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 900);
    setWindowIcon(QIcon(":/res/kingRed.png"));
    //    setWindowTitle("XiangQi v"
    //                   + QString("%1.%2").arg(XiangQi_VERSION_MAJOR).arg(XiangQI_VERSION_MINOR));
    // widgets
    boardview = new BoardView(this);
    tabview = new QTabWidget(this);
    tabwidget1 = new QWidget(tabview);
    tabwidget2 = new QWidget(tabview);
    menubar = new QMenuBar(this);
    menu1 = new QMenu(menubar);
    table = new QTableView(tabwidget1);
    navigationwidget = new QWidget(this);
    navigationview = new QWidget(navigationwidget);

    // navigation buttons
    lleft = new QPushButton("<<-");
    left = new QPushButton("<-");
    right = new QPushButton("->");
    rright = new QPushButton("-->");
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

    // menu buttons
    openbutton = new QAction(QIcon(":/res/icons/open.png"), tr("Laden"), menubar);
    //openbutton->setIcon();
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(openbutton, &QAction::triggered, this, &MainWindow::open);

    savebutton = new QAction(tr("Speichern"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(savebutton, &QAction::triggered, this, &MainWindow::save);

    enginestartsbutton = new QAction(tr("Engine starten"), menubar);
    //openbutton->setShortcuts(QKeySequence::New);
    //openbutton->setStatusTip(tr("Create a new file"));
    connect(enginestartsbutton, &QAction::triggered, this, &MainWindow::enginestarts);

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

    statusBar()->showMessage(tr("Ready"));

    //uci.moveToThread(&uciThread);
    //qDebug() << "Starting uci engine in extra thread";
    //uci.start();
    //uciThread.start();

    connect(&uci, SIGNAL(updateView(int, int, int, int, int)), SLOT(game(int, int, int, int, int)));
    connect(boardview,
            SIGNAL(updateView(int, int, int, int, int)),
            SLOT(game(int, int, int, int, int)));
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
    /* QFile opfile(openFile);
    if (!opfile.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        statusBar()->showMessage(tr("Lade Datei: Öffnen fehlgeschlagen"));
        return;
    }
*/
    /*   
    QTextStream textstream(&opfile);
    auto str = textstream.readAll();
    for (auto s : str.split('\n'))
    {
        if(s.contains("[Event "))
            
    }
    
    uci.moves = str.simplified();
   */

    // TODO: Remove pgnLib dependence

    std::ifstream pgnfile(openFile.toLatin1());
    qDebug() << "Open pgn" << &pgnfile;
    pgn::Game game;
    qDebug() << "game" << game.moves().size();

    //QString str;
    //QTextStream textstream(&opfile);
    //str = textstream.readAll();
    //uci.moves = str.simplified();
    // collecting games from file
    pgnfile >> game;

    qDebug() << "Game: " << game.moves().size();

    // TODO: Read tags too
    //for (auto i = game.tags().begin(); i != game.tags().end(); i++) {
    for (const auto &i : game.tags()) {
        if (i.name() == "Round")
            round->setText(i.value().c_str());
        else if (i.name() == "White")
            opp1->setText(i.value().c_str());
        else if (i.name() == "Black")
            opp2->setText(i.value().c_str());
        else if (i.name() == "Date")
            date->setText(i.value().c_str());
        else if (i.name() == "Event")
            loca->setText(i.value().c_str());
        else
            qDebug() << QString("Error tag is: %1").arg(i.value().c_str());
    }

    int j = 0;
    for (auto i = game.moves().begin(); i != game.moves().end(); ++i, j++) {
        //uci.moves =

        std::string ltrwhite = game.moves()[j].white().str();
        std::string ltrblack = game.moves()[j].black().str();

        uci.moves.append(ltrwhite + " ");
        qDebug() << "Moves from pgnlib : " << uci.moves.append(ltrblack + " ");
    }
    uci.moves.removeLast();

    qDebug() << uci.moves;
    //opfile.close();

    statusBar()->showMessage(tr("Lade Datei: ") + openFile);
    int c = 0;
    basemodel.board.initBoard();
    update();
    int i = 0;

    QStandardItem *item;
    for (QString &item1 : uci.moves.split(' ', Qt::SkipEmptyParts)) {
        item = new QStandardItem(item1);
        if (c % 2 == 0)
            model->setItem(c / 2, 0, item);
        if (c % 2 == 1)
            model->setItem(c / 2, 1, item);
        c++;

        qDebug() << item1.at(0).toLatin1();
        auto fx = ((char) item1.at(0).toLatin1()) - 'a';
        qDebug() << fx;
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
    for (const auto &m : uci.moves.split(' ')) {
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
    QStringList items;
    // see Wikipedia: https://de.wikipedia.org/wiki/Xiangqi
    items << tr("Algebraische") << tr("Klassische") << tr("Numerische");

    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Notation und Figuren"),
                                         tr("Notaion + Figuren"),
                                         items,
                                         0,
                                         false,
                                         &ok);
    if (ok && !item.isEmpty())
        //    itemLabel->setText(item);
        QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::enginestarts()
{
    uci.engineGo();
}

void MainWindow::newgame()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::lleftPressed()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void MainWindow::leftPressed()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
    //boardmodel.currentMove--;
}

void MainWindow::rightPressed()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
    //boardmodel.currentMove++;
}

void MainWindow::rrightPressed()
{
    QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

// sender = -1 -> model
// sender = 0 -> human
// sender = 1 -> uci
void MainWindow::game(int fromX, int fromY, int toX, int toY, int sender)
{
    int l_fx = fromX;
    int l_fy = fromY;
    int l_tx = toX;
    int l_ty = toY;
    qDebug() << l_fx << l_fy << l_tx << l_ty;
    qDebug() << "sender:" << sender;
    switch (sender) {
    case -1:
        break;
    case 0:

        //basemodel.currentMove++;

        //basemodel.board.movePiece(fromX, fromY, toX, toY);
        uci.move(l_fy, l_fx, l_ty, l_tx);
        addMoveToList();
        uci.engineGo();

        break;
    case 1:
        //basemodel.currentMove++;
        basemodel.board.movePiece(fromY, fromX, toY, toX);
        repaint();
        qDebug() << "from: " << fromX << fromY << "to: " << toX << toY;
        addMoveToList();

        break;
    default:
        qDebug() << "Error in game";
        break;
    }

    //row++;

    repaint();
}

void MainWindow::addMoveToList()
{
    QString mv = QString(uci.moves.split(' ').last());

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
