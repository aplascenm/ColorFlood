#include "cuadro.h"
#include <cstdlib>
#include <QBrush>


Cuadro::Cuadro(QGraphicsItem *parent)
{
    flag=false;
    //crear
    int c;
    c=rand()%4;
    //definir tamano
    setRect(0,0,50,50);
    //definir color
    QBrush a(Qt::blue);
    QBrush v(Qt::green);
    QBrush am(Qt::yellow);
    QBrush r(Qt::red);
    switch(c)
    {
    case 0:
        setBrush(a);
        color=0;
    break;
    case 1:
        setBrush(v);
        color=1;
    break;
    case 2:
        setBrush(am);
        color=2;
    break;
    case 3:
        setBrush(r);
        color=3;
    break;
    default:
    break;
    }
}

void Cuadro::cambiac(int c)
{
    QBrush a(Qt::blue);
    QBrush v(Qt::green);
    QBrush am(Qt::yellow);
    QBrush r(Qt::red);
    switch(c)
    {
    case 0:
        setBrush(a);
        color=0;
    break;
    case 1:
        setBrush(v);
        color=1;
    break;
    case 2:
        setBrush(am);
        color=2;
    break;
    case 3:
        setBrush(r);
        color=3;
    break;
    default:
    break;
    }
}
