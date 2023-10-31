#include "test_xiangqi_app.h"
#include "../sources/boardview.h"
#include "../sources/genmove.h"
#include "../sources/mainwindow.h"
#include "../sources/types.h"


test_xiangqi_app::test_xiangqi_app(QObject *parent)
    : QObject{parent}
{}

void test_xiangqi_app::testMainWindow()
{
    MainWindow w;
    QTest::mouseClick(&w, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0));
}

void test_xiangqi_app::initTestCase()
{
    // Called before the first testfunction is executed
}

void test_xiangqi_app::testBoardView()
{
    for (int i = 0; i < 600; i += 10) {
        BoardView b;
        QTest::mouseClick(&b, Qt::LeftButton, Qt::NoModifier, QPoint(i, 0));
    }
}

void test_xiangqi_app::benchmarkMoveGenerator()
{
    QBENCHMARK
    {
        GenMove g;
        g.GenerateLegalMoves(color::Red);
    }
}

void test_xiangqi_app::benchmarkPaintEvent()
{
    QBENCHMARK
    {
        BoardView g;
        g.repaint();
    }
}

void test_xiangqi_app::isValidGeneralMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidGeneralMove({0, 4}, {1, 4}, color::Red));
    QCOMPARE(false, g.IsValidGeneralMove({0, 4}, {2, 4}, color::Red));
}

void test_xiangqi_app::isValidAdvisorMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidAdvisorMove({0, 3}, {1, 4}, color::Red));
    QCOMPARE(false, g.IsValidAdvisorMove({0, 3}, {2, 4}, color::Red));
}

void test_xiangqi_app::isValidElephantMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidElephantMove({0, 2}, {2, 4}, color::Red));
    QCOMPARE(false, g.IsValidElephantMove({0, 2}, {3, 5}, color::Red));
}

void test_xiangqi_app::isValidHorseMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidHorseMove({0, 1}, {2, 2}, color::Red));
    QCOMPARE(false, g.IsValidHorseMove({0, 1}, {3, 3}, color::Red));
}

void test_xiangqi_app::isValidChariotMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidChariotMove({0, 0}, {0, 1}, color::Red));
    QCOMPARE(false, g.IsValidChariotMove({0, 0}, {0, 2}, color::Red));
}

void test_xiangqi_app::isValidCannonMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidCannonMove({2, 1}, {2, 2}, color::Red));
    QCOMPARE(false, g.IsValidCannonMove({2, 1}, {2, 3}, color::Red));
}

void test_xiangqi_app::isValidSoldierMove()
{
    GenMove g;
    QCOMPARE(true, g.IsValidSoldierMove({3, 0}, {4, 0}, color::Red));
    QCOMPARE(false, g.IsValidSoldierMove({3, 0}, {5, 0}, color::Red));
}

QTEST_MAIN(test_xiangqi_app)
#include "test_xiangqi_app.moc"