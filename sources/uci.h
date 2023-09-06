#ifndef UCI_H
#define UCI_H

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QProcess>
#include <QResource>
#include <QStringList>
#include <QThread>

#include "types.h"

class UCI : public QObject
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
    QStringList moves;

    QString engineName = "chameleon";

private:
    QByteArray buffer;
    bool waitForReadyOK;
    bool newGame;

    QByteArray posToken(int fromX, int fromY, int toX, int toY);
    QByteArray posToken(QByteArray token);
signals:
    void updateView(Position from, Position to);
    void boardChanged(int fromX, int fromY, int toX, int toY);

public slots:
    void start();
    void readData();
    void anError(QProcess::ProcessError error);
};

#endif // UCI_H
