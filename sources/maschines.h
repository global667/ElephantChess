#ifndef MASCHINES_H
#define MASCHINES_H

#include <QObject>

class Maschines : public QObject
{
    Q_OBJECT

public:
    Maschines(){};
    ~Maschines(){};

    virtual void start() = 0;
    virtual void engineGo() = 0;
    virtual void MovePiece(int fromRow, int fromCol, int toRow, int toCol) = 0;
};

#endif // MASCHINES_H
