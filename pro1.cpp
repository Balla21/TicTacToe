#include <unistd.h> 
#include <fcntl.h> 
#include <sys/stat.h>  
#include <sys/types.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>

#include "const.h"

using namespace std;


char userInput();
void resetGameBoard(char board[][7], int s);
void drawGameBoard(char board[][7], int s);
void player1Move(char board[][7], char find, int s);
int gameCheck(char board[][7], int s);


int main() {
	int gameResult, numberMoves = 1;;
	char choice;
	// board game
	char board[7][7];

	// establishing connection between players
	int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	printf("@p1: f1 = %d  f2 = %d\n", f1, f2);
	printf("Searching for opponent... \n");
	int fd_wr = open(myfifo_1to2, O_WRONLY);
	int	fd_rd = open(myfifo_2to1, O_RDONLY);

	//beginning of the game
	printf("-------TIC TAC TOE GAME-------\n");
	resetGameBoard(board, 7);
	drawGameBoard(board, 7);

	//starting game
	while (true || gameResult != 1 || gameResult != 2 ) {
		//check playing status 
		gameResult = gameCheck(board, 7);
		if (gameResult == 1 || gameResult == 2 ){
			break;
		}
		if(numberMoves == 5){
			break;
		}

		// user input validation checking for the board
		choice = userInput();
		player1Move(board, choice, 7);

		// communication with the other player
		write(fd_wr, board, sizeof(board) );
		read(fd_rd, board, sizeof(board) );
		drawGameBoard(board, 7);
		
		numberMoves++;
	}

	// display game result
	printf("--------GAME OVER--------\n");
	if(gameResult == 1){
		printf("PLAYER 1 WINS\n\n");
	}
	else if(gameResult == 2){
		printf("PLAYER 2 WINS\n\n");
	}
	else{
		printf("DRAW\n\n");
	}

	//closing connection between players
	close(fd_wr);
	close(fd_rd);
	unlink(myfifo_1to2);
	unlink(myfifo_2to1);
}

// input validation
char userInput(){
	char position;
	do{
		printf("Your turn[X], pick a position from board: ");
		cin >> position;
	}
	while( (position != '.') && (position != '1') && (position != '2') && (position != '3') && (position != '4') && 
			(position != '5') && (position != '6') && (position != '7') && (position != '8') );
	return position;
}


//Initializing game board
void resetGameBoard(char board[][7], int s){
	for (int i = 0; i<7; i++)
	{
		for (int j = 0; j<7; j++)
		{
			board[i][j] = 'E';
		}
	}
	board[1][1] = '.';
	board[1][3] = '1';
	board[1][5] = '2';
	board[3][1] = '3';
	board[3][3] = '4';
	board[3][5] = '5';
	board[5][1] = '6';
	board[5][3] = '7';
	board[5][5] = '8';
}

//drawing game board
void drawGameBoard(char board[][7], int s){
	for(int line = 0; line < s; line++){
		for (int col = 0; col < 7; col++) {
			if( (line % 2 == 0) && (col % 2 == 0) ) {
				board[line][col] = '+';
			}
			else if( (line % 2 == 0) && (col % 1 == 0)  ) {
				board[line][col] = '-';
			}
			else if( (line % 1 == 0) && (col % 2 == 0) ){
				board[line][col] = '|';
			}
		}
	}

	for(int line = 0; line < s; line++){
		printf("\n");
		for (int col = 0; col < 7; col++) {
			printf("%c", board[line][col]);
		}
	}
	printf("\n");
}

//processing player 1 move
void player1Move(char board[][7], char find, int s){
	bool signal = 1;
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; i < 7; j++)
		{
			if (board[i][j] == find)
			{
				board[i][j] = 'X';
				signal = 0;
				break;
			}
		}
		if (signal == 0)
		{
			break;
		}
	}
}


//verification of gaming status
int gameCheck(char board[][7], int s){
	int gameResult = 0;
	// player 1 is winner
	if (
		//horizontal checking
		board[1][1] == 'X' && board[1][3] == 'X' && board[1][5] == 'X' ||
		board[3][1] == 'X' && board[3][3] == 'X' && board[3][5] == 'X' ||
		board[5][1] == 'X' && board[5][3] == 'X' && board[5][5] == 'X' ||

		//vertical checking
		board[1][1] == 'X' && board[3][1] == 'X' && board[5][1] == 'X' ||
		board[1][3] == 'X' && board[3][3] == 'X' && board[5][3] == 'X' ||
		board[1][5] == 'X' && board[3][5] == 'X' && board[5][5] == 'X' ||

		//diagonal checking
		board[1][1] == 'X' && board[3][3] == 'X'&& board[5][5] == 'X' ||
		board[1][5] == 'X' && board[3][3] == 'X'&& board[5][1] == 'X')
	{
		gameResult = 1;
	}

	// player 2 is winner
	else if (
			//horizontal checking
			board[1][1] == 'O' && board[1][3] == 'O' && board[1][5] == 'O' ||
			board[3][1] == 'O' && board[3][3] == 'O' && board[3][5] == 'O' ||
			board[5][1] == 'O' && board[5][3] == 'O' && board[5][5] == 'O' ||

			//vertical checking
			board[1][1] == 'O' && board[3][1] == 'O' && board[5][1] == 'O' ||
			board[1][3] == 'O' && board[3][3] == 'O' && board[5][3] == 'O' ||
			board[1][5] == 'O' && board[3][5] == 'O' && board[5][5] == 'O' ||

			//diagonal checking
			board[1][1] == 'O' && board[3][3] == 'O' && board[5][5] == 'O' ||
			board[1][5] == 'O' && board[3][3] == 'O' && board[5][1] == 'O')
	{
		gameResult = 2;
	}
	return gameResult;
}

