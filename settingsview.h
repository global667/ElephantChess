#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>

#include "basemodel.h"

class SettingsView : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);

    void setModel(BaseModel *newModel);

private:
    BaseModel *model;
    QComboBox *comboBox;
    QPushButton *button;

    void chooseEngine();

signals:
};

#endif // SETTINGSVIEW_H
