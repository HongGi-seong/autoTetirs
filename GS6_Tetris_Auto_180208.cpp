#if 1
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define BLOCK_IMAGE_SIZE 21
#define BACKGROUND_WIDTH 1600
#define BACKGROUND_HEIGHT 900
#define SCORE_BOARD_WIDTH 105
#define SCORE_BOARD_HEIGHT 147
#define DEFAULT_HOR 4
#define DEFAULT_VER -1
#define DEFAULT_TETRIS_IMAGE_X 680
#define DEFAULT_TETRIS_IMAGE_Y 140


typedef struct strct_block_info {
	unsigned char model ;				// the value of current block model
	unsigned char next_model ;			// the value of next block model
	unsigned char spin ;				// the next rotating block model value of the current block model 
	int hor ;							// the horizon value of the current block model
	int ver ;							// the vertical value of the current block model 
} block_info ;


block_info bval = { 0, 0, 0, DEFAULT_HOR, DEFAULT_VER } ;
unsigned int is_score, target_score, game, image_x = DEFAULT_TETRIS_IMAGE_X, image_y = DEFAULT_TETRIS_IMAGE_Y, board [ 30 ] [ 16 ] ;
const unsigned int block [ 7 ] [ 4 ] [ 4 ] [ 4 ] = {

{                                                                               // l tetromino
{ {0, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 2, 0}, {0, 0, 0, 0} },
{ {0, 0, 2, 0}, {0, 0, 2, 0}, {0, 2, 2, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 2, 0, 0}, {0, 2, 2, 2}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 2, 2, 0}, {0, 2, 0 ,0}, {0, 2, 0 ,0} } 
},

{                                                                               // i tetromino
{ {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 3, 3, 3}, {0, 0, 0, 0} },
{ {0, 3, 0, 0}, {0, 3, 0, 0}, {0, 3, 0, 0}, {0, 3, 0, 0} },
{ {0, 0, 0, 0}, {3, 3, 3, 3}, {0, 0, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0} } 
},

{                                                                               // j tetromino
{ {0, 0, 0, 0}, {0, 0, 4, 0}, {4, 4, 4, 0}, {0, 0, 0, 0} },
{ {0, 4, 0, 0}, {0, 4, 0, 0}, {0, 4, 4, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 4, 4, 4}, {0, 4, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 4, 4, 0}, {0, 0, 4, 0}, {0, 0, 4, 0} }
},

{                                                                               // z tetromino 
{ {0, 0, 0, 0}, {0, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0} },
{ {0, 0, 0, 0}, {0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0} },
{ {0, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 0, 5, 5}, {0, 5, 5, 0}, {0, 0, 0, 0} }
},

{                                                                               // s tetromino
{ {0, 0, 0, 0}, {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0 ,0} },
{ {0, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0} },
{ {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 6, 6, 0}, {0, 0, 6, 6}, {0, 0, 0, 0} }
},

{                                                                               // t tetromino
{ {0, 0, 0, 0}, {0, 7, 0, 0}, {7, 7, 7, 0}, {0, 0, 0, 0} },
{ {0, 7, 0, 0}, {0, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {7, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 0, 7, 0}, {0, 7, 7, 0}, {0, 0, 7, 0} }      
},

{                                                                               // o tetromino 
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } } 
}      

}; 


char *image [ 11 ], image_name [ 9 ] [ 40 ], score [ 3 ] [ 30 ] ;


