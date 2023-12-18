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
    char name[MAX_CHARNAME]; // ����: name�� �迭 ũ�� ����
    int accumCredit;
    int flag_graduate;
} player_t;

static player_t *cur_player; // ����: �ߺ� ���� ����

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
        // ���� �ʿ��� �κ��� ������ ���⿡ �߰�
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
        cur_player[i].accumCredit = 0; // ����: ���� ���� �ʱ�ȭ
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
    // ���⿡ �ʿ��� ������ �߰��Ͽ� ���� ���θ� Ȯ���� �� ����
    // ��: ��� �÷��̾��� ���� ������ ���� �̻��̸� �������� ����
    int i;
    for (i = 0; i < player_nr; i++) {
        if (cur_player[i].accumCredit < GRADUATE_CREDIT) {
            return 0; // ���� ������ �������� ����
        }
    }
    return 1; // ��� �÷��̾ ������
}

int main(int argc, const char *argv[]) {
    FILE *fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy = 0; // ���� �ʱ�ȭ�� �ʿ��� �κ� �߰�

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

    // �߰�: �÷��̾� �迭 ���� �Ҵ�
    cur_player = (player_t *)malloc(player_nr * sizeof(player_t));

    generatePlayers(player_nr, initEnergy);

    // 3. SM Marble game starts ---------------------------------------------------------------------------------
while (!isGraduated()) // is anybody graduated?
{
    int die_result;
    int currentPlayerIndex = 0; // �߰�: ���� ���� �÷��̾� �ε���

    // 4-1. initial printing
    printPlayerStatus();

    // 4-2. die rolling (if not in experiment)
    die_result = rolldie(currentPlayerIndex);

    // 4-3. go forward
    int newPosition = cur_player[currentPlayerIndex].position + die_result;
    if (newPosition >= board_nr) {
        newPosition -= board_nr; // ���带 �� ���� �� �� ���
        // ���⼭ �߰����� �۾��� �� �� ���� (��: ���ʽ� ȹ�� ��)
    }
    cur_player[currentPlayerIndex].position = newPosition;

    // 4-4. take action at the destination node of the board
    actionNode(currentPlayerIndex);

    // �߰�: ���� ���� ���� �˻� (���� ���� ��)
    // ���÷� ��� �÷��̾ Ư�� ������ �����ϸ� ���� ����
    if (isGraduated()) {
        printf("Game Over! All players graduated.\n");
        break;
    }

    // �߰�: ���� ������ �Ѿ��
    // ���⼭�� �ֻ����� ���� �� ���� �ѱ�
    // ���� ���� �꿡 ���� �پ��� ������ ����Ͽ� ���� ������ �� ����
    currentPlayerIndex = (currentPlayerIndex + 1) % player_nr;

    // �߰�: �÷��̾��� �������� 0 ������ ��� ���ӿ��� ����
    if (cur_player[currentPlayerIndex].energy <= 0) {
        printf("Player %s is out of energy and excluded from the game.\n", cur_player[currentPlayerIndex].name);
        // ���⼭ �÷��̾ ���ӿ��� �����ϴ� ���� �߰� �۾��� ������ �� ����
    }
}

// �߰�: ���� �Ҵ��� �޸� ����
free(cur_player);

// �߰�: ������ ������ ���� �ð� ��� �� ���α׷� ����
printf("Thank you for playing! Exiting in 3 seconds...\n");
sleep(3);

return 0;
}
