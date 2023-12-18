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

typedef struct player
{
    int energy;
    int position;
    int name[MAX_CHARNAME];
    int accumCredit;
    int flag_graduate;
} player_t;

static player_t *cur_player;

// function prototypes
void printPlayerStatus(void);
void generatePlayers(int n, int initEnergy);
int rolldie(void); // 수정: 매개변수가 필요없는 경우 void로 변경

// ...

// 수정: generatePlayers 함수에서 name 배열을 입력 받도록 변경
void generatePlayers(int n, int initEnergy)
{
    int i;
    // n time loop
    for (i = 0; i < n; i++)
    {
        // 수정: name 배열을 입력 받음
        printf("Enter player name: ");
        scanf("%s", cur_player[i].name);
        // set position
        cur_player[i].position = 0;
        // set energy
        cur_player[i].energy = initEnergy;
    }
}

int rolldie(void)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);

#if 0
    if (c == 'g')
        printGrades(player);
#endif

    return (rand() % MAX_DIE + 1);
}
int main(int argc, const char *argv[])
{
    FILE *fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;

    int initEnergy = 0;

    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;

    srand(time(NULL));

    // 1. import parameters ---------------------------------------------------------------------------------
    // 1-1. boardConfig
    if ((fp = fopen(BOARDFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }

    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) != EOF)
    {
        void *boardObj = (void *)smmObj_genObject(name, smmObjType_board, type, credit, energy, NULL);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if (type == SMMNODE_TYPE_HOME)
            initEnergy = energy;
        board_nr++;
    }
    fclose(fp);

    printf("Total number of board nodes : %i\n", board_nr);

    for (i = 0; i < board_nr; i++)
        printf("node %i : %s, %i\n", i, smmObj_getNodeName(i), smmObj_getNodeType(i));

#if 0
    // 2. food card config
    if ((fp = fopen(FOODFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }

    printf("\n\nReading food card component......\n");
    while () // read a food parameter set
    {
        // store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);

    // 3. festival card config
    if ((fp = fopen(FESTFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }

    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) != EOF)
    {
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, NULL);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if (type == SMMNODE_TYPE_HOME)
            initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
#endif

    // 2. Player configuration ---------------------------------------------------------------------------------

    do
    {
        printf("input player number:");
        scanf("%d", &player_nr);
        fflush(stdin);

    } while (player_nr < 0 || player_nr > MAX_PLAYER);

    cur_player = (player_t *)malloc(player_nr * sizeof(player_t));

    // 수정: generatePlayers 함수에서 name 배열을 입력 받도록 변경
    generatePlayers(player_nr, initEnergy);

#if 0
    // 3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1)
    {
        int die_result;

        // 4-1. initial printing
        printPlayerStatus();

        // 4-2. die rolling (if not in experiment)
        die_result = rolldie();

        // 4-3. go forward
        goForward(turn, die_result);

        // 4-4. take action at the destination node of the board
        actionNode();

        // 4-5. next turn
        turn = (turn + 1) % player_nr;
    }
#endif

    free(cur_player);
    system("PAUSE");
    return 0;
}
