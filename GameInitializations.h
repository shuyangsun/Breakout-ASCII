//
//  GameInitializations.h
//  BlockBreaker
//
//  Created by Shuyang Sun on 4/29/13.
//  Copyright (c) 2013 Shuyang Sun. All rights reserved.
//

#ifndef BlockBreaker_GameInitializations_h
#define BlockBreaker_GameInitializations_h

/**
 * Defining some game data, can be changed later.
 * The function "int areValidSymbols(void)" will determin whether there are overlaped symbols.
 */
#define MAX(a,b) ((a) > (b) ? (a):(b)) // Macro for MAX
#define LINE_NUMBER_OF_BLOCKS (4) // Line number of bricks.
#define GAME_WINDOW_WIDTH (27) // Width of whole game window.
#define SPACE_BETWEEN_LAST_LINE_OF_BRICKS_AND_BOTTOM_EDGE (8) // Space between the last line of bricks and the bottom edge
#define BOARD_WIDTH (3) // Width of board.
#define SYMBOL_FOR_LEFT_AND_RIGHT_EDGE ('|') // Symbol to represent left and right edge of window.
#define SYMBOL_FOR_TOP_AND_BOTTOM_EDGE ('X') // Symbol to represent top and bottom edge of window.
#define SYMBOL_FOR_BOARD ('T') // Symbol for board. (will be multiplied by BOARD_WIDTH later)
#define SYMBOL_FOR_BALL ('O') // Symbol for ball.
#define SYMBOL_FOR_BLOCK ('=') // Symbol for block.
#define SYMBOL_FOR_SPACE (' ') // Symbol for space.
#define SYMBOL_FOR_TRACE_OF_BALL_SLASH ('/') // Symbol for ball tracing (left to right).
#define SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH ('\\') // Symbol for ball tracing (right to left).

/******|| Redefine symbols for trace if game window width is one. ||******/
#if (GAME_WINDOW_WIDTH == 1)
#undef SYMBOL_FOR_TRACE_OF_BALL_SLASH
#undef SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH
#define SYMBOL_FOR_TRACE_OF_BALL_SLASH ('|') // Symbol for ball tracing.
#define SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH ('|') // Symbol for ball tracing.
#endif
/******|| Redefine symbols for trace if game window width is one. ||******/

/******|| Constant Values ||******/

/* Calculate the row and colum needed for the whole array. */
// The extra 3 rows are for holding top, bottom edge and board.
const short int totalRow = LINE_NUMBER_OF_BLOCKS + SPACE_BETWEEN_LAST_LINE_OF_BRICKS_AND_BOTTOM_EDGE + 3;
// The extra 3 colums are for holding left, right edges and the new line character '\n'.
const short int totalColum = GAME_WINDOW_WIDTH + 3;

/******|| Constant Values ||******/

/******|| Enums ||******/

/**
 * Typedefed enum to mark the current direction of ball.
 * If the ball hit the Left edge, direction += 1.
 * If the ball hit the Right edge, direction -= 1.
 * If the ball hit the Top edge, direction += 2.
 * If the ball hit the Board, direction -= 2.
 */
typedef enum{
	None = 0,
	Up_Left = 1,
	Up_Right = 2,
	Down_Left = 3,
	Down_Right = 4,
} Direction;

// // Typedefed enum to indicate the status of game.
typedef enum{
	Game_Over = 0,
	Game_Playing = 1,
} GameStatus;

/******|| Enums ||******/

/******|| Structures ||******/

// Structure indicating a point, contain two parameters: x, y.
typedef struct {
	int x;
	int y;
} Point;

/******|| Structures ||******/

