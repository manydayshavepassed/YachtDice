#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW 224
#define SPACEBAR 32


void gotoxy(int x,int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

int enter_keyboard(void) {
	if (_kbhit()) {
		int nkey = _getch();
        if(nkey==SPACEBAR){
            system("cls");
            printf("space bar");
        }
		if (nkey == ARROW) {
			nkey = _getch();
			switch (nkey) {
			case UP:
                system("cls");
				return UP;
				break;
			case LEFT:
                system("cls");
				return LEFT;
			case RIGHT:
                system("cls");
				return RIGHT;
			case DOWN:
                system("cls");
				return DOWN;
			}
		}
	}
}

void main()
{
	while(1)
	{
		CONSOLE_SCREEN_BUFFER_INFO presentCur; 
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
		switch (enter_keyboard())
		{
		case UP:
			gotoxy(presentCur.dwCursorPosition.X,presentCur.dwCursorPosition.Y+1);
			break;
		case DOWN:
			gotoxy(presentCur.dwCursorPosition.X,presentCur.dwCursorPosition.Y+1);
			break;
		case LEFT:
			gotoxy(presentCur.dwCursorPosition.X-1,presentCur.dwCursorPosition.Y);
			break;
		case RIGHT:
			gotoxy(presentCur.dwCursorPosition.X+1,presentCur.dwCursorPosition.Y);
			break;
		}
	}	
}