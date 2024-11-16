// 주사위 숫자를 * 로 표현해서 나타내는 함수
void displayDicestar(int dice) {
    switch (dice) {
        case 1:
            printf("*********\n");
            printf("*       *\n");
            printf("*   *   *\n");
            printf("*       *\n");
            printf("*********\n");
            break;
        case 2:
            printf("*********\n");
            printf("* *     *\n");
            printf("*       *\n");
            printf("*     * *\n");
            printf("*********\n");
            break;
        case 3:
            printf("*********\n");
            printf("* *     *\n");
            printf("*   *   *\n");
            printf("*     * *\n");
            printf("*********\n");
            break;
        case 4:
            printf("*********\n");
            printf("* *   * *\n");
            printf("*       *\n");
            printf("* *   * *\n");
            printf("*********\n");
            break;
        case 5:
            printf("*********\n");
            printf("* *   * *\n");
            printf("*   *   *\n");
            printf("* *   * *\n");
            printf("*********\n");
            break;
        case 6:
            printf("*********\n");
            printf("* *   * *\n");
            printf("* *   * *\n");
            printf("* *   * *\n");
            printf("*********\n");
            break;
        default:
            printf("Invalid dice number!\n");
            break;
    }
}

//주사위 여러개를 동시에 그림으로 표시하는 코드
void displayAllDice(int dice[], int numDice) {
    for (int i = 0; i < numDice; i++) {
        displayDicestar(dice[i]);
        printf("\n"); // 주사위 간 간격
    }
}
#include <stdio.h>

// 점수판 출력 함수
void displayScoreboard(int scores[], int numCategories) {
    const char *categories[] = {
        "1: Ones", "2: Twos", "3: Threes", "4: Fours", "5: Fives", "6: Sixes",
        "7: Yacht", "8: Four of a Kind", "9: Full House",
        "10: Little Straight", "11: Big Straight", "12: Choice"
    };

    printf("\n======== Scoreboard ========\n");
    for (int i = 0; i < numCategories; i++) {
        if (scores[i] == 0) {
            // 아직 선택되지 않은 카테고리
            printf("%-20s: Not chosen yet\n", categories[i]);
        } else {
            // 이미 점수가 기록된 카테고리
            printf("%-20s: %d points\n", categories[i], scores[i]);
        }
    }
    printf("============================\n");

//시작 할 때 게임 제목 출력
void displayWelcomeScreen() {
    printf("================================\n");
    printf("  _     _     _     _     _     \n");
    printf(" | |   | |   | |   | |   | |    \n");
    printf(" | |   | |   | |   | |   | |    \n");
    printf(" | |   | |   | |   | |   | |    \n");
    printf(" |_____|_____|_____|_____|____| \n");
    printf("          YACHT DICE           \n");
    printf("================================\n");
    printf("Welcome to the Yacht Dice Game!\n");
    printf("Roll the dice and choose categories to score points.\n\n");
}
