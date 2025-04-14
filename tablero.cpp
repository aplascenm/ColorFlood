#include "tablero.h"
#include "juego.h"
#include <QGraphicsTextItem>

extern Juego *juego;

Tablero::Tablero()
{

}


QList<Cuadro *> Tablero::getCuadros()
{
    return cuadros;
}

int Tablero::getColor()
{
    return matriz.at(0)->color;
}

void Tablero::colocarCuadros(int x, int y, int c, int f)
{
    int X=50;
    //Grid
    for(size_t i=0, n=c; i<n; i++)
    {
        crearColumna(x+X*i,y,f);
    }
}

void Tablero::crearColumna(int x, int y, int n)
{
    //crea varias columnas
    for(size_t i=0,num=n; i<num; i++)
    {
        Cuadro *cuadro=new Cuadro();
        cuadro->setPos(x,y+50*i);
        cuadros.append(cuadro);
        juego->scene->addItem(cuadro);
    }
}

void Tablero::crearMatriz(int x, int y, int z){
    tam=z;
    for(int i=0; i<z; i++)
    {
        for(int j=0; j<z; j++)
        {
            Cuadro *cuadro=new Cuadro();
            //c[i][j].setPos(x+50*i,y+50*j);
            //juego->scene->addItem(c);
            cuadro->setPos(x+50*i,y+50*j);
            if(i==0&&j==0)
            {
                matriz.append(cuadro);
                matriz.at(0)->flag=true;
            }else
            {
                matriz.append(cuadro);
            }
            //juego->scene->addItem(cuadro);
        }
    }
}

void Tablero::crearMatrizm(int x, int y, int z, char s[]){
    tam=z;
    int cont=2, n;
    for(int i=0; i<z; i++)
    {
        for(int j=0; j<z; j++)
        {
            Cuadro *cuadro=new Cuadro();
            n=s[cont]-'0';
            cuadro->setPos(x+50*i,y+50*j);
            cuadro->cambiac(n);
            if(i==0&&j==0)
            {
                matriz.append(cuadro);
                matriz.at(0)->flag=true;
                cont++;
            }else
            {
                matriz.append(cuadro);
                cont++;
            }
            //juego->scene->addItem(cuadro);
        }
    }
}

void Tablero::cambiarColor(int c)
{
    //Cambia color respecto al boton
    if(c==0)
    {
        //azul
        for(int i=0;i<tam*tam;i++){
            if(matriz.at(i)->flag==true)
            {
                matriz.at(i)->setBrush(Qt::blue);
                matriz.at(i)->color=0;
            }
        }
    }else if(c==1)
    {
        //verde
        for(int i=0;i<tam*tam;i++){
            if(matriz.at(i)->flag==true)
            {
                matriz.at(i)->setBrush(Qt::green);
                matriz.at(i)->color=1;
            }
        }
    }else if(c==2)
    {
        //amarillo
        for(int i=0;i<tam*tam;i++){
            if(matriz.at(i)->flag==true)
            {
                matriz.at(i)->setBrush(Qt::yellow);
                matriz.at(i)->color=2;
            }
        }
    }else if(c==3)
    {
        //rojo
        for(int i=0;i<tam*tam;i++){
            if(matriz.at(i)->flag==true)
            {
                matriz.at(i)->setBrush(Qt::red);
                matriz.at(i)->color=3;
            }
        }
    }
}

void Tablero::muestraTam(){
    QGraphicsTextItem *movTxt = new QGraphicsTextItem();
    movTxt->setPlainText(QString::number(matriz.size()));
    QFont fmov("times",20);
    movTxt->setFont(fmov);
    movTxt->setPos(500,10);
    juego->scene->addItem(movTxt);
}

void Tablero::mostrar()
{
    for(int i=0; i<tam*tam; i++){
        juego->scene->addItem(matriz.at(i));
    }
}

void Tablero::quitar()
{
    for(int i=0; i<tam*tam; i++){
        juego->scene->removeItem(matriz.at(i));
    }
}

void Tablero::actualiza()
{
    /*
    QGraphicsTextItem *movTxt = new QGraphicsTextItem();
    QFont fmov("times",20);
    movTxt->setFont(fmov);
    movTxt->setPlainText("Entre1");
    juego->scene->addItem(movTxt);*/
    int i=1;
    for(i; i<(tam*tam); i++)
    {
        if(i<tam-1){
            if((matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i+1)->color&&matriz.at(i+1)->flag==true)||(matriz.at(i)->color==matriz.at(i+tam)->color&&matriz.at(i+tam)->flag==true)){
               matriz.at(i)->flag=true;
            }
        }else if((i>=tam-1&&i<(tam*tam)-tam)&&((i+1)%tam==0)){
            if(i==tam-1){
                if((matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i+tam)->color&&matriz.at(i+tam)->flag==true)){
                    matriz.at(i)->flag=true;
                }
            }else{
                if((matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)||(matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i+tam)->color&&matriz.at(i+tam)->flag==true)){
                    matriz.at(i)->flag=true;
                }
            }
        }else if((i>=tam-1&&i<(tam*tam)-tam)&&(i%tam==0)){
            if((matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)||(matriz.at(i)->color==matriz.at(i+1)->color&&matriz.at(i+1)->flag==true)||(matriz.at(i)->color==matriz.at(i+tam)->color&&matriz.at(i+tam)->flag==true)){
                matriz.at(i)->flag=true;
            }
        }else if((i>tam&&i<(tam*tam)-tam)&&(i%tam!=0)&&((i+1)%tam!=0)){
            if((matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)||(matriz.at(i)->color==matriz.at(i+1)->color&&matriz.at(i+1)->flag==true)||(matriz.at(i)->color==matriz.at(i+tam)->color&&matriz.at(i+tam)->flag==true)){
                matriz.at(i)->flag=true;
            }
        }else if((i>=(tam*tam)-tam)){
            if(i==(tam*tam)-tam){
                if((matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)||(matriz.at(i)->color==matriz.at(i+1)->color&&matriz.at(i+1)->flag==true)){
                    matriz.at(i)->flag=true;
                }
            }else if(i==tam*tam-1){
                if((matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)){
                    matriz.at(i)->flag=true;
                }
            }else{
                if((matriz.at(i)->color==matriz.at(i-1)->color&&matriz.at(i-1)->flag==true)||(matriz.at(i)->color==matriz.at(i-tam)->color&&matriz.at(i-tam)->flag==true)||(matriz.at(i)->color==matriz.at(i+1)->color&&matriz.at(i+1)->flag==true)){
                    matriz.at(i)->flag=true;
                }
            }
        }
    }
}

bool Tablero::verificaf()
{
    bool v=false;
    int c=0;
    for(int i=0; i<tam*tam; i++){
        if(matriz.at(i)->flag==true){
            c++;
        }
    }
    if(c==tam*tam-1){
        v=true;
    }
    return v;
}

bool Tablero::verificac()
{
    bool v=false;
    for(int i=0; i<tam*tam-1; i++){
        if(matriz.at(i)->color==matriz.at(i+1)->color){
            v=true;
        }else{
            v=false;
            break;
        }
    }
    return v;
}
