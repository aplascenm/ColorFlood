#ifndef CUADRO_H
#define CUADRO_H

#include <QGraphicsRectItem>

class Cuadro: public QGraphicsRectItem
{
public:
    Cuadro(QGraphicsItem *parent=NULL);

    //Funciones para cambiar el color

    //cuando juntas
    bool flag;
    int color;
    void cambiac(int c);
private:

};

#endif // CUADRO_H
