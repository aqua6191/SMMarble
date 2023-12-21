//  main.c
//  SMMarble
//  made by. 2114034 �濵�к� Yang ji won 

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_DICE        6 
#define MAX_NODE		100

static int board_nr;
static int food_nr;
static int festival_nr;
static int player_nr;

// �÷��̾� ������ ��� ����ü ���� 
typedef struct player {
    int energy; // ������ 
    int position; // �÷��̾��� ���� ��ġ
    char name[MAX_CHARNAME]; // �÷��̾��� �̸� (���ڿ� �迭)
    int accumCredit; // ���� ����
    int flag_graduate; // �÷��̾ ������ �� �ִ� �������� Ȯ��: 1 - graduated, 0 - yet
    int Lab; // ������ ���� Ȯ���� ���� �߰�  
} player_t;

static player_t *cur_player; // ���� �÷��̾ ����Ű�� ������

#if 0
static int player_energy[MAX_PLAYER]; // �÷��̾��� ������ �迭 (���� ������� ����)
static int player_position[MAX_PLAYER]; // �÷��̾��� ��ġ �迭 (���� ������� ����)
static char player_name[MAX_PLAYER][MAX_CHARNAME]; // �÷��̾��� �̸� �迭 (���� ������� ����)
#endif

// �÷��̾ ������ �� �ִ��� Ȯ���ϴ� �Լ�
int isGraduated(int player)
{
    // �÷��̾ Home ��忡 �ְ� ���� ������ ���� ���� �̻��� ���
    if (cur_player[player].position == 0 && cur_player[player].accumCredit >= GRADUATE_CREDIT)
    {
        cur_player[player].flag_graduate = 1; // ���� ���� ���·� �÷��� ����
    }
    else
    {
        cur_player[player].flag_graduate = 0; // �׷��� ������ ���� �Ұ��� ���·� �÷��� ����
    }
    return cur_player[player].flag_graduate;
    // ���� �÷��̾ Home ��忡 �ְ� ���� ������ ���� ���� �̻��̶�� 1�� ��ȯ�Ͽ� ���� �������� ��Ÿ��
} // �÷��̾ �����ߴ��� ���θ� Ȯ���ϴ� �Լ�





// �ֻ����� ������ �� �÷��̾ ���� �󿡼� �̵���Ű�� �Լ� 
void goForward(int player, int step)
{
    void *boardPtr; // ���� ����� �����͸� ����Ű�� ������ 
    // ���带 �� ���� �� ������ ���, ���� ��ġ�� �ٽ� ó������ �ǵ��� 
    if ((cur_player[player].position + step) >= smmdb_len(LISTNO_NODE))
    {
        cur_player[player].position -= smmdb_len(LISTNO_NODE);
    }
    // �÷��̾��� ���� ��ġ�� �ֻ��� ���� ������� ���� 
    cur_player[player].position += step;
    // �÷��̾ �̵��� ����� �����͸� ������ 
    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    // �̵��� ����� ��� 
    printf("\n%s �� %i�� �̵��մϴ�. (���: %s)\n\n",
           cur_player[player].name, cur_player[player].position,
           smmObj_getNodeName(boardPtr));
}

void printGrades(int player)
{
	int i;
	void *gradePtr;
    // �÷��̾ ������ ���ǿ� �ش� ������ ������ ����ϴ� �Լ� 
	for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		
		
		// smmObj_getGradeName �Լ����� ��ȯ�� ���ڿ��� �� �� ���� ��������
        char *gradeName = smmObj_getGradeName(smmObj_getNodeGrade(gradePtr));
        char firstTwoChars[3]; // ���� NULL ����('\0')�� ����Ͽ� ũ�⸦ 3���� ����
        strncpy(firstTwoChars, gradeName, 2);
        firstTwoChars[2] = '\0'; // NULL ���� �߰��ϱ�
		printf("\n%s: %d����, ����: %s\n", smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr), firstTwoChars);
     
	//	printf("%s: %s\n", smmObj_getNodeName(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));
	}
/*	lectureGradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, rand()%9);
	printf("%s: %s\n", smmObj_getNodeName(lectureGradePtr), smmObj_getGradeName(smmObj_getNodeGrade(lectureGradePtr)));
*/ //�ּ�ó�� 
}
// ������ ������ �̸�, ����, ������ ���


