#include "settingsview.h"

SettingsView::SettingsView(QWidget *parent)
    : QDialog{parent}
{}

void SettingsView::setModel(BaseModel *newModel)
{
    model = newModel;
}
