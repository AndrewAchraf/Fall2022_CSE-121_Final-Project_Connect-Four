#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>
#define h 6
#define w 7


char board[h][w];
void clear_board();
void draw_board();
void take_player_turn();
void mainMenu();

int main(){
    mainMenu();
    clear_board();
    draw_board();
    take_player_turn();
    draw_board();
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

void clear_board(){
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            board[i][j]= ' ';
        }
    }
}


void draw_board(){
    system("cls");
    printf("\tConnect Four\n");
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf("| %c |", board[i][j]);
        }
        printf("\n");
        printf("-----------------------------------\n");
    }
}

void take_player_turn(){
    char m;
    int r, c;
    printf("Enter new value: "); scanf("%c", &m);
    printf("Enter row num: "); scanf("%d", &r); r--;
    printf("Enter col num: "); scanf("%d", &c); c--;
    board [r][c] = m;
}


