#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>

const char PLAYER1='X';         //The player represented by this shape is displayed on the left of the screen in red
const char PLAYER2='O';         //The player represented by this shape is displayed on the right of the screen in yellow

typedef struct{
    int score;
    char name[257];
    char color;
    int numbOfMoves;
}Player;

typedef struct{
    time_t start;
    time_t end;
    double diff;
    int hrs,mins,secs;
}Time;

typedef struct{
    char board[100][100];
    Player plr1;
    Player plr2;
    Time gameTime;
    char oneOrTwoPlayers;
}State;


int stateIndex= -1;         // Keeps track of the current state in the stack
int maxRedo;                //
char gameMode;


void clearBoard();                          // Empties the board array from all garbage values
void drawBoard();                           // Prints the board and other information on the screen
void red();                                 // Makes the next outputs of the console colored in red
void yellow();                              // Makes the next outputs of the console colored in yellow
void reset();                               // Restores the text output color to the regular white
void printNamesAndScores();                 // Prints the name of the player along with his score
void playerVsComputer();                    // Launches a player vs computer game
void playerVsPlayer();                      // Launches a player vs player game
void takePlayerTurn();                      // Takes and processes the player's input
void takeComputerTurn();                    // Generates the next move for the computer
int  checkIfValidInput();                   // Checks whether the user's input is valid or not
void checkScores();                         // Changes the score based on the last entry to the board
bool checkForFreeSlots();                   // Checks whether there are still empty slots in the board so that the game may continue
void printWinnerPlayerVsComputer();         // Prints whether the player has won, lost or drawn against the computer
void printWinnerPlayerVsPlayer();           // Prints the game winner (or draw) in case of the two player mode
void saveState();                           // Saves the current state of the game in the timeline/stack array
void undo();                                // Undo the last move
void redo();                                // Redo one move
bool saveGame();                            // Saves the current game to a user chosen file
State *loadGame();                          // Loads a game from a user chosen file
void xml();                                 // Reads the xml file and determine the width,height and highscores

int main(){
    // Main Menu
    int quit = 0;
    //char newOrLoaded;
    do{
        system("cls");
        printf(" 1: Start a new game\n 2: Load a previous game\n 3: Highscores\n 4: Quit\n");
        char choice;
        scanf(" %1c", &choice);

        switch(choice){
            case '1':           // Launch a new game
                system("cls");
                printf(" 1: One player\n 2: Two players\n 3: Back\n");
                scanf(" %c",&choice);
                switch(choice){
                    case '1':
                        // Player v Computer
                        playerVsComputer(NULL);
                        break;

                    case '2':
                        // Player v Player
                        playerVsPlayer(NULL);
                        break;
                    case '3':
                        break;
                    default:
                        printf("Invalid Input ! A single digit is required.\n");
                }
                break;

            case '2':              // Load a game
                system("cls");
                int gameSave;
                do{
                    printf("Choose the game you would like to load:\n1: Game Save 1\n2: Game Save 2\n3: Game Save 3\n4: Back\n");
                    scanf("%d", &gameSave);
                    if(gameSave>=1 && gameSave<=4){
                        break;
                    }
                    printf("You must enter an integer from 1 to 4\n");
                }while( gameSave<1 || gameSave>3 );

                if(gameSave == 4) { break; }

                State *loadedGame;
                loadedGame = loadGame(gameSave);

                if(loadedGame[0].oneOrTwoPlayers == 't'){ playerVsPlayer(*loadedGame); }
                else{ playerVsComputer(*loadedGame); }

                break;

            case '3':  break;    //Highscores
            case '4':       // Quit the game
                quit = 1;
                break;
            //default: printf("Invalid Input !\n");
        }
       }
       /*else{
        system("cls");
        printf(" 1:Start a new game\n 2:Load a previous game\n 3:Quit\n");
        scanf("%c", &choice);
       }*/

    while(quit == 0);

    return 0;
}



void clearBoard(int rows,int columns,char array[rows][columns]){
    int i,j;
    for (i=0; i<rows; i++){
        for(j=0; j<columns; j++){
            array[i][j]=' ';
        }
    }

}


