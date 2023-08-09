#include "uci.h"

UCI::UCI(QObject *parent)
    : QObject(parent)
    , waitForReadyOK(true)
    , newGame(true)
{
    connect(&engine, SIGNAL(readyRead()), SLOT(readData()));
    connect(&engine,
            SIGNAL(errorOccurred(QProcess::ProcessError)),
            SLOT(anError(QProcess::ProcessError)));

    // Set the program for the engine
    engine.setProgram("/eleeye.old.exe");
    engine.setReadChannel(QProcess::StandardOutput);

    engine.start(QIODevice::Text | QIODevice::ReadWrite);
    engine.waitForStarted();

    // Wait for the engine to be ready
    writeDatas("ucci");
    writeDatas("isready");
    engine.waitForReadyRead();
}

void UCI::start()
{
    // Start the UCI communication
}

void UCI::readData()
{
    QByteArray data;
    while (engine.canReadLine()) {
        data = engine.readAll();
        for (const auto &c : data.split('\n')) {
            if (c.contains("readyok") && waitForReadyOK) {
                qDebug() << c;
                if (newGame) {
                    // Start a new game
                    writeDatas("newgame");
                    writeDatas("position startpos");
                    newGame = false;
                } else {
                    // Stop the engine
                    //writeDatas("stop");
                }
            } else if (c.contains("uciok")) {
                qDebug() << c;
                // The engine is ready
                writeDatas("isready");
                waitForReadyOK = true;
            } else if (c.contains("bestmove")) {
                // Received a move from the engine
                QByteArray mv = posToken(c);
                auto fx = (mv.at(0) - 'a');
                auto fy = (mv.at(1) - '0');
                auto tx = (mv.at(2) - 'a');
                auto ty = (mv.at(3) - '0');
                emit updateView(fx, fy, tx, ty, 1);
                qDebug() << c << ", send new move " << c.split(' ').at(1) << " as " << fx << fy
                         << tx << ty;
            } else {
                // Handle other cases (if necessary)
            }
        }
    }
}

QByteArray UCI::posToken(QByteArray token)
{
    QByteArray t = token.split(' ').at(1);
    moves.append(" " + t);
    return t;
}

QByteArray UCI::posToken(int fromX, int fromY, int toX, int toY)
{
    QByteArray m;
    char c1 = fromX + 'a';
    char c2 = (fromY) + '0';
    char c3 = toX + 'a';
    char c4 = (toY) + '0';
    m.clear();
    m.append(c1);
    m.append(c2);
    m.append(c3);
    m.append(c4);
    if (moves.isNull())
        moves = QString();
    moves.append(" " + m);

    return m;
}

void UCI::move(int fromX, int fromY, int toX, int toY)
{
    posToken(fromX, fromY, toX, toY);
    QString tmp;
    for (const auto &m : moves.split(" ", Qt::SkipEmptyParts))
        tmp += m + " ";
    if (!tmp.isEmpty())
        tmp.remove(tmp.size() - 1, tmp.size());
    moves = tmp;

    writeDatas("position startpos moves " + moves.toUtf8());
}

void UCI::engineGo()
{
    // Start the engine with a time constraint
    writeDatas("go time 2");
    writeDatas("isready");
}

void UCI::writeDatas(QByteArray d)
{
    qint64 bytesWritten = engine.write(d + "\n");
    qDebug() << "Datas written:" << bytesWritten << d;
}

void UCI::anError(QProcess::ProcessError error)
{
    qDebug() << "Error:" << error;
}

UCI::~UCI()
{
    // Stop the engine and close the process
    writeDatas("quit");
    engine.waitForFinished();
    engine.close();
}
