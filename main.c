//
//  main.c
//  BlockBreaker
//
//  Created by Shuyang Sun on 4/29/13.
//  Copyright (c) 2013 Shuyang Sun. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>

#include "GameInitializations.h" // Header file containning the initialization for game.

/*********************************/
/******|| Custom Content! ||******/
/*********************************/
int displayTraceOfBall = 1; // Boolean to determin whether to display the trace of ball.
int maxTraceNumber = 2; // The maximum number of trace to display.
int sleepTime = 8 * 10000; // Time to sleep, 1 second = 1000000(one million) micro seconds.
/*********************************/
/******|| Custom Content! ||******/
/*********************************/

/******|| Game Status ||******/
int blocksCount; // Number of blocks in game.s
Direction currentDirectionOfBall; // Current direction of ball.
GameStatus currentGameStatus; // Current game status.
Point currentCell; // The current location of ball.
short int currentBoardMiddleIndex; // The current index of left most side of board. (Prefer the middle-right one if the length is even.)
// Two macros indicate the current location of board:
#define DISTANCE_FROM_LEFT_MOST_BOARD (BOARD_WIDTH/2) // The distance between left index and middle index.
#define DISTANCE_FROM_RIGHT_MOST_BOARD	((BOARD_WIDTH-1)/2) // The distance between right index and middle index.
/******|| Game Status ||******/

/******|| Function Declarations ||******/

// Declaration of local functions.
GameStatus updateCells(char arr[totalRow][totalColum], int ballTrace, Point traces[], int traceNumber);
void initArray(char arr[totalRow][totalColum]);

/* Declaration of global function. */
extern short int areValidSymbols(void); // Function from header file "GameInitializations.h"
extern Point PointMake(int, int); // Function from header file "GameInitializations.h"
extern void printArr(char arr[totalRow][totalColum]); // Function from header file "GameInitializations.h"

/******|| Function Declarations ||******/

int main(int argc, const char * argv[])
{
	// See if the game is playable. (There may be overlaps for symbols.)
	if (!areValidSymbols()){
		printf("INVALID GAME INITIALIZATION\n");
		return 1;
	}

	/* The 2D array containing all the data. the first space is row, the second is colum. */
	char arr[totalRow][totalColum];

	// Initialize the data in array.
	initArray(arr);

	// Initialize current game status and direction of ball.
	currentGameStatus = Game_Playing;
	currentDirectionOfBall = Up_Right;

	// If maxTraceNumber is -1, display all the traces. If it's not -1, the minimum value is 1.
	if (maxTraceNumber != -1)
		maxTraceNumber = MAX(1, maxTraceNumber);

	// Array storing pointers to grid points containning traces.
	int arrLen = MAX(maxTraceNumber, 1); // Prevent assigning arr[-1]
	Point arrForTrace[arrLen];

	// Update and print arr while the game is not over.
    putchar('\n');
	while (currentGameStatus == Game_Playing) {
		printArr(arr);
		putchar('\n');
		usleep(sleepTime);
		updateCells(arr, displayTraceOfBall, arrForTrace, maxTraceNumber);
	}
	// Print out the final array.
	printArr(arr);

	// If the game is over, generate message.
	if (blocksCount == 0){
		printf("YOU WON!\n");
	} else {
		printf("GAME OVER\n");
	}

	return 0;
}

/**********************************************************************************************
 * Function to update game.
 * Every time it update the current state once, depending on the current direction.
 * If it hits the wall or board, it will rebound.
 * If it hits the block, the block disapear and the ball will rebound.
 * If it hits the bottom edge, the ball disapears and game over.
 **********************************************************************************************/