void drawBoard(int rows,int columns,char array[rows][columns], Player player1, Player player2, Time timeTaken){
    system("cls");
    printf("\tConnect Four\n");
    printf("\t");

    red();
    printNamesAndScores(player1);
    reset();

    printf("\t\t");

    yellow();
    printNamesAndScores(player2);
    reset();

    printf("\n");
    timeTaken.diff=difftime(timeTaken.end,timeTaken.start);
    timeTaken.hrs=(timeTaken.diff/(60*60));
    timeTaken.mins=((int)timeTaken.diff%(60*60))/60;
    timeTaken.secs=((int)timeTaken.diff%(60*60))%60;
    printf("\n\t\tTime: %d hours : %d minutes : %d seconds\n\n",timeTaken.hrs,timeTaken.mins,timeTaken.secs);

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
            printf("| ");
            if(array[i][j]==PLAYER1){
                red();
                printf("%c ", array[i][j]);
                reset();
            }
            else if(array[i][j]==PLAYER2){
                yellow();
                printf("%c ", array[i][j]);
                reset();
            }
            else{
                printf(" %c",array[i][j]);
            }

        }
        printf("|\n");
        for(j=0;j<columns;j++){
            printf("----");
        }
        printf("-\n");
    }
}

//color fn
void red () {
  printf("\033[1;31m");
}

void yellow (){
  printf("\033[1;33m");
}

void reset () {
  printf("\033[0m");
}


void printNamesAndScores(Player player){
    fputs(player.name,stdout);
    printf(": Score= %d & Number of moves= %d", player.score,player.numbOfMoves);
}

