#include "juego.h"
#include "tablero.h"
#include "boton.h"
#include <QGraphicsTextItem>
#include <QLineEdit>
#include <iostream>
#include <string.h>
#include <stdio.h>
//Sockets
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
//IP
#include <sys/ioctl.h>
#include <net/if.h>
//NoB
#include <fcntl.h>
#include <errno.h>



using namespace std;

Juego::Juego(QWidget *parent)//constructor del juego, agrega el scene
{
    //pantalla
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800,600);
    //scene
    scene=new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);
    setScene(scene);
}

void Juego::unirse(){//Opcion unirse 2J
    //limpiar
    scene->clear();
    //Ingresar
    QGraphicsTextItem *aviso = new QGraphicsTextItem(QString("Ingresa ip del otro Jugador:"));
    int axPos = this->width()/3 -aviso->boundingRect().width()/2;
    int ayPos = 100;
    QFont atitulo("times",35);
    aviso->setFont(atitulo);
    aviso->setPos(axPos, ayPos);
    scene->addItem(aviso);

    //QLineEdit
    //-No eliminar- QLineEdit *edit = new QLineEdit();
    edit=new QLineEdit();
    edit->setGeometry(QRect(300,200,200,30));
    scene->addWidget(edit);

    //Unirse
    Boton *unir = new Boton(QString("Unirse"), 200, 50, Qt::darkCyan, Qt::cyan);
    int uxPos = this->width()/2 -unir->boundingRect().width()/2;
    int uyPos = 250;
    unir->setPos(uxPos,uyPos);
    connect(unir,SIGNAL(clicked()),this,SLOT(iniciamc()));
    scene->addItem(unir);

    //Cancelar
    Boton *men = new Boton(QString("Cancelar!"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -men->boundingRect().width()/2;
    int syPos = 325;
    men->setPos(sxPos,syPos);
    connect(men,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(men);
}

void Juego::crear(){//Opcion Crear 2J
    //limpiar
    scene->clear();
    //IP
    int fdi;
    struct ifreq ifr;
    fdi=socket(AF_INET, SOCK_DGRAM, 0);
    //obtener ipv4
    ifr.ifr_addr.sa_family = AF_INET;
    //IP en wlan0
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    ioctl(fdi, SIOCGIFADDR, &ifr);
    ::close(fdi);

    //Aviso
    QGraphicsTextItem *aviso = new QGraphicsTextItem(QString("Entrega esta ip a el otro Jugador:"));
    int axPos = this->width()/4 -aviso->boundingRect().width()/2;
    int ayPos = 100;
    QFont atitulo("times",35);
    aviso->setFont(atitulo);
    aviso->setPos(axPos, ayPos);
    scene->addItem(aviso);

    //IP
    QGraphicsTextItem *ip = new QGraphicsTextItem(QString(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)));
    int ixPos = this->width()/3 - ip->boundingRect().width()/2;
    int iyPos = 200;
    QFont ititulo("times",50);
    ip->setFont(ititulo);
    ip->setPos(ixPos, iyPos);
    scene->addItem(ip);

    //Iniciar
    Boton *in = new Boton(QString("Iniciar"), 200, 50, Qt::darkCyan, Qt::cyan);
    int inxPos = this->width()/2 -in->boundingRect().width()/2;
    int inyPos = 300;
    in->setPos(inxPos,inyPos);
    connect(in,SIGNAL(clicked()),this,SLOT(iniciam()));
    scene->addItem(in);

    //Cancelar
    Boton *men = new Boton(QString("Abortar!"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -men->boundingRect().width()/2;
    int syPos = 375;
    men->setPos(sxPos,syPos);
    connect(men,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(men);
}

void Juego::iniciam(){//Inicia 2J servidor
    //limpiar
    scene->clear();
    //
    //-INICIA-Serv
    int s;
    s=socket(AF_INET, SOCK_STREAM, 0);
    if(s<0)
    {
        scene->clear();
        Boton *error = new Boton(QString("No se creo socket"), 200, 50, Qt::darkCyan, Qt::cyan);
        int sxPos = this->width()/3 -error->boundingRect().width()/2;
        int syPos = 300;
        error->setPos(sxPos,syPos);
        connect(error,SIGNAL(clicked()),this,SLOT(preparam()));
        scene->addItem(error);
    }
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(1025);
    serv.sin_addr.s_addr=INADDR_ANY;
    int res;
    res=bind(s, (struct sockaddr*)&serv, sizeof(serv));
    if(res<0)
    {
        scene->clear();
        Boton *error = new Boton(QString("Error Bind"), 200, 50, Qt::darkCyan, Qt::cyan);
        int sxPos = this->width()/2 -error->boundingRect().width()/2;
        int syPos = 300;
        error->setPos(sxPos,syPos);
        connect(error,SIGNAL(clicked()),this,SLOT(preparam()));
        scene->addItem(error);
    }
    //NoB
    int flags;
    flags=fcntl(s, F_GETFL);
    flags=flags|O_NONBLOCK;
    fcntl(s, F_SETFL, flags);
    //
    res=listen(s, 5);
    if(res<0)
    {
        scene->clear();
        Boton *error = new Boton(QString("Error listen"), 200, 50, Qt::darkCyan, Qt::cyan);
        int sxPos = this->width()/2 -error->boundingRect().width()/2;
        int syPos = 300;
        error->setPos(sxPos,syPos);
        connect(error,SIGNAL(clicked()),this,SLOT(preparam()));
        scene->addItem(error);
    }

    int c;
    char buffer[100];
    bool v=false;

    do{
        c=accept(s, NULL, NULL);
        if(c>0)
        {
            res=read(c, buffer, 100);
            if(res<0)
            {
                cout<<endl<<"Error Trying to Write";
            }else{
                if(buffer[0]=='1'){
                    if(buffer[1]=='1'&&res==2){
                        //Cliente listo envia y crea matriz
                        //limpiar
                        scene->clear();
                        //Crea Tablero
                        tablero = new Tablero();
                        //tablero->colocarCuadros(250,100,6,6);
                        tablero->crearMatriz(250,100,6);
                        //color del primero
                        lp=tablero->getColor();
                        //Muestra Tablero
                        tablero->mostrar();
                        //verifica
                        tablero->verificac();
                        //movimientos en 0;
                        mov=-1;
                        //panel+mov
                        dibujarPanelm(true);
                        //botones
                        dibujarBotonesm();
                        //Envia matriz
                        QString m;
                        m.append("22");
                        for(int i=0; i<36; i++){
                            m.append(QString::number(tablero->matriz.at(i)->color));
                        }
                        string mm;
                        mm=m.toStdString();
                        do{
                            res=write(c,mm.c_str(),38);
                            if(res>0){
                                cout<<"ok";
                                break;
                            }
                        }while(1);

                        break;

                    }
                }
            }
            if(v){
                break;
            }

        }else if(c<0){
            if(errno==EAGAIN||errno==EWOULDBLOCK)
            {
                //cout<<"Esperando Cliente.";
            }else{
                cout<<endl<<"Fatal Error";
            }
        }
    }while(1);

    ::close(s);
    ::close(c);
}

void Juego::iniciamc(){//Inicia Juego 2J Cliente
    QString ip=edit->text();
    //limpiar
    scene->clear();
    //Conexion
    int s,res;
    s=socket(AF_INET, SOCK_STREAM, 0);
    if(s<0)
    {
        scene->clear();
        Boton *error = new Boton(QString("Error Socket"), 200, 50, Qt::darkCyan, Qt::cyan);
        int sxPos = this->width()/2 -error->boundingRect().width()/2;
        int syPos = 300;
        error->setPos(sxPos,syPos);
        connect(error,SIGNAL(clicked()),this,SLOT(preparam()));
        scene->addItem(error);
    }
    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_port=htons(1025);
    client.sin_addr.s_addr=inet_addr(ip.toStdString().c_str());
    //
    res=::connect(s, (struct sockaddr*)&client, sizeof(client));
    if(res<0){
        scene->clear();
        Boton *error = new Boton(QString("Error Connect"), 200, 50, Qt::darkCyan, Qt::cyan);
        int sxPos = this->width()/2 -error->boundingRect().width()/2;
        int syPos = 300;
        error->setPos(sxPos,syPos);
        connect(error,SIGNAL(clicked()),this,SLOT(preparam()));
        scene->addItem(error);
    }
    int flags;
    flags=fcntl(s,F_GETFL);
    flags=flags|O_NONBLOCK;
    fcntl(s,F_SETFL,flags);
    char buffer[100];
    res=write(s, "11", 2);
    if(res>0){
        cout<<endl<<"Ok";
    }else if(res==0){
        cout<<endl<<"Cerraron la conexion.";
    }else{
        if(errno==EAGAIN||errno==EWOULDBLOCK){
            cout<<endl<<"No hay datos.";
        }else{
            cout<<endl<<"ERROR";
        }
    }

    do{
        res=read(s,buffer,100);
        if(res==38){
            if(buffer[0]=='2'){
                if(buffer[1]=='2'){
                    break;
                }
            }
        }
    }while(1);

    //Serv envia matriz
    //limpiar
    scene->clear();
    //Crea Tablero
    tablero = new Tablero();
    tablero->crearMatrizm(250,100,6,buffer);
    //color del primero
    lp=tablero->getColor();
    //Muestra Tablero
    tablero->mostrar();
    //verifica
    tablero->verificac();
    //movimientos en 0;
    mov=-1;
    //panel+mov
    dibujarPanelm(true);
    //botones
    dibujarBotonesm();

    if(ver()||mov==9){
        QString m;
        string mm;
        m.append("33");
        m.append(QString::number(mov));
        for(int i=0; i<36; i++){
            m.append(QString::number(tablero->matriz.at(i)->color));
        }
        mm=m.toStdString();
        do{
            res=write(s,mm.c_str(),39);
            if(res>0){
                cout<<"Envio 33";
                break;
            }
        }while(1);

        do{
            res=read(s,buffer,100);
            if(res>0){
                if(buffer[0]=='3'){
                    if(buffer[1]=='3'){
                        cout<<"reci 33";
                        break;
                    }
                }
            }
        }while(1);
    }


    ::close(s);
}

void Juego::preparam()//Menu Multijugador
{
    //limpiar
    scene->clear();
    //Elegir
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Elige una Opcion:"));
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Crear
    Boton *crea = new Boton(QString("Crear Partida"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPos = this->width()/2 -crea->boundingRect().width()/2;
    int jyPos = 275;
    crea->setPos(jxPos, jyPos);
    connect(crea,SIGNAL(clicked()),this,SLOT(crear()));
    scene->addItem(crea);

    //Unirse
    Boton *unir = new Boton(QString("Unirse a Partida"),200, 50, Qt::darkCyan, Qt::cyan);
    int uxPos = this->width()/2 -unir->boundingRect().width()/2;
    int uyPos = 350;
    unir->setPos(uxPos, uyPos);
    connect(unir,SIGNAL(clicked()),this,SLOT(unirse()));
    scene->addItem(unir);

    //Menu
    Boton *men = new Boton(QString("Ir a menu"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -men->boundingRect().width()/2;
    int syPos = 425;
    men->setPos(sxPos,syPos);
    connect(men,SIGNAL(clicked()),this,SLOT(menu()));
    scene->addItem(men);
}

void Juego::inicia()//Inicia 1J
{
    //limpiar
    scene->clear();
    //Crea Tablero
    tablero = new Tablero();
    //tablero->colocarCuadros(250,100,6,6);
    tablero->crearMatriz(250,100,6);
    //color del primero
    lp=tablero->getColor();
    //Muestra Tablero
    tablero->mostrar();
    //verifica
    tablero->verificac();
    //movimientos en 0;
    mov=-1;
    //panel+mov
    dibujarPanel(true);
    //botones
    dibujarBotones();
    //movimientos
}

void Juego::bAzul()//clic boton azul 1J
{
    if(lp!=0){
        lp=0;
        tablero->actualiza();
        //dibujarPanel(false);
        tablero->cambiarColor(0);
        tablero->actualiza();
        dibujarPanel(false);
    }
}
void Juego::bVerde()//clic boton verde 1J
{
    if(lp!=1){
        lp=1;
        tablero->actualiza();
        //dibujarPanel(false);
        tablero->cambiarColor(1);
        tablero->actualiza();
        dibujarPanel(false);
    }
}
void Juego::bAmarillo()//clic boton amarillo 1J
{
    if(lp!=2)
    {
        lp=2;
        tablero->actualiza();
        //dibujarPanel(false);
        tablero->cambiarColor(2);
        tablero->actualiza();
        dibujarPanel(false);
    }
}
void Juego::bRojo()//clic boton rojo 1J
{
    if(lp!=3){
        lp=3;
        tablero->actualiza();
        //dibujarPanel(false);
        tablero->cambiarColor(3);
        tablero->actualiza();
        dibujarPanel(false);
    }
}

void Juego::bAzulm()//clic boton azul 2J
{
    if(mov<10&&!ver()){
        if(lp!=0){
            lp=0;
            tablero->actualiza();
            //dibujarPanel(false);
            tablero->cambiarColor(0);
            tablero->actualiza();
            dibujarPanelm(false);
        }
    }
}
void Juego::bVerdem()//clic boton verde 2J
{
    if(mov<10&&!ver()){
        if(lp!=1){
            lp=1;
            tablero->actualiza();
            //dibujarPanel(false);
            tablero->cambiarColor(1);
            tablero->actualiza();
            dibujarPanelm(false);
        }
    }
}
void Juego::bAmarillom()//clic boton amarillo 2J
{
    if(mov<10&&!ver()){
        if(lp!=2)
        {
            lp=2;
            tablero->actualiza();
            //dibujarPanel(false);
            tablero->cambiarColor(2);
            tablero->actualiza();
            dibujarPanelm(false);
        }
    }
}
void Juego::bRojom()//clic boton rojo 2J
{
    if(mov<10&&!ver()){
        if(lp!=3){
            lp=3;
            tablero->actualiza();
            //dibujarPanel(false);
            tablero->cambiarColor(3);
            tablero->actualiza();
            dibujarPanelm(false);
        }
    }
}

void Juego::panel(int x, int y, int w, int h, QColor c, double o)//crea panel
{
    //dibujar
    QGraphicsRectItem *panel = new QGraphicsRectItem(x,y,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(c);
    panel->setBrush(brush);
    panel->setOpacity(o);
    scene->addItem(panel);
}

void Juego::dibujarPanel(bool b)//dibuja el panel, muestra turno valida gana/pierde 1J
{
    if(b)
    {
        //dibujar panel abajo
        panel(250,450,300,70,Qt::gray,0.5);

        //mostrar turno
        movTxt = new QGraphicsTextItem();
        setMov(mov-1);
        QFont fmov("times",20);
        movTxt->setFont(fmov);
        movTxt->setPos(380,10);
        scene->addItem(movTxt);
    }else
    {
        int m;
        scene->removeItem(movTxt);
        m=setMov(mov-1);
        QFont fmov("times",20);
        movTxt->setFont(fmov);
        movTxt->setPos(380,10);
        scene->addItem(movTxt);
        if(m==10)
        {
            if(ver()){
                gana();
            }else{
                gameover();
            }
        }else if(ver())
        {
            gana();
        }
    }

}

void Juego::dibujarPanelm(bool b)//dibuja el panel, muestra turno valida gana/pierde 2J
{
    if(b)
    {
        //dibujar panel abajo
        panel(250,450,300,70,Qt::gray,0.5);

        //mostrar turno
        movTxt = new QGraphicsTextItem();
        setMov(mov-1);
        QFont fmov("times",20);
        movTxt->setFont(fmov);
        movTxt->setPos(380,10);
        scene->addItem(movTxt);
    }else
    {
        int m;
        scene->removeItem(movTxt);
        m=setMov(mov-1);
        QFont fmov("times",20);
        movTxt->setFont(fmov);
        movTxt->setPos(380,10);
        scene->addItem(movTxt);
    }

}


void Juego::dibujarBotones()//Dibuja los botones de color en el panel inferior 1J
{
    //azul
    Boton *azul = new Boton(QString(""),50,50,Qt::darkBlue, Qt::blue);
    azul->setPos(270, 460);
    connect(azul,SIGNAL(clicked()),this,SLOT(bAzul()));
    scene->addItem(azul);
    //verde
    Boton *verde = new Boton(QString(""),50,50,Qt::darkGreen, Qt::green);
    verde->setPos(340, 460);
    connect(verde,SIGNAL(clicked()),this,SLOT(bVerde()));
    scene->addItem(verde);
    //amarillo
    Boton *amarillo = new Boton(QString(""),50,50,Qt::darkYellow, Qt::yellow);
    amarillo->setPos(410, 460);
    connect(amarillo,SIGNAL(clicked()),this,SLOT(bAmarillo()));
    scene->addItem(amarillo);
    //rojo
    Boton *rojo = new Boton(QString(""),50,50,Qt::darkRed, Qt::red);
    rojo->setPos(480, 460);
    connect(rojo,SIGNAL(clicked()),this,SLOT(bRojo()));
    scene->addItem(rojo);
}

void Juego::dibujarBotonesm()//Dibuja los botones de color en el panel inferior 2J
{
    //azul
    Boton *azul = new Boton(QString(""),50,50,Qt::darkBlue, Qt::blue);
    azul->setPos(270, 460);
    connect(azul,SIGNAL(clicked()),this,SLOT(bAzulm()));
    scene->addItem(azul);
    //verde
    Boton *verde = new Boton(QString(""),50,50,Qt::darkGreen, Qt::green);
    verde->setPos(340, 460);
    connect(verde,SIGNAL(clicked()),this,SLOT(bVerdem()));
    scene->addItem(verde);
    //amarillo
    Boton *amarillo = new Boton(QString(""),50,50,Qt::darkYellow, Qt::yellow);
    amarillo->setPos(410, 460);
    connect(amarillo,SIGNAL(clicked()),this,SLOT(bAmarillom()));
    scene->addItem(amarillo);
    //rojo
    Boton *rojo = new Boton(QString(""),50,50,Qt::darkRed, Qt::red);
    rojo->setPos(480, 460);
    connect(rojo,SIGNAL(clicked()),this,SLOT(bRojom()));
    scene->addItem(rojo);
}

void Juego::gameover()//Pierde
{
    scene->clear();
    //Perdiste
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Perdiste! 10/10"));
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Intentar de Nuevo
    Boton *jugar = new Boton(QString("Intentar de nuevo 1J"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPos = this->width()/2 -jugar->boundingRect().width()/2;
    int jyPos = 275;
    jugar->setPos(jxPos, jyPos);
    connect(jugar,SIGNAL(clicked()),this,SLOT(inicia()));
    scene->addItem(jugar);

    //Salir
    Boton *salir = new Boton(QString("Salir :v"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -salir->boundingRect().width()/2;
    int syPos = 350;
    salir->setPos(sxPos,syPos);
    connect(salir,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(salir);
}

void Juego::gana()//gana
{
    scene->clear();
    //Ganaste
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Ganaste! ")+QString::number(mov)+"/10");
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Intentar de Nuevo
    Boton *jugar = new Boton(QString("Intentar de nuevo 1J"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPos = this->width()/2 -jugar->boundingRect().width()/2;
    int jyPos = 275;
    jugar->setPos(jxPos, jyPos);
    connect(jugar,SIGNAL(clicked()),this,SLOT(inicia()));
    scene->addItem(jugar);

    //Salir
    Boton *salir = new Boton(QString("Salir :v"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -salir->boundingRect().width()/2;
    int syPos = 350;
    salir->setPos(sxPos,syPos);
    connect(salir,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(salir);
}

void Juego::ganaste(){//Gana 2J
    scene->clear();
    //Ganaste
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Ganaste! ")+QString::number(mov)+"/10");
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Regresar
    Boton *jugar = new Boton(QString("Regresar!"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPos = this->width()/2 -jugar->boundingRect().width()/2;
    int jyPos = 275;
    jugar->setPos(jxPos, jyPos);
    connect(jugar,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(jugar);

}

void Juego::perdiste(){//Pierde 2J
    scene->clear();
    //Empate
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Perdiste! ")+QString::number(mov)+"/10");
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Salir
    Boton *salir = new Boton(QString("Regresar"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -salir->boundingRect().width()/2;
    int syPos = 350;
    salir->setPos(sxPos,syPos);
    connect(salir,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(salir);
}

void Juego::empate(){//Empate 2J
    scene->clear();
    //Empate
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Empate! ")+QString::number(mov)+"/10");
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Salir
    Boton *salir = new Boton(QString("Regresar"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -salir->boundingRect().width()/2;
    int syPos = 350;
    salir->setPos(sxPos,syPos);
    connect(salir,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(salir);
}

bool Juego::ver()//verifica la matriz
{
    //verifica si todos son del mismo color
    //return tablero->verificaf();
    return tablero->verificac();
}

void Juego::menu()//Muestra el menu principal
{
    scene->clear();
    //crear titulo
    QGraphicsTextItem *titulo = new QGraphicsTextItem(QString("Color Flood"));
    int txPos = this->width()/3 - titulo->boundingRect().width()/2;
    int tyPos = 150;
    QFont ftitulo("times",50);
    titulo->setFont(ftitulo);
    titulo->setPos(txPos, tyPos);
    scene->addItem(titulo);

    //Jugar 1J
    Boton *jugar = new Boton(QString("Jugar 1J"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPos = this->width()/2 -jugar->boundingRect().width()/2;
    int jyPos = 275;
    jugar->setPos(jxPos, jyPos);
    connect(jugar,SIGNAL(clicked()),this,SLOT(inicia()));
    scene->addItem(jugar);

    //Jugar Multijugador
    Boton *jugarm = new Boton(QString("Jugar 2J"),200, 50, Qt::darkCyan, Qt::cyan);
    int jxPosm = this->width()/2 -jugarm->boundingRect().width()/2;
    int jyPosm = 350;
    jugarm->setPos(jxPosm, jyPosm);
    connect(jugarm,SIGNAL(clicked()),this,SLOT(preparam()));
    scene->addItem(jugarm);

    //salir
    Boton *salir = new Boton(QString("Salir :v"), 200, 50, Qt::darkCyan, Qt::cyan);
    int sxPos = this->width()/2 -salir->boundingRect().width()/2;
    int syPos = 425;
    salir->setPos(sxPos,syPos);
    connect(salir,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(salir);
}

int Juego::getMov()//retorna mov
{
    return mov;
}

int Juego::setMov(int m)//aumenta mov
{
    //aumenta numero
    mov++;
    m=mov;
    //Texto
    movTxt->setPlainText(QString::number(m)+"/10");
    return m;
}
