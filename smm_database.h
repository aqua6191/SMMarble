//  smm_database.h
//  infection Path
//  made by. 2114034 Yang ji won 

#ifndef smm_database_h
#define smm_database_h
#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

int smmdb_addTail(int list_nr, void* obj);      // �����͸� ���� �߰�
int smmdb_deleteData(int list_nr, int index);   // ������ ���� 
int smmdb_len(int list_nr);                   // �����ͺ��̽� ���� ��������
void* smmdb_getData(int list_nr, int index);     // �ε����� ������ ��������

#endif /* smm_database_h */
