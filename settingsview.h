#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

#include "basemodel.h"

class SettingsView : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

    void setModel(BaseModel *newModel);

    QString engineName;

private:
    BaseModel *model;
    QComboBox *engines_comboBox;
    QPushButton *engine_button;
    QPushButton *style_button;
    QPushButton *board_style_button;

    void chooseEngine();
    void chooseStyle();
    void chooseBoardStyle();

public slots:
    void comboBoxSetEngineName();

signals:
    void boardStyleChanged();
};

#endif // SETTINGSVIEW_H