void playerVsComputer(State *loadedGame){
    system("cls");
    int response = 1;
    gameMode = 'o';
    do{
            int rows,columns;
            xml(&rows,&columns);
            char array[rows][columns];
            State Timeline[rows*columns];

            Player playerOne;
            Player computer;
            Time timeTaken;

            if(loadedGame == NULL){
                printf("Enter Player name: ");
                fgets(playerOne.name, 257, stdin);
                playerOne.name[strlen(playerOne.name) - 1]='\0';
                playerOne.color = PLAYER1;
                playerOne.score = 0;
                playerOne.numbOfMoves=0;

                strcpy(computer.name, "Computer");

                computer.color = PLAYER2;
                computer.score = 0;
                computer.numbOfMoves=0;

                timeTaken.start= time(NULL);
                timeTaken.end= time(NULL);

                clearBoard(rows,columns,array);
            }
            else{
                for (int i=0; i<rows; i++){
                    for (int j=0; j<columns; j++){
                        array[i][j] = loadedGame[0].board[i][j];
                    }
                }

                strcpy(playerOne.name, loadedGame[0].plr1.name);
                playerOne.color = loadedGame[0].plr1.color;
                playerOne.score = loadedGame[0].plr1.score;
                playerOne.numbOfMoves= loadedGame[0].plr1.numbOfMoves;

                strcpy(computer.name, loadedGame[0].plr2.name);
                computer.color = loadedGame[0].plr2.color;
                computer.score = loadedGame[0].plr2.score;
                computer.numbOfMoves= loadedGame[0].plr2.numbOfMoves;

                timeTaken.start= loadedGame[0].gameTime.start;
                timeTaken.end= loadedGame[0].gameTime.end;
            }

            saveState(rows,columns,array, playerOne, computer, timeTaken, Timeline);
            drawBoard(rows,columns,array, playerOne, computer, timeTaken);

            timeTaken.start=time(NULL);

            while( checkForFreeSlots(rows,columns,array) ){
                red();
                takePlayerTurn(rows,columns,array, &playerOne, &computer, &timeTaken, Timeline);
                reset();
                if(loadedGame == NULL) { timeTaken.end= time(NULL); }
                else{ timeTaken.end= time(NULL) + difftime(loadedGame[0].gameTime.end, loadedGame[0].gameTime.start); }
                drawBoard(rows,columns,array, playerOne, computer, timeTaken);

                if(checkForFreeSlots(rows,columns,array)== 0){ break; }
                takeComputerTurn(rows,columns,array, &computer, &playerOne, Timeline);
                if(loadedGame == NULL) { timeTaken.end= time(NULL); }
                else { timeTaken.end= time(NULL) + difftime(loadedGame[0].gameTime.end, loadedGame[0].gameTime.start); }
                saveState(rows,columns,array, playerOne, computer, timeTaken, Timeline);
                drawBoard(rows,columns,array, playerOne, computer, timeTaken);
            }

            printWinnerPlayerVsComputer(playerOne,computer);

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

void playerVsPlayer(State *loadedGame){
    system("cls");
    int response = 1;
    gameMode = 't';
    do{
            int rows,columns;
            xml(&rows,&columns);
            char array[rows][columns];
            State Timeline[rows*columns];

            Player playerOne;
            Player playerTwo;
            Time timeTaken;

            if(loadedGame == NULL){
                printf("Enter Player 1 name: ");
                fgets(playerOne.name, 257, stdin);
                playerOne.name[strlen(playerOne.name) - 1]='\0';
                playerOne.color = PLAYER1;
                playerOne.score = 0;
                playerOne.numbOfMoves=0;

                printf("Enter Player 2 name: ");
                fgets(playerTwo.name, 257, stdin);
                playerTwo.name[strlen(playerTwo.name) - 1]='\0';
                playerTwo.color = PLAYER2;
                playerTwo.score = 0;
                playerTwo.numbOfMoves=0;

                timeTaken.start=time(NULL);
                timeTaken.end=time(NULL);

                clearBoard(rows,columns,array);
            }

            else{
                for (int i=0; i<rows; i++){
                    for (int j=0; j<columns; j++){
                        array[i][j] = loadedGame[0].board[i][j];
                    }
                }

                strcpy(playerOne.name, loadedGame[0].plr1.name);
                playerOne.color = loadedGame[0].plr1.color;
                playerOne.score = loadedGame[0].plr1.score;
                playerOne.numbOfMoves= loadedGame[0].plr1.numbOfMoves;

                strcpy(playerTwo.name, loadedGame[0].plr2.name);
                playerTwo.color = loadedGame[0].plr2.color;
                playerTwo.score = loadedGame[0].plr2.score;
                playerTwo.numbOfMoves= loadedGame[0].plr2.numbOfMoves;

                timeTaken.start= loadedGame[0].gameTime.start;
                timeTaken.end= loadedGame[0].gameTime.end;
            }



            saveState(rows,columns, array, playerOne, playerTwo, timeTaken, Timeline);
            drawBoard(rows,columns, array, playerOne, playerTwo, timeTaken);

            timeTaken.start=time(NULL);

            while( checkForFreeSlots(rows,columns,array) ){
                red();
                takePlayerTurn(rows,columns,array, &playerOne, &playerTwo, &timeTaken, Timeline);
                reset();
                if(loadedGame == NULL) { timeTaken.end= time(NULL); }
                else{ timeTaken.end= time(NULL) + difftime(loadedGame[0].gameTime.end, loadedGame[0].gameTime.start); }
                drawBoard(rows,columns,array, playerOne, playerTwo, timeTaken);

                if(checkForFreeSlots(rows,columns,array)== 0){ break; }

                yellow();
                takePlayerTurn(rows,columns,array, &playerTwo, &playerOne, &timeTaken, Timeline);
                reset();
                if(loadedGame == NULL) { timeTaken.end= time(NULL); }
                else{ timeTaken.end= time(NULL) + difftime(loadedGame[0].gameTime.end, loadedGame[0].gameTime.start); }
                drawBoard(rows,columns,array, playerOne, playerTwo, timeTaken);
            }

            printWinnerPlayerVsPlayer(playerOne,playerTwo);

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


void takePlayerTurn(int rows,int columns,char array[rows][columns], Player *playerA, Player *playerB, Time *timeTaken, State Timeline[rows*columns] ){
    char colDesired[100];
    long int enteredCol;
    double floatCol;
    while(1){
        do{
           for (int i=0; i<strlen((*playerA).name); i++){
           printf("%c", (*playerA).name[i]);
           }
           printf("'s turn, please enter col num (-1:Undo, -2:Redo, -3:Save and Exit, -4:Exit) : ");
           scanf("%s", colDesired);
           enteredCol=atol(colDesired);
           floatCol=atof(colDesired);
           if (enteredCol == 0 || (double)enteredCol!=floatCol){
            printf("Invalid Input. You have to enter an Integer\n");
           }
        }
        while(enteredCol == 0 || (double)enteredCol!=floatCol);
        if (enteredCol==-4){
                reset();
                exit(0);
        }
        enteredCol--;
        if( checkIfValidInput(rows, columns, array, enteredCol) == 1){
            int i=0;
            while(array[i][enteredCol] ==' ' && i<rows){
                i++;
            }
            array[i-1][enteredCol]= (*playerA).color;
            (*playerA).numbOfMoves++;
            checkScores(rows, columns, array, i-1, enteredCol, &((*playerA).score) );
            if(gameMode == 't'){
                if( (*playerA).color == PLAYER1 ){
                    (*timeTaken).end=time(NULL);
                    saveState(rows,columns,array, *playerA, *playerB, *timeTaken, Timeline);
                }
                else{
                    (*timeTaken).end=time(NULL);
                    saveState(rows,columns,array, *playerB, *playerA, *timeTaken, Timeline);
                }
            }

            maxRedo = stateIndex;
            break;
        }
        else if( checkIfValidInput(rows, columns, array, enteredCol) == 2 ){   //Perform Undo
            if(stateIndex > 0){
                if( (*playerA).color == PLAYER1 ){
                    undo( rows, columns, array, &(*playerA), &(*playerB), Timeline);
                    if(gameMode == 't'){
                        break;
                    }
                    else{
                        (*timeTaken).end=time(NULL);
                        drawBoard(rows,columns,array, *playerA, *playerB, *timeTaken);
                    }
                }
                else{
                    undo( rows, columns, array, &(*playerB), &(*playerA), Timeline);
                    if(gameMode == 't'){
                        break;
                    }
                    else{
                        (*timeTaken).end=time(NULL);
                        drawBoard(rows,columns,array, *playerA, *playerB, *timeTaken);
                    }
                }
            }
            else{
                printf("Nothing To Undo !\n");
            }
        }

        else if( checkIfValidInput(rows, columns, array, enteredCol) == 3 ){       //Perform Redo
            if(stateIndex < maxRedo){
                if( (*playerA).color == PLAYER1 ){
                    redo( rows, columns, array, &(*playerA), &(*playerB), Timeline);
                    if(gameMode == 't'){
                        break;
                    }
                    else{
                        (*timeTaken).end=time(NULL);
                        drawBoard(rows,columns,array, *playerA, *playerB, *timeTaken);
                    }
                }
                else{
                    redo( rows, columns, array, &(*playerB), &(*playerA), Timeline);
                    if(gameMode == 't'){
                        break;
                    }
                    else{
                        (*timeTaken).end=time(NULL);
                        drawBoard(rows,columns,array, *playerA, *playerB, *timeTaken);
                    }
                }
            }
            else{
                printf("Nothing To Redo !\n");
            }
        }

        else if( checkIfValidInput(rows, columns, array, enteredCol) == 4 ){  //Save & quit
            system("cls");
            reset();
            int gameSave;
            do{
                printf("Choose the file you would like to save to:\n1:Game Save 1\n2:Game Save 2\n3:Game Save 3\n");
                scanf("%d", &gameSave);
                if(gameSave>=1 && gameSave<=3){
                    break;
                }
                printf("You must enter an integer from 1 to 3\3");
            }while( gameSave<1 || gameSave>3 );

            if(saveGame(gameSave, &Timeline[stateIndex])){
                printf("Save Successful");
            }
            else{
                printf("Failed to save");
            }
            exit(0);
        }


        else{
            printf("Invalid Input !\n");
        }
        }
    }


void takeComputerTurn(int rows,int columns,char array[rows][columns], Player *computer, Player *player, State Timeline[rows*columns] ){
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
    (*computer).numbOfMoves++;
    checkScores(rows, columns, array, i-1, j, &((*computer).score) );
    //saveState(rows,columns,array, *player, *computer, Timeline);
    maxRedo = stateIndex;
}


int checkIfValidInput(int rows,int columns,char array[rows][columns], int enteredCol){
    if(enteredCol>=columns || enteredCol < -4){
        return 0;
    }
    else if(array[0][enteredCol]== PLAYER1 || array[0][enteredCol]==PLAYER2 ){
        return 0;
    }
    else if (enteredCol == -2){
        return 2;
    }
    else if (enteredCol == -3){
        return 3;
    }
    else if (enteredCol == -4){
        return 4;
    }
    else{
        return 1;
    }
}

void checkScores(int rows,int columns,char array[rows][columns], int enteredRow, int enteredCol, int *playerScore){
    //Check Horizontal
    if(enteredCol<columns-3 && ( array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+3] ) ){
        (*playerScore)++;
    }

    if(enteredCol>=3 && ( array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-3] ) ){
        (*playerScore)++;
    }

    if(enteredCol<=columns-3 && enteredCol>0 && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+2] ){
        (*playerScore)++;
    }

    if(enteredCol<=columns-2 && enteredCol>1 && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol+1] ){
        (*playerScore)++;
    }


    //Check Vertical
    if(enteredRow<rows-3 && (array[enteredRow][enteredCol] == array[enteredRow+1][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol])){
        (*playerScore)++;
    }

    //Check Diagonals
    if(enteredCol<columns-3){
       if(enteredRow<=rows-3 && (( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol+3] )) ){
            (*playerScore)++;
        }
         if(enteredRow<=rows-3 &&  ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] )){
            (*playerScore)++;
        }
         if(enteredRow<=rows-3 && enteredCol>1 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] )){
            (*playerScore)++;
        }
        if(enteredRow<=rows-3 && enteredCol>0 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow-3][enteredCol+3] )){
            (*playerScore)++;
        }
         if(enteredRow>=3 && enteredCol>1 && enteredRow<rows-2 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol-2] )){
            (*playerScore)++;
        }
         if(enteredRow>=3 && enteredCol>0 && enteredRow<rows-1 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && enteredCol>1 && enteredRow<rows-1 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] )){
            (*playerScore)++;
        }

    }
    if(enteredCol>=3){
        if(enteredRow<rows-3 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow+3][enteredCol-3] )){
            (*playerScore)++;
        }
        if(enteredRow<rows-3 && enteredCol<columns-1 && enteredRow>0 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] )){
            (*playerScore)++;
        }
        if(enteredRow<rows-3 && enteredCol<columns-1 && enteredRow>1 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] )){
            (*playerScore)++;
        }
        if(enteredRow<rows-3 && enteredCol<columns-2 && enteredRow>2 && ( array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] )){
            (*playerScore)++;
        }
        if(enteredRow<rows-3 && enteredCol<columns-1 && enteredRow>0 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow-3][enteredCol-3] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && enteredCol<columns-1 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] )){
            (*playerScore)++;
        }
         if(enteredRow>=3 && enteredCol<columns-2 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol-1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol+1] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && enteredCol<columns-2 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow-2][enteredCol+2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] )){
            (*playerScore)++;
        }
        if(enteredRow>=3 && enteredCol<columns-1 && ( array[enteredRow][enteredCol]==array[enteredRow-1][enteredCol+1] && array[enteredRow][enteredCol]==array[enteredRow+2][enteredCol-2] && array[enteredRow][enteredCol]==array[enteredRow+1][enteredCol-1] )){
            (*playerScore)++;
        }
    }

