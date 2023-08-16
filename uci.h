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
//#include "player.h"
#include "basemodel.h"

class UCI : public QObject //, public Player
{
    Q_OBJECT
public:
    explicit UCI();
    ~UCI();

    void writeDatas(QByteArray d);
    void MovePiece(Position from, Position to); // override; //
    void MovePiece(int fromX, int fromY, int toX, int toY);

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
