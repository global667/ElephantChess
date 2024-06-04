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

#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

#include "basemodel.h"

class SettingsView : public QFrame {
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

private:
    QComboBox *enginesComboBox;
    QPushButton *engineButton;
    QPushButton *styleButton;
    QPushButton *boardStyleButton;

    void ChooseEngine();
    void ChooseStyle();
    void ChooseBoardStyle();

    QPushButton *saveButton;
    void save();
public slots:
              // void comboBoxSetEngineName();

signals:
    void boardStyleChanged();
    void finished();
};

#endif // SETTINGSVIEW_H
