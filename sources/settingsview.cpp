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

#include <QVBoxLayout>

#include "settingsview.h"

extern BaseModel basemodel;

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{
    enginesComboBox = new QComboBox{this};
    enginesComboBox->addItem("built-in");
    enginesComboBox->addItem("chameleon");
    enginesComboBox->setCurrentIndex(0);

    styleButton = new QPushButton{"Choose notation style", this};
    engineButton = new QPushButton{"Choose engine", this};
    boardStyleButton = new QPushButton{"Choose board style", this};

    QVBoxLayout *layout = new QVBoxLayout{this};
    layout->addWidget(enginesComboBox);
    layout->addWidget(engineButton);
    layout->addWidget(styleButton);
    layout->addWidget(boardStyleButton);
    setLayout(layout);

    connect(engineButton, &QPushButton::clicked, this, &SettingsView::ChooseEngine);
    connect(styleButton, &QPushButton::clicked, this, &SettingsView::ChooseStyle);
    connect(boardStyleButton, &QPushButton::clicked, this, &SettingsView::ChooseBoardStyle);
    connect(enginesComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &SettingsView::comboBoxSetEngineName);
}

void SettingsView::comboBoxSetEngineName()
{
    if (enginesComboBox->currentText().contains("chameleon")) {
        basemodel.engineName = "/home/wsk/Chameleon/Chameleon";
    } else {
        basemodel.engineName = enginesComboBox->currentText();
    }
}

SettingsView::~SettingsView()
{
    delete enginesComboBox;
    delete engineButton;
    delete styleButton;
    delete boardStyleButton;
}

void SettingsView::ChooseBoardStyle()
{
    QStringList items;
    items << tr("Traditionel") << tr("Traditionel/PNG") << tr("Western simplified");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Pieces"), tr("Pieces"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        if (item == tr("Traditionel")) {
            basemodel.viewStyleModeVar = viewStyleMode::traditional_native;
        } else if (item == tr("Traditionel/PNG")) {
            basemodel.viewStyleModeVar = viewStyleMode::traditional_png;
        } else if (item == tr("Western simplified")) {
            basemodel.viewStyleModeVar = viewStyleMode::western_png;
        }
        emit boardStyleChanged();
    }
}

void SettingsView::ChooseStyle()
{
    // see Wikipedia: https://de.wikipedia.org/wiki/Xiangqi
    QStringList items;
    items << tr("Algebraic") << tr("Classical") << tr("Numeric");

    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Notation + Pieces"),
                                         tr("Notation + Pieces"),
                                         items,
                                         0,
                                         false,
                                         &ok);
    if (ok && !item.isEmpty())
        //    itemLabel->setText(item);
        QMessageBox::information(this, "Information", "Noch nicht implementiert");
}

void SettingsView::ChooseEngine()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Choose engine",
                                                    QDir::homePath(),
                                                    nullptr);
    if (filename.isEmpty()) {
        filename = "built-in";
        basemodel.engineName = filename;
        return;
    }
    enginesComboBox->addItem(filename);
    enginesComboBox->show();
    basemodel.engineName = filename;
}
