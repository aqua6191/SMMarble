//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9


typedef enum smmObjGrade{
        smmObjGrade_Ap =0,
        smmObjGrade_A0,
        smmObjGrade_A-,
        smmObjGrade_Bp,
        smmObjGrade_B0,
        smmObjGrade_B-,
        smmObjGrade_Cp,
        smmObjGrade_C0,
        smmObjGrade_C-,
        }smmObjGrade_e;
//1. 구조체 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objtype; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
       
       } smmObject_t;

       smmObject_t smm_node[MAX_NODE];   
       
#if 0       
static char smm_node_name[MAX_NODE][MAX_CHARNAME];
static int smm_node_type[MAX_NODE];
static int smm_node_credit[MAX_NODE];
static int smm_node_energy[MAX_NODE];
#end if

//2. 구조체 변수 정의  
//static int smmObj_noNode = 0;
//object generation
void smmObj_genNode(void)
{
    strcpy(smm_node[smmObj_noNode].name, name);
    smm_node_type[smmObj_noNode].type = type; //수정 
    smm_node_credit[smmObj_noNode].credit = credit;
    smm_node_energy[smmObj_noNode].energy = energy;
}

//3. 수정  
void* smmObj_genObject(char* name, smmObjGrade_e grade, smmObjGrade_e objtype)
{
 smmObject_t* ptr;
 ptr = (smmObject_t*)malloc(sizeof(smmObject_y));

    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    
    return ptr;
   
}

//member retrieving


//3. 관련함수 변경  
//element to string
char* smmObj_getNodeName(void* obj) 
{
    smmObject_t* ptr = (smmObject_t*)obj;
    return ptr->name;
}

int smmObj_getNodeType(int node_nr){
    return smm_node[node.nr].type;
    }
    
int smmObj_getNodeCredit(int node_nr){ //미정..... 
    return smm_node[node.nr].credit;
    }

int smmObj_getNodeEnergy(int node_nr){
    return smm_node[node.nr].energy;
    }

char* smmObj_getGradeName(smmGrade_e grade) //미정.... 
{
    return smmGradeName[grade];
}

