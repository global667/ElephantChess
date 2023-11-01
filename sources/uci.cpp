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

#include "uci.h"

extern BaseModel basemodel;

UCI::UCI()
    : waitForReadyOK(true)
    , newGame(true)
{
    connect(&engine, SIGNAL(readyRead()), SLOT(readData()));
    connect(&engine,
            SIGNAL(errorOccurred(QProcess::ProcessError)),
            SLOT(anError(QProcess::ProcessError)));

    //basemodel.engineName = "Chameleon";

    // Set the program for the engine
    engine.setProgram(basemodel.engineName);
    qDebug() << "Starting uci engine:" << basemodel.engineName;
    engine.setReadChannel(QProcess::StandardOutput);

    engine.start(QIODevice::Text | QIODevice::ReadWrite);
    engine.waitForStarted();

    // Wait for the engine to be ready
    writeDatas("uci");
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
        qDebug() << "Data read: " << data;
        for (const auto &c : data.split('\n')) {
            if (c.contains("readyok") && waitForReadyOK) {
                if (newGame) {
                    // Start a new game
                    writeDatas("ucinewgame");
                    writeDatas("position startpos");
                    newGame = false;
                } else {
                    // Stop the engine
                    //writeDatas("stop");
                    waitForReadyOK = false;
                }
            } else if (c.contains("uciok")) {
                // The engine is ready
                writeDatas("isready");
                waitForReadyOK = true;
            } else if (c.contains("bestmove")) {
                // Received a move from the engine
                QByteArray mv = basemodel.posToken(c);
                auto fx = (mv.at(0) - 'a');
                auto fy = (mv.at(1) - '0');
                auto tx = (mv.at(2) - 'a');
                auto ty = (mv.at(3) - '0');
                // Ruft game auf
                emit updateView(QPoint(fy, fx), QPoint(ty, tx), "engine");
                //qDebug() << c << ", send new move " << c.split(' ').at(1) << " as " << fx << fy
                //         << tx << ty;
            } else {
                // Handle other cases (if necessary)
                Q_ASSERT(1);
            }
        }
    }
}

void UCI::engineGo()
{
    // get a move
    writeDatas("position startpos moves " + basemodel.moves.join(" ").toUtf8());
    writeDatas("go");
    writeDatas("isready");
}

void UCI::writeDatas(QByteArray d)
{
    qint64 bytesWritten = engine.write(d + "\n");
    qDebug() << "Data written:" << bytesWritten << d;
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
