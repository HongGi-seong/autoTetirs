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
	int a, size ;
	
	sprintf ( image_name [ 0 ] , "./image/block_blank.jpg" ) ;
	sprintf ( image_name [ 1 ] , "./image/block_color_gray.jpg" ) ;
	sprintf ( image_name [ 2 ] , "./image/block_color_blue.jpg" ) ;
	sprintf ( image_name [ 3 ] , "./image/block_color_green.jpg" ) ;
	sprintf ( image_name [ 4 ] , "./image/block_color_land.jpg" ) ;
	sprintf ( image_name [ 5 ] , "./image/block_color_purple.jpg" ) ;
	sprintf ( image_name [ 6 ] , "./image/block_color_red.jpg" ) ;
	sprintf ( image_name [ 7 ] , "./image/block_color_mint.jpg" ) ;
	sprintf ( image_name [ 8 ] , "./image/block_color_yellow.jpg" ) ;
	
	for ( a = 0; a < 9; a++ ) {
		readimagefile ( image_name [ a ], 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE ) ;
		size = imagesize ( 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE ) ;
		image [ a ] = ( char * ) malloc ( size ) ;
		getimage ( 0, 0, BLOCK_IMAGE_SIZE, BLOCK_IMAGE_SIZE, image [ a ] ) ;
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
	int a, b ;
	
	// The default for all Tetris blocks is 20
	for ( a = 0; a < 26; a++ ) {
		for ( b = 0; b < 16; b++ ) {
			board [ a ] [ b ] = 20 ;
		} 
	} 
	
	// Build Tetris play space
	for ( a = 0; a < 22; a++ ) {
		for ( b = 0; b < 12; b++ ) {
			if ( a == 0 || a == 21 || b == 0 || b == 11 ) {
				// The value of the Tetris wall is 1
				board [ a ] [ b ] = 1 ;
			} else { 
				// The value of the Tetris empty space is 0
				board [ a ] [ b ] = 0 ;
			}
		} 
	} 
	
	// Build Tetris block preview space
	for ( a = 22; a < 29; a++ ) {
		for ( b = 0; b < 7; b++ ) {
			if ( b == 0 || b == 6 || a == 22 || a == 28 ) {
				board [ a ] [ b ] = 1 ;
			} else { 
				board [ a ] [ b ] = 0 ;
			}
		}
	}
}


// The function is used to detect when two blocks collide
// If two blocks collide, the function returns 1
// The information of block is input as a parameter
int CollisionBlock ( block_info f_bval ) {
	int a, b ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b++ ) {
			// When the value of the board at the current block position is not 0
			if ( ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ b ] != 0 )
				&& ( board [ f_bval.ver + a ] [ f_bval.hor + b ] != 0 ) )
				return 1 ;
		} 
	} 
	return 0 ;
}



void Clear_Block ( void ) {
	int a, b ;
	block_info ghost ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b++ ) {
			if ( ( block [ bval.model ] [ bval.spin ] [ a ] [ b ] > 1 ) 
				&& ( board [ bval.ver + a ] [ bval.hor + b ] == 0 ) ) {
				putimage ( image_x + ( bval.hor + b ) * BLOCK_IMAGE_SIZE, image_y + ( bval.ver + a ) * BLOCK_IMAGE_SIZE, image [ 0 ], 0 ) ;
			} // end if
		} // end b-loop
	} // end a-loop
}




void Draw_Block ( void ) {
	int a, b ;
	block_info ghost ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b ++ ) {
			if ( block [ bval.model ] [ bval.spin ] [ a ] [ b ] > 1 ) {
				putimage ( image_x + ( bval.hor + b ) * BLOCK_IMAGE_SIZE, image_y + ( bval.ver + a ) * BLOCK_IMAGE_SIZE, image [ block [ bval.model ] [ bval.spin ] [ a ] [ b ] ], 0 ) ;
			} // end if
		} // end b-loop
	} // end a-loop
}


void Draw_Next ( void ) {
	int a, b ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b ++ ) {
			if ( block [ bval.next_model ] [ 0 ] [ a ] [ b ] > 1 ) {
				putimage ( image_x + ( b + 2 ) * BLOCK_IMAGE_SIZE, image_y + ( 23 + a ) * BLOCK_IMAGE_SIZE, image [ block [ bval.next_model ] [ 0 ] [ a ] [ b ] ], 0 ) ;
			} // end if
		} // end b-loop
	} // end a-loop
}



void Clear_Next ( void ) {
	int a, b ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b ++ ) {
			if ( block [ bval.next_model ] [ 0 ] [ a ] [ b ] > 1 ) {
				putimage ( image_x + ( b + 2 ) * BLOCK_IMAGE_SIZE, image_y + ( 23 + a ) * BLOCK_IMAGE_SIZE, image [ 0 ], 0 ) ;
			} // end if
		} // end b-loop
	} // end a-loop
}



void Save_Block ( void ) {
	int a, b ;
	
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b++ ) { 
			if ( block [ bval.model ] [ bval.spin ] [ a ] [ b ] != 0 ) board [ bval.ver + a -1 ] [ bval.hor + b ] = block [ bval.model ] [ bval.spin ] [ a ] [ b ] ;
		}
	}	
}


