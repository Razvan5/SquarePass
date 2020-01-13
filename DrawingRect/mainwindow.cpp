#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QRectF>
#include <QFrame>

#define KING 11
#define QUEEN 12
#define TOWER 13
#define KNIGHT 14
#define BISHOP 15
#define PEON 16

#define EMPTY 1
#define WHITE 1
#define BLACK 2

#define CHECK_BLACK 3
#define CHECK_WHITE 4
#define CHECKMATE_BLACK 5
#define CHECKMATE_WHITE 6
#define TABLESIZE


int gameTableQT[10][10];
void initGameTable(int GT[10][10])
{
    for(int i=0;i<=10;i++)
        for(int j=0;j<=10;j++)
             GT[i][j]=0;

    for(int i=1;i<=8;i++){
        for(int j=1;j<=8;j++)
             GT[i][j]=EMPTY;
    }


    for(int i=1;i<=8;i++){
        GT[2][i]=PEON;
        GT[7][i]=PEON+10;
    }

    GT[1][1]=GT[1][8]=TOWER;
    GT[8][1]=GT[8][8]=TOWER+10;

    GT[1][2]=GT[1][7]=KNIGHT;
    GT[8][2]=GT[8][7]=KNIGHT+10;

    GT[1][3]=GT[1][6]=BISHOP;
    GT[8][3]=GT[8][6]=BISHOP+10;

    GT[1][4]=KING;
    GT[8][4]=KING+10;

    GT[1][5]=QUEEN;
    GT[8][5]=QUEEN+10;

}
int convertToIJ(int poz_x,int poz_y){

    if(poz_x>=100 && poz_y>=100 && poz_x<=900 && poz_y<=900){
            qDebug()<<poz_y/100<<' '<<poz_x/100<<endl;
    }

    return 0;
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    for(int i=1;i<=8;++i){
        for(int j=1;j<=8;++j){
            QLinearGradient b_gradient(0,0,0,100),w_gradient(0,0,0,100);
            b_gradient.setColorAt(0.0, Qt::gray);
            w_gradient.setColorAt(0.0,Qt::white);
            if(i%2==j%2){
                painter.fillRect(QRect(i*100,j*100,100,100),b_gradient);
            }
            else {
                painter.fillRect(QRect(i*100,j*100,100,100),w_gradient);
            }

            painter.drawRect(QRect(i*100,j*100,0,0));
      }
    }


    initGameTable(gameTableQT);

    for (int i = 1;i<=8;i++){

            for(int j = 1;j<=8;j++ ){

                switch(gameTableQT[j][i]){
                        case 11:{
                        QPixmap pixmap(":/images/king.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 12:{
                        QPixmap pixmap(":/images/queen.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 13:{
                        QPixmap pixmap(":/images/rook.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 14:{
                        QPixmap pixmap(":/images/horse.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 15:{
                        QPixmap pixmap(":/images/bishop.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 16:{
                        QPixmap pixmap(":/images/pawn.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;

                        case 21:{
                        QPixmap pixmap(":/images/king1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 22:{
                        QPixmap pixmap(":/images/queen1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 23:{
                        QPixmap pixmap(":/images/rook1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 24:{
                        QPixmap pixmap(":/images/horse1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 25:{
                        QPixmap pixmap(":/images/bishop1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                        break;
                        case 26:{
                        QPixmap pixmap(":/images/pawn1.png");
                        painter.drawPixmap(i*100,j*100,100,100,pixmap);
                        }
                    }

            }

        }




}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit Mouse_Pos();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->x = event->x();
    this->y = event->y();
    //qDebug()<<event->x()<<' '<<event->y()<<endl;
    convertToIJ(event->x(),event->y());
    emit Mouse_Pressed();
}

void MainWindow::leaveEvent(QEvent *)
{
    emit Mouse_Left();
}






