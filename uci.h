#ifndef UCI_H
#define UCI_H

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QProcess>
#include <QResource>
#include <QString>
#include <QThread>

//#include "opponent.h"

class UCI : public QObject //, Opponent
{
    Q_OBJECT
public:
    explicit UCI(QObject *parent = nullptr);
    ~UCI();

    void writeDatas(QByteArray d);
    void move(int fromX, int fromY, int toX, int toY);
    QProcess engine;
    void engineGo();
    QString moves;

private:
    QByteArray buffer;
    bool waitForReadyOK;
    bool newGame;

    QByteArray posToken(int fromX, int fromY, int toX, int toY);
    QByteArray posToken(QByteArray token);
signals:
    // connected in boardview
    //void newMove(QString mv);
    void updateView(int, int, int, int, int);
    void boardChanged(int fromX, int fromY, int toX, int toY);

public slots:
    void start();
    void readData();
    void anError(QProcess::ProcessError error);
};

#endif // UCI_H