//if( array[enteredRow][enteredCol]==array[enteredRow][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol] && array[enteredRow][enteredCol]==array[enteredRow][enteredCol] )
}


bool checkForFreeSlots(int rows,int columns,char array[rows][columns]){
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


void printWinnerPlayerVsComputer(Player player1,Player computer){
    if (player1.score>computer.score){
            red();
            printf("WINNER! Great job, ");
            fputs(player1.name,stdout);
            printf(".\n");
            reset();
        }
        else if (player1.score<computer.score){
            yellow();
            printf("You Lost. Hard luck. Have another try.\n");
            reset();
        }
        else{
            printf("Draw.\n");
        }
}


void printWinnerPlayerVsPlayer(Player player1,Player player2){
    if(player1.score>player2.score){
            red();
            fputs(player1.name,stdout);
            printf(" Wins! Congrats !\n");
            reset();
        }
        else if (player1.score<player2.score){
            yellow();
            fputs(player2.name,stdout);
            printf(" Wins! Congrats !\n");
            reset();
        }
        else{
            printf("Draw.\n");
        }
}

void saveState(int rows, int columns, char array[rows][columns], Player p1, Player p2, Time timeTaken, State Timeline[rows*columns]){
    if(stateIndex < (rows*columns)-1){
        stateIndex += 1;
        for (int i=0; i<rows; i++){
            for(int j=0; j<columns; j++){
                Timeline[stateIndex].board[i][j] = array[i][j];
            }
        }


        strcpy(Timeline[stateIndex].plr1.name, p1.name);
        Timeline[stateIndex].plr1.score = p1.score;
        Timeline[stateIndex].plr1.color= p1.color;
        Timeline[stateIndex].plr1.numbOfMoves = p1.numbOfMoves;

        strcpy(Timeline[stateIndex].plr2.name, p2.name);
        Timeline[stateIndex].plr2.score = p2.score;
        Timeline[stateIndex].plr2.color= p2.color;
        Timeline[stateIndex].plr2.numbOfMoves = p2.numbOfMoves;

        Timeline[stateIndex].gameTime.start = timeTaken.start;
        Timeline[stateIndex].gameTime.end = timeTaken.end;

        Timeline[stateIndex].oneOrTwoPlayers = gameMode;
    }
}

void undo(int rows,int columns,char array[rows][columns], Player *p1, Player *p2, State Timeline[rows*columns]){
    if(stateIndex > 0){ stateIndex -= 1 ; }
    //if(stateIndex > 1 && gameMode == 'o'){ stateIndex -= 2 ; }

    for (int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            array[i][j]= Timeline[stateIndex].board[i][j];
        }
    }

    ((*p1).score) = Timeline[stateIndex].plr1.score;
    ((*p1).numbOfMoves) = Timeline[stateIndex].plr1.numbOfMoves;
    ((*p2).score) = Timeline[stateIndex].plr2.score;
    ((*p2).numbOfMoves) = Timeline[stateIndex].plr2.numbOfMoves;
}

