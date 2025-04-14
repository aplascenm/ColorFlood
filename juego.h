#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <iostream>
#include <QLineEdit>
#include "tablero.h"

using namespace std;

class Juego: public QGraphicsView
{
    Q_OBJECT
public:
    Juego(QWidget *parent=NULL);

    int getMov();
    int setMov(int m);

    QGraphicsScene *scene;
    Tablero *tablero;
    Tablero *tanlero2;
    QString turno;
    QLineEdit *edit;
public slots:
    void menu();
    void inicia();
    void iniciam();
    void iniciamc();
    void preparam();
    void crear();
    void unirse();
    void bAzul();
    void bAmarillo();
    void bVerde();
    void bRojo();
    void bAzulm();
    void bAmarillom();
    void bVerdem();
    void bRojom();
private:
    void panel(int x, int y, int w, int h, QColor c, double o);
    void dibujarPanel(bool b);
    void dibujarPanelm(bool b);
    void dibujarBotones();
    void dibujarBotonesm();
    void gameover();
    void gana();
    void ganaste();
    void perdiste();
    void empate();
    bool ver();
    int mov;
    int lp;
    QGraphicsTextItem *movTxt;
};

#endif // JUEGO_H
