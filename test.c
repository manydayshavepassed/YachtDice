#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define NUM_DICE 5
#define NUM_CATEGORIES 12
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define ENTER 13
#define ESC 27

#define SELECTED_COLOR 10
#define DEFAULT_COLOR 15

// 함수 선언
void rollDice(int dice[], int keep[]);
void displayDice(int dice[], int keep[], int cursor);
void setColor(int color);
int handleKeyboardInput(int dice[], int keep[]);

int calculateScore(int category, int dice[]);
int checkYacht(int dice[]);
int checkFourOfAKind(int dice[]);
int checkFullHouse(int dice[]);
int checkLittleStraight(int dice[]);
int checkBigStraight(int dice[]);
int calculateSum(int dice[], int num);

// 텍스트 색상을 설정하는 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 주사위를 굴리는 함수
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) { // 선택되지 않은 주사위만 다시 굴림
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
}

// 주사위를 출력하는 함수 (화살표 및 선택 상태 포함)
void displayDice(int dice[], int keep[], int cursor) {
    system("cls"); // 화면 초기화
    printf("Use arrow keys to move, SPACE to toggle selection, ESC to exit.\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor) {
            setColor(SELECTED_COLOR); // 커서가 위치한 곳을 녹색으로 강조
        } else if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위도 녹색
        } else {
            setColor(DEFAULT_COLOR); // 기본 색상
        }

        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR); // 색상 초기화
    }
    printf("\n");
}

// 키보드 입력 처리 함수
int handleKeyboardInput(int dice[], int keep[]) {
    int cursor = 0; // 커서 초기 위치
    int running = 1;

    while (running) {
        displayDice(dice, keep, cursor);

        int key = _getch();
        if (key == ARROW) {
            key = _getch(); // 화살표 키 처리
            if (key == LEFT) {
                cursor = (cursor == 0) ? NUM_DICE - 1 : cursor - 1; // 왼쪽 이동
            } else if (key == RIGHT) {
                cursor = (cursor == NUM_DICE - 1) ? 0 : cursor + 1; // 오른쪽 이동
            }
        } else if (key == SPACEBAR || key == ENTER) {
            keep[cursor] = !keep[cursor]; // 선택 상태 토글
        } else if (key == ESC) {
            running = 0; // ESC 키로 종료
        }
    }
    return 0;
}

int main() {
    int dice[NUM_DICE] = {0};
    int keep[NUM_DICE] = {0};

    srand(time(0)); // 랜덤 시드 설정

    // 주사위 게임 시작
    printf("Welcome to Yacht Dice!\n");
    printf("Rolling the dice...\n");

    rollDice(dice, keep); // 첫 주사위 굴리기
    handleKeyboardInput(dice, keep); // 주사위 선택 및 유지 처리

    // 최종 결과 출력
    printf("\nFinal Dice:\n");
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위 강조
        }
        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR);
    }
    printf("\nGame Over.\n");

    return 0;
}