void Draw_Board ( void ) {
	int a, b ;
	
	for ( a = 0; a < 22; a++ ) {
		for ( b = 0; b < 12; b++ ) {
			putimage ( image_x + b * BLOCK_IMAGE_SIZE, image_y + a * BLOCK_IMAGE_SIZE, image [ board [ a ] [ b ] ], 0 ) ; 
		} // end b-loop
	} // end a-loop
	
	for ( a = 22; a < 29; a++ ) {
		for ( b = 0; b < 7; b++ ) {
			if ( b == 0 || b == 6 || a == 22 || a == 28 ) {
				putimage ( image_x + b * BLOCK_IMAGE_SIZE, image_y + a * BLOCK_IMAGE_SIZE, image [ 1 ], 0 ) ;
			} else { 
				putimage ( image_x + b * BLOCK_IMAGE_SIZE, image_y + a * BLOCK_IMAGE_SIZE, image [ 0 ], 0 ) ;
			}
		}
	}
}

int Clear_Line ( void ) {
	int a, b, c, d ; 
	
	for ( a = 1; a < 21; a++ ) {
		for ( b = 1; b < 11; b++ ) {
			if ( board [ a ] [ b ] == 0 ) 
				break;
		} // end b-loop
		
		if ( b == 11 ) {
			is_score += 10 ;
			for ( c = a; c > 1; c-- ) {
				for ( d = 1; d < 11; d++ )  {
					board [ c ] [ d ] = board [ c - 1 ] [ d ] ;
				}
			}
		} // end if
	} // end a-loop
}


int Count_Block ( block_info f_bval ) {																// 블록이 멈췄을 때 주변에 블록 면들의 개수를 세는 함수
	int a, b, count = 0 ;
	
	f_bval.ver --;																					// 블록이 겹쳤을 때의 ver값을 가져오기 때문에 -1
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b ++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ b ] > 1 ) {
				if ( board [ f_bval.ver + a ] [ f_bval.hor + b - 1 ] > 1 ) count++ ;
				if ( board [ f_bval.ver + a ] [ f_bval.hor + b + 1 ] > 1 ) count++ ;
				if ( board [ f_bval.ver + a + 1 ] [ f_bval.hor + b ] > 1 ) count++ ;
			}
		}
	}

	return count ;
}


int Count_Bottom ( block_info f_bval ) {															// 블록이 바닥과 만나는 블록 면의 개수를 세는 함수
	int a, b, count = 0 ;
	
	if ( f_bval.ver > 17  ) {
		for ( a = 0; a < 4; a++ ) {
			for ( b = 0; b < 4; b++ ) {
				if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ b ] > 1 && board [ f_bval.ver + a ] [ f_bval.hor + b ] == 1 )
					count++ ;
			}
		}
	}

	return count ;
}


int Count_Side( block_info f_bval ) {																// 블록이 양쪽 벽과 만나는 면의 개수를 세는 함수
	int a, b, count = 0 ;
	
	f_bval.ver = 1 ;  																				// 충돌 했을때는 정확히 벽과 만나는 면의 개수를 구하기 까다로움 
	if ( f_bval.hor == 0 ) {
		for ( a = 0; a < 4; a++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ 1 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 1 ) {
		for ( a = 0; a < 4; a++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ 0 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 8 ) {
		for ( a = 0; a < 4; a++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ 2 ] > 1 ) 
				count++ ;
		}
	}
	
	if ( f_bval.hor == 9 ) {
		for ( a = 0; a < 4; a++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ 1 ] > 1 ) 
				count++ ;
		}
	}

	return count ;
}


int Count_Blank ( block_info f_bval ) {																// 블록이 멈췄을 때 블록 면 바로 아래의 빈공간 세는 함수
	int a, b, count = 0 ;
	
	f_bval.ver -- ;
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ b ] > 1 ) {
				if ( block [ f_bval.model ] [ f_bval.spin ] [ a + 1 ] [ b ] == 0 ) {				// 블록 면 바로 아래에 자신의 블록이 없는지 체크
					if ( board [ f_bval.ver + a + 1 ] [ f_bval.hor + b ] == 0 ) count++ ;
				}
			}
		}
	}

	return count ;	
}


int Complete_Line ( block_info f_bval ) {															// 블록이 멈춘 위치의 라인에 얼마나 많은 블록 면들이 채워져 있는지 확인하는 함수
	int a, b, c, count = 0;
	
	f_bval.ver-- ;
	for ( a = 0; a < 4; a++ ) {
		for ( b = 0; b < 4; b++ ) {
			if ( block [ f_bval.model ] [ f_bval.spin ] [ a ] [ b ] != 0 ) {
				for ( c = 1; c < 11; c++ ) {
					if ( board [ f_bval.ver + a ] [ c ] > 1 ) 
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
			cur_score = 1.2 * test.ver + 5.2 * Count_Block ( test ) - 12.1 * Count_Blank ( test ) + 5.4 * Count_Bottom ( test ) + 5.3 * Count_Side ( test )
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
	
		Draw_Board ( ) ;
	
		Draw_Next ( ) ;
		
		Clear_Block ( ) ;
		
		if ( bval.ver == 0 ) {
			ai = Simul_Block ( ) ;    
			bval.hor = ai.hor ;
			bval.spin = ai.spin ;
		}
		
		bval.ver++ ;
		
		if ( CollisionBlock ( bval ) == 1 ) {
			
			Save_Block ( ) ;
			
			Draw_Board ( ) ;
			
			Clear_Line ( ) ;
			
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
			
			Clear_Next ( ) ;
			bval.ver = 0 ;
			bval.hor = 4 ;
			bval.model = bval.next_model ;
			bval.next_model = rand ( ) % 7 ;
			bval.spin = 0 ;
			Draw_Next ( ) ;
		}
		
		Draw_Block ( ) ;
		
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
