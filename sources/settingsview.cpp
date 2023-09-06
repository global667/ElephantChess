#include "settingsview.h"
#include "qboxlayout.h"

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{
    engines_comboBox = new QComboBox{this};
    engines_comboBox->addItem("eleeye");
    engines_comboBox->addItem("chameleon");

    style_button = new QPushButton{"Wähle den Notationsstil", this};
    engine_button = new QPushButton{"Wähle die Engine", this};
    board_style_button = new QPushButton{"Wähle das Brett-Design", this};

    QVBoxLayout *layout = new QVBoxLayout{this};
    layout->addWidget(engines_comboBox);
    layout->addWidget(engine_button);
    layout->addWidget(style_button);
    layout->addWidget(board_style_button);
    setLayout(layout);

    connect(engine_button, &QPushButton::clicked, this, &SettingsView::chooseEngine);
    connect(style_button, &QPushButton::clicked, this, &SettingsView::chooseStyle);
    connect(board_style_button, &QPushButton::clicked, this, &SettingsView::chooseBoardStyle);
    engineName = engines_comboBox->currentText();
    connect(engines_comboBox,
            &QComboBox::currentIndexChanged,
            this,
            &SettingsView::comboBoxSetEngineName);
}

void SettingsView::comboBoxSetEngineName()
{
    engineName = engines_comboBox->currentText();
}

SettingsView::~SettingsView()
{
    delete engines_comboBox;
    delete engine_button;
    delete style_button;
    delete board_style_button;
}

void SettingsView::setModel(BaseModel *newModel)
{
    model = newModel;
}

void SettingsView::chooseBoardStyle()
{
    QStringList items;
    QFont font;
    font.setFamily("YaHei");

    items << tr("Traditionell") << tr("Traditionell/PNG") << tr("Westlich vereinfacht");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Figuren"), tr("Figuren"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        if (item == tr("Traditionell")) {
            model->board.viewStyleMode = ViewStyleMode::traditional_native;
        } else if (item == tr("Traditionell/PNG")) {
            model->board.viewStyleMode = ViewStyleMode::traditional_png;
        } else if (item == tr("Westlich vereinfacht")) {
            model->board.viewStyleMode = ViewStyleMode::western_png;
        }
        emit boardStyleChanged();
    }
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
