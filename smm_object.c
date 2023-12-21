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

// ���� ���
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
//���� ���ڷ� ���� 

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
//���� �̸� 


typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

//2. ����ü �迭 ���� ���� 

//static smmObject_t smm_node[MAX_NODE];
 
#if 0 //if 0 - end if �ϸ� �߰��� �ִ� �� �� ������! 
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
/*������ �̸��� ����� �� �ִ� �Լ� */

//1. ����ü ���� ����

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t)); // �޸� �Ҵ�
	
	// �Ӽ� ����
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
//����� �̸��� �ҷ��� 
int smmObj_getNodeGrade(void* obj){
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr->grade;	
}
//������ �ҷ��� 

char* smmObj_getGradeName(int grade)
{
//printf("%s\n",smmGradeName[grade]);
    return smmGradeName[grade];
}
//���� ȣ��  
