#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Windows.h>

#include "graphics.c"

#define NUM_DICE 5
#define NUM_CATEGORIES 12

// 함수 선언
void rollDice(int dice[], int keep[]);
void displayDice(int dice[]);

int calculateScore(int category, int dice[]);
int checkYacht(int dice[]);
int checkFourOfAKind(int dice[]);
int checkFullHouse(int dice[]);
int checkLittleStraight(int dice[]);
int checkBigStraight(int dice[]);
int calculateSum(int dice[], int num);

//UI 함수

void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

enum ColorType{
BLACK,  	//0
darkBLUE,	//1
DarkGreen,	//2
darkSkyBlue,    //3
DarkRed,  	//4
DarkPurple,	//5
DarkYellow,	//6
GRAY,		//7
DarkGray,	//8
BLUE,		//9
GREEN,		//10
SkyBlue,	//11
RED,		//12
PURPLE,		//13
YELLOW,		//14
WHITE		//15
} COLOR;

// 메인 함수
int main() {
    int dice[NUM_DICE]; // 현재 주사위 눈
    int keep[NUM_DICE] = {0}; // 유지할 주사위 표시
    int scores[NUM_CATEGORIES] = {0};
    int category, rolls, i;

    srand(time(0)); // 랜덤 시드 설정

    printf("Welcome to Yacht Dice!\n");

    for (int turn = 0; turn < NUM_CATEGORIES; turn++) {
	for (i = 0;i < NUM_DICE; i++) {
	    keep[i] = 0;
	}
        // 주사위 굴리기 최대 3번
        for (rolls = 0; rolls < 3; rolls++) {
            rollDice(dice, keep);
            printf("\nRoll %d: ", rolls + 1);
            displayDice(dice);

            // 3번째 굴림이 아닌 경우 주사위 유지 결정
            if (rolls < 2) {
                printf("Enter 1 to keep dice or 0 to re-roll (e.g., 1 0 0 1 1): ");
                for (i = 0; i < NUM_DICE; i++) {
                    scanf("%d", &keep[i]);
                }
            }
        }

        int chosen = 1;
	while (chosen) {
    	    // 카테고리 선택 및 점수 계산
            printf("Choose a category (1:Ones, 2:Twos, 3:Threes, 4:Fours, 5:Fives, 6:Sixes, 7:Yacht, 8:Four of a Kind, 9:Full House, 10:Little Straight, 11:Big Straight, 12:Choice): ");
	    scanf("%d", &category);
    	    category--; // 배열 인덱스 맞추기

    	    if (scores[category] == 0) { // 아직 선택되지 않은 카테고리
        	scores[category] = calculateScore(category, dice);
                printf("You scored %d points in this category.\n", scores[category]);
        	chosen = 0;
    	    }
    	    else {
        	printf("Category already chosen, try another category.\n\n");
	    }
	}
    }

    // 최종 점수 출력
    int totalScore = 0;
    printf("\nFinal Scores:\n");
    for (i = 0; i < NUM_CATEGORIES; i++) {
        totalScore += scores[i];
    }
    printf("Your total score is %d.\n", totalScore);

    return 0;
}

// 주사위 굴리기
void rollDice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i] == 0) {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
}

// 주사위 출력
void displayDice(int dice[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

// 점수 계산 함수
int calculateScore(int category, int dice[]) {
    switch (category) {
        case 0: return calculateSum(dice, 1); // Ones
        case 1: return calculateSum(dice, 2); // Twos
        case 2: return calculateSum(dice, 3); // Threes
        case 3: return calculateSum(dice, 4); // Fours
        case 4: return calculateSum(dice, 5); // Fives
        case 5: return calculateSum(dice, 6); // Sixes
        case 6: return checkYacht(dice) ? 50 : 0; // Yacht
        case 7: return checkFourOfAKind(dice); // Four of a Kind
        case 8: return checkFullHouse(dice) ? 25 : 0; // Full House
        case 9: return checkLittleStraight(dice) ? 30 : 0; // Little Straight
        case 10: return checkBigStraight(dice) ? 30 : 0; // Big Straight
        case 11: return calculateSum(dice, 0); // Choice
        default: return 0;
    }
}

// 특정 숫자 합산
int calculateSum(int dice[], int num) {
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        if (num == 0 || dice[i] == num) {
            sum += dice[i];
        }
    }
    return sum;
}

// Yacht 점수 계산 : 모든 눈이 같을 때
int checkYacht(int dice[]) {
    for (int i = 1; i < NUM_DICE; i++) {
        if (dice[i] != dice[0]) {
            return 0;
        }
    }
    return 1;
}

// Four of a Kind 점수 계산 : 4개의 눈이 같을 때
int checkFourOfAKind(int dice[]) {
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 4) {
            return i * 4;
        }
    }
    return 0;
}

// Full House 점수 계산 : 3개,2개
int checkFullHouse(int dice[]) {
    int counts[6] = {0};
    int two = 0, three = 0;
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] == 3) three = 1;
        if (counts[i] == 2) two = 1;
    }
    return two && three;
}

// Little Straight 점수 계산 : 4개가 연속수일 때
int checkLittleStraight(int dice[]) {
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 4; i++)
    {
        int result = counts[i]*counts[i+1]*counts[i+2]*counts[i+3];
        if (result != 0) return counts[i]&&counts[i+1]&&counts[i+2]&&counts[i+3];
    }
}

// Big Straight 점수 계산 : 5개가 연속수일 때
int checkBigStraight(int dice[]) {
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 2; i++)
    {
        int result = counts[i]*counts[i+1]*counts[i+2]*counts[i+3]*counts[i+4];
        if (result != 0) return counts[i]&&counts[i+1]&&counts[i+2]&&counts[i+3]&&counts[i+4];
    }
}
