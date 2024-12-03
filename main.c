#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#include "graphics.c"

#define NUM_DICE 5
#define NUM_CATEGORIES 12
//키보드 입력값
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define ENTER 13
#define ESC 27
//출력되는 글자 색
#define SELECTED_COLOR 10 // 녹색
#define CURSOR_COLOR 9    // 파란색
#define DEFAULT_COLOR 15  // 기본 색상


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

int chooseBestCategory(int scores[], int dice[]);
void decideKeepDice(int dice[], int keep[], int targetCategory);

// UI 함수

// 텍스트 색상 설정 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 주사위 출력 함수 (커서와 선택 상태 표시)
void displayDiceWithCursor(int dice[], int keep[], int cursor) {
    system("cls"); // 화면 초기화
    printf("Use arrow keys to move, SPACE to toggle selection, ENTER to confirm, ESC to exit.\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (i == cursor) {
            setColor(CURSOR_COLOR); // 커서에 위치한 주사위는 파란색
        } else if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위는 녹색
        } else {
            setColor(DEFAULT_COLOR); // 기본 색상
        }

        printf(" %d ", dice[i]);
        setColor(DEFAULT_COLOR); // 색상 초기화
    }
    printf("\n");
}

// 최종 선택 상태 확인 함수
int confirmSelection(int dice[], int keep[]) {
    system("cls");
    printf("Confirm your selection:\n");

    for (int i = 0; i < NUM_DICE; i++) {
        if (keep[i]) {
            setColor(SELECTED_COLOR); // 선택된 주사위는 녹색
        } else {
            setColor(DEFAULT_COLOR); // 기본 색상
        }
        printf(" %d ", dice[i]);
    }
    setColor(DEFAULT_COLOR); // 색상 초기화
    printf("\nPress ENTER to confirm or ESC to go back.\n");

    while (1) {
        int key = _getch();
        if (key == ENTER) {
            return 1; // 선택 확정
        } else if (key == ESC) {
            return 0; // 선택 취소, 이전 단계로 돌아감
        }
    }
}

// 화살표 키로 주사위를 선택하거나 유지/해제 처리
void handleDiceSelection(int dice[], int keep[]) {
    int cursor = 0; // 커서 초기 위치
    int running = 1;

    while (running) {
        displayDiceWithCursor(dice, keep, cursor); // 주사위 출력

        int key = _getch(); // 키 입력 처리
        if (key == ARROW) {
            key = _getch();
            if (key == LEFT) {
                cursor = (cursor == 0) ? NUM_DICE - 1 : cursor - 1; // 왼쪽 이동
            } else if (key == RIGHT) {
                cursor = (cursor == NUM_DICE - 1) ? 0 : cursor + 1; // 오른쪽 이동
            }
        } else if (key == SPACEBAR) {
            keep[cursor] = !keep[cursor]; // 선택 상태 토글
        } else if (key == ENTER) {
            if (confirmSelection(dice, keep)) {
                printf("Selection confirmed. Re-rolling dice...\n");
                Sleep(2000); // 2초 대기 후 종료
                running = 0;
            }
        } else if (key == ESC) {
            printf("Exiting dice selection...\n");
            Sleep(2000); // 2초 대기 후 종료
            running = 0;
        }
    }
}

