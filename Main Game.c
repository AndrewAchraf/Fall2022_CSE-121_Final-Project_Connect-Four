#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>



void clear_board();
void draw_board();
void take_player_turn();
void mainMenu();
int check_if_valid_col();


int main(){
    mainMenu();
    int rows,columns;
    printf("Enter the size of the grid: ");
    scanf("%d %d",&rows,&columns);
    char array[rows][columns];

    clear_board(rows,columns,array);
    draw_board(rows,columns,array);
    take_player_turn(rows,columns,array);
    draw_board(rows,columns,array);

    take_player_turn(rows,columns,array);
    draw_board(rows,columns,array);

    return 0;
}


void mainMenu(){
    printf(" 1:Start a new game\n 2:Load a previous game\n 3:Quit\n");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1: printf(" 1:One player\n 2:Two players\n");
                scanf("%d",&choice);
                switch(choice){
                    case 1: draw_board(); break;
                    case 2: draw_board(); break;
                }
        case 2: /*extract from a file*/ break;
        case 3: break;
    }
}

void clear_board(int rows,int columns,char array[rows][columns]){
    int i,j;
    for (i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            array[i][j]=' ';
        }
    }
    int header[columns];
    for(j=0;j<columns;j++){
        header[j]=j+1;
    }
    for(j=0;j<columns;j++){
            printf("  %d ",header[j]);
    }
    printf("\n");
}


void draw_board(int rows,int columns,char array[rows][columns]){
    system("cls");
    printf("\tConnect Four\n");
    int i,j;
    for (i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            printf("| %c ", array[i][j]);
        }
        printf("|\n");
        for(j=0;j<columns;j++){
            printf("----");
        }
        printf("-\n");
    }
}

void take_player_turn(int rows,int columns,char array[rows][columns]){
    int enteredCol;
    while(1){
        printf("Enter col num: "); scanf("%d", &enteredCol); enteredCol--;
        if( check_if_valid_col(rows, columns, array, enteredCol) ){
            int i=0;
            while(array[i][enteredCol] ==' ' && i<rows){
                i++;
            }
            array[i-1][enteredCol]= 'X';
            break;
        }
        else{
            printf("Invalid Input !\n");
        }
    }
}

int check_if_valid_col(int rows,int columns,char array[rows][columns], int enteredCol){
    if(enteredCol>columns || enteredCol < 1){
        return 0;
    }
    else if(array[0][enteredCol]== 'X'){
        return 0;
    }
    else{
        return 1;
    }
}


