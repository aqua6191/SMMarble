//  main.c
//  SMMarble
//  made by. 2114034 경영학부 Yang ji won 

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

// 플레이어 정보를 담는 구조체 정의 
typedef struct player {
    int energy; // 에너지 
    int position; // 플레이어의 현재 위치
    char name[MAX_CHARNAME]; // 플레이어의 이름 (문자열 배열)
    int accumCredit; // 누적 학점
    int flag_graduate; // 플레이어가 졸업할 수 있는 상태인지 확인: 1 - graduated, 0 - yet
    int Lab; // 실험중 상태 확인을 위해 추가  
} player_t;

static player_t *cur_player; // 현재 플레이어를 가리키는 포인터

#if 0
static int player_energy[MAX_PLAYER]; // 플레이어의 에너지 배열 (현재 사용하지 않음)
static int player_position[MAX_PLAYER]; // 플레이어의 위치 배열 (현재 사용하지 않음)
static char player_name[MAX_PLAYER][MAX_CHARNAME]; // 플레이어의 이름 배열 (현재 사용하지 않음)
#endif

// 플레이어가 졸업할 수 있는지 확인하는 함수
int isGraduated(int player)
{
    // 플레이어가 Home 노드에 있고 누적 학점이 졸업 기준 이상인 경우
    if (cur_player[player].position == 0 && cur_player[player].accumCredit >= GRADUATE_CREDIT)
    {
        cur_player[player].flag_graduate = 1; // 졸업 가능 상태로 플래그 설정
    }
    else
    {
        cur_player[player].flag_graduate = 0; // 그렇지 않으면 졸업 불가능 상태로 플래그 설정
    }
    return cur_player[player].flag_graduate;
    // 만약 플레이어가 Home 노드에 있고 누적 학점이 졸업 기준 이상이라면 1을 반환하여 졸업 가능함을 나타냄
} // 플레이어가 졸업했는지 여부를 확인하는 함수





// 주사위를 굴렸을 때 플레이어를 보드 상에서 이동시키는 함수 
void goForward(int player, int step)
{
    void *boardPtr; // 현재 노드의 데이터를 가리키는 포인터 
    // 보드를 한 바퀴 다 돌았을 경우, 현재 위치를 다시 처음으로 되돌림 
    if ((cur_player[player].position + step) >= smmdb_len(LISTNO_NODE))
    {
        cur_player[player].position -= smmdb_len(LISTNO_NODE);
    }
    // 플레이어의 현재 위치에 주사위 던진 결과값을 더함 
    cur_player[player].position += step;
    // 플레이어가 이동한 노드의 데이터를 가져옴 
    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    // 이동한 결과를 출력 
    printf("\n%s 가 %i로 이동합니다. (노드: %s)\n\n",
           cur_player[player].name, cur_player[player].position,
           smmObj_getNodeName(boardPtr));
}

void printGrades(int player)
{
	int i;
	void *gradePtr;
    // 플레이어가 수강한 강의와 해당 강의의 성적을 출력하는 함수 
	for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		
		
		// smmObj_getGradeName 함수에서 반환된 문자열의 앞 두 글자 가져오기
        char *gradeName = smmObj_getGradeName(smmObj_getNodeGrade(gradePtr));
        char firstTwoChars[3]; // 끝에 NULL 문자('\0')를 고려하여 크기를 3으로 지정
        strncpy(firstTwoChars, gradeName, 2);
        firstTwoChars[2] = '\0'; // NULL 문자 추가하기
		printf("\n%s: %d학점, 성적: %s\n", smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr), firstTwoChars);
     
	//	printf("%s: %s\n", smmObj_getNodeName(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));
	}
/*	lectureGradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, rand()%9);
	printf("%s: %s\n", smmObj_getNodeName(lectureGradePtr), smmObj_getGradeName(smmObj_getNodeGrade(lectureGradePtr)));
*/ //주석처리 
}
// 수강한 강의의 이름, 학점, 성적을 출력


void printPlayerStatus()
{
	int i;
// 플레이어들의 현재 상태를 출력하는 함수
	for (i=0; i<player_nr; i++)
	{
		printf("\n%s: 누적 학점 %i, 에너지 %i, 위치 %i, 실험 중 상태(0: 실험 중 아님, 1: 실험 중) %i",
		cur_player[i].name,
		cur_player[i].accumCredit,
		cur_player[i].energy,
		cur_player[i].position,
		cur_player[i].Lab );
	}
}
 // 현재 턴의 시작 시 플레이어의 상태를 출력: 이름, 누적 학점, 에너지, 현재 위치, 실험 중 여부
 
 /*
float calcAverageGrade(int player); //플레이어의 평균 성적 계산 
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName)
void printGrades(int player); 
*/
//오류로 주석 처리  

int checkLecture(int list_nr, char *lectureName)
{
	int index = 0;
	int length = smmdb_len(list_nr);
	
// 주어진 강의 이름이 목록에 있는지 확인
	for (index = 0; index < length; index++)
	{
//printf("비교: %s , %s\n\n", smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName);
		if(strcmp(smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName) == 0){
			return index; // 강의를 찾으면 해당 인덱스 반환
		}
	}
	
	return -1;  // 강의를 찾지 못하면 -1 반환
}
// 이 플레이어가 해당 강의를 이전에 수강했는지 확인하는 함수, 강의 이름을 목록에서 검색합니다. 

void generatePlayers(int n, int initEnergy) 
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("\n이름을 입력하세요: "); 
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		cur_player[i].position = 0;
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
	}
}

// 플레이어 정보를 생성하고 초기화합니다. 이름, 위치, 에너지, 누적 학점, 졸업 상태를 포함합니다.

