#include "settingsview.h"

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{
    comboBox = new QComboBox{this};
    //comboBox->addItem()
    button = new QPushButton{"Wähle die Engine", this};
    connect(button, &QPushButton::clicked, this, &SettingsView::chooseEngine);
    //button->setGeometry(0, 0, 100, 50);
    button->show();
}

void SettingsView::setModel(BaseModel *newModel)
{
    model = newModel;
}

void SettingsView::chooseEngine()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Wähle die Engine",
                                                    QDir::homePath(),
                                                    "Engines (*.exe)");
    if (filename.isEmpty())
        return;
    comboBox->addItem(filename);
    comboBox->show();
    //parent()->setEngine(filename);
}