// The function is used to call images that make up the Tetris
// There is no return value
// There is no parameter
void LoadImage ( void ) {
	int index, size ;
	
	sprintf ( image_name [ 0 ] , "./image/block_blank.jpg" ) ;
	sprintf ( image_name [ 1 ] , "./image/block_color_gray.jpg" ) ;
	sprintf ( image_name [ 2 ] , "./image/block_color_blue.jpg" ) ;
	sprintf ( image_name [ 3 ] , "./image/block_color_green.jpg" ) ;
	sprintf ( image_name [ 4 ] , "./image/block_color_land.jpg" ) ;
	sprintf ( image_name [ 5 ] , "./image/block_color_purple.jpg" ) ;
	sprintf ( image_name [ 6 ] , "./image/block_color_red.jpg" ) ;
	sprintf ( image_name [ 7 ] , "./image/block_color_mint.jpg" ) ;
	sprintf ( image_name [ 8 ] , "./image/block_color_yellow.jpg" ) ;
	
	for ( index = 0; index < 9; index++ ) {
		readimagefile ( image_name [ index ], 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE ) ;
		size = imagesize ( 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE ) ;
		image [ index ] = ( char * ) malloc ( size ) ;
		getimage ( 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE, image [ index ] ) ;
	}
	
	readimagefile ( "./image/background.jpg" , 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT ) ;
	size = imagesize ( 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT ) ;
	image [ 9 ] = ( char * ) malloc ( size ) ;
	getimage ( 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, image [ 9 ] ) ;
	
	readimagefile ( "./image/score.jpg" , 0, 0, SCORE_BOARD_WIDTH, SCORE_BOARD_HEIGHT ) ;
	size = imagesize ( 0, 0, SCORE_BOARD_WIDTH, SCORE_BOARD_HEIGHT ) ;
	image [ 10 ] = ( char * ) malloc ( size ) ;
	getimage ( 0, 0, SCORE_BOARD_WIDTH, SCORE_BOARD_HEIGHT, image [ 10 ] ) ;
}


// The function is used to set the default value of the Tetris board
// There is no return value
// There is no parameter
void InitBoard ( void ) {
	int x, y ;
	
	// The default for all Tetris blocks is 20
	for ( x = 0; x < 26; x++ ) {
		for ( y = 0; y < 16; y++ ) {
			board [ x ] [ y ] = 20 ;
		} 
	} 
	
	// Build Tetris play space
	for ( x = 0; x < 22; x++ ) {
		for ( y = 0; y < 12; y++ ) {
			if ( x == 0 || x == 21 || y == 0 || y == 11 ) {
				// The value of the Tetris wall is 1
				board [ x ] [ y ] = 1 ;
			} else { 
				// The value of the Tetris empty space is 0
				board [ x ] [ y ] = 0 ;
			}
		} 
	} 
	
	// Build Tetris block preview space
	for ( x = 22; x < 29; x++ ) {
		for ( y = 0; y < 7; y++ ) {
			if ( y == 0 || y == 6 || x == 22 || x == 28 ) {
				board [ x ] [ y ] = 1 ;
			} else { 
				board [ x ] [ y ] = 0 ;
			}
		}
	}
}


// The function is used to detect when two blocks collide
// If two blocks collide, the function returns 1
// The information of block is input as a parameter
int CollisionBlock ( block_info f_bval ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y++ ) {
			// When the value of the board at the current block position is not 0
			if ( ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ y ] != 0 )
				&& ( board [ f_bval.ver + x ] [ f_bval.hor + y ] != 0 ) )
			
				return 1 ;
		} 
	} 
	return 0 ;
}


// The function is used to remove afterimage when block moves
// There is no return value
// There is no parameter
void RemoveAfterImage ( void ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y++ ) {
			// Do not erase the blocks on the Tetris board
			if ( ( block [ bval.model ] [ bval.spin ] [ x ] [ y ] > 1 ) 
				&& ( board [ bval.ver + x ] [ bval.hor + y ] == 0 ) ) {
					
				putimage ( image_x + ( bval.hor + y ) * BLOCK_IMAGE_SIZE,
							image_y + ( bval.ver + x ) * BLOCK_IMAGE_SIZE, 
							image [ 0 ],
							0 ) ;
			} 
		} 
	} 
}


