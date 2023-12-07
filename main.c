//
//  main.c
//  SMMarble
//
//  
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE     100


//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

typedef struct player{
        int energy;
        int position;
        int name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
        } player_t;
        
static player_t cur_player[MAX_PLAYER];

static player_t *cur_player;

#if0
static int player_energy[MAX_NODE];
static int player_position[MAX_NODE];
static char player_name[MAX_NODE][MAX_CHARNAME};
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void printGrades(int player); //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char* lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char* lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif

void printPlayerStatus(void){
     int i;
     for(i=0;i<player_nr;i++){
                              printf("%s : credit %i,"  //수정필요!~ 
                              
                              }
     
     
     }


void generatePlayers(int n, int initEnergy){ ////여기 수정!!!  
     
     int i;
     //n time loop
     for(i=0;i<n;i++){
     printf();
     scanf("%s", player_name[i]);
     //set position
     player_position[i] = 0;
     //set energy
     player_energy[i] = initEnergy;
     }
     }
     cur_player[i].energy= initEnergy;
      
     
     


int rolldie(int player)
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

#if 0
//action code when a player stays at a node
void actionNode(int player)
{    
     void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    // int type = smmObj_getNodeType(cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);
    
    switch (type)
    {
        //case lecture:
    case SMMNODE_TYPE_LECTURE;
    //if
    cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
    break;
    
    
    default:
        break;
    }
}
#endif



int main(int argc, const char* argv[]) {

    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;

    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;

    srand(time(NULL));


    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }

    printf("Reading board component......\n");
     while (fscanf (fp, "%s %i %i %i", name, &type, &credit, &energy, &grade)) //read a festival card string
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if(type == SMMNODE_TYPE_HOME)
        initEnergy = energy;
        board_nr++;
         
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);

    for (i = 0; i < board_nr; i++)
        printf("node %i : %s, %i\n", i, smmObj_getNodeName(i), smmObj_getNodeType(i));

        #if0
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }

    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);



    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }

    printf("\n\nReading festival card component......\n");
    while (fscanf (fp, "%s %i %i %i", name, &type, &credit, &energy, &grade)) //read a festival card string
    {
        //store the parameter set
        //수정
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if(type == SMMNODE_TYPE_HOME)
        initEnergy = energy;
        board_nr++;
         
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    #endif




    //2. Player configuration ---------------------------------------------------------------------------------

    do
    {
        //input player number to player_nr
        printf("inpur player number:");
        scanf("%d", &player_nr);
        fflush(stdin);
    
    }
    while (player_nr< 0 || player > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr size of(player_t)); ///애매함 수정...... 
    
    generatePlayers(player_nr, initEnergy);
    
    
    #if 0
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;

        //4-1. initial printing
        printPlayerStatus();

        //4-2. die rolling (if not in experiment)
        die_result = rolldie();


        //4-3. go forward
        goForward(turn, die_result);

        //4-4. take action at the destination node of the board
        actionNode();

        //4-5. next turn
        turn = (turn +1)%player_nr;

    }
#endif
      free(cur_player);
    system("PAUSE");
    return 0;
}
