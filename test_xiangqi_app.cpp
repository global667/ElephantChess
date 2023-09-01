#include "test_xiangqi_app.h"

test_xiangqi_app::test_xiangqi_app(QObject *parent)
    : QObject{parent}
{

}

void test_xiangqi_app::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(test_xiangqi_app)
#include "test_xiangqi_app.moc"