// The function is used to draw the current block on Tetris board
// There is no return value
// There is no parameter
void DrawCurrentBlock ( void ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y ++ ) {
			if ( block [ bval.model ] [ bval.spin ] [ x ] [ y ] > 1 ) {
				putimage ( image_x + ( bval.hor + y ) * BLOCK_IMAGE_SIZE,
							image_y + ( bval.ver + x ) * BLOCK_IMAGE_SIZE,
							image [ block [ bval.model ] [ bval.spin ] [ x ] [ y ] ],
							0 ) ;
			} 
		} 
	} 
}


// The function is used to draw next block
// There is no return value
// There is no parameter
void DrawNextBlock ( void ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y ++ ) {
			if ( block [ bval.next_model ] [ 0 ] [ x ] [ y ] > 1 ) {
				putimage ( image_x + ( y + 2 ) * BLOCK_IMAGE_SIZE,
							image_y + ( 23 + x ) * BLOCK_IMAGE_SIZE, 
							image [ block [ bval.next_model ] [ 0 ] [ x ] [ y ] ],
							0 ) ;
			} 
		} 
	} 
}


// The function is used to clear the next block
// There is no return value
// There is no parameter
void ClearNextBlock ( void ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y ++ ) {
			if ( block [ bval.next_model ] [ 0 ] [ x ] [ y ] > 1 ) {
				putimage ( image_x + ( y + 2 ) * BLOCK_IMAGE_SIZE, 
							image_y + ( 23 + x ) * BLOCK_IMAGE_SIZE, 
							image [ 0 ], 
							0 ) ;
			} 
		} 
	} 
}


// This function is used to store blocks on a Tetris board if a block collides
// There is no return value
// There is no parameter
void SaveBlock ( void ) {
	int x, y ;
	
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y++ ) { 
			if ( block [ bval.model ] [ bval.spin ] [ x ] [ y ] != 0 )
				// Block is stored above the collision point
				board [ bval.ver + x - 1 ] [ bval.hor + y ] = block [ bval.model ] [ bval.spin ] [ x ] [ y ] ;
		}
	}	
}


// This function is used to draw the Tetris board
// There is no return value
// There is no parameter
void DrawBoard ( void ) {
	int x, y ;
	
	// Draw the play area
	for ( x = 0; x < 22; x++ ) {
		for ( y = 0; y < 12; y++ ) {
			putimage ( image_x + y * BLOCK_IMAGE_SIZE,
						image_y + x * BLOCK_IMAGE_SIZE, 
						image [ board [ x ] [ y ] ], 
						0 ) ; 
		} 
	} 
	
	// Draw preview area
	for ( x = 22; x < 29; x++ ) {
		for ( y = 0; y < 7; y++ ) {
			if ( y == 0 || y == 6 || x == 22 || x == 28 ) {
				putimage ( image_x + y * BLOCK_IMAGE_SIZE, 
							image_y + x * BLOCK_IMAGE_SIZE,
							image [ 1 ], 
							0 ) ;
			} else { 
				putimage ( image_x + y * BLOCK_IMAGE_SIZE, 
							image_y + x * BLOCK_IMAGE_SIZE, 
							image [ 0 ], 
							0 ) ;
			}
		}
	}
}


// This function is used to erase a line if it is filled with blocks
// Also lower the block above the line
// There is no return value
// There is no parameter
void ClearLine ( void ) {
	int a, b, c, d ; 
	
	for ( a = 1; a < 21; a++ ) {
		for ( b = 1; b < 11; b++ ) {
			// There is no need to check for empty spaces
			if ( board [ a ] [ b ] == 0 ) 
				break;
		} 
		
		if ( b == 11 ) {
			is_score += 10 ;
			for ( c = a; c > 1; c-- ) {
				for ( d = 1; d < 11; d++ )  {
					board [ c ] [ d ] = board [ c - 1 ] [ d ] ;
				}
			}
		} 
	}
}


