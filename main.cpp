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

#include <QApplication>
#include <QLocale>
#include <QStyleFactory>
#include <QTranslator>
#include <QtGlobal>
#include <QtQuick3D/qquick3d.h>
#include <QQmlApplicationEngine>

#include "config.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    //qputenv("QT_QPA_PLATFORM", "wayland:darkmode=2");

    QApplication a(argc, argv);

    /*QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ElephantChess_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    */
    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint
                     | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    w.setFixedSize(1200, 900);
    w.setMaximumSize(1200, 900);
    w.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    w.setWindowIcon(QIcon(":/res/generalRed.png"));
    w.setWindowTitle(
        "ElephantChess v"
        + QString("%1.%2").arg(ElephantChess_VERSION_MAJOR).arg(ElephantChess_VERSION_MINOR)
        + QString(PROJECT_DESC));
    // w.setWindowTitle("ElephantChess " + QString(PROJECT_VERSION));
    w.show();

    //QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");


    return a.exec();
}
