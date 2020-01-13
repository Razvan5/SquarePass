#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>

#define TABLESIZE 8

extern int errno;

int port;

void printMatrix(int GT[10][10]){

    printf("%s\n","  1 2 3 4 5 6 7 8");
    for (int i = 1;i<=TABLESIZE;i++){
        printf("%d ",i);
        for(int j = 1;j<=TABLESIZE;j++ ){
            /* if(GT[i][j]==EMPTY)
                printf(" "); */
            switch(GT[i][j]){
                case 11:{
                    printf("%s ","♔");
                }
                break;
                case 12:{
                    printf("%s ","♕");
                }
                break;
                case 13:{
                    printf("%s ","♖");
                }
                break;
                case 14:{
                    printf("%s ","♘");
                }
                break;
                case 15:{
                    printf("%s ","♗");
                }
                break;
                case 16:{
                    printf("%s ","♙");
                }
                break;

                case 21:{
                    printf("%s ","♚");
                }
                break;
                case 22:{
                    printf("%s ","♛");
                }
                break;
                case 23:{
                    printf("%s ","♜");
                }
                break;
                case 24:{
                    printf("%s ","♞");
                }
                break;
                case 25:{
                    printf("%s ","♝");
                }
                break;
                case 26:{
                    printf("%s ","♟");
                }
                break;
                case -1:{
                    printf("%s ","■");//just for debugging!;
                }
                break;
                default:{
                    printf("%d ",GT[i][j]);
                }//☐

            }
            
        }
        printf("\n");
    }

}

int main (int argc, char *argv[])
{
  int sd;		
  struct sockaddr_in server;	
  char msg[100];	
  int clientGameTable[10][10];	

  if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
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
          printMatrix(clientGameTable);
        }  

    }

  close (sd);
}
