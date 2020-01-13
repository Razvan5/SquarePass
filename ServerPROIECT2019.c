#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "chess.h"
#include "sqlite3.h"

#define PORT 2728
#define FD_NOT_CONNECTED -2
#define FD_NOT_PAIRED -1
#define MAX_PAIRS_NUMBER 100

int TurnDecider[100]={[0 ... 99]=0};
int MatrixSharing[100]={[0 ... 99]=0};
int pairing_vector[100];
int CurrentTurn=0;

extern int errno;		

char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  strcpy (str, inet_ntoa (address.sin_addr));	
  
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}
int PlayerTurn[100]={[0 ... 99]=1};
int previosStates[100][10][10];

int manageGames(int GT[10][10],int fd,int playerTurn)
{
  char buffer[100];		
  int bytes;			
  char msg[100];		
  char msgrasp[100]=" ";

  if(PlayerTurn[fd]==1){
      bytes = read (fd, msg, sizeof (buffer));

      if (bytes < 0){
          perror ("Eroare la read() de la client.\n");
          return 0;
        }
    
        printf ("[server]Mesajul a fost receptionat...%s\n", msg);
        
        int nr=0;
        int x1=0,y1=0,x2=0,y2=0;
        if(strlen(msg)==5 && atoi(msg)!=0){
            int nr = atoi (msg);
            x1=nr/1000;
            y1=(nr/100)%10;
            x2=(nr/10)%10;
            y2=nr%10;
        } 
        
         
        printf("%d\n%d\n%d\n%d\n%d\n",nr,x1,y1,x2,y2);
        int verif;
            if((verif=confirmSelect(GT,x1,y1,x2,y2,PlayerTurn[fd]))==CHECKMATE_BLACK){
                            printf("ceva 5?");
                            return -5;
            }  
            else{printf("VERIF:%d\n",verif);
              if((verif=confirmSelect(GT,x1,y1,x2,y2,PlayerTurn[fd]))==CHECKMATE_WHITE){
              printf("ceva 6?");
              return -6;
               }
            } 

            for(int i=1;i<=TABLESIZE;i++){
              for(int j=1;j<=TABLESIZE;j++){
                if(GT[i][j]!=previosStates[MatrixSharing[fd]][i][j]){
                    PlayerTurn[fd]=2;
                    GT[9][9]=2;
                    if(fd%2==0){
                        for(int k=1;k<=TABLESIZE;k++){
                          for(int l=1;l<=TABLESIZE;l++){
                        previosStates[MatrixSharing[fd]][i][j]=GT[i][j];
                      }
                    }
                  } 
                }
              }
            }     
        //am modificat
        //printMatrix(GT);

        printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
              
            if (bytes && (write (pairing_vector[fd], GT, 100*sizeof(int)) < 0 || write (fd, GT, 100*sizeof(int))<0)){
              perror ("[server] Eroare la write() catre client.\n");
              return 0;
            }
  }
  else if(PlayerTurn[fd]==2){

    bytes = read (fd, msg, sizeof (buffer));

      if (bytes < 0){
          perror ("Eroare la read() de la client.\n");
          return 0;
        }
    
        printf ("[server]Mesajul a fost receptionat...%s\n", msg);
        
        int nr=0;
        int x1=0,y1=0,x2=0,y2=0;
        if(strlen(msg)==5 && atoi(msg)!=0){
            int nr = atoi (msg);
            x1=nr/1000;
            y1=(nr/100)%10;
            x2=(nr/10)%10;
            y2=nr%10;
        } 
         
        printf("%d\n%d\n%d\n%d\n%d\n",nr,x1,y1,x2,y2);
        int verif;
            if((verif=confirmSelect(GT,x1,y1,x2,y2,PlayerTurn[fd]))==CHECKMATE_BLACK){
                            printf("ceva 5?");
                            return -5;
            }  
            else{printf("VERIF:%d\n",verif);
              if((verif=confirmSelect(GT,x1,y1,x2,y2,PlayerTurn[fd]))==CHECKMATE_WHITE){
              printf("ceva 6?");
              return -6;
               }
            } 

            for(int i=1;i<=TABLESIZE;i++){
              for(int j=1;j<=TABLESIZE;j++){
                if(GT[i][j]!=previosStates[MatrixSharing[fd]][i][j]){
                    PlayerTurn[fd]=1;
                    GT[9][9]=1;
                    if(fd%2==0){
                      for(int k=1;k<=TABLESIZE;k++){
                        for(int l=1;l<=TABLESIZE;l++){
                          previosStates[MatrixSharing[fd]][i][j]=GT[i][j];
                        }
                      }
                    }
                }
              }
            }     
        //am modificat
        //printMatrix(GT);

        printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
              
            if (bytes && (write (pairing_vector[fd], GT, 100*sizeof(int)) < 0 || write (fd, GT, 100*sizeof(int))<0)){
              perror ("[server] Eroare la write() catre client.\n");
              return 0;
            }

  }
  
  return bytes;
}

