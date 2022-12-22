#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>
#include <stdbool.h>
#include <string.h>

const char PLAYER1='X';
const char PLAYER2='O';

typedef struct{
    int score;
    char name[257];
    char color;
}Player;


void clear_board();
void draw_board();
void take_player_turn();
void mainMenu();
int check_if_valid_col();
void takeComputerTurn();
bool check_for_free_slots();
void print_names_and_scores();
void check_scores();
void player_vs_computer();
void player_vs_player();



int main(){

    // Main Menu
    int quit = 0;
    do{
        system("cls");
        printf(" 1:Start a new game\n 2:Load a previous game\n 3:Quit\n");
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1: printf(" 1: One player\n 2: Two players\n 3: Back\n");
                    scanf("%d",&choice);
                    switch(choice){
                        case 1:
                            // Player v Computer
                            player_vs_computer();
                            break;

                        case 2:
                            // Player v Player
                            player_vs_player();
                            break;
                        case 3:
                            break;
                        default:
                            printf("Invalid Input !\n");
                    }
                    break;

            case 2: /*extract from a file*/ break;
            case 3: quit = 1; break;
            default: printf("Invalid Input !\n");
        }
    }while(quit == 0);



    /*take_player_turn(rows,columns,array);
    draw_board(rows,columns,array);
    takeComputerTurn(rows,columns,array);
    draw_board(rows,columns,array);*/

    return 0;
}



void clear_board(int rows,int columns,char array[rows][columns]){
    int i,j;
    for (i=0; i<rows; i++){
        for(j=0; j<columns; j++){
            array[i][j]=' ';
        }
    }

}


void draw_board(int rows,int columns,char array[rows][columns], Player player1, Player player2){
    system("cls");
    printf("\tConnect Four\n");
    printf("\t");
    print_names_and_scores(player1.name, player1.score);
    printf("\t\t");
    print_names_and_scores(player2.name, player2.score); printf("\n");
    int i,j;
    int header[columns];
    for(j=0;j<columns;j++){
        header[j]=j+1;
    }
    for(j=0;j<columns;j++){
            printf("  %d ",header[j]);
    }
    printf("\n");
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

void take_player_turn(int rows,int columns,char array[rows][columns], Player *player){
    int enteredCol;
    while(1){
        for (int i=0; i<strlen((*player).name); i++){
        printf("%c", (*player).name[i]);
        }
        printf("'s turn, please enter col num: "); scanf("%d", &enteredCol); enteredCol--;
        if( check_if_valid_col(rows, columns, array, enteredCol) ){
            int i=0;
            while(array[i][enteredCol] ==' ' && i<rows){
                i++;
            }
            array[i-1][enteredCol]= (*player).color;
            check_scores(rows, columns, array, i-1, enteredCol, &((*player).score) );
            break;
        }
        else{
            printf("Invalid Input !\n");
        }
    }
}

void takeComputerTurn(int rows,int columns,char array[rows][columns], Player *computer){
    srand(time(0));
    int i,j;
        do{
           j= rand()%columns;
        }
        while(array[0][j]!=' ');
        for(i=0;i<rows;i++){
            if(array[i][j]!=' '){
                break;
            }
        }

    array[i-1][j]=(*computer).color;
    check_scores(rows, columns, array, i-1, j, &((*computer).score) );
}

int check_if_valid_col(int rows,int columns,char array[rows][columns], int enteredCol){
    if(enteredCol>=columns || enteredCol < 0){
        return 0;
    }
    else if(array[0][enteredCol]== PLAYER1 || array[0][enteredCol]==PLAYER2 ){
        return 0;
    }
    else{
        return 1;
    }
}



bool check_for_free_slots(int rows,int columns,char array[rows][columns]){
    bool free = false;
    for(int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            if(array[i][j] == ' '){
                return 1;
            }
        }
    }
    return free;
}

void print_names_and_scores(char name[], int score){
    for (int i=0; i<strlen(name); i++){
        printf("%c", name[i]);
    }
    printf(": %d", score);
}

