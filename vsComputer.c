#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Windows.h>

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

int chooseBestCategory(int scores[], int dice[]);
void decideKeepDice(int dice[], int keep[], int targetCategory);
void recordComputerScore(int scores[], int dice[]);

void printComputerScores(int scores[]);

// UI 함수
void textcolor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

enum ColorType {
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
    int keep[NUM_DICE] = { 0 }; // 유지할 주사위 표시
    int playerScores[NUM_CATEGORIES];
    int computerScores[NUM_CATEGORIES];
    int category, rolls, chosen, i, mode;


    // 배열 초기화: 모든 카테고리를 미사용 상태로 설정
    for (i = 0; i < NUM_CATEGORIES; i++) {
        playerScores[i] = -1;
        computerScores[i] = -1;
    }

    srand(time(0)); // 랜덤 시드 설정

    printf("Welcome to Yacht Dice!\n");
    printf("\nEnter a number to select a mode (1: Single play, 2: VS Computer): ");
    scanf("%d", &mode);

    for (int turn = 0; turn < NUM_CATEGORIES; turn++) {
        printf("\n--- Turn %d ---\n", turn + 1);

        // 플레이어 턴
        if (mode == 2) {
            printf("\nYour turn!\n");
        }

        // 각 라운드 시작 시 keep 배열 초기화
        for (int i = 0; i < NUM_DICE; i++) {
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
                for (int i = 0; i < NUM_DICE; i++) {
                    scanf("%d", &keep[i]);
                }
            }
        }

        // 카테고리 선택
        chosen = 1;
        while (chosen) {
            printf("Choose a category (1:Ones, 2:Twos, 3:Threes, 4:Fours, 5:Fives, 6:Sixes, 7:Yacht, 8:Four of a Kind, 9:Full House, 10:Little Straight, 11:Big Straight, 12:Choice): ");
            scanf("%d", &category);
            category--; // 배열 인덱스 보정

            if (playerScores[category] == -1) { // 사용되지 않은 카테고리인지 확인
                int score = calculateScore(category, dice);
                playerScores[category] = score;
                printf("You scored %d points in this category.\n", score);
                chosen = 0; // 선택 완료
            }
            else {
                printf("Category already chosen, try another category.\n");
            }
        }

        // 컴퓨터의 턴
        if (mode == 2) {
            printf("\nComputer's turn!\n");

            // 컴퓨터의 주사위 유지 여부 초기화
            for (i = 0; i < NUM_DICE; i++) {
                keep[i] = 0;
            }

            // 컴퓨터 주사위 굴리기 (3회)
            for (rolls = 0; rolls < 3; rolls++) {
                rollDice(dice, keep);
                printf("\nComputer's Roll %d: ", rolls + 1);
                displayDice(dice);

                // 마지막 굴림이 아닐 때, 컴퓨터가 유지할 주사위 선택
                if (rolls < 2) {
                    int targetCategory = chooseBestCategory(computerScores, dice);
                    decideKeepDice(dice, keep, targetCategory);
                }
            }

            // 컴퓨터 점수 기록
            recordComputerScore(computerScores, dice);

            // 컴퓨터 점수 출력
            printComputerScores(computerScores);
        }

    }

    // 최종 점수 계산
    int totalPlayerScore = 0, totalComputerScore = 0;
    for (i = 0; i < NUM_CATEGORIES; i++) {
        if (playerScores[i] != -1) totalPlayerScore += playerScores[i];
        if (computerScores[i] != -1) totalComputerScore += computerScores[i];
    }

    printf("Your total score: %d\n", totalPlayerScore);
    if (mode == 2) {
        printf("Computer's total score: %d\n", totalComputerScore);

        // 승패 결정
        if (totalPlayerScore > totalComputerScore) {
            textcolor(BLUE);
            printf("\nYou Won!\n");
            textcolor(WHITE);
        }
        else if (totalPlayerScore < totalComputerScore) {
            textcolor(RED);
            printf("\nYou Lost!\n");
            textcolor(WHITE);
        }
        else {
            textcolor(PURPLE);
            printf("\nIt's a Draw!\n");
            textcolor(WHITE);
        }
    }

    return 0;
}

// 주사위 굴리기
void rollDice(int dice[], int keep[]) {
    for (i = 0; i < NUM_DICE; i++) {
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
    case 0: return calculateSum(dice, 1);
    case 1: return calculateSum(dice, 2);
    case 2: return calculateSum(dice, 3);
    case 3: return calculateSum(dice, 4);
    case 4: return calculateSum(dice, 5);
    case 5: return calculateSum(dice, 6);
    case 6: return checkYacht(dice) ? 50 : 0;
    case 7: return checkFourOfAKind(dice);
    case 8: return checkFullHouse(dice) ? 25 : 0;
    case 9: return checkLittleStraight(dice) ? 30 : 0;
    case 10: return checkBigStraight(dice) ? 40 : 0;
    case 11: return calculateSum(dice, 0);
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
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    for (int i = 0; i < 6; i++) {
        if (counts[i] >= 4) {
            return (i + 1) * 4;
        }
    }
    return 0;
}

// Full House 점수 계산 : 3개,2개
int checkFullHouse(int dice[]) {
    int counts[6] = { 0 };
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
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    if ((counts[0] > 0 && counts[1] > 0 && counts[2] > 0 && counts[3] > 0) ||
        (counts[1] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0) ||
        (counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0)) {
        return 1;
    }
    return 0;
}

// Big Straight 점수 계산 : 5개가 연속수일 때
int checkBigStraight(int dice[]) {
    int counts[6] = { 0 };
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i] - 1]++;
    }
    if ((counts[0] > 0 && counts[1] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0) ||
        (counts[1] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0)) {
        return 1;
    }
    return 0;
}

