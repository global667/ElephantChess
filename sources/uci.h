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

#include "basemodel.h"

class UCI final : public QObject
{
    Q_OBJECT
public:
    explicit UCI();
    ~UCI() override;
    void writeDatas(const QByteArray &d);
    QProcess engine;
    void engineGo(bool tipp);
    QStringList moves;
private:
    QByteArray buffer;
    bool waitForReadyOK;
    bool newGame;
    bool tipp = false;
signals:
    void updateView(Point from, Point to, BaseModel::Mode);
    void boardChanged(int fromX, int fromY, int toX, int toY);
    void giveTipp(Point point, Point point1);
public slots:
    static void start();
    void readData();
    static void anError(QProcess::ProcessError error);
};

#endif // UCI_H