// 메인 함수
int main()
{
    displayWelcomeScreen();   // 시작 화면을 표시
    int totalRound = 3;
    int dice[NUM_DICE];       // 현재 주사위 눈
    int keep[NUM_DICE] = {0}; // 유지할 주사위 표시
    int scores[NUM_CATEGORIES] = {0};
    int category, rolls, i, mode;
    int computerScores[NUM_CATEGORIES] = {0};

    srand(time(0)); // 랜덤 시드 설정

    printf("Welcome to Yacht Dice!\n");
    printf("\nEnter a number to select a mode (1: Single play, 2: VS Computer): ");
    scanf("%d", &mode);

    for (int turn = 0; turn < totalRound; turn++)
    {
        printf("\n--- Turn %d ---\n", turn + 1);

        // 플레이어 턴
        if (mode == 2)
        {
            printf("\nYour turn!\n");
        }

        for (i = 0; i < NUM_DICE; i++)
        {
            keep[i] = 0;
        }

        // 주사위 굴리기 최대 3번
        for (rolls = 0; rolls < 3; rolls++) {
            rollDice(dice, keep);
            printf("\nRoll %d: ", rolls + 1);
            displayDice(dice); // 주사위를 시각적으로 출력
            
            if (rolls < 2) {
                printf("You can choose which dice to keep or re-roll.\n");
                Sleep(3000); // 메시지를 읽을 시간 제공
                handleDiceSelection(dice, keep); // 화살표 키를 사용해 주사위 선택
            }
        }


        int chosen = 1;
        while (chosen)
        {
            // 카테고리 선택 및 점수 계산
            displayDice(dice);
            printf("Choose a category (1:Ones, 2:Twos, 3:Threes, 4:Fours, 5:Fives, 6:Sixes, 7:Yacht, 8:Four of a Kind, 9:Full House, 10:Little Straight, 11:Big Straight, 12:Choice): ");
            scanf("%d", &category);
            category--; // 배열 인덱스 맞추기

            if (scores[category] == 0)
            { // 아직 선택되지 않은 카테고리
                scores[category] = calculateScore(category, dice);
                printf("You scored %d points in this category.\n", scores[category]);
                Sleep(1000);
                chosen = 0;
            }
            else
            {
                printf("Category already chosen, try another category.\n\n");
            }
        }

        if (mode == 2)
        {
            // 컴퓨터의 턴
            for (i = 0; i < NUM_DICE; i++)
            {
                keep[i] = 0;
            }

            printf("\nComputer's turn!\n");
            for (i = 0; i < NUM_DICE; i++)
                keep[i] = 0;
            for (rolls = 0; rolls < 3; rolls++)
            {
                rollDice(dice, keep);
                printf("\nComputer's Roll %d: ", rolls + 1);
                displayDice(dice);

                if (rolls < 2)
                {
                    int targetCategory = chooseBestCategory(computerScores, dice);
                    decideKeepDice(dice, keep, targetCategory);
                }
            }

            int bestCategory = chooseBestCategory(computerScores, dice);
            if (computerScores[bestCategory] == 0)
            {
                computerScores[bestCategory] = calculateScore(bestCategory, dice);
                printf("Computer scored %d points in category %d.\n", computerScores[bestCategory], bestCategory + 1);
            }
            else
            {
                printf("Category already chosen by computer.\n");
            }
        }

        // 최종 점수 출력
        int totalScore = 0;
        printf("\nFinal Scores:\n");
        for (i = 0; i < NUM_CATEGORIES; i++)
        {
            totalScore += scores[i];
        }
        printf("Your total score is %d.\n", totalScore);
        if (mode == 2)
        {
            // 컴퓨터 최종 점수 출력
            int Com_totalScore = 0;
            for (i = 0; i < NUM_CATEGORIES; i++)
            {
                Com_totalScore += computerScores[i];
            }
            printf("Computer's total score is %d.\n", Com_totalScore);

            if (totalScore > Com_totalScore)
            {
                printf("You Won!\n");
            }
            else if (totalScore < Com_totalScore)
            {
                printf("You Lost!\n");
            }
            else
            {
                printf("Draw!\n");
            }
        }
        displayScoreboard(scores, NUM_CATEGORIES); // 최종 점수 점수판에 출력
    }
}

// 주사위 굴리기
void rollDice(int dice[], int keep[])
{
    for (int i = 0; i < NUM_DICE; i++)
    {
        if (keep[i] == 0)
        {
            dice[i] = rand() % 6 + 1; // 1 ~ 6
        }
    }
}

