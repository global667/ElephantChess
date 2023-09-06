#include "test_xiangqi_app.h"
#include "boardview.h"
#include "genmove.h"
#include "mainwindow.h"
#include "types.h"

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
        g.generateLegalMoves(Color::Red);
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
    QCOMPARE(true, g.isValidGeneralMove({0, 4}, {1, 4}, Color::Red));
    QCOMPARE(false, g.isValidGeneralMove({0, 4}, {2, 4}, Color::Red));
}

void test_xiangqi_app::isValidAdvisorMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidAdvisorMove({0, 3}, {1, 4}, Color::Red));
    QCOMPARE(false, g.isValidAdvisorMove({0, 3}, {2, 4}, Color::Red));
}

void test_xiangqi_app::isValidElephantMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidElephantMove({0, 2}, {2, 4}, Color::Red));
    QCOMPARE(false, g.isValidElephantMove({0, 2}, {3, 5}, Color::Red));
}

void test_xiangqi_app::isValidHorseMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidHorseMove({0, 1}, {2, 2}, Color::Red));
    QCOMPARE(false, g.isValidHorseMove({0, 1}, {3, 3}, Color::Red));
}

void test_xiangqi_app::isValidChariotMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidChariotMove({0, 0}, {0, 1}, Color::Red));
    QCOMPARE(false, g.isValidChariotMove({0, 0}, {0, 2}, Color::Red));
}

void test_xiangqi_app::isValidCannonMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidCannonMove({2, 1}, {2, 2}, Color::Red));
    QCOMPARE(false, g.isValidCannonMove({2, 1}, {2, 3}, Color::Red));
}

void test_xiangqi_app::isValidSoldierMove()
{
    GenMove g;
    QCOMPARE(true, g.isValidSoldierMove({3, 0}, {4, 0}, Color::Red));
    QCOMPARE(false, g.isValidSoldierMove({3, 0}, {5, 0}, Color::Red));
}

QTEST_MAIN(test_xiangqi_app)
#include "test_xiangqi_app.moc"
