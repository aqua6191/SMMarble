//  SMMarble
//  made by. 2114034 Yang ji won 
#include <time.h>
#include <string.h>
#include "smm_common.h"
#include "smm_object.h"

#define MAX_NODE        100
#define MAX_NODETYPE    7 //0-6
#define MAX_GRADE       9

static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"lecture",
	"restaurant",
	"laboratory",
	"home",
	"gotolab",
	"foodchance",
	"festival"
};

// 성적 등급
typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
} smmObjGrade_e;
//성적 숫자로 저장 

static char smmGradeName [9][2]={
	"A+",
	"A0",
	"A-", 
	"B+",
	"B0",
	"B-",
	"C+",
	"C0",
	"C-",
};
//성적 이름 


typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

//2. 구조체 배열 변수 정의 

//static smmObject_t smm_node[MAX_NODE];
 
#if 0 //if 0 - end if 하면 중간에 있는 건 다 지워짐! 
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode=0;
#endif

char* smmObj_getTypeName (int type)
{
	return (char*)smmNodeName[type];
}
/*유형의 이름을 출력할 수 있는 함수 */

//1. 구조체 형식 정의

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t)); // 메모리 할당
	
	// 속성 설정
	strcpy (ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

int smmObj_getNodeType(void* obj) 
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->type;
}

int smmObj_getNodeCredit(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->credit;
}

int smmObj_getNodeEnergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->energy;
}

//element to string
char* smmObj_getNodeName(void* obj)
{
	
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->name;
}
//노드의 이름을 불러옴 
int smmObj_getNodeGrade(void* obj){
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr->grade;	
}
//성적을 불러옴 

char* smmObj_getGradeName(int grade)
{
//printf("%s\n",smmGradeName[grade]);
    return smmGradeName[grade];
}
//성적 호출  