void redo(int rows,int columns,char array[rows][columns], Player *p1, Player *p2, State Timeline[rows*columns]){
    if(stateIndex < maxRedo){ stateIndex += 1 ; }

    for (int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            array[i][j]= Timeline[stateIndex].board[i][j];
        }
    }
    ((*p1).score) = Timeline[stateIndex].plr1.score;
    ((*p1).numbOfMoves) = Timeline[stateIndex].plr1.numbOfMoves;
    ((*p2).score) = Timeline[stateIndex].plr2.score;
    ((*p2).numbOfMoves) = Timeline[stateIndex].plr2.numbOfMoves;
}

bool saveGame(int gameSave, State *currentState){

    FILE *saveFile;

    switch(gameSave){
        case 1:
            saveFile = fopen("Game Save 1.bin", "wb");
            break;
        case 2:
            saveFile = fopen("Game Save 2.bin", "wb");
            break;
        case 3:
            saveFile = fopen("Game Save 3.bin", "wb");
            break;
    }

    if(saveFile == NULL){ return false; }

    if( fwrite(currentState, sizeof(State), 1, saveFile) != 1){return false;}

    if(fclose(saveFile) == EOF){return false;}

    fclose(saveFile);
    return true;
}

State *loadGame(int gameSave){
    FILE *saveFile;

    switch(gameSave){
        case 1:
            saveFile = fopen("Game Save 1.bin", "rb");
            break;
        case 2:
            saveFile = fopen("Game Save 2.bin", "rb");
            break;
        case 3:
            saveFile = fopen("Game Save 3.bin", "rb");
            break;
    }

    if(saveFile == NULL){return NULL;}

    State *loadedGame = malloc(sizeof(State) * 1);

    if(fread(loadedGame, sizeof(State), 1, saveFile) != 1){
            free(loadedGame);
            return NULL;
    }

    if(fclose(saveFile) == EOF){
            free(loadedGame);
            return NULL;
    }

    return loadedGame;
}