void check_scores(int rows,int columns,char array[rows][columns], int enteredRow, int enteredCol, int *playerScore){
    //Check Horizontal
    if(enteredCol<columns-3 && ( array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+3] ) ){
        (*playerScore)++;
    }

    if(enteredCol>=3 && ( array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-3] ) ){
        (*playerScore)++;
    }

    if(array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+2] ){
        (*playerScore)++;
    }

    if(array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] ){
        (*playerScore)++;
    }


    //Check Vertical
    if(enteredRow<rows-3 && (array[enteredRow][enteredCol] == array[enteredRow+1][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol])){
        (*playerScore)++;
    }

    //Check Diagonals
    if(enteredCol<columns-3){
        if(enteredRow<rows-3 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol+3] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow-3][enteredCol+3] )){
            (*playerScore)++;
        }
    }
    if(enteredCol>=3){
        if(enteredRow<rows-3 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol-3] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow-3][enteredCol-3] )){
            (*playerScore)++;
        }
    }
}

//if( array[enteredRow][enteredCol]==array[enteredRow][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol] )


void player_vs_computer(){
    int response = 1;
    do{
        int rows,columns;
        while(1){
            printf("Enter the size of the grid: ");
            scanf("%d %d",&rows,&columns);
            if(rows<4 || columns<4){
                printf("Rows and Columns must be greater or equal to 4 !\n");
            }
            else{
                break;
            }
        }

        char array[rows][columns];

        Player player1;
        Player computer;

        printf("Enter Player name: "); scanf("%c");
        fgets(player1.name, 257, stdin);
        player1.name[strlen(player1.name) - 1]='\0';
        player1.color = PLAYER1;
        player1.score = 0;

        computer.name[0] = 'P'; computer.name[1] = 'C', computer.name[2] = '\0';
        computer.color = PLAYER2;
        computer.score = 0;

        clear_board(rows,columns,array);
        draw_board(rows,columns,array, player1, computer);



        while( check_for_free_slots(rows,columns,array) ){
            take_player_turn(rows,columns,array, &player1 );
            draw_board(rows,columns,array, player1, computer);

            if(check_for_free_slots(rows,columns,array)== 0){ break; }

            takeComputerTurn(rows,columns,array, &computer );
            draw_board(rows,columns,array, player1, computer);
        }
        //print_winner();
        if (player1.score>computer.score){
            printf("WINNER! Great Job ");
            fputs(player1.name,stdout);
            printf(".\n");
        }
        else if (player1.score<computer.score){
            printf("You Lost. Hard luck. Have another try.\n");
        }
        else{
            printf("Draw.\n");
        }

        printf("1: Play Again\n2: Main Menu\n");
        scanf("%d", &response);
        switch(response){
            case 1:
                break;
            case 2:
                response = 0;
                break;
        }
    }while(response);
}


void player_vs_player(){
    int response = 1;
    do{
        int rows,columns;
        while(1){
            printf("Enter the size of the grid: ");
            scanf("%d %d",&rows,&columns);
            if(rows<4 || columns<4){
                printf("Rows and Columns must be greater or equal to 4 !\n");
            }
            else{
                break;
            }
        }

        char array[rows][columns];

        Player player1;
        Player player2;

        printf("Enter Player 1 name: "); scanf("%c");
        fgets(player1.name, 257, stdin);
        player1.name[strlen(player1.name) - 1]='\0';
        player1.color = PLAYER1;
        player1.score = 0;

        printf("Enter Player 2 name: ");
        fgets(player2.name, 257, stdin);
        player2.name[strlen(player2.name) - 1]='\0';
        player2.color = PLAYER2;
        player2.score = 0;

        clear_board(rows,columns,array);
        draw_board(rows,columns,array, player1, player2);



        while( check_for_free_slots(rows,columns,array) ){
            take_player_turn(rows,columns,array, &player1 );
            draw_board(rows,columns,array, player1, player2);

            if(check_for_free_slots(rows,columns,array)== 0){ break; }

            take_player_turn(rows,columns,array, &player2 );
            draw_board(rows,columns,array, player1, player2);
        }
        //print_winner();
        if(player1.score>player2.score){
            printf("You Won! Congrats ");
            fputs(player1.name,stdout);
            printf(".\n");
        }
        else if (player1.score<player2.score){
            printf("You Won! Congrats ");
            fputs(player2.name,stdout);
            printf(".\n");
        }
        else{
            printf("Draw.\n");
        }

        printf("1: Play Again\n2: Main Menu\n");
        scanf("%d", &response);
        switch(response){
            case 1:
                break;
            case 2:
                response = 0;
                break;
        }
    }while(response);
}