// 주사위 출력
void displayDice(int dice[])
{
    for (int i = 0; i < NUM_DICE; i++)
    {
        printf("%d ", dice[i]);
        //displayAllDiceHorizontal(dice); // 주사위를 별로 출력
    }
    printf("\n");
}
// 점수 계산 함수
int calculateScore(int category, int dice[])
{
    switch (category)
    {
    case 0:
        return calculateSum(dice, 1); // Ones
    case 1:
        return calculateSum(dice, 2); // Twos
    case 2:
        return calculateSum(dice, 3); // Threes
    case 3:
        return calculateSum(dice, 4); // Fours
    case 4:
        return calculateSum(dice, 5); // Fives
    case 5:
        return calculateSum(dice, 6); // Sixes
    case 6:
        return checkYacht(dice) ? 50 : 0; // Yacht
    case 7:
        return checkFourOfAKind(dice); // Four of a Kind
    case 8:
        return checkFullHouse(dice) ? 25 : 0; // Full House
    case 9:
    {
        return checkLittleStraight(dice) ? 30 : 0; // Little Straight
    }
    case 10:
    {
        return checkBigStraight(dice) ? 30 : 0; // Big Straight
    }
    case 11:
    {
        return calculateSum(dice, 0); // Choice
    }
    default:
        return 0;
    }
}
// 특정 숫자 합산
int calculateSum(int dice[], int num)
{
    int sum = 0;
    for (int i = 0; i < NUM_DICE; i++)
    {
        if (num == 0 || dice[i] == num)
        {
            sum += dice[i];
        }
    }
    return sum;
}
// Yacht 점수 계산 : 모든 눈이 같을 때
int checkYacht(int dice[])
{
    for (int i = 1; i < NUM_DICE; i++)
    {
        if (dice[i] != dice[0])
        {
            return 0;
        }
    }
    return 1;
}
// Four of a Kind 점수 계산 : 4개의 눈이 같을 때
int checkFourOfAKind(int dice[])
{
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++)
    {
        if (counts[i] >= 4)
        {
            return i * 4;
        }
    }
    return 0;
}
// Full House 점수 계산 : 3개,2개
int checkFullHouse(int dice[])
{
    int counts[6] = {0};
    int two = 0, three = 0;
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++)
    {
        if (counts[i] == 3)
            three = 1;
        if (counts[i] == 2)
            two = 1;
    }
    return two && three;
}
// Little Straight 점수 계산 : 4개가 연속수일 때
int checkLittleStraight(int dice[])
{
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 4; i++)
    {
        int result = counts[i] * counts[i + 1] * counts[i + 2] * counts[i + 3];
        if (result != 0)
            return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3];
    }
}
// Big Straight 점수 계산 : 5개가 연속수일 때
int checkBigStraight(int dice[])
{
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 2; i++)
    {
        int result = counts[i] * counts[i + 1] * counts[i + 2] * counts[i + 3] * counts[i + 4];
        if (result != 0)
            return counts[i] && counts[i + 1] && counts[i + 2] && counts[i + 3] && counts[i + 4];
    }
}
// 컴퓨터 최적 카테고리 선택
int chooseBestCategory(int scores[], int dice[])
{
    int maxScore = -1;
    int bestCategory = -1;
    int counts[6] = {0};
    // 주사위 값 카운팅
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    // 우선 순위: 같은 숫자가 여러 개 나왔다면 Yacht 또는 Four of a Kind
    for (int i = 0; i < NUM_CATEGORIES; i++)
    {
        if (scores[i] == 0)
        { // 아직 선택되지 않은 카테고리만 검사
            int score = calculateScore(i, dice);
            // Yacht 우선 선택, 단 Yacht 조건을 만족해야 선택
            if (i == 6 && checkYacht(dice))
            { // Yacht
                maxScore = score;
                bestCategory = i;
                break; // Yacht가 선택되면 종료
            }
            // Four of a Kind 조건도 만족할 경우 선택
            if (i == 7 && checkFourOfAKind(dice) > 0)
            { // Four of a Kind
                maxScore = score;
                bestCategory = i;
            }
        }
    }
    // 연속수가 여러 개 나오면 Little Straight 또는 Big Straight
    if (bestCategory == -1)
    {
        // Little Straight 조건 만족 시
        if (scores[9] == 0 && checkLittleStraight(dice))
        {
            maxScore = calculateScore(9, dice); // Little Straight
            bestCategory = 9;
        }
        // Big Straight 조건 만족 시
        else if (bestCategory == -1 && scores[10] == 0 && checkBigStraight(dice))
        {
            maxScore = calculateScore(10, dice); // Big Straight
            bestCategory = 10;
        }
    }
    // Full House가 가능하면 선택
    else if (bestCategory == -1 && scores[8] == 0)
    {
        if (checkFullHouse(dice))
        {
            bestCategory = 8; // Full House
            maxScore = 25;
        }
    }
    // 특수 카테고리 조건을 만족하지 못하면 Ones~Sixes 또는 Choice 선택
    else if (bestCategory == -1)
    {
        // Ones ~ Sixes 또는 Choice 중 하나를 선택
        for (int i = 0; i < 6; i++)
        {
            if (scores[i] == 0)
            { // Ones~Sixes
                int score = calculateScore(i, dice);
                if (score > maxScore)
                {
                    maxScore = score;
                    bestCategory = i;
                }
            }
        }
        // Choice 카테고리 선택
        if (bestCategory == -1 && scores[11] == 0)
        {
            int score = calculateScore(11, dice); // Choice
            if (score > maxScore)
                bestCategory = 11;
        }
    }
    // 특수 카테고리나 Ones ~ Sixes, Choice를 선택할 수 없다면 남은 카테고리 중 하나 선택
    else if (bestCategory == -1)
    {
        for (int i = 0; i < NUM_CATEGORIES; i++)
        {
            if (scores[i] == 0)
            { // 아직 선택되지 않은 카테고리만 선택
                bestCategory = i;
                break;
            }
        }
    }
    return bestCategory;
}
// 주사위 유지 여부를 결정하는 함수
void decideKeepDice(int dice[], int keep[], int targetCategory)
{
    int counts[6] = {0};
    for (int i = 0; i < NUM_DICE; i++)
    {
        counts[dice[i] - 1]++;
    }
    if (targetCategory < 1 || targetCategory > 12)
    {
        printf("Invalid category!\n");
        return;
    }
    switch (targetCategory)
    {
    case 6: // Yacht (모든 눈이 같을 때)
    {
        for (int i = 0; i < NUM_DICE; i++)
        {
            keep[i] = (dice[i] == dice[0]);
        }
    }
    case 7: // Four of a Kind (4개의 같은 눈)
        for (int i = 0; i < 6; i++)
        {
            if (counts[i] >= 4)
            {
                for (int j = 0; j < NUM_DICE; j++)
                {
                    keep[j] = (dice[j] == i + 1);
                }
                break;
            }
        }
    case 8: // Full House (3개, 2개)
    {
        int three = -1, two = -1;
        for (int i = 0; i < 6; i++)
        {
            if (counts[i] >= 3)
                three = i + 1;
            if (counts[i] == 2)
                two = i + 1;
        }
        for (int i = 0; i < NUM_DICE; i++)
        {
            if (dice[i] == three || dice[i] == two)
                keep[i] = 1;
        }
        break;
    }
    case 9: // Little Straight (4개 연속 숫자)
    {
        for (int i = 1; i <= 4; i++)
        {
            for (int j = 0; j < NUM_DICE; j++)
            {
                if (dice[j] == i)
                    keep[j] = 1;
            }
        }
        break;
    }
    case 10: // Big Straight (5개 연속 숫자)
    {
        for (int i = 2; i <= 6; i++)
        {
            for (int j = 0; j < NUM_DICE; j++)
            {
                if (dice[j] == i)
                    keep[j] = 1;
            }
        }
        break;
    }
    case 11: // Choice (최고값을 선택)
    {
        int maxVal = 0;
        for (int i = 0; i < NUM_DICE; i++)
        {
            if (dice[i] > maxVal)
                maxVal = dice[i];
        }
        for (int i = 0; i < NUM_DICE; i++)
        {
            keep[i] = (dice[i] == maxVal);
        }
        break;
    }
    default: // Ones ~ Sixes (숫자에 맞는 주사위 선택)
    {
        for (int i = 0; i < NUM_DICE; i++)
        {
            keep[i] = (dice[i] == targetCategory + 1);
        }
        break;
    }
    }
}
