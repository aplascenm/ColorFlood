#ifndef TABLERO_H
#define TABLERO_H

#include <QList>
#include "cuadro.h"


class Tablero
{
public:
    Tablero();

    QList<Cuadro*>getCuadros();
    int getColor();

    void colocarCuadros(int x, int y, int c, int f);
    void cambiarColor(int c);
    void crearMatriz(int x, int y, int z);
    void crearMatrizm(int x, int y, int z, char s[]);
    void muestraTam();
    void mostrar();
    void quitar();
    void actualiza();
    bool verificaf();
    bool verificac();

    //Cuadro c[6][6];
public:
    QList<Cuadro*> matriz;
private:
    void crearColumna(int x, int y, int n);
    QList<Cuadro*> cuadros;

    int tam;
};

#endif // TABLERO_H