void printPlayerStatus()
{
	int i;
// �÷��̾���� ���� ���¸� ����ϴ� �Լ�
	for (i=0; i<player_nr; i++)
	{
		printf("\n%s: ���� ���� %i, ������ %i, ��ġ %i, ���� �� ����(0: ���� �� �ƴ�, 1: ���� ��) %i",
		cur_player[i].name,
		cur_player[i].accumCredit,
		cur_player[i].energy,
		cur_player[i].position,
		cur_player[i].Lab );
	}
}
 // ���� ���� ���� �� �÷��̾��� ���¸� ���: �̸�, ���� ����, ������, ���� ��ġ, ���� �� ����
 
 /*
float calcAverageGrade(int player); //�÷��̾��� ��� ���� ��� 
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName)
void printGrades(int player); 
*/
//������ �ּ� ó��  

int checkLecture(int list_nr, char *lectureName)
{
	int index = 0;
	int length = smmdb_len(list_nr);
	
// �־��� ���� �̸��� ��Ͽ� �ִ��� Ȯ��
	for (index = 0; index < length; index++)
	{
//printf("��: %s , %s\n\n", smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName);
		if(strcmp(smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName) == 0){
			return index; // ���Ǹ� ã���� �ش� �ε��� ��ȯ
		}
	}
	
	return -1;  // ���Ǹ� ã�� ���ϸ� -1 ��ȯ
}
// �� �÷��̾ �ش� ���Ǹ� ������ �����ߴ��� Ȯ���ϴ� �Լ�, ���� �̸��� ��Ͽ��� �˻��մϴ�. 

void generatePlayers(int n, int initEnergy) 
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("\n�̸��� �Է��ϼ���: "); 
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		cur_player[i].position = 0;
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
	}
}

// �÷��̾� ������ �����ϰ� �ʱ�ȭ�մϴ�. �̸�, ��ġ, ������, ���� ����, ���� ���¸� �����մϴ�.

int rolldie(int player)
{
    char c;
    int dice_no;
    printf("\n\n%d ��° �÷��̾��� �����Դϴ�! �ֻ����� ������ ���� �ƹ� Ű�� �����ּ���. ������ ������ gŰ �� �Է��ϼ���: ", player + 1);
    c = getchar();
    fflush(stdin);
    
    // 'g'�� ������ �÷��̾��� ������ ǥ���մϴ�.
    if (c == 'g')
        printGrades(player);

    dice_no = (rand() % MAX_DICE + 1);
    printf("\n���� �ֻ�����  ���ڴ� %d �Դϴ�.\n", dice_no);
    return dice_no;
}
// ������ �÷��̾��� �ֻ����� ���� ����� ��ȯ�մϴ�.

void actionNode(int player)
{
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	int type = smmObj_getNodeType(boardPtr);
	char *name = smmObj_getNodeName(boardPtr);
	int labPosition;
	int randomFoodCard;
	char *CardName;
	int randomFestCard;
	int answer;
	void *gradePtr;

    switch (type)
    {
        case SMMNODE_TYPE_LECTURE:
            if (cur_player[player].energy < smmObj_getNodeEnergy(boardPtr))
            {
                printf("\n�������� �����մϴ�.\n");
                break;
            } // �÷��̾ ���Ǹ� ��⿡ ����� �������� ������: break
            if (checkLecture(LISTNO_OFFSET_GRADE + player, smmObj_getNodeName(boardPtr)) > -1)
            {
                printf("\n�̹� ������ �����Դϴ�.\n");
                break;
            } // �÷��̾ �̹� ���� ���Ǹ�: break

            printf("\n�����Ͻðڽ��ϱ�? Y: 1, N: press any key - "); // ���� �ǻ� ����
            scanf("%d", &answer);
            fflush(stdin);
            if (answer == 1)
            {
                cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
                // ���� ����
                gradePtr = (void *)smmObj_genObject(smmObj_getNodeName(boardPtr), smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), cur_player[player].position, rand() % 9); //�Լ� ��ȯ ���� �����ͷ� ĳ���� 
                smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                printGrades(player); // �÷��̾��� ����ǥ ����ϱ�
                break;
            }
            else
            {
                printf("���� ����...\n\n");
            }
            break;

        case SMMNODE_TYPE_RESTAURANT:
            cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
            printf("\n%d ��ŭ�� �������� �����մϴ�. \n\n\n", smmObj_getNodeEnergy(boardPtr));
            break;

        case SMMNODE_TYPE_LABORATORY:
            break;

        case SMMNODE_TYPE_HOME:
            cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
            break;

        case SMMNODE_TYPE_GOTOLAB:
            labPosition = checkLecture(LISTNO_NODE, "���ڰ��н����");
            cur_player[player].position = labPosition;
            cur_player[player].Lab = 1;
            break;

        case SMMNODE_TYPE_FOODCHANCE:
            randomFoodCard = rand() % smmdb_len(LISTNO_FOODCARD);
            CardName = smmObj_getNodeName(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));
            cur_player[player].energy += smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));

            printf("\n���õ� ������ %s�Դϴ�. %d ��ŭ�� �������� �����մϴ�.\n\n", CardName, smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard)));
            break;

        case SMMNODE_TYPE_FESTIVAL:
            randomFestCard = rand() % smmdb_len(LISTNO_FESTCARD);
            printf("\n%s\n\n", smmObj_getNodeName(smmdb_getData(LISTNO_FESTCARD, randomFestCard)));
            break;

        default:
            break;
    }
} // �ֻ��� ����� ���� �÷��̾ ��ġ�� ��忡�� �߻��ϴ� �پ��� �̺�Ʈ�� ó���ϴ� �Լ�


