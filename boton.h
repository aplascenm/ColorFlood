#ifndef BOTON_H
#define BOTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Boton:public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Boton(QString nombre, int w, int h, QColor c, QColor cc, QGraphicsItem *parent=NULL);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked();
private:
    QGraphicsTextItem *Text;
    QColor heColor;
    QColor hlColor;
};

#endif // BOTON_H
