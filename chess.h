//COMENTARIU MARE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h>

#define TABLESIZE 8

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

int possibleMoves[3][9],numberOfMoves=0,blockedMoves=0,totalNumberofSpaces=0,numberOfSameColorPieces=0;
int gameTable[10][10] ;
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

void initGameTable(int GT[10][10])
{
    for(int i=0;i<=10;i++)
        for(int j=0;j<=10;j++)
             GT[i][j]=0;

    for(int i=1;i<=TABLESIZE;i++)
        for(int j=1;j<=TABLESIZE;j++)
             GT[i][j]=EMPTY;

    for(int i=1;i<=TABLESIZE;i++){
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
void deselectPawn(int GT[10][10]){
    
    for (int i = 1;i<=TABLESIZE;i++){
        for(int j = 1;j<=TABLESIZE;j++ ){
            if(GT[i][j]<0)
                GT[i][j]*=-1;
        }
    }
}
void peonMoves(int GT[10][10],int line,int col){

    if(GT[line][col]%10==PEON%10){
        if(GT[line][col]/10==WHITE){
            //player1
            //spatiu liber
            if(GT[line+1][col]==EMPTY){
                GT[line+1][col]=-1;
            }
            //stanga atac
            if(GT[line+1][col+1]/10==BLACK){
                GT[line+1][col+1]*=-1;
            }
            //dreapta atac
            if(GT[line+1][col-1]/10==BLACK){
                GT[line+1][col-1]*=-1;
            }
            //miscare speciala
            if(GT[line+2][col]==EMPTY && line ==2){
                GT[line+2][col]=-1;
            }
            
        }
        else if(GT[line][col]/10==BLACK){
            //player2
            if(GT[line-1][col]==EMPTY){
                GT[line-1][col]=-1;
            }
            //stanga atac 
            if((GT[line-1][col+1])/10==WHITE){
                GT[line-1][col+1]*=-1;
            }
            //dreapta atac
            if(GT[line-1][col-1]/10==WHITE){
                GT[line-1][col-1]*=-1;
            }
            //miscare speciala
            if(GT[line-2][col]==EMPTY && line ==7){
                GT[line-2][col]=-1;
            }
        }
    }   
}
void knightMoves(int GT[10][10],int line,int col){

    if(GT[line][col]%10==KNIGHT%10){

        if(GT[line][col]/10==WHITE){

            if(GT[line-1][col+2]==EMPTY || GT[line-1][col+2]/10==BLACK){
                GT[line-1][col+2]*=-1;
            }

            if(GT[line+1][col+2]==EMPTY || GT[line+1][col+2]/10==BLACK){
                GT[line+1][col+2]*=-1;
            }

            if(GT[line-2][col+1]==EMPTY || GT[line-2][col+1]/10==BLACK){
                GT[line-2][col+1]*=-1;
            }

            if(GT[line+2][col+1]==EMPTY || GT[line+2][col+1]/10==BLACK){
                GT[line+2][col+1]*=-1;
            }

            if(GT[line-2][col-1]==EMPTY || GT[line-2][col-1]/10==BLACK){
                GT[line-2][col-1]*=-1;
            }

            if(GT[line+2][col-1]==EMPTY || GT[line+2][col-1]/10==BLACK){
                GT[line+2][col-1]*=-1;
            }

            if(GT[line-1][col-2]==EMPTY || GT[line-1][col-2]/10==BLACK){
                GT[line-1][col-2]*=-1;
            }

            if(GT[line+1][col-2]==EMPTY || GT[line+1][col-2]/10==BLACK){
                GT[line+1][col-2]*=-1;
            }
        }

        //////

        if(GT[line][col]/10==BLACK){

            if(GT[line-1][col+2]==EMPTY || GT[line-1][col+2]/10==WHITE){
                GT[line-1][col+2]*=-1;
            }

            if(GT[line+1][col+2]==EMPTY || GT[line+1][col+2]/10==WHITE){
                GT[line+1][col+2]*=-1;
            }

            if(GT[line-2][col+1]==EMPTY || GT[line-2][col+1]/10==WHITE){
                GT[line-2][col+1]*=-1;
            }

            if(GT[line+2][col+1]==EMPTY || GT[line+2][col+1]/10==WHITE){
                GT[line+2][col+1]*=-1;
            }

            if(GT[line-2][col-1]==EMPTY || GT[line-2][col-1]/10==WHITE){
                GT[line-2][col-1]*=-1;
            }

            if(GT[line+2][col-1]==EMPTY || GT[line+2][col-1]/10==WHITE){
                GT[line+2][col-1]*=-1;
            }

            if(GT[line-1][col-2]==EMPTY || GT[line-1][col-2]/10==WHITE){
                GT[line-1][col-2]*=-1;
            }

            if(GT[line+1][col-2]==EMPTY || GT[line+1][col-2]/10==WHITE){
                GT[line+1][col-2]*=-1;
            }
        }
    }

}
void towerMoves(int GT[10][10],int line,int col){

    if(GT[line][col]%10==TOWER%10 || GT[line][col]%10==QUEEN%10){

        int i=line,j=col;

        if(GT[line][col]/10==WHITE){
            
            i=line-1,j=col;
            //sus I--
            while(i>=1 && (GT[i][col]==EMPTY || GT[i][col]==-1) ){
                GT[i][col]=-1;
                i--;
            }
            if(GT[i][col]/10==BLACK){
                GT[i][col]*=-1;
            }

            i=line+1,j=col;
            //jos I++
            while(i<=TABLESIZE && (GT[i][col]==EMPTY || GT[i][col]==-1) ){
                GT[i][col]=-1;
                i++;
            }
            if(GT[i][col]/10==BLACK){
                GT[i][col]*=-1;
            }

            i=line,j=col-1;
            //drp J--
            while(j>=1 && (GT[line][j]==EMPTY || GT[line][j]==-1 )){
                GT[line][j]=-1;
                j--;
            }
            if(GT[line][j]/10==BLACK){
                GT[line][j]*=-1;
            }

            i=line,j=col+1;
            //stg J++
            while(j<=TABLESIZE && (GT[line][j]==EMPTY || GT[line][j]==-1)){
                GT[line][j]=-1;
                j++;
            }
            if(GT[line][j]/10==BLACK){
                GT[line][j]*=-1;
            }

        }
        //-------------------------------------------------------------------------
        if(GT[line][col]/10==BLACK){
            
            i=line-1,j=col;
            //sus I--
            while(i>=1 && (GT[i][col]==EMPTY || GT[i][col]==-1)){
                GT[i][col]=-1;
                i--;
            }
            if(GT[i][col]/10==WHITE){
                GT[i][col]*=-1;
            }

            i=line+1,j=col;
            //jos I++
            while(i<=TABLESIZE && (GT[i][col]==EMPTY || GT[i][col]==-1)){
                GT[i][col]=-1;
                i++;
            }
            if(GT[i][col]/10==WHITE){
                GT[i][col]*=-1;
            }

            i=line,j=col-1;
            //drp J--
            while(j>=1 && (GT[line][j]==EMPTY || GT[line][j]==-1)){
                GT[line][j]=-1;
                j--;
            }
            if(GT[line][j]/10==WHITE){
                GT[line][j]*=-1;
            }

            i=line,j=col+1;
            //stg J++
            while(j<=TABLESIZE && (GT[line][j]==EMPTY || GT[line][j]==-1)){
                GT[line][j]=-1;
                j++;
            }
            if(GT[line][j]/10==WHITE){
                GT[line][j]*=-1;
            }

        }
    }
}

void bishopMoves(int GT[10][10],int line,int col){

    if(GT[line][col]%10==BISHOP%10 || GT[line][col]%10==QUEEN%10){
        int k=0;
        if(GT[line][col]/10==WHITE){

            k=1;
            while((line+k<=TABLESIZE && col+k<=TABLESIZE) && (GT[line+k][col+k]==EMPTY || GT[line+k][col+k]==-1 )){
                GT[line+k][col+k]=-1;
                k++;
            }
            if( (line+k<=TABLESIZE && col+k<=TABLESIZE) && GT[line+k][col+k]/10==BLACK){
                GT[line+k][col+k]*=-1;
            }

            k=1;
            while((line-k>=1 && col-k>=1) && (GT[line-k][col-k]==EMPTY || GT[line-k][col-k]==-1)){
                GT[line-k][col-k]=-1;
                k++;
            }
            if((line-k>=1 && col-k>=1) && GT[line-k][col-k]/10==BLACK){
                GT[line-k][col-k]*=-1;
            }

            k=1;
            while((line-k>=1 && col+k<=TABLESIZE) && (GT[line-k][col+k]==EMPTY || GT[line-k][col+k]==-1)){
                GT[line-k][col+k]=-1;
                k++;
            }
            if((line-k>=1 && col+k<=TABLESIZE) && GT[line-k][col+k]/10==BLACK){
                GT[line-k][col+k]*=-1;
            }

            k=1;
            while((line+k<=TABLESIZE && col-k>=1) && (GT[line+k][col-k]==EMPTY || GT[line+k][col-k]==-1)){
                GT[line+k][col-k]=-1;
                k++;
            }
            if((line+k<=TABLESIZE && col-k>=1) && GT[line+k][col-k]/10==BLACK){
                GT[line+k][col-k]*=-1;
            }

        }

        if(GT[line][col]/10==BLACK){

            k=1;
            while((line-k>=1 && col-k>=1) && (GT[line+k][col+k]==EMPTY || GT[line+k][col+k]==-1)){
                GT[line+k][col+k]=-1;
                k++;
            }
            if((line-k>=1 && col-k>=1) && GT[line+k][col+k]/10==WHITE){
                GT[line+k][col+k]*=-1;
            }

            k=1;
            while((line-k>=1 && col-k>=1) && (GT[line-k][col-k]==EMPTY || GT[line-k][col-k]==-1)){
                GT[line-k][col-k]=-1;
                k++;
            }
            if((line-k>=1 && col-k>=1) && GT[line-k][col-k]/10==WHITE){
                GT[line-k][col-k]*=-1;
            }

            k=1;
            while((line-k>=1 && col+k<=TABLESIZE) && (GT[line-k][col+k]==EMPTY || GT[line-k][col+k]==-1)){
                GT[line-k][col+k]=-1;
                k++;
            }
            if((line-k>=1 && col+k<=TABLESIZE) && GT[line-k][col+k]/10==WHITE){
                GT[line-k][col+k]*=-1;
            }

            k=1;
            while((line+k<=TABLESIZE && col-k>=1) && (GT[line+k][col-k]==EMPTY || GT[line+k][col-k]==-1)){
                GT[line+k][col-k]=-1;
                k++;
            }
            if((line+k<=TABLESIZE && col-k>=1) && GT[line+k][col-k]/10==WHITE){
                GT[line+k][col-k]*=-1;
            }

        }
    }

}
void QueenMoves(int GT[10][10],int line,int col){
    
    if(GT[line][col]%10==QUEEN%10){

        bishopMoves(GT,line,col);
        towerMoves(GT,line,col);
       

    }
}
void allEnemiesMoves(int GT[10][10], int color){

    for(int i=1;i<=TABLESIZE;i++)
        for(int j=1;j<=TABLESIZE;j++)
            if(GT[i][j]/10==color){
                switch(GT[i][j]%10){
                    case 2 :{
                        QueenMoves(GT,i,j);
                    }
                    break;
                    case 3 :{
                        towerMoves(GT,i,j);
                    }
                    break;
                    case 4 :{
                        knightMoves(GT,i,j);
                    }
                    break;
                    case 5 :{
                        bishopMoves(GT,i,j);
                    }
                    break;
                    case 6 :{
                        peonMoves(GT,i,j);
                    }
                    break;
                }
            }

}
int KingMoves(int GT[10][10],int line,int col){

    int removedPiece;
 
    if(GT[line][col]%10==KING%10){

        if(GT[line][col]/10==WHITE){
            
            allEnemiesMoves(GT,BLACK);

            //if blocked moves == total nr of moves && same color pieces arent surounding
            if(GT[line+1][col-1]==-1 || GT[line+1][col-1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line+1][col-1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col-1]/10== BLACK){

                    removedPiece=GT[line+1][col-1];
                    GT[line+1][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                    printf("a1");
                    //printMatrix(GT);

                    if(GT[line+1][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }

                    
                }
                else if(GT[line+1][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }

                deselectPawn(GT);
                allEnemiesMoves(GT,BLACK);
            //|i+1_j|
            if(GT[line+1][col]==-1 || GT[line+1][col]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line+1][col]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col]/10== BLACK){

                    removedPiece=GT[line+1][col];
                    GT[line+1][col]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                    printf("a2");
                    //printMatrix(GT);


                    if(GT[line+1][col]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col;
                    }

                    
                }
                else if(GT[line+1][col]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col;
                     }
            deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            //|_i+1 j+1
            if(GT[line+1][col+1]==-1 || GT[line+1][col+1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line+1][col+1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col+1]/10== BLACK){

                    removedPiece=GT[line+1][col+1];
                    GT[line+1][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                    printf("a3");
                    //printMatrix(GT);


                    if(GT[line+1][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }

                   
                }
                else if(GT[line+1][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col+1;
                     } 
            deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            //_
            //_|i j-1
            if(GT[line][col-1]==-1 || GT[line][col-1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line][col-1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line][col-1]/10== BLACK){

                    removedPiece=GT[line][col-1];
                    GT[line][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                     printf("a4");
                    //printMatrix(GT);


                    if(GT[line][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }

                    
                }
                else if(GT[line][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }  
            deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            // _
            //|_ i,j+1
            if(GT[line][col+1]==-1 || GT[line][col+1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line][col+1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line][col+1]/10== BLACK){

                    removedPiece=GT[line][col+1];
                    GT[line][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                     printf("a5");
                    //printMatrix(GT);


                    if(GT[line][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }

                   
                }
                else if(GT[line][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        
                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col+1;
                     }
             deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            // _
            //  | i-1,j-1
            if(GT[line-1][col-1]==-1 || GT[line-1][col-1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line-1][col-1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col-1]/10== BLACK){

                    removedPiece=GT[line-1][col-1];
                    GT[line-1][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                    printf("a6");
                    //printMatrix(GT);


                    if(GT[line-1][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }
                   
                }
                else if(GT[line-1][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }
             deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            //i-1 ,j

            if(GT[line-1][col]==-1 || GT[line-1][col]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line-1][col]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col]/10== BLACK){

                    removedPiece=GT[line-1][col];
                    GT[line-1][col]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                     printf("a7");
                    //printMatrix(GT);


                    if(GT[line-1][col]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col;
                    }

                    
                }
                else if(GT[line-1][col]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col;
                     }
            deselectPawn(GT);
            allEnemiesMoves(GT,BLACK);
            //i-1,j+1
            if(GT[line-1][col+1]==-1 || GT[line-1][col+1]/10== WHITE){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line-1][col+1]/10== WHITE){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col+1]/10== BLACK){

                    removedPiece=GT[line-1][col+1];
                    GT[line-1][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,BLACK);
                    printf("a8");
                    //printMatrix(GT);

                    if(GT[line-1][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }
                    
                }
                else if(GT[line-1][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col+1;
                     }
                
                deselectPawn(GT);

            if(totalNumberofSpaces==blockedMoves && totalNumberofSpaces!=numberOfSameColorPieces){
                //printf("CheckMate!");
                printf("Number of Moves :%d\nBlocked Moves: %d\nSpace Around:%d\nWhitePieces:%d\n",numberOfMoves,blockedMoves,totalNumberofSpaces,numberOfSameColorPieces);
                return CHECKMATE_WHITE;
            }
            else {
                  printf("Number of Moves :%d\nBlocked Moves: %d\nSpace Around:%d\nWhitePieces:%d\n",numberOfMoves,blockedMoves,totalNumberofSpaces,numberOfSameColorPieces);
                  allEnemiesMoves(GT,BLACK);
                  //printMatrix(GT);
                  //regele este atacat
                  if(GT[line][col]<0){
                    deselectPawn(GT);
                    return CHECK_WHITE;
                }
            }
        }
        //---------------------------------------------------------------------------------------------------------------
        if(GT[line][col]/10==BLACK){

        if(GT[line+1][col-1]==-1 || GT[line+1][col-1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                    if(GT[line+1][col-1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col-1]/10== WHITE){

                    removedPiece=GT[line+1][col-1];
                    GT[line+1][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                    printf("a1");
                    //printMatrix(GT);

                    if(GT[line+1][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }

                    
                }
                else if(GT[line+1][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }

                deselectPawn(GT);
                allEnemiesMoves(GT,WHITE);
            //|i+1_j|
            if(GT[line+1][col]==-1 || GT[line+1][col]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line+1][col]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col]/10== WHITE){

                    removedPiece=GT[line+1][col];
                    GT[line+1][col]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                    printf("a2");
                    //printMatrix(GT);


                    if(GT[line+1][col]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col;
                    }

                    
                }
                else if(GT[line+1][col]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col;
                     }
            deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            //|_i+1 j+1
            if(GT[line+1][col+1]==-1 || GT[line+1][col+1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line+1][col+1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line+1][col+1]/10== WHITE){

                    removedPiece=GT[line+1][col+1];
                    GT[line+1][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                    printf("a3");
                    //printMatrix(GT);


                    if(GT[line+1][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line+1][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line+1][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }

                   
                }
                else if(GT[line+1][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line+1;
                        possibleMoves[2][numberOfMoves]=col+1;
                     } 
            deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            //_
            //_|i j-1
            if(GT[line][col-1]==-1 || GT[line][col-1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line][col-1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line][col-1]/10== WHITE){

                    removedPiece=GT[line][col-1];
                    GT[line][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                     printf("a4");
                    //printMatrix(GT);


                    if(GT[line][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }

                    
                }
                else if(GT[line][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }  
            deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            // _
            //|_ i,j+1
            if(GT[line][col+1]==-1 || GT[line][col+1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line][col+1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line][col+1]/10== WHITE){

                    removedPiece=GT[line][col+1];
                    GT[line][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                     printf("a5");
                    //printMatrix(GT);


                    if(GT[line][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }

                   
                }
                else if(GT[line][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        
                        possibleMoves[1][numberOfMoves]=line;
                        possibleMoves[2][numberOfMoves]=col+1;
                     }
            deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            // _
            //  | i-1,j-1
            if(GT[line-1][col-1]==-1 || GT[line-1][col-1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line-1][col-1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col-1]/10== WHITE){

                    removedPiece=GT[line-1][col-1];
                    GT[line-1][col-1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                    printf("a6");
                    //printMatrix(GT);


                    if(GT[line-1][col-1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col-1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col-1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col-1;
                    }
                   
                }
                else if(GT[line-1][col-1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col-1;
                     }
             deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            //i-1 ,j

            if(GT[line-1][col]==-1 || GT[line-1][col]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line-1][col]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col]/10== WHITE){

                    removedPiece=GT[line-1][col];
                    GT[line-1][col]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                     printf("a7");
                    //printMatrix(GT);


                    if(GT[line-1][col]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col;
                    }

                    
                }
                else if(GT[line-1][col]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col;
                     }
            deselectPawn(GT);
            allEnemiesMoves(GT,WHITE);
            //i-1,j+1
            if(GT[line-1][col+1]==-1 || GT[line-1][col+1]/10== BLACK){
                    blockedMoves++;
                    totalNumberofSpaces++;
                     if(GT[line-1][col+1]/10== BLACK){
                        numberOfSameColorPieces++;
                    }
            }
            else if(GT[line-1][col+1]/10== WHITE){

                    removedPiece=GT[line-1][col+1];
                    GT[line-1][col+1]=KING;
                    GT[line][col]=-1;

                    allEnemiesMoves(GT,WHITE);
                    printf("a8");
                    //printMatrix(GT);

                    if(GT[line-1][col+1]<0){
                        blockedMoves++;
                        totalNumberofSpaces++;

                        GT[line-1][col+1]=removedPiece;
                        GT[line][col]=KING;
                    }
                    else {
                        totalNumberofSpaces++;
                        numberOfMoves++;

                        GT[line-1][col+1]=removedPiece;
                        GT[line][col]=KING;

                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col+1;
                    }
                    
                }
                else if(GT[line-1][col+1]==EMPTY){
                        totalNumberofSpaces++;
                        numberOfMoves++;
                        possibleMoves[1][numberOfMoves]=line-1;
                        possibleMoves[2][numberOfMoves]=col+1;
                     }
                
                deselectPawn(GT);

            if(totalNumberofSpaces==blockedMoves && totalNumberofSpaces!=numberOfSameColorPieces){
                //printf("CheckMate!");
                printf("Number of Moves:%d\nBlocked Moves:%d\nSpace Around:%d\nBlackPieces:%d\n",numberOfMoves,blockedMoves,totalNumberofSpaces,numberOfSameColorPieces);
                return CHECKMATE_BLACK;
            }
            else {
                    printf("Number of Moves :%d\nBlocked Moves: %d\nSpace Around:%d\n",numberOfMoves,blockedMoves,totalNumberofSpaces);
                    allEnemiesMoves(GT,WHITE);
                    //printMatrix(GT);
                    //regele este atacat
                    if(GT[line][col]<0){
                        deselectPawn(GT);
                        return CHECK_BLACK;
                    }
            }
        }
    }
    return 0;
}
void KingMovesNoCheck(int GT[10][10],int line,int col){

    if(GT[line][col]%10==KING%10){
        if(GT[line][col]/10==WHITE){

            if(GT[line-1][col-1]==EMPTY || GT[line-1][col-1]==BLACK){
                GT[line-1][col-1]*=-1;
            }

            if(GT[line-1][col]==EMPTY || GT[line-1][col]==BLACK){
                GT[line-1][col]*=-1;
            }
            if(GT[line-1][col+1]==EMPTY || GT[line-1][col+1]==BLACK){
                GT[line-1][col+1]*=-1;
            }
            if(GT[line][col-1]==EMPTY || GT[line][col-1]==BLACK){
                GT[line][col-1]*=-1;
            }
            if(GT[line][col+1]==EMPTY || GT[line][col+1]==BLACK){
                GT[line][col+1]*=-1;
            }
            if(GT[line+1][col-1]==EMPTY || GT[line+1][col-1]==BLACK){
                GT[line+1][col-1]*=-1;
            }
            if(GT[line+1][col]==EMPTY || GT[line+1][col]==BLACK){
                GT[line+1][col]*=-1;
            }
            if(GT[line+1][col+1]==EMPTY || GT[line+1][col+1]==BLACK){
                GT[line+1][col+1]*=-1;
            }
           
        }
    }
}
int confirmSelect(int GT[10][10],int line1,int col1,int line2,int col2,int playerTurn){

int isInCheck;
    for(int i=1;i<TABLESIZE;i++)
        for(int j=1;j<TABLESIZE;j++){
            if(GT[i][j]/10==playerTurn && GT[i][j]%10==KING%10){
                isInCheck=KingMoves(GT,i,j);
                switch (isInCheck)
                {
                case CHECKMATE_WHITE : {
                    return CHECKMATE_WHITE;
                } 
                break;
                case CHECKMATE_BLACK : {
                    return CHECKMATE_BLACK;
                }
                break;
                case CHECK_WHITE :{
                    printf("Regele Alb este in sah, protejeaza-l sau muta-l!\n");
                    deselectPawn(GT);
                        numberOfMoves=0;
                        totalNumberofSpaces=0;
                        blockedMoves=0;
                        numberOfSameColorPieces=0;
                        if(playerTurn==WHITE) return WHITE; else return BLACK;
                    
                }
                break;
                case CHECK_BLACK : {
                    printf("Regele Negru este in sah, protejeaza-l sau muta-l!\n");
                    deselectPawn(GT);
                        numberOfMoves=0;
                        totalNumberofSpaces=0;
                        blockedMoves=0;
                        numberOfSameColorPieces=0;
                        if(playerTurn==WHITE) return WHITE; else return BLACK;
                }
                break;
                default: {
                    printf("Ce a returnat cand a verifcat pt checkmate confirmSelect : %d\n",isInCheck);
                }
                }
            }
        }
    
    
    if(GT[line1][col1]/10==playerTurn){
        switch (GT[line1][col1]%10)
        {
            case 1:{
                
                KingMoves(GT,line1,col1);
                deselectPawn(GT);
                if(numberOfMoves>=1){
                    for(int i=1;i<=numberOfMoves;i++){
                         GT[possibleMoves[1][i]][possibleMoves[2][i]]*=-1;
                    }
                }
            }
            break;
            case 2:{
                deselectPawn(GT);
                QueenMoves(GT,line1,col1);
            }
            break;
            case 3:{
                deselectPawn(GT);
                towerMoves(GT,line1,col1);
            }
            break;
            case 4:{
                deselectPawn(GT);
                knightMoves(GT,line1,col1);
            }
            break;
            case 5:{
                deselectPawn(GT);
                bishopMoves(GT,line1,col1);
            }
            break;
            case 6:{
                deselectPawn(GT);
                peonMoves(GT,line1,col1);
                
            }
            
        }

                if(GT[line2][col2]<0){
                        GT[line2][col2]=GT[line1][col1];
                        GT[line1][col1]=EMPTY;
                        deselectPawn(GT);
                    
                }
                else {
                        printf("%s\n","Invalid Move : Can't Move Piece Here!");
                        deselectPawn(GT);
                        numberOfMoves=0;
                        totalNumberofSpaces=0;
                        blockedMoves=0;
                        numberOfSameColorPieces=0;
                        if(playerTurn==WHITE) return WHITE; else return BLACK;
                     }
        
    }
    else{
            printf("%s\n","Invalid Move : Wrong Colored Piece or You Tried To Move Nothing!");
            deselectPawn(GT);
            numberOfMoves=0;
            totalNumberofSpaces=0;
            blockedMoves=0;
            numberOfSameColorPieces=0;
            if(playerTurn==WHITE){return WHITE;} else {return BLACK;}
        }
        
    numberOfMoves=0;
    totalNumberofSpaces=0;
    blockedMoves=0;
    numberOfSameColorPieces=0;
   
    return 10;//confirm
}
void playChess(int line1,int col1,int line2, int col2)
{
    //faci o copie a matricei la inceputul threadului 
    
    int player=WHITE,x1=line1,x2=line2,y1=col1,y2=col2;
    char buff[1024];

        printMatrix(gameTable);

        if(player==WHITE){

            player=confirmSelect(gameTable,x1,y1,x2,y2,player);
            printf("PLAYERTURN:!%d\n",player);
            if(player == CHECKMATE_BLACK){
                printf("A castigat alb!");
                return;
            }
            else if (player == CHECKMATE_WHITE){
                printf("A castigat negru!");
                return;
            }
            
        }
        else{

            player=confirmSelect(gameTable,x1,y1,x2,y2,player);
            if(player==CHECKMATE_BLACK){
                printf("A castigat alb!");
                return;
            }
            else if (player ==CHECKMATE_WHITE){
                printf("A castigat negru!");
                return;
            }
            
        }
        
    
    
}