void xml(int *rows,int *columns){
    int highScores;
    char conf1[]="<Configurations>",width1[]="<Width>",height1[]="<Height>",highscores1[]="<Highscores>";
    char conf2[]="</Configurations>",width2[]="</Width>",height2[]="</Height>",highscores2[]="</Highscores>";
    int i=0,p, len=0, fileCorrupted;
    int j,k,l,m,n,q,h,s,r,f;
    int numbOfTrials=0;


 while(numbOfTrials<3){
        //system("cls");
        char y,x[500]= {'0'};
        char path[500]={'0'};
        char height[256],width[256],scores[256];
        printf("Enter the path of the file without the quotations: \n");
        fileCorrupted=0,i=0,len=0;
        fflush(stdin); gets(path);
        for (p=0;p<strlen(path);p++){
           if(path[p]=='\\'){
               path[p]='/';
            }
        }


    FILE *file= fopen(path, "r");

    while((y=fgetc(file)) && y!=EOF){
        if(y!=' '&&y!='\n'&&y!='\t'){
            x[i]=y;
            i++;
            len++;
        }
    }

    for(i=0; i<sizeof(conf1)-1; i++){
        if(x[i]!=conf1[i]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(j=0;j<sizeof(height1)-1; j++){
        if(x[i+j]!=height1[j]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }
    if (fileCorrupted==0){
        for (k=0; k<sizeof(height)-1; k++){
            if(isdigit(x[i+j+k])){
                height[k]=x[i+j+k];
            }
            else{
                break;
            }
        }
    }
    *rows=atoi(height);
    if(*rows<4){
        printf("The width and height should be greater or equal to 4\n");
        fileCorrupted=1;
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(l=0; l<sizeof(height2)-1; l++){
        if(x[i+j+k+l]!=height2[l]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(m=0; m<sizeof(width1)-1; m++){
        if(x[i+j+k+l+m]!=width1[m]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }

    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    if (fileCorrupted==0){
        for (n=0; n<sizeof(width)-1; n++){
            if(isdigit(x[i+j+k+l+m+n])){
                width[n]=x[i+j+k+l+m+n];
            }
            else{
                break;
            }
        }
    }
    *columns=atoi(width);
    if(*rows<4){
        printf("The width and height should be greater or equal to 4\n");
        fileCorrupted=1;
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(q=0; q<sizeof(width2)-1; q++){
        if(x[i+j+k+l+m+n+q]!=width2[q]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(h=0; h<sizeof(highscores1)-1; h++){
        if(x[i+j+k+l+m+n+q+h]!=highscores1[h]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    if (fileCorrupted==0){
        for (s=0; s<sizeof(scores)-1; s++){
            if(isdigit(x[i+j+k+l+m+n+q+h+s])){
                scores[s]=x[i+j+k+l+m+n+q+h+s];
            }
            else{
                break;
            }
        }
    }
    highScores=atoi(scores);

    for(r=0; r<sizeof(highscores2)-1; r++){
        if(x[i+j+k+l+m+n+q+h+s+r]!=highscores2[r]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }

    for(f=0; f<sizeof(conf2)-1; f++){
        if(x[i+j+k+l+m+n+q+h+s+r+f]!=conf2[f]){
            printf("FILE CORRUPTED!\n");
            fileCorrupted=1;
            break;
        }
    }
    if(fileCorrupted==1){
        numbOfTrials++;
        fclose(file);
        continue;
    }
    if(fileCorrupted==0){
        fclose(file);
        break;
    }


    }
    if (numbOfTrials==3){
        printf("Default Values Are Loaded: Columns=7 , Rows=9 , Highscores=10\n");
        *columns=7;
        *rows=9;
        highScores=10;

    }
}




