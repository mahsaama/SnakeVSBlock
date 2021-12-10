#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>

void printsnake(int size);
void gotoxy(double x, double y);
int  move(int size);
void delay();
void printpowerup(int size,int *puiptr);
void printblock(int size,int *biptr);
int  blocknum();
void printwall(int size,int *wiptr);
int  accident(int size);
int score=0;
int length(int x,int y);
void print(int size);
void file(char name[20],int s);
void record(char name[20]);
/*void printmargin(int size);*/

struct snake{
    double x;
    double y;
    int length;
}snake;
struct wall{
    int x;
    int y;
}wall[100];
struct block{
    double x;
    double y;
    int num;
    int super;
}block[100];
struct powerup{
    double x;
    double y;
    int num;
}powerup[100];
FILE *info;
int main(){

    for(int i=0;i<100;i++){
        block[i].super=0;
    }
    srand(time(NULL));
    int blockindex=0,wallindex=0,powerupindexptr=0,score,size,y=1,yesno;
    int *biptr=blockindex;
    int *wiptr=wallindex;
    int *puiptr=powerupindexptr;
    printf("welcome!please Enter your name:\n");
    char name[20];
    gets(name);
    printf("Do you want to start a newgame or resume your previous game? (previousgame:1 , newgame:0)\n");
    scanf("%d",&yesno);
    printf("Enter a number for the size of window(2,5):\n");
    scanf("%d",&size);
    if(yesno==0){
        snake.length=4;
        score=0;
    }
    if(yesno==1){
        info=fopen(name,"r");
        int s,l;
        fscanf(info,"Player Name:%s\nScore:%d\nlength:%d",name,&score,&snake.length);
        fclose(info);
    }
    system("cls");
    while(y==1){
      /*  printmargin(size);*/
        printsnake(size);
        printblock(size,&blockindex);
        printwall(size,&wallindex);
        printpowerup(size,&powerupindexptr);
        gotoxy(2.5*size,9*size);
        y=move(size);
        if(y==2){/* s pressed*/
            file(name,1);
            system("cls");
            printf("Game saved!");
            record(name);
            break;
        }
        if(y==0){/* esc pressed*/
            system("cls");
            file(name,0);
            printf("GAME OVER!\n");
            printf("name:%s\nscore:%d\n",name,score);
            record(name);
            break;
        }
        if(y==1){
            system("cls");
        }
    }
}
int move(int size){
    int row=0;
    int q=2;
    while(row!=9*size-1){
        getch();
        if(GetAsyncKeyState(VK_ESCAPE) && snake.x+1<=5*size){
            return 0;
        }
        if(GetAsyncKeyState('S')){
            return 2;
        }
        if(GetAsyncKeyState(VK_RIGHT) && snake.x+1<=5*size){
            snake.x++;
          /* if(accident(size)==0){
                snake.x--;
                printf("\a");
                continue;
            }*/
            gotoxy(snake.x,snake.y);
        }
        if(GetAsyncKeyState(VK_LEFT) && snake.x-1>=0){
           snake.x--;
          /* if(accident(size)==0){
                snake.x++;
                printf("\a");
                continue;
            }*/
           gotoxy(snake.x,snake.y);
        }
        if(GetAsyncKeyState(VK_UP)){
            snake.y--;
          /*  if(accident(size)==0){
                snake.y++;
                printf("\a");
                continue;
            }*/
            gotoxy(snake.x,snake.y);
            if(q==1){
                for(int i=0;i<100;i++){
                    if((block[i].x==snake.x+1 || block[i].x==snake.x || block[i].x==snake.x+2) && block[i].y==snake.y){
                        score+=block[i].num;
                    }
                }
            }
            if(q==2){
                q=length(snake.x,snake.y);
                if(q!=1 && q!=0){
                    q=2;
                }
            }
            if(q==0){
                return 0;
            }
            row++;
         }
         print(size);
         gotoxy(snake.x,snake.y);
    }
    delay();
    return 1;
}
void print(int size){
    gotoxy(6*size,size);
    printf("length:%2d",snake.length);
    gotoxy(6*size,2*size);
    printf("score:%d",score);
}
void gotoxy(double x, double y){
  static HANDLE h = NULL;
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = { x, y };
  SetConsoleCursorPosition(h,c);
}
void printsnake(int size){
    snake.x=2.5*size;
    snake.y=9*size;
    gotoxy(snake.x,snake.y);
    printf("s");
}
void printpowerup(int size,int *puiptr){
    int purand;
    for(int i=size;i<9*size;i+=size){
        purand=rand()%(5*size);
        for(int j=0;j<100;j++){
            if((i==block[j].y &&(purand==block[j].x ||purand==block[j].x+1 || purand==block[j].x+2 || purand==block[j].x+3 || purand==block[j].x+4)) || (i==wall[j].y && purand==wall[j].x)){
                purand=rand()%(5*size);
                j=0;
            }
        }
        powerup[*puiptr].x=purand;
        powerup[*puiptr].y=i;
        powerup[*puiptr].num=rand()%9+1;
        (*puiptr)++;
    }

    for(int k=0;k<*puiptr;k++){
        gotoxy(powerup[k].x,powerup[k].y);
        printf("%d",powerup[k].num);
    }
}
void printblock(int size,int *biptr){
    int randnum,xrand;
    for(int i=size;i<9*size;i+=size){
        randnum=rand()%100;
        if(randnum>=50 && randnum<70){
            xrand=rand()%(5*size);
            block[*biptr].x=xrand;
            block[*biptr].y=i;
            block[*biptr].num=blocknum();
            (*biptr)++;
        }
        if(randnum>=70 && randnum<80){/*2n->1*/
                xrand=rand()%(5*size);
                block[*biptr].x=xrand;
                block[*biptr].y=i;
                block[*biptr].num=blocknum();
                (*biptr)++;
        }
       if(randnum>=80 && randnum<87){/*3n->2*/
            for(int k=0;k<2;k++){
                xrand=rand()%(5*size-4);
                while(i==block[(*biptr)-1].y && (xrand==block[(*biptr)-1].x || xrand==block[(*biptr)-1].x+1 || xrand==block[(*biptr)-1].x+2 || xrand==block[(*biptr)-1].x+3 || xrand==block[(*biptr)-1].x+4)){
                    xrand=rand()%(5*size-4);
                }
                block[*biptr].x=xrand;
                block[*biptr].y=i;
                block[*biptr].num=blocknum();
                (*biptr)++;
            }
        }
        if(randnum>=87 && randnum<90){/*4n->2*/
             for(int k=0;k<2;k++){
                xrand=rand()%(5*size-4);
                while(i==block[(*biptr)-1].y && (xrand==block[(*biptr)-1].x || xrand==block[(*biptr)-1].x+1 || xrand==block[(*biptr)-1].x+2 || xrand==block[(*biptr)-1].x+3 || xrand==block[(*biptr)-1].x+4)){
                    xrand=rand()%(5*size-4);
                }
                block[*biptr].x=xrand;
                block[*biptr].y=i;
                block[*biptr].num=blocknum();
                (*biptr)++;
            }
        }
       if(randnum>=90 && randnum<100){/*5n->2*/
             for(int k=0;k<2;k++){
                xrand=rand()%(5*size-4);
                while(i==block[(*biptr)-1].y && (xrand==block[(*biptr)-1].x || xrand==block[(*biptr)-1].x+1 || xrand==block[(*biptr)-1].x+2 || xrand==block[(*biptr)-1].x+3 || xrand==block[(*biptr)-1].x+4)){
                    xrand=rand()%(5*size-4);
                }
                block[*biptr].x=xrand;
                block[*biptr].y=i;
                block[*biptr].num=blocknum();
                (*biptr)++;
            }
        }
    }
    for(int j=0;j<*biptr;j++){
        gotoxy(block[j].x,block[j].y);
        if(block[j].num>20){
            int x=rand()%2;
            if(x==0){
               printf("(*%d)",block[j].num);
               block[j].super=1;
            }
            if(x==1){
                printf("(%d)",block[j].num);
            }
        }
        else{
            printf("(%d)",block[j].num);
        }
    }
}
int blocknum(){
    int randomnum;
    randomnum=rand()%102;
    if(randomnum>=0 && randomnum<60){
        return snake.length-1;
    }
    if(randomnum>=60 && randomnum<80){
        return randomnum%5 +1;
    }
    if(randomnum>=80 && randomnum<90){
        return  randomnum%6 +5;
    }
    if(randomnum>=90 && randomnum<97){
        return randomnum%16 +10;
    }
    if(randomnum>=97 && randomnum<102){
        return randomnum%26 +25;
    }
}
void printwall(int size,int *wiptr){
        int wallrand,temp;
    for(int i=size;i<9*size;i+=size){
        wallrand=rand()%100;
        if(wallrand>=60 && wallrand<75){/*n->1*/
               temp=rand()%(5*size);
                for(int j=0;j<100;j++){
                    if((i==block[j].y && (temp==block[j].x || temp==block[j].x+1 || temp==block[j].x+2 || temp==block[j].x+3 || temp==block[j].x+4)) || (i==wall[j].y && temp==wall[j].x)){
                        temp=rand()%(5*size);
                        j=0;
                    }
                }
                wall[*wiptr].x=temp;
                wall[*wiptr].y=i;
                (*wiptr)++;
        }
        if(wallrand>=75 && wallrand<85){/*2n->1*/
               temp=rand()%(5*size);
                for(int j=0;j<100;j++){
                    if((i==block[j].y && (temp==block[j].x || temp==block[j].x+1 || temp==block[j].x+2 || temp==block[j].x+3 || temp==block[j].x+4)) || (i==wall[j].y && temp==wall[j].x)){
                        temp=rand()%(5*size);
                        j=0;
                    }
                }
                wall[*wiptr].x=temp;
                wall[*wiptr].y=i;
                (*wiptr)++;
        }
        if(wallrand>=85 && wallrand<92){/*3n->1*/
            for(int k=0;k<1;k++){
               temp=rand()%(5*size);
                for(int j=0;j<100;j++){
                    if((i==block[j].y && (temp==block[j].x || temp==block[j].x+1 || temp==block[j].x+2 || temp==block[j].x+3 || temp==block[j].x+4)) || (i==wall[j].y && temp==wall[j].x)){
                        temp=rand()%(5*size);
                        j=0;
                    }
                }
                wall[*wiptr].x=temp;
                wall[*wiptr].y=i;
                (*wiptr)++;
           }
        }
        if(wallrand>=92 && wallrand<97){/*4n->1*/
            for(int k=0;k<1;k++){
               temp=rand()%(5*size);
                for(int j=0;j<100;j++){
                    if((i==block[j].y && (temp==block[j].x || temp==block[j].x+1 || temp==block[j].x+2 || temp==block[j].x+3 || temp==block[j].x+4)) || (i==wall[j].y && temp==wall[j].x)){
                        temp=rand()%(5*size);
                        j=0;
                    }
                }
                wall[*wiptr].x=temp;
                wall[*wiptr].y=i;
                (*wiptr)++;
           }
        }
        if(wallrand>=97 && wallrand<100){/*5n->1*/
            for(int k=0;k<1;k++){
               temp=rand()%(5*size);
                for(int j=0;j<100;j++){
                    if((i==block[j].y && (temp==block[j].x || temp==block[j].x+1 || temp==block[j].x+2 || temp==block[j].x+3 || temp==block[j].x+4)) || (i==wall[j].y && temp==wall[j].x)){
                        temp=rand()%(5*size);
                        j=0;
                    }
                }
                wall[*wiptr].x=temp;
                wall[*wiptr].y=i;
                (*wiptr)++;
           }
        }
    }
    for(int p=0;p<*wiptr;p++){
        gotoxy(wall[p].x,wall[p].y);
        printf("|");
    }
}
int accident(int size){
    for(int i=0;i<100;i++){
        if(snake.y==wall[i].y){
            if(snake.x==wall[i].x){
                return 0;
            }
        }
        if(snake.x==wall[i].x){
            if(snake.y==wall[i].y){
                return 0;
            }
        }
    }
}
void delay(){
    for(long long int i=0;i<1000000;i++);
}
int length(int x,int y){
    for(int i=0;i<100;i++){
        if(powerup[i].x==x && powerup[i].y==y){
            snake.length+=powerup[i].num;
        }
        if((block[i].x==x+1 || block[i].x==x || block[i].x==x+2) && block[i].y==y){
            if(snake.length>=block[i].num && block[i].super==1){
                snake.length-=block[i].num;
                score+=block[i].num;
                return 1;
            }
            if(snake.length>=block[i].num && block[i].super==0){
                snake.length-=block[i].num;
                score+=block[i].num;
            }
            if(snake.length<block[i].num){
                score+=snake.length;
                return 0;
            }
        }
    }
}
/*void printmargin(int size){
    for(int i=0;i<5*size;i++){
        gotoxy(i,0);
        printf("%c",254);
        gotoxy(i,10*size);
        printf("%c",254);
    }
    for(int j=0;j<10*size;j++){
        gotoxy(0,j);
        printf("%c",219);
        gotoxy(5*size,j);
        printf("%c",219);
    }
}*/
void file(char name[20],int s){
    info=fopen(name,"w");
    fprintf(info,"Player Name:%s\n",name);
    fprintf(info,"Score:%d\n",score);
    if(s==1){
        fprintf(info,"length:%d\n",snake.length);
    }
    fclose(info);
}
void record(char name[20]){
    FILE *records;
    records=fopen("highscores","a+");
    fprintf(records,"name:%s\nscore:%d\n",name,score);
    fclose(records);
}
