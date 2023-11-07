#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QMessageBox>
#include <QTabWidget>
#include <QWidget>

class AboutView : public QTabWidget
{
    Q_OBJECT
public:
    explicit AboutView(QWidget *parent = nullptr);

private:
    //QTabWidget *tabview;
    QWidget *tabwidget1;
    QWidget *tabwidget2;
    QWidget *tabwidget3;

signals:
};

#endif // ABOUTVIEW_H
