#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QDialog>
#include "basemodel.h"

class SettingsView : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);

    void setModel(BaseModel *newModel);

private:
    BaseModel *model;

signals:

};

#endif // SETTINGSVIEW_H
