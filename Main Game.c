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

int main(){
    clear_board();
    draw_board();
    take_player_turn();
    draw_board();
    return 0;
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