/******|| Function to determine wether the game will be able to play. (There may be symbol conflicts.) ||******/
short int areValidSymbols(void)
{
	// If there are overlaped symbols, the function will not be able to run.
	int result;
	
	result = // All the conditions for areValidSymbols:
	(SYMBOL_FOR_BOARD != SYMBOL_FOR_BLOCK)&& // Board != Block
	(SYMBOL_FOR_BOARD != SYMBOL_FOR_BALL)&& // Board != Ball
	(SYMBOL_FOR_BLOCK != SYMBOL_FOR_BALL)&& // Block != Ball
	// SYMBOL_FOR_LEFT_AND_RIGHT_EDGE doesn't equal to the other three:
	(SYMBOL_FOR_LEFT_AND_RIGHT_EDGE != SYMBOL_FOR_BALL)&&
	(SYMBOL_FOR_LEFT_AND_RIGHT_EDGE != SYMBOL_FOR_BLOCK)&&
	(SYMBOL_FOR_LEFT_AND_RIGHT_EDGE != SYMBOL_FOR_BOARD)&&
	// SYMBOL_FOR_TOP_AND_BOTTOM_EDGE doesn't equal to the other three:
	(SYMBOL_FOR_TOP_AND_BOTTOM_EDGE != SYMBOL_FOR_BALL)&&
	(SYMBOL_FOR_TOP_AND_BOTTOM_EDGE != SYMBOL_FOR_BLOCK)&&
	(SYMBOL_FOR_TOP_AND_BOTTOM_EDGE != SYMBOL_FOR_BOARD)&&
	// SYMBOL_FOR_SPACE doesn't equal to all of them.
	(SYMBOL_FOR_SPACE != SYMBOL_FOR_BALL)&&
	(SYMBOL_FOR_SPACE != SYMBOL_FOR_BLOCK)&&
	(SYMBOL_FOR_SPACE != SYMBOL_FOR_BOARD)&&
	(SYMBOL_FOR_SPACE != SYMBOL_FOR_LEFT_AND_RIGHT_EDGE)&&
	(SYMBOL_FOR_SPACE != SYMBOL_FOR_TOP_AND_BOTTOM_EDGE)&&
	// SYMBOL_FOR_TRACE_OF_BALL_SLASH doesn't equal to all of them.
	(SYMBOL_FOR_TRACE_OF_BALL_SLASH != SYMBOL_FOR_BALL)&&
	(SYMBOL_FOR_TRACE_OF_BALL_SLASH != SYMBOL_FOR_BLOCK)&&
	(SYMBOL_FOR_TRACE_OF_BALL_SLASH != SYMBOL_FOR_BOARD)&&
	(SYMBOL_FOR_TRACE_OF_BALL_SLASH != SYMBOL_FOR_LEFT_AND_RIGHT_EDGE)&&
	(SYMBOL_FOR_TRACE_OF_BALL_SLASH != SYMBOL_FOR_TOP_AND_BOTTOM_EDGE)&&
	// #define SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH doesn't equal to all of them.
	(SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH != SYMBOL_FOR_BALL)&&
	(SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH != SYMBOL_FOR_BLOCK)&&
	(SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH != SYMBOL_FOR_BOARD)&&
	(SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH != SYMBOL_FOR_LEFT_AND_RIGHT_EDGE)&&
	(SYMBOL_FOR_TRACE_OF_BALL_BACK_SLASH != SYMBOL_FOR_TOP_AND_BOTTOM_EDGE)&&
	// Game window width must more or equal than one.
	(GAME_WINDOW_WIDTH >= 1)&&
	// Board width must be more than 0, and less or equal than game window width.
	(BOARD_WIDTH >= 1)&&
	(BOARD_WIDTH <= GAME_WINDOW_WIDTH);
	
	return result;
}

// Function make a point.
Point PointMake(int x, int y)
{
	Point res = {x, y};
	return res;
}

// Function to print array.
void printArr(char arr[totalRow][totalColum]){
	// Static variable to determin how many time this function is called.
	static int timeCalled = 0;
	// If this is not the first time function is called, delete characters printed out last time.
	if (timeCalled != 0){
		int numberOfEscapeCharacterB = (totalColum+1)*totalRow+2;
		for (int i = 0; i < numberOfEscapeCharacterB; ++i)
			putchar('\b');
	} else {
		++timeCalled;
	}

	// Print out the array.
	for (short int row = 0; row < totalRow - 1; ++row)
		for (short int colum = 0; colum < totalColum; ++colum)
				putchar(arr[row][colum]);
}

#endif

