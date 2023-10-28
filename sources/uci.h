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
    void MovePiece(position from, position to); // override; //
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
    void updateView(position from, position to);
    void boardChanged(int fromX, int fromY, int toX, int toY);

public slots:
    void start();
    void readData();
    void anError(QProcess::ProcessError error);
};

#endif // UCI_H