int main(int argc, const char * argv[])
{
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;
    
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = (void*)smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);

        smmdb_addTail(LISTNO_NODE, boardObj);
		  
        if (type == SMMNODE_TYPE_HOME)
        	initEnergy = energy;
		
		board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
    	void *boardObj = smmdb_getData(LISTNO_NODE, i);
    	
        printf("node %i : %s, %i(%s), credit %i, energy %i\n\n",
			i, smmObj_getNodeName(boardObj),
			smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
			smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
		 
	}
    

    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component....\n\n");

    
    while ( fscanf(fp, "%s %i", name, &energy) == 2 ) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = (void*)smmObj_genObject(name, smmObjType_card, 0, 0, energy, 0);

        smmdb_addTail(LISTNO_FOODCARD, foodObj);
		
		
        
		
		food_nr++;
    }
    fclose(fp);

    
    printf("Total number of food cards : %i\n\n", food_nr);  
    //3. festival card config
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
    printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
                    return -1;
    }

    printf("\n\n�佺Ƽ�� ī�� ���� ��� �д� ��......\n\n");

    while (fscanf(fp, "%s", name) == 1) // �佺Ƽ�� ī�� ���ڿ� �б�
    {
    // �Ķ���� ��Ʈ ���� 
    void *festObj = (void*)smmObj_genObject(name, smmObjType_card, 0, 0, 0, 0);

    smmdb_addTail(LISTNO_FESTCARD, festObj);
  
    festival_nr++;
    }
    fclose(fp);

    printf("�� �佺Ƽ�� ī�� �� : %i\n\n", festival_nr);
    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        // �÷��̾� ���� �Է¹޾� player_nr�� ����
        printf("\n�÷��̾� ���� �Է��ϼ��� :");
  		scanf("%d", &player_nr);
  		fflush(stdin);
    }
    while (player_nr<0 || player_nr > MAX_PLAYER);

    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); // cur_player�� �÷��̾� ����ŭ �������� �޸� �Ҵ�
    
    generatePlayers(player_nr, initEnergy);
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int dice_result;
        int go_outLab;

        
        //4-1. initial printing
        printPlayerStatus();

        if (cur_player[turn].Lab > 0){
        	go_outLab = rand()%MAX_DICE+1;
        	cur_player[turn].energy -= smmObj_getNodeEnergy(smmdb_getData(LISTNO_NODE, SMMNODE_TYPE_LABORATORY));
        	printf("\n%s�� �������Դϴ�. \n\n", cur_player[turn].name);
	        printf("\n\nLab exodus no is. %d !!\n\n", go_outLab);
	        if (rolldie(turn) >= go_outLab){
	        	cur_player[turn].Lab = 0;
	        	printf("\n\n������ ����Ǿ����ϴ�.\n\n");
			}
			else
	        	printf("\n\n���� �� �������ּ���!! \n\n");
		}
		else
		{ //Normal Process

	        // 4-2. �ֻ��� ������ (���� ���� �ƴ� ���)
	        dice_result = rolldie(turn);
	        
            // 4-3. ������ ���ư���
	        goForward(turn, dice_result); 
	        
	        // ���� ���� ���θ� Ȯ���ϴ� �Լ� ȣ��
			if(isGraduated(turn) == 1)
				break;
			// isGraduated() == 1�̸� ���� ����
            // isGraduated() == 0�̸� actionNode�� �Ѿ��
			// 4-4. ������ ������ ��忡�� �׼� ����
	        actionNode(turn);

		}
        //4-5. next turn
        turn ++;
        if (turn == player_nr) turn = 0;
    }
    
    //���� �ȳ� ���� ��� 
    printf("\n\nCongratulations! game is end\n\n");
    
    //�÷��̾��� ����ǥ ��� 
    for (i=0;i<player_nr;i++)
	{
		printf("%d�� ����ǥ�Դϴ�.\n\n", i+1);
    	printGrades(i); // �÷��̾��� ����ǥ ��� �Լ� ȣ��
    	printf("\n\n");
	}
    free(cur_player);
    system("PAUSE");
    return 0; // ���α׷� ����
	
}
