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

#include "settingsview.h"
#include "qboxlayout.h"

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{
    enginesComboBox = new QComboBox{this};
    enginesComboBox->addItem("eleeye");
    enginesComboBox->addItem("chameleon");

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
    engineName = enginesComboBox->currentText();
    connect(enginesComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &SettingsView::comboBoxSetEngineName);
}

void SettingsView::comboBoxSetEngineName()
{
    engineName = enginesComboBox->currentText();
}

SettingsView::~SettingsView()
{
    delete enginesComboBox;
    delete engineButton;
    delete styleButton;
    delete boardStyleButton;
}

void SettingsView::SetModel(BaseModel *newModel)
{
    model = newModel;
}

void SettingsView::ChooseBoardStyle()
{
    QStringList items;
    //QFont font;
    //font.setFamily("YaHei");

    items << tr("Traditionel") << tr("Traditionel/PNG") << tr("Western simplified");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Figuren"), tr("Figuren"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        if (item == tr("Traditionel")) {
            model->board.viewStyleModeVar = viewStyleMode::traditional_native;
        } else if (item == tr("Traditionel/PNG")) {
            model->board.viewStyleModeVar = viewStyleMode::traditional_png;
        } else if (item == tr("Western simplified")) {
            model->board.viewStyleModeVar = viewStyleMode::western_png;
        }
        emit boardStyleChanged();
    }
}

void SettingsView::ChooseStyle()
{
    // see Wikipedia: https://de.wikipedia.org/wiki/Xiangqi
    QStringList items;
    //QFont font;
    //font.setFamily("YaHei");

    items << tr("Algebraic") << tr("Classical") << tr("Numeric");

    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Notation und Figuren"),
                                         tr("Notaion + Figuren"),
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
    if (filename.isEmpty())
        return;
    enginesComboBox->addItem(filename);
    enginesComboBox->show();

    //parent()->setEngine(filename);
}
