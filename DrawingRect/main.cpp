#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QRectF>
#include <QFrame>
#include <QDebug>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>

#define TABLESIZE 8
#define PORT 2728

extern int errno;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.setGeometry (0, 0, 1000, 1000);

    for(int i=1;i<=8;++i){
        for(int j=1;j<=8;++j){
            QLabel *label=new QLabel(" ", &w);
            label->setGeometry (i*100, j*100, 100, 100);
            label->show();
        }
    }

    int sd;
    struct sockaddr_in server;
    char msg[100];
    int clientGameTable[10][10];

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
      {
        perror ("[client] Eroare la socket().\n");
        return errno;
      }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (PORT);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
      {
        perror ("[client]Eroare la connect().\n");
        return errno;
      }
   int pid;
   while(1)
      {
          if((pid=fork())==-1){
            printf("eroare fork");
            close (sd);

          }
          if(pid==0){

            bzero (msg, 100);
            fflush (stdout);
            read (0, msg, 100);

            if (write (sd, msg, 100) <= 0)
            {
              perror ("[client]Eroare la write() spre server.\n");
              return errno;
            }

          }
          else
          {
            if (read (sd, clientGameTable, 100*sizeof(int)) < 0)
              {
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
                close (sd);
              }
            printf ("[client]Mesajul primit este: %s\n", msg);
            if(clientGameTable[9][9]==1){
              printf("White player Turn\n");
            }
            else if(clientGameTable[9][9]==2){
              printf("Black player Turn\n");
            }

          }

      }

    close (sd);
    /*
    QLabel test1("AAAAAAAAAAAAAAA", &w);
    test1.setGeometry (100, 100, 100, 100);
    test1.show();
    QLabel b("BBB", &w);
    b.setGeometry (200, 200, 100, 100);
    b.show();
    */

    return a.exec();
}
