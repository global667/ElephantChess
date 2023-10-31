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
    void isValidGeneralMove();
    void isValidAdvisorMove();
    void isValidElephantMove();
    void isValidHorseMove();
    void isValidChariotMove();
    void isValidCannonMove();
    void isValidSoldierMove();

    void benchmarkMoveGenerator();
    void benchmarkPaintEvent();

    void testBoardView();

    void testMainWindow();

    void initTestCase();

signals:
};

#endif // TEST_XIANGQI_APP_H