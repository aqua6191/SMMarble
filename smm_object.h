//
//  smm_object.h
//  SMMarble object


#define smm_object_h
#define SMMNODE_TYPE_LECTURE			0
#define SMMNODE_TYPE_RESTAURANT			1
#define SMMNODE_TYPE_LABORATORY			2
#define SMMNODE_TYPE_HOME				3
#define SMMNODE_TYPE_GOTOLAB			4
#define SMMNODE_TYPE_FOODCHANCE			5
#define SMMNODE_TYPE_FESTIVAL			6
#define SMMNODE_TYPE_MAX				7 
/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/

/* grade :
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/

//object generation 객체 생성  
void smmObj_getNode(char* name, int type, int credit, int energy);
// 멤버 검색

char* smmObj_getNodeName(void* obj);
int smmObj_getNodeType(void* obj);
int smmObj_getNodeCredit(void* obj);
int smmObj_getNodeEnergy(void* obj);
int smmObj_getNodeGrade(void* obj);
char* smmObj_getGradeName(int grade); // 요소를 문자열로 변환

typedef enum smmObjType {
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
} smmObjType_e;

 /* smm_object_h */