GameStatus updateCells(char arr[totalRow][totalColum], int displayBallTrace, Point traces[], int traceNumber){
	// The total trace added count.
	static int totaltraceCount = 0;

	// The default game status is Game_Over, need to be set to Game_Playing if game playing.
	// If the game is playing,
	if (currentGameStatus == Game_Playing){
		Point nextCell; // Next cell the ball is going to hit.
		// Determin which cell is going to hit.
		if (GAME_WINDOW_WIDTH > 1){ // If board width is more than one.
			switch (currentDirectionOfBall) {
				case Up_Right:
					if (arr[currentCell.y-1][currentCell.x] != SYMBOL_FOR_SPACE) // If the up one is not space.
						nextCell = PointMake(currentCell.x, currentCell.y - 1); // Make it the one above current cell.
					else if (arr[currentCell.y][currentCell.x+1] != SYMBOL_FOR_SPACE) // If the right one is not space.
						nextCell = PointMake(currentCell.x + 1, currentCell.y); // Make it the one on the right of current cell.
					else // If it's gonna hit the upper-right one.
						nextCell = PointMake(currentCell.x + 1, currentCell.y - 1); // Make it the upper-right one.
					break;
				case Up_Left:
					if (arr[currentCell.y-1][currentCell.x] != SYMBOL_FOR_SPACE) // If the up one is not space.
						nextCell = PointMake(currentCell.x, currentCell.y - 1); // Make it the one above current cell.
					else if (arr[currentCell.y][currentCell.x-1] != SYMBOL_FOR_SPACE) // If the left one is not space.
						nextCell = PointMake(currentCell.x - 1, currentCell.y); // Make it the one on the left of current cell.
					else // If it's gonna hit the upper-left one.
						nextCell = PointMake(currentCell.x - 1, currentCell.y - 1); // Make it the upper-left one.
					break;
				case Down_Left:
					if (arr[currentCell.y+1][currentCell.x] != SYMBOL_FOR_SPACE) // If the down one is not space.
						nextCell = PointMake(currentCell.x, currentCell.y + 1); // Make it the one under current cell.
					else if (arr[currentCell.y][currentCell.x-1] != SYMBOL_FOR_SPACE) // If the left one is not space.
						nextCell = PointMake(currentCell.x - 1, currentCell.y); // Make it the one on the left of current cell.
					else // If it's gonna hit the bottom-left one.
						nextCell = PointMake(currentCell.x - 1, currentCell.y + 1); // Make it the bottom-left one.
					break;
				case Down_Right:
					if (arr[currentCell.y+1][currentCell.x] != SYMBOL_FOR_SPACE) // If the down one is not space.
						nextCell = PointMake(currentCell.x, currentCell.y + 1); // Make it the one under current cell.
					else if (arr[currentCell.y][currentCell.x+1] != SYMBOL_FOR_SPACE) // If the right one is not space.
						nextCell = PointMake(currentCell.x + 1, currentCell.y); // Make it the one on the right of current cell.
					else // If it's gonna hit the bottom-right one.
						nextCell = PointMake(currentCell.x + 1, currentCell.y + 1); // Make it the bottom-right one.
					break;
				default:
					nextCell = PointMake(0, 0);
					break;
			}
		} else {
			switch (currentDirectionOfBall) {
				case Up_Right:case Up_Left: // If it's going up:
					nextCell = PointMake(currentCell.x, currentCell.y-1); // The one above the current cell.
					break;
				case Down_Left: case Down_Right: // If it's going down:
					nextCell = PointMake(currentCell.x, currentCell.y+1); // The one under the current cell.
					break;
				default:
					nextCell = PointMake(0, 0);
					break;
			}
		}


		char contentOfNextCell = arr[nextCell.y][nextCell.x];
		// If the next cell equal to space or ball trace, update ball location and display it.
		if (contentOfNextCell == SYMBOL_FOR_SPACE || contentOfNextCell == SYMBOL_FOR_TRACE_OF_BALL_SLASH || contentOfNextCell == SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH){
			arr[nextCell.y][nextCell.x] = SYMBOL_FOR_BALL;

			/******|| BALL TRACING ||******/
			if (displayBallTrace == 0) {
				arr[currentCell.y][currentCell.x] = SYMBOL_FOR_SPACE;
			} else { // If we want to keep track of ball.
				if (currentDirectionOfBall == Up_Right || currentDirectionOfBall == Down_Left){ // User '/' for up-right or down-left
					arr[currentCell.y][currentCell.x] = SYMBOL_FOR_TRACE_OF_BALL_SLASH;
				} else { // User '\' for up-left or down-right
					arr[currentCell.y][currentCell.x] = SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH;
				}
				/******|| If there is a max number of trace, remove earlier trace. ||******/
				if (traceNumber != -1) {
					// If the list is not full, keep adding.
					if (totaltraceCount < traceNumber){
						traces[totaltraceCount] = currentCell;
					} else {
						Point tracePointToRemove = traces[0];
						if (arr[tracePointToRemove.y][tracePointToRemove.x] != SYMBOL_FOR_BALL) // If this point is not occupied by ball, remove it.
							arr[tracePointToRemove.y][tracePointToRemove.x] = SYMBOL_FOR_SPACE;
						// Shift array of traces to left by one.
						for (int i = 0; i < traceNumber-1; ++i)
							traces[i] = traces[i + 1];
						traces[traceNumber - 1] = currentCell; // Add the currentCell to array of traces.
					}
				}
				/******|| If there is a max number of trace, remove earlier trace. ||******/
				++totaltraceCount;
			}
			/******|| BALL TRACING ||******/

			currentCell = PointMake(nextCell.x, nextCell.y);
		} else {
			if (nextCell.y == totalRow - 1) { // Hit the bottom edge. (Game Over)
				arr[currentCell.y][currentCell.x] = SYMBOL_FOR_SPACE;
				currentCell = PointMake(0, 0);
				currentGameStatus = Game_Over;
			} else { // Hit the edge or block.
				// If it hits the block, make block disapear then switch direction, reduce the blockCount.
				// If it hit the edge, just switch direction.
				if (contentOfNextCell == SYMBOL_FOR_BLOCK){
					arr[nextCell.y][nextCell.x] = SYMBOL_FOR_SPACE;
					--blocksCount;
				}

				// Change the direction of ball.
				switch (currentDirectionOfBall) {
					case Up_Right:
						if (currentCell.y == nextCell.y) // Hit the right one.
							currentDirectionOfBall = Up_Left;
						else if (currentCell.x == nextCell.x) // Hit the top one.
							currentDirectionOfBall = Down_Right;
						else // Hit the upper-right one.
							currentDirectionOfBall = Down_Left;
						updateCells(arr, displayBallTrace, traces, traceNumber);
						break;
					case Up_Left:
						if (currentCell.y == nextCell.y) // Hit the left one.
							currentDirectionOfBall = Up_Right;
						else if (currentCell.x == nextCell.x) // Hit the top one.
							currentDirectionOfBall = Down_Left;
						else // Hit the upper-left one.
							currentDirectionOfBall = Down_Right;
						updateCells(arr, displayBallTrace, traces, traceNumber);
						break;
					case Down_Left:
						if (currentCell.y == nextCell.y) // Hit the left one.
							currentDirectionOfBall = Down_Right;
						else if (currentCell.x == nextCell.x) // Hit the bottom one.
							currentDirectionOfBall = Up_Left;
						else // Hit the bottom-left one.
							currentDirectionOfBall = Up_Right;
						updateCells(arr, displayBallTrace, traces, traceNumber);
						break;
					case Down_Right:
						if (currentCell.y == nextCell.y) // Hit the right one.
							currentDirectionOfBall = Down_Left;
						else if (currentCell.x == nextCell.x) // Hit the bottom one.
							currentDirectionOfBall = Up_Right;
						else // Hit the bottom-right one.
							currentDirectionOfBall = Up_Left;
						updateCells(arr, displayBallTrace, traces, traceNumber);
						break;
					default:
						nextCell = PointMake(0, 0);
						break;
				}
			}
		}
	}
	// If all the blocks disapeared, set currentGameStatus to Game_Over, then return.
	if (blocksCount == 0)
		currentGameStatus = Game_Over;

	/******|| Synchronize Middle Of the Board With the Ball ||******/
	short int tempMiddleIndex = currentCell.x;
	// If the board is not out of range:
	if (tempMiddleIndex - DISTANCE_FROM_LEFT_MOST_BOARD >= 1 && tempMiddleIndex + DISTANCE_FROM_RIGHT_MOST_BOARD <= totalColum - 3){
		// Erase current board:
		for (int i = currentBoardMiddleIndex - DISTANCE_FROM_LEFT_MOST_BOARD; i <= currentBoardMiddleIndex + DISTANCE_FROM_RIGHT_MOST_BOARD; ++i)
			arr[totalRow - 2][i] = SYMBOL_FOR_SPACE;
		// Draw the new board:
		currentBoardMiddleIndex = tempMiddleIndex;
		for (int i = currentBoardMiddleIndex - DISTANCE_FROM_LEFT_MOST_BOARD; i <= currentBoardMiddleIndex + DISTANCE_FROM_RIGHT_MOST_BOARD; ++i)
			arr[totalRow - 2][i] = SYMBOL_FOR_BOARD;
	}
	/******|| Synchronize Middle Of the Board With the Ball ||******/

	// Return current game status.
	return currentGameStatus;
}
/**********************************************************************************************
 ********************************|| END OF UPDATE CELL ||**************************************
 **********************************************************************************************/


