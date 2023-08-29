#include "settingsview.h"
#include "qboxlayout.h"

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{
    engines_comboBox = new QComboBox{this};
    //comboBox->addItem()
    engine_button = new QPushButton{"Wähle die Engine", this};
    connect(engine_button, &QPushButton::clicked, this, &SettingsView::chooseEngine);
    //button->setGeometry(0, 0, 100, 50);
    engine_button->show();

    style_button = new QPushButton{"Wähle den Style", this};

    QVBoxLayout *layout = new QVBoxLayout{this};
    layout->addWidget(engines_comboBox);
    layout->addWidget(engine_button);
    layout->addWidget(style_button);
    setLayout(layout);

    connect(style_button, &QPushButton::clicked, this, &SettingsView::chooseStyle);
}

void SettingsView::setModel(BaseModel *newModel)
{
    model = newModel;
}

void SettingsView::chooseStyle()
{
    // see Wikipedia: https://de.wikipedia.org/wiki/Xiangqi
    QStringList items;
    QFont font;
    font.setFamily("YaHei");

    items << tr("Algebraische") << tr("Klassische") << tr("Numerische");

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

void SettingsView::chooseEngine()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Wähle die Engine",
                                                    QDir::homePath(),
                                                    "Engines (*.exe)");
    if (filename.isEmpty())
        return;
    engines_comboBox->addItem(filename);
    engines_comboBox->show();

    //parent()->setEngine(filename);
}
