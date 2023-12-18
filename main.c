// main.c

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE 100

// board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

typedef struct player {
    int energy;
    int position;
    char name[MAX_CHARNAME]; // 수정: name의 배열 크기 수정
    int accumCredit;
    int flag_graduate;
} player_t;

static player_t *cur_player; // 수정: 중복 선언 제거

// function prototypes
void printPlayerStatus(void);
void generatePlayers(int n, int initEnergy);
int rolldie(int player);
void actionNode(int player);
int isGraduated(void);

void printPlayerStatus(void) {
    int i;
    for (i = 0; i < player_nr; i++) {
        printf("%s : credit %i,", cur_player[i].name, cur_player[i].accumCredit);
        // 수정 필요한 부분이 있으면 여기에 추가
    }
}

void generatePlayers(int n, int initEnergy) {
    int i;
    // n time loop
    for (i = 0; i < n; i++) {
        printf("Enter player name: ");
        scanf("%s", cur_player[i].name);
        // set position
        cur_player[i].position = 0;
        // set energy
        cur_player[i].energy = initEnergy;
        cur_player[i].accumCredit = 0; // 수정: 누적 학점 초기화
    }
}

int rolldie(int player) {
    char c;
    printf("Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    return (rand() % MAX_DIE + 1);
}

void actionNode(int player) {
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);

    switch (type) {
        // case lecture:
        case SMMNODE_TYPE_LECTURE:
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
            break;

        default:
            break;
    }
}

int isGraduated(void) {
    // 여기에 필요한 조건을 추가하여 졸업 여부를 확인할 수 있음
    // 예: 모든 플레이어의 누적 학점이 일정 이상이면 졸업으로 판정
    int i;
    for (i = 0; i < player_nr; i++) {
        if (cur_player[i].accumCredit < GRADUATE_CREDIT) {
            return 0; // 아직 누구도 졸업하지 않음
        }
    }
    return 1; // 모든 플레이어가 졸업함
}

int main(int argc, const char *argv[]) {
    FILE *fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy = 0; // 변수 초기화가 필요한 부분 추가

    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;

    srand(time(NULL));

    // 1. import parameters ---------------------------------------------------------------------------------
    // 1-1. boardConfig
    if ((fp = fopen(BOARDFILEPATH, "r")) == NULL) {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }

    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) != EOF) // read a festival card string
    {
        // store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if (type == SMMNODE_TYPE_HOME)
            initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);

    // 2. Player configuration ---------------------------------------------------------------------------------
    do {
        // input player number to player_nr
        printf("Enter the number of players: ");
        scanf("%d", &player_nr);
        fflush(stdin);
    } while (player_nr < 0 || player_nr > MAX_PLAYER);

    // 추가: 플레이어 배열 동적 할당
    cur_player = (player_t *)malloc(player_nr * sizeof(player_t));

    generatePlayers(player_nr, initEnergy);

    // 3. SM Marble game starts ---------------------------------------------------------------------------------
while (!isGraduated()) // is anybody graduated?
{
    int die_result;
    int currentPlayerIndex = 0; // 추가: 현재 턴의 플레이어 인덱스

    // 4-1. initial printing
    printPlayerStatus();

    // 4-2. die rolling (if not in experiment)
    die_result = rolldie(currentPlayerIndex);

    // 4-3. go forward
    int newPosition = cur_player[currentPlayerIndex].position + die_result;
    if (newPosition >= board_nr) {
        newPosition -= board_nr; // 보드를 한 바퀴 다 돈 경우
        // 여기서 추가적인 작업을 할 수 있음 (예: 보너스 획득 등)
    }
    cur_player[currentPlayerIndex].position = newPosition;

    // 4-4. take action at the destination node of the board
    actionNode(currentPlayerIndex);

    // 추가: 게임 종료 조건 검사 (졸업 여부 등)
    // 예시로 모든 플레이어가 특정 조건을 만족하면 게임 종료
    if (isGraduated()) {
        printf("Game Over! All players graduated.\n");
        break;
    }

    // 추가: 다음 턴으로 넘어가기
    // 여기서는 주사위를 굴린 후 턴을 넘김
    // 원래 게임 룰에 따라 다양한 조건을 고려하여 턴을 결정할 수 있음
    currentPlayerIndex = (currentPlayerIndex + 1) % player_nr;

    // 추가: 플레이어의 에너지가 0 이하인 경우 게임에서 제외
    if (cur_player[currentPlayerIndex].energy <= 0) {
        printf("Player %s is out of energy and excluded from the game.\n", cur_player[currentPlayerIndex].name);
        // 여기서 플레이어를 게임에서 제외하는 등의 추가 작업을 수행할 수 있음
    }
}

// 추가: 동적 할당한 메모리 해제
free(cur_player);

// 추가: 게임이 끝나면 일정 시간 대기 후 프로그램 종료
printf("Thank you for playing! Exiting in 3 seconds...\n");
sleep(3);

return 0;
}