// 컴퓨터 최적 카테고리 선택
int chooseBestCategory(int scores[], int dice[]) {
    int maxScore = -1;
    int bestCategory = -1;

    // 가장 높은 점수를 기록할 카테고리 선택
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] != -1) continue; // 이미 사용된 카테고리 건너뛰기
        int currentScore = calculateScore(i, dice);
        if (currentScore > maxScore) {
            maxScore = currentScore;
            bestCategory = i;
        }
    }

    // 모든 점수가 0인 경우 사용하지 않은 남은 카테고리를 선택
    if (bestCategory == -1) {
        for (int i = 0; i < NUM_CATEGORIES; i++) {
            if (scores[i] == -1) {
                bestCategory = i;
                break;
            }
        }
    }

    return bestCategory;
}

void decideKeepDice(int dice[], int keep[], int targetCategory) {
    // targetCategory에 따른 주사위 유지 로직을 결정
    switch (targetCategory) {
    case 0: // Ones
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 1) {
                keep[i] = 1; // 1이 나온 주사위는 유지
            }
            else {
                keep[i] = 0; // 다른 주사위는 다시 굴림
            }
        }
        break;

    case 1: // Twos
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 2) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 2: // Threes
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 3) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 3: // Fours
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 4) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 4: // Fives
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 5) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 5: // Sixes
        for (int i = 0; i < NUM_DICE; i++) {
            if (dice[i] == 6) {
                keep[i] = 1;
            }
            else {
                keep[i] = 0;
            }
        }
        break;

    case 6: // Yacht
        if (checkYacht(dice)) {
            for (int i = 0; i < NUM_DICE; i++) {
                keep[i] = 1; // 모두 동일하면 모두 유지
            }
        }
        break;

    case 7: // Four of a Kind
        if (checkFourOfAKind(dice)) {
            // 4개 이상 동일한 눈이 있을 경우, 해당 눈 유지
            int counts[6] = { 0 };
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i] - 1]++;
            }
            for (int i = 0; i < 6; i++) {
                if (counts[i] >= 4) {
                    for (int j = 0; j < NUM_DICE; j++) {
                        if (dice[j] == i + 1) {
                            keep[j] = 1;
                        }
                    }
                }
            }
        }
        break;

    case 8: // Full House
        if (checkFullHouse(dice)) {
            // 3개와 2개 유지
            int counts[6] = { 0 };
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i] - 1]++;
            }
            for (int i = 0; i < 6; i++) {
                if (counts[i] == 3 || counts[i] == 2) {
                    for (int j = 0; j < NUM_DICE; j++) {
                        if (dice[j] == i + 1) {
                            keep[j] = 1;
                        }
                    }
                }
            }
        }
        break;

    case 9: // Little Straight
        if (checkLittleStraight(dice)) {
            // 연속된 4개 숫자 유지
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1 || dice[i] == 2 || dice[i] == 3 || dice[i] == 4) {
                    keep[i] = 1;
                }
                if (dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5) {
                    keep[i] = 1;
                }
                if (dice[i] == 3 || dice[i] == 4 || dice[i] == 5 || dice[i] == 6) {
                    keep[i] = 1;
                }
            }
        }
        break;

    case 10: // Big Straight
        if (checkBigStraight(dice)) {
            // 연속된 5개 숫자 유지
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1 || dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5) {
                    keep[i] = 1;
                }
                if (dice[i] == 2 || dice[i] == 3 || dice[i] == 4 || dice[i] == 5 || dice[i] == 6) {
                    keep[i] = 1;
                }
            }
        }
        break;

    case 11: // Choice
        for (int i = 0; i < NUM_DICE; i++) {
            keep[i] = 1;
        }
        break;

    default:
        printf("Invalid category!\n");
        break;
    }
}


// 컴퓨터 점수 기록
void recordComputerScore(int scores[], int dice[]) {
    int chosenCategory = chooseBestCategory(scores, dice);
    int score = calculateScore(chosenCategory, dice);
        scores[chosenCategory] = score; // 해당 카테고리에 점수 기록
        printf("Computer scored %d points in category %d.\n", score, chosenCategory + 1);
}

// 컴퓨터의 선택 후 컴퓨터가 선택한 카테고리와 점수 표시
void printComputerScores(int scores[]) {
    printf("Computer scores: ");
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        if (scores[i] == -1) printf("[ ] "); // 미사용 카테고리
        else printf("[%d] ", scores[i]);
    }
    printf("\n");
}
