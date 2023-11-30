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


typedef struct smmObject {
       char name[MAX_CHARNAME];
       int type;
       int credit;
       int energy;
       
       } smmObject_t;

       smmObject_t smm_node[MAX_NODE];   
       
#if 0       
static char smm_node_name[MAX_NODE][MAX_CHARNAME];
static int smm_node_type[MAX_NODE];
static int smm_node_credit[MAX_NODE];
static int smm_node_energy[MAX_NODE];
#end if

static int smmObj_noNode = 0;
//object generation
void smmObj_genNode(void)
{
    strcpy(smm_node[smmObj_noNode].name, name);
    smm_node_type[smmObj_noNode].type = type; //수정 
    smm_node_credit[smmObj_noNode].credit = credit;
    smm_node_energy[smmObj_noNode].energy = energy;
}



//member retrieving



//element to string
char* smmObj_getNodeName(int node_nr) 
{
    return smm_node[node.nr].name;
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

