//  smm_database.h
//  infection Path
//  made by. 2114034 Yang ji won 

#ifndef smm_database_h
#define smm_database_h
#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

int smmdb_addTail(int list_nr, void* obj);      // 데이터를 끝에 추가
int smmdb_deleteData(int list_nr, int index);   // 데이터 삭제 
int smmdb_len(int list_nr);                   // 데이터베이스 길이 가져오기
void* smmdb_getData(int list_nr, int index);     // 인덱스의 데이터 가져오기

#endif /* smm_database_h */
