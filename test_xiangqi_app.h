#ifndef TEST_XIANGQI_APP_H
#define TEST_XIANGQI_APP_H

#include <QObject>
#include <QTest>

class test_xiangqi_app : public QObject
{
    Q_OBJECT
public:
    explicit test_xiangqi_app(QObject *parent = nullptr);

private slots:
    void toUpper();

signals:

};

#endif // TEST_XIANGQI_APP_H