int currentEmptyGT=1;

int main ()
{
  struct sockaddr_in server;	
  struct sockaddr_in from;
  fd_set readfds;		
  fd_set actfds;	
  struct timeval tv;		
  int sd, client;		
  int optval=1; 			
  int fd;			
              
  int nfds;		
  int len;			
  
   int gameTableCopy[10][10];
   int multipleGameTable[100][10][10];
   int mainPlayerTurn=WHITE;
   initGameTable(gameTableCopy);

  for(int i=0;i<=MAX_PAIRS_NUMBER;i++){
    initGameTable(multipleGameTable[i]);
    initGameTable(previosStates[i]);
    PlayerTurn[i]=1;
    //printMatrix(multipleGameTable[i]);
  }

  for(int k=0;k<=MAX_PAIRS_NUMBER;k++){
	  pairing_vector[k]=FD_NOT_CONNECTED;
  }
  
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

  
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  
  bzero (&server, sizeof (server));

  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

 
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  
  if (listen (sd, 6) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
  
  
  FD_ZERO (&actfds);	
  FD_SET (sd, &actfds);	

  tv.tv_sec = 1;		
  tv.tv_usec = 0;
  
  
  nfds = sd;

  printf ("[server] Asteptam la portul %d...\n", PORT);
  fflush (stdout);
  

  
  while (1)
   {
        bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

        if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0){
            perror ("[server] Eroare la select().\n");
            return errno;
         }
        if (FD_ISSET (sd, &readfds)){
                len = sizeof (from);
                bzero (&from, sizeof (from));
                client = accept (sd, (struct sockaddr *) &from, &len);

                if (client < 0){
                    perror ("[server] Eroare la accept().\n");
                    continue;
                  }

                  if (nfds < client){
                    nfds = client;
                  }
                          
	              FD_SET (client, &actfds);

	              printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));


                for(int j=0;j<=MAX_PAIRS_NUMBER;j++){
                  if(pairing_vector[j]==FD_NOT_PAIRED && j!=client){
                    pairing_vector[j]=client;
                    pairing_vector[client]=j;
                    MatrixSharing[j]=currentEmptyGT;
                    MatrixSharing[client]=currentEmptyGT;
                    currentEmptyGT++;
                    printf("CurrentEmptyGT: %d,\n",currentEmptyGT);
                  }
                  printf("%d ",pairing_vector[j]);
                }

                if(pairing_vector[client]==FD_NOT_CONNECTED){
                    pairing_vector[client]=FD_NOT_PAIRED;
                }

                fflush (stdout);
	      }
        int finish;
      	for (fd = 0; fd <= nfds; fd++)	{
	  
            if (fd != sd && FD_ISSET (fd, &readfds)){
              //aici am modificat
              
              if ((finish=manageGames(multipleGameTable[MatrixSharing[fd]],fd,mainPlayerTurn))==0) {
                printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
                pairing_vector[pairing_vector[fd]]=FD_NOT_CONNECTED;
                pairing_vector[fd]=FD_NOT_CONNECTED;
                fflush (stdout);
                FD_CLR (fd, &actfds);
                close (fd);		                
              }
              else if (finish==CHECKMATE_WHITE*(-1)){
                  write(fd,"Black is the winner!",sizeof("Black is the winner!"));
                  write(pairing_vector[fd],"Black is the winner!",sizeof("Black is the winner!"));                  
                  printf ("[server] S-a deconectat clientii cu descriptorul %d si %d.\n",fd,pairing_vector[fd]);
                  fflush (stdout);
                  //sleep(5);
                  FD_CLR (fd, &actfds);
                  FD_CLR (pairing_vector[fd], &actfds);
                  //close(fd);
                  //close(pairing_vector[fd]);                  
                  pairing_vector[fd]=FD_NOT_CONNECTED;
                  pairing_vector[pairing_vector[fd]]=FD_NOT_CONNECTED;
                  //break;
              }
              else if (finish==CHECKMATE_BLACK*(-1)){
                  write(fd,"White is the winner!",sizeof("White is the winner!"));
                  write(pairing_vector[fd],"White is the winner!",sizeof("White is the winner!"));  
                  printf ("[server] S-a deconectat clientul cu descriptorul %d si %d.\n",fd,pairing_vector[fd]);
                  fflush (stdout);
                  //sleep(5);
                  FD_CLR (fd, &actfds);
                  FD_CLR (pairing_vector[fd], &actfds); 
                  //close(fd);
                  //close(pairing_vector[fd]);                 
                  pairing_vector[fd]=FD_NOT_CONNECTED;
                  pairing_vector[pairing_vector[fd]]=FD_NOT_CONNECTED;
                  //break;
              }

              if(mainPlayerTurn==WHITE){
                mainPlayerTurn=BLACK;
              }
              else if (mainPlayerTurn==BLACK){
                mainPlayerTurn=WHITE;
              }

            }
        }			   /* for   */
      }				  /* while */
}				       /* main  */
