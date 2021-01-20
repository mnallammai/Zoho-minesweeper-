#include<stdio.h>
#include<stdlib.h>
#ifdef _WIN32
#include<conio.h>
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

int size;
// Function to choose the difficulty level of the game
void chooseDifficultyLevel(int *size, int *mine){
    int level;
    printf("Enter the Difficulty Level\n");
    printf("1. BEGINNER (9 * 9 Cells and 10 Mines)\n");
    printf("2. INTERMEDIATE (16 * 16 Cells and 40 Mines)\n");
    printf("3. ADVANCED (24 * 24 Cells and 99 Mines)\n");
    scanf ("%d", &level);

    while(level > 3 || level < 0){
        printf("Please enter proper difficulty level\n");
        scanf ("%d", &level);
    }
    switch(level){
        case 1:
            *size = 9;
            *mine = 10;
            break;

        case 2:
            *size = 16;
            *mine = 40;
            break;

        case 3:
            *size = 24;
            *mine = 99;
    }
}

// Function to print the board
void printBoard(int size, char board[size][size]){
    system(CLEAR_CMD);
    printf("      ");
    for(int i=0; i<size; i++)
        if(i<10)
            printf(" %d  ",i);
        else
            printf("%d  ",i);
    printf("\n    ");

    for(int i=0; i<size; i++)
        printf("----");

    for(int i=0; i<size; i++){
        printf("\n");
        for(int j=0; j<size; j++){
            if(j == 0)
                if(i<10) {
                    printf(" %d |  ", i);
                } else {
                    printf("%d |  ", i);
                }
            printf(" %c  ", board[i][j]);
        }
    }
    printf("\n");
}

// Function to check if player won
int win(int size, char actual_board[size][size], char display_board[size][size]){
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(display_board[i][j] == '-' || (display_board[i][j] == 'f' && actual_board[i][j] != '*'))//if display board has vaccent space then skip or if display board has flag at that location and no bomb is present in actual then skip
                return 0;
    return 1;
}

void dfs(int row, int col, char actual_board[size][size], char display_board[size][size], int size) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return;
    }
    if (display_board[row][col] != '-' || actual_board[row][col] == '*') {
        return;
    }
    display_board[row][col] = actual_board[row][col];
    if (actual_board[row][col] == '0') {
        dfs(row + 1, col + 1, actual_board, display_board, size);
        dfs(row + 1, col - 1, actual_board, display_board, size);
        dfs(row + 1, col, actual_board, display_board, size);

        dfs(row - 1, col + 1, actual_board, display_board, size);
        dfs(row - 1, col - 1, actual_board, display_board, size);
        dfs(row - 1, col, actual_board, display_board, size);

        dfs(row, col + 1, actual_board, display_board, size);
        dfs(row, col - 1, actual_board, display_board, size);
    }
}

void startGame(int size, char actual_board[size][size], char display_board[size][size]){
    int lost = 0, won = 0;
    while(1){
        printBoard(size, display_board);
        printf("\n1. Set flag");
        printf("\n2. Dig place\n");
        int choice;
        scanf("%d",&choice);
        while(choice < 1 || choice > 2){
            printf("Please enter a proper choice\n");
            scanf("%d",&choice);
        }
        int row, col;
        printf("Enter the place (row, column): ");
        scanf("%d %d",&row,&col);
        if(choice == 1){
            display_board[row][col] = 'f';
        } else if(choice == 2) {
            if(actual_board[row][col] == '*'){
                lost = 1;
                break;
            } else if (actual_board[row][col] != '0') {
                display_board[row][col] = actual_board[row][col];
            } else {
                dfs(row, col, actual_board, display_board, size);
            }
        }
        if(win(size, actual_board, display_board)){
            won = 1;
            break;
        }
    }
    if(lost){
        printBoard(size, actual_board);
        printf("\n\n\t You Lost !!\n");
    }
    else if(won){
        printBoard(size, actual_board);
        printf("\n\n\t You Won !!\n");
    }
}

// Function to return the number of mines around
int countMinesAround(int size, char actual_board[size][size], int row, int col){
    int count = 0;
    for(int i=row-1; i<row+2; i++)
        for(int j=col-1; j<col+2; j++)
            if(i>=0 && j>=0 && i<size && j<size)
                if(actual_board[i][j] == '*')
                    count++;
    return count;
}

// Function to assign values to the board
void assignValuesToBoard(int size, int mine, char actual_board[size][size], char display_board[size][size]){
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++){
            display_board[i][j] = '-';
            actual_board[i][j] = '0';
        }

    for(int i=0; i<mine; i++){
        int row = rand() % size;
        int col = rand() % size;
        while(actual_board[row][col] == '*'){
            row = rand() % size;
            col = rand() % size;
        }
        actual_board[row][col] = '*';
    }

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(actual_board[i][j] != '*'){
                int count = countMinesAround(size, actual_board, i, j);
                if(count > 0)
                    actual_board[i][j] = count + '0';
            }
}

int main(){
    int mine;
    chooseDifficultyLevel(&size, &mine);

    char actual_board[size][size], display_board[size][size];
    assignValuesToBoard(size, mine, actual_board, display_board);

    startGame(size, actual_board, display_board);
    return 0;
}
