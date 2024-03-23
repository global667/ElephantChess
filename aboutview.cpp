#include "aboutview.h"

#include <QFile>
#include <QIODeviceBase>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>

AboutView::AboutView(QWidget *parent)
    : QTabWidget{parent}
{
    setGeometry(100, 100, 400, 400);

    //tabview = new QTabWidget;
    tabwidget1 = new QWidget;
    QVBoxLayout *tab1layout = new QVBoxLayout;

    QFile license(":/COPYING");
    license.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
    QTextStream licenseStream(&license);
    QTextEdit *licenseView = new QTextEdit();
    licenseView->setPlainText(licenseStream.readAll());
    tab1layout->addWidget(licenseView);
    license.close();

    tabwidget1->setLayout(tab1layout);

    tabwidget2 = new QWidget;
    QVBoxLayout *tab2layout = new QVBoxLayout;

    QFile credits(":/CREDITS");
    credits.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
    QTextStream creditsStream(&credits);
    QTextEdit *creditsView = new QTextEdit();
    creditsView->setPlainText(creditsStream.readAll());
    tab2layout->addWidget(creditsView);
    credits.close();

    tabwidget2->setLayout(tab2layout);

    tabwidget3 = new QWidget;

    // add Krita-like about-widget layout
    QVBoxLayout *tab3layout = new QVBoxLayout;

    QPushButton *qtButton = new QPushButton("About Qt");
    QObject::connect(qtButton, &QPushButton::pressed, [=]() {
        QMessageBox::aboutQt(nullptr, "Information");
    });

    tab3layout->addWidget(qtButton);
    tabwidget3->setLayout(tab3layout);
    // tabview
    addTab(tabwidget1, "License");
    addTab(tabwidget2, "Third-party Contents");
    addTab(tabwidget3, "About");

    show();
}
