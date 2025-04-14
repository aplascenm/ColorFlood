#include "boton.h"
#include <QGraphicsTextItem>
#include <QBrush>

Boton::Boton(QString nombre, int w, int h, QColor c, QColor cc, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    //dibujar boton
    heColor=cc;
    hlColor=c;
    setRect(0,0,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(c);
    setBrush(brush);

    //texto
    QGraphicsTextItem *text= new QGraphicsTextItem(nombre,this);
    int xPos = rect().width()/2 - text->boundingRect().width()/2;
    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);

    //permitir respuesta con hover
    setAcceptHoverEvents(true);
}

void Boton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
}

void Boton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(heColor);
    setBrush(brush);
}

void Boton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(hlColor);
    setBrush(brush);
}