// This function is used to count the number of blocks around when a block collides
// The return value is the number of adjacent blocks
// Use the information of the block as a parameter
int CountAroundBlock ( block_info f_bval ) {																
	int x, y, count = 0 ;
	
	// The block should be one line higher than the point at which it collides
	f_bval.ver --;																					
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y ++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ y ] > 1 ) {
				if ( ( board [ f_bval.ver + x ] [ f_bval.hor + y - 1 ] > 1 )
					|| ( board [ f_bval.ver + x ] [ f_bval.hor + y + 1 ] > 1 )
					|| ( board [ f_bval.ver + x + 1 ] [ f_bval.hor + y ] > 1 ) ) 
					
					count++ ;
			}
		}
	}

	return count ;
}


// The function is used to count the number of blocks that meet the bottom of the Tetris board
// Returns the number of blocks that meet the bottom of the Tetris board
// Use the information of the block as a parameter
int CountBottom ( block_info f_bval ) {															
	int x, y, count = 0 ;
	
	if ( f_bval.ver > 17  ) {
		for ( x = 0; x < 4; x++ ) {
			for ( y = 0; y < 4; y++ ) {
				if ( ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ y ] > 1 )
					&& ( board [ f_bval.ver + x ] [ f_bval.hor + y ] == 1 ) )
				
					count++ ;
			}
		}
	}

	return count ;
}


// This function is used to count the number of blocks that meet each side wall of the Tetris board
// Returns the number of blocks that meet each side of the Tetris board
// Use the information of the block as a parameter
int CountSide( block_info f_bval ) {															
	int x, count = 0 ;
	
	f_bval.ver = 1 ;  																				
	if ( f_bval.hor == 0 ) {
		for ( x = 0; x < 4; x++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ 1 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 1 ) {
		for ( x = 0; x < 4; x++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ 0 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 8 ) {
		for ( x = 0; x < 4; x++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ 2 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 9 ) {
		for ( x = 0; x < 4; x++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ 1 ] > 1 ) 
				count++ ;
		}
	}

	return count ;
}


// This function is used to count the number of spaces below the block when the block collides
// Returns the number of empty spaces
// Use the information of the block as a parameter
int CountBlank ( block_info f_bval ) {																
	int x, y, count = 0 ;
	
	f_bval.ver -- ;
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ y ] > 1 ) {
				// The block below a line must not be itself
				if ( block [ f_bval.model ] [ f_bval.spin ] [ x + 1 ] [ y ] == 0 ) {				
					if ( board [ f_bval.ver + x + 1 ] [ f_bval.hor + y ] == 0 ) 
						count++ ;
				}
			}
		}
	}

	return count ;	
}


// The function is used to count the number of blocks that exist on the line when the block collides
// Returns the number of blocks in the line
// Use the information of the block as a parameter
int Complete_Line ( block_info f_bval ) {															
	int x, y, c, count = 0;
	
	f_bval.ver-- ;
	for ( x = 0; x < 4; x++ ) {
		for ( y = 0; y < 4; y++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ x ] [ y ] != 0 ) {
				for ( c = 1; c < 11; c++ ) {
					if ( board [ f_bval.ver + x ] [ c ] > 1 ) 
						count++ ;
				}
				break ;
			}
		}
	}

	return count ;
}