/**************************************************/
/******|| Function Initializing Array Data ||******/
/**************************************************/
void initArray(char arr[totalRow][totalColum]){
	// Initialize block count.
	blocksCount = GAME_WINDOW_WIDTH * LINE_NUMBER_OF_BLOCKS;

	// Add edge character 'X' for top and bottom edge.
	for (short int colum = 0; colum < totalColum; ++colum){
		arr[0][colum] = SYMBOL_FOR_TOP_AND_BOTTOM_EDGE;
		arr[totalRow - 1][colum] = SYMBOL_FOR_TOP_AND_BOTTOM_EDGE;
	}

	// Add edge character 'X' for left and right edge, then add new line character '\n' for the right most colum.
	for (short int row = 0; row < totalRow; ++row){
		arr[row][0] = SYMBOL_FOR_LEFT_AND_RIGHT_EDGE;
		arr[row][totalColum - 2] = SYMBOL_FOR_LEFT_AND_RIGHT_EDGE;
		arr[row][totalColum - 1] = '\n';
	}

	// Add the data for blocks.
	for (short int row = 1; row <= LINE_NUMBER_OF_BLOCKS; ++row)
		for (short int colum = 1; colum <= GAME_WINDOW_WIDTH; ++colum)
			arr[row][colum] = SYMBOL_FOR_BLOCK;

	// Add the data for spaces.
	for (short int row = LINE_NUMBER_OF_BLOCKS + 1; row < totalRow - 1; ++row)
		for (short int colum = 1; colum <= GAME_WINDOW_WIDTH; ++colum)
			arr[row][colum] = SYMBOL_FOR_SPACE;

	// Draw the board on game window.
	short int boardLeftMostColumIndex = (totalColum - 3 - BOARD_WIDTH)/2+1; // The extra 3 spaces is for left, right edge and '\n'.
	short int boardRightMostColumIndex = boardLeftMostColumIndex + BOARD_WIDTH - 1;
	short int boardRowIndex = totalRow - 2; // The board should be one line above bottom edge.
	currentBoardMiddleIndex = (boardLeftMostColumIndex + boardRightMostColumIndex + 1)/2;
	for (short int colum = boardLeftMostColumIndex; colum <= boardRightMostColumIndex; ++colum)
		arr[boardRowIndex][colum] = SYMBOL_FOR_BOARD;

	// Draw the ball on game window.
	short int ballRowIndex = totalRow - 3; // The ball should be one line above the board.
	short int ballColumIndex = boardLeftMostColumIndex+ BOARD_WIDTH/2; // The ball should be at the middle of board.
	arr[ballRowIndex][ballColumIndex] = SYMBOL_FOR_BALL;
	currentCell = PointMake(ballColumIndex, ballRowIndex); // Set the current ball location.
}
/**************************************************/
/******|| Function Initializing Array Data ||******/
/**************************************************/