int rolldie(int player)
{
    char c;
    int dice_no;
    printf("\n\n%d 번째 플레이어의 차례입니다! 주사위를 굴리기 위해 아무 키나 눌러주세요. 성적을 보려면 g키 를 입력하세요: ", player + 1);
    c = getchar();
    fflush(stdin);
    
    // 'g'가 눌리면 플레이어의 성적을 표시합니다.
    if (c == 'g')
        printGrades(player);

    dice_no = (rand() % MAX_DICE + 1);
    printf("\n굴린 주사위의  숫자는 %d 입니다.\n", dice_no);
    return dice_no;
}
// 지정된 플레이어의 주사위를 굴려 결과를 반환합니다.

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
                printf("\n에너지가 부족합니다.\n");
                break;
            } // 플레이어가 강의를 듣기에 충분한 에너지가 없으면: break
            if (checkLecture(LISTNO_OFFSET_GRADE + player, smmObj_getNodeName(boardPtr)) > -1)
            {
                printf("\n이미 수강한 강의입니다.\n");
                break;
            } // 플레이어가 이미 들은 강의면: break

            printf("\n수강하시겠습니까? Y: 1, N: press any key - "); // 수강 의사 묻기
            scanf("%d", &answer);
            fflush(stdin);
            if (answer == 1)
            {
                cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
                // 성적 생성
                gradePtr = (void *)smmObj_genObject(smmObj_getNodeName(boardPtr), smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), cur_player[player].position, rand() % 9); //함수 반환 값을 포인터로 캐스팅 
                smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
                printGrades(player); // 플레이어의 성적표 출력하기
                break;
            }
            else
            {
                printf("수강 포기...\n\n");
            }
            break;

        case SMMNODE_TYPE_RESTAURANT:
            cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
            printf("\n%d 만큼의 에너지를 보충합니다. \n\n\n", smmObj_getNodeEnergy(boardPtr));
            break;

        case SMMNODE_TYPE_LABORATORY:
            break;

        case SMMNODE_TYPE_HOME:
            cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
            break;

        case SMMNODE_TYPE_GOTOLAB:
            labPosition = checkLecture(LISTNO_NODE, "전자공학실험실");
            cur_player[player].position = labPosition;
            cur_player[player].Lab = 1;
            break;

        case SMMNODE_TYPE_FOODCHANCE:
            randomFoodCard = rand() % smmdb_len(LISTNO_FOODCARD);
            CardName = smmObj_getNodeName(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));
            cur_player[player].energy += smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));

            printf("\n선택된 음식은 %s입니다. %d 만큼의 에너지를 보충합니다.\n\n", CardName, smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard)));
            break;

        case SMMNODE_TYPE_FESTIVAL:
            randomFestCard = rand() % smmdb_len(LISTNO_FESTCARD);
            printf("\n%s\n\n", smmObj_getNodeName(smmdb_getData(LISTNO_FESTCARD, randomFestCard)));
            break;

        default:
            break;
    }
} // 주사위 결과에 따라 플레이어가 위치한 노드에서 발생하는 다양한 이벤트를 처리하는 함수


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

    printf("\n\n페스티벌 카드 구성 요소 읽는 중......\n\n");

    while (fscanf(fp, "%s", name) == 1) // 페스티벌 카드 문자열 읽기
    {
    // 파라미터 세트 저장 
    void *festObj = (void*)smmObj_genObject(name, smmObjType_card, 0, 0, 0, 0);

    smmdb_addTail(LISTNO_FESTCARD, festObj);
  
    festival_nr++;
    }
    fclose(fp);

    printf("총 페스티벌 카드 수 : %i\n\n", festival_nr);
    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        // 플레이어 수를 입력받아 player_nr에 저장
        printf("\n플레이어 수를 입력하세요 :");
  		scanf("%d", &player_nr);
  		fflush(stdin);
    }
    while (player_nr<0 || player_nr > MAX_PLAYER);

    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); // cur_player에 플레이어 수만큼 동적으로 메모리 할당
    
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
        	printf("\n%s는 실험중입니다. \n\n", cur_player[turn].name);
	        printf("\n\nLab exodus no is. %d !!\n\n", go_outLab);
	        if (rolldie(turn) >= go_outLab){
	        	cur_player[turn].Lab = 0;
	        	printf("\n\n실험이 종료되었습니다.\n\n");
			}
			else
	        	printf("\n\n조금 더 실험해주세요!! \n\n");
		}
		else
		{ //Normal Process

	        // 4-2. 주사위 굴리기 (실험 중이 아닌 경우)
	        dice_result = rolldie(turn);
	        
            // 4-3. 앞으로 나아가기
	        goForward(turn, dice_result); 
	        
	        // 학점 졸업 여부를 확인하는 함수 호출
			if(isGraduated(turn) == 1)
				break;
			// isGraduated() == 1이면 게임 종료
            // isGraduated() == 0이면 actionNode로 넘어가기
			// 4-4. 보드의 목적지 노드에서 액션 수행
	        actionNode(turn);

		}
        //4-5. next turn
        turn ++;
        if (turn == player_nr) turn = 0;
    }
    
    //종료 안내 문구 출력 
    printf("\n\nCongratulations! game is end\n\n");
    
    //플레이어의 성적표 출력 
    for (i=0;i<player_nr;i++)
	{
		printf("%d의 성적표입니다.\n\n", i+1);
    	printGrades(i); // 플레이어의 성적표 출력 함수 호출
    	printf("\n\n");
	}
    free(cur_player);
    system("PAUSE");
    return 0; // 프로그램 종료
	
}