block_info Simul_Block ( void ) {
	block_info test = { bval.model, 0, 0, 0, 2 }, max = { bval.model, 0, 0, 0, 2 } ;
	double top_score = 0 , cur_score = 0 ;
	
	for ( test.spin = 0; test.spin < 4; test.spin++ ) {											// 검사할 블록의 회전 경우의 수
		for ( test.hor = 0; test.hor < 10; test.hor++ ) {										// 검사할 블록의 가로 범위
			test.ver = 1 ;
			if ( CollisionBlock ( test ) == 1 )  
				continue ;										// 시작하자마자 블록이 충돌하면 다음 순서의 블록을 확인
			while ( CollisionBlock ( test ) != 1 ) 
				test.ver++ ;									// 검사할 블록을 멈출 때 까지 내림
			cur_score = 1.2 * test.ver 
						+ 5.2 * CountAroundBlock ( test ) 
						- 12.1 * CountBlank ( test ) 
						+ 5.4 * CountBottom ( test )
						+ 5.3 * CountSide ( test )
						+ 0.43 * Complete_Line ( test ) ;										// 블록이 도착했을 때 최적화 점수 값 계산
				
			if ( top_score < cur_score ) {														// 최적화 점수 값이 최고로 높은 값을 max에 입력
				max = test ;
				top_score = cur_score ;
			}
		}
	}
	return max ;
}


void Memory_Free ( void ) {
	int a ;
	
	for ( a = 10; a >= 0; a-- ) 
		free ( image [ a ] ) ;
}	





int main ( ) {
	
	int page = 0, apage, vpage;
	
	sprintf ( score [ 0 ], "%d", game + 1 ) ;
	sprintf ( score [ 1 ], "%d", is_score ) ;
	sprintf ( score [ 2 ], "%d", target_score ) ;
	
	srand ( GetTickCount ( ) ) ;
		
	block_info ai ;
	
	initwindow ( 1600, 900, "Tetris" ) ; 
	
	LoadImage ( ) ;
	
	InitBoard ( ) ; 
	
	bval.model = rand ( ) % 7 ;
	bval.next_model = rand ( ) % 7 ;
	
	
	while ( 1 ) {
		
		setactivepage ( page ) ;
		setvisualpage ( 1 - page ) ;
		
		cleardevice ( ) ;
					
		putimage ( 0, 0, image [ 9 ], 0 ) ;
		putimage ( image_x + 147 , image_y + 462, image [ 10 ], 0) ;
	
		DrawBoard ( ) ;
	
		DrawNextBlock ( ) ;
		
		RemoveAfterImage ( ) ;
		
		if ( bval.ver == 0 ) {
			ai = Simul_Block ( ) ;    
			bval.hor = ai.hor ;
			bval.spin = ai.spin ;
		}
		
		bval.ver++ ;
		
		if ( CollisionBlock ( bval ) == 1 ) {
			
			SaveBlock ( ) ;
			
			DrawBoard ( ) ;
			
			ClearLine ( ) ;
			
			sprintf ( score [ 1 ], "%d", is_score ) ;
			
			if ( target_score < is_score ) {
				target_score = is_score ;
				sprintf ( score [ 2 ], "%d", target_score ) ;
			}
			
			if ( bval.ver < 2 ) {
				InitBoard ( ) ;
				game++ ;
				sprintf ( score [ 0 ], "%d", game + 1 ) ;
				is_score = 0;
				sprintf ( score [ 1 ], "%d", 0 ) ;
			}
			
			ClearNextBlock ( ) ;
			bval.ver = 0 ;
			bval.hor = 4 ;
			bval.model = bval.next_model ;
			bval.next_model = rand ( ) % 7 ;
			bval.spin = 0 ;
			DrawNextBlock ( ) ;
		}
		
		DrawCurrentBlock ( ) ;
		
		outtextxy ( image_x + 150, image_y + 470, "Games" ) ;
		outtextxy ( image_x + 150, image_y + 490, score [ 0 ] ) ;
		
		outtextxy ( image_x + 150, image_y + 515, "Score" ) ;
		outtextxy ( image_x + 150, image_y + 535, score [ 1 ] ) ;
		
		outtextxy ( image_x + 150, image_y + 560, "Top" ) ;
		outtextxy ( image_x + 150, image_y + 580, score [ 2 ] ) ;
		
		page = 1 - page ;
		
		delay ( 1 ) ;
	}
	
	Memory_Free ( ) ;
	
	closegraph ( ) ;
	
	return 0 ;
	
}
#endif
