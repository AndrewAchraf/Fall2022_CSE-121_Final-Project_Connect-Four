#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>
#define h 6
#define w 7


char board[h][w]={};
void draw_board();
void take_player_turn();

int main(){
    draw_board();
    take_player_turn();
    draw_board();
    return 0;
}

void draw_board(){
    system("cls");
    printf("\tConnect Four\n");
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            printf("| %d |", board[i][j]);
        }
        printf("\n");
        printf("-----------------------------------\n");
    }
}

void take_player_turn(){
    int m, r, c;
    printf("Enter new value: "); scanf("%d", &m);
    printf("Enter row num: "); scanf("%d", &r); r--;
    printf("Enter col num: "); scanf("%d", &c); c--;
    board [r][c] = m;
}

