#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define FIND_NUM 10     //출력하려는 학생 수

/*
학생의 정보를 담고 있는 구조체
next는 그 다음으로 점수가 같거나 낮은 학생
prev는 그 다음으로 점수가 같거나 높은 학생
*/
typedef struct student {
    char name[100];
    int student_number;
    int score;
    struct Student *next;
    struct Student *prev;
} Student;

/*
가장 앞에 있는 학생을 가리키는 head와
Double Linked List의 길이를 나타내는 length를 가지는
Student의 Double Linked List
*/
typedef struct DoubleLinkedStudentType {
    Student *head;
    int length;
}DStype;

void init_student(DStype *ds);
void init_linkedList(DStype *ds);
void dinsert_node(DStype *ds, Student **p, int student_number, char *name, int score);
void swap_node(DStype *ds, Student *a, Student *b);
Student *get_node(DStype *ds, int pos);


int main()
{
    DStype *ds = (DStype *)malloc(sizeof(DStype));
    init_linkedList(ds);
    init_student(ds);
    Student *s = (Student *)malloc(sizeof(Student));

    //상위 10명
    for (int i = 0; i < FIND_NUM; i++) {
        s = get_node(ds, i);
        printf("%d\t%-20s\t%d\n", s->student_number, s->name, s->score);
    }
    printf("---------------------------\n");
    //하위 10명
    for (int i = ds->length - 1; i >= ds->length - FIND_NUM; i--) {
        s = get_node(ds, i);
        printf("%d\t%-20s\t%d\n", s->student_number, s->name, s->score);
    }
    printf("---------------------------\n");

    //전체 확인용
    /*
    for (int i = 0; i < ds->length; i++) {
        s = get_node(ds, i);
        printf("%d\t%-20s\t%d\n", s->student_number, s->name, s->score);
    }*/
    
   
    free(ds);
    free(s);
    return 0;
}


/*
Linked List 초기화
*/
void init_linkedList(DStype *ds) {
    if (ds == NULL) return;
    ds->head = NULL;
    ds->length = 0;
}

/*
학생들의 정보가 담긴 파일을 불러와
점수에 따라 내림차순으로 삽입
*/
void init_student(DStype *ds) {
    FILE *fp;
    fopen_s(&fp, "Student_info(1).txt", "r");
    Student **p = (Student *)malloc(sizeof(Student));   //리스트의 끝을 가리키는 포인터

    int std_num, score;
    char name[100];

    srand(time(NULL));  //시드 초기화

    //파일 열기에 실패
    if (fp == NULL) {
        printf("Fail to open the file.\n");
        return -1;
    }

    //정보를 리스트에 입력
    for (int i = 0; i < 100 && !feof(fp); i++) {
        fscanf_s(fp, "%d\t", &std_num);
        fscanf_s(fp, "%[^\n]", name, _countof(name));
        score = rand() % 101;
        dinsert_node(ds, p, std_num, name, score);
    }

    free(p);
    fclose(fp);
}

/*
리스트의 정보를 담는 DStype,
리스트의 끝을 가리키는 포인터 p,
새 노드의 정보를 인자로 받아서 노드 삽입
*/
void dinsert_node(DStype *ds, Student **p, int student_number, char *name, int score) {
    Student *new_node = (Student *)malloc(sizeof(Student));
    //새 노드에 정보 입력
    new_node->student_number = student_number;
    strcpy_s(new_node->name, 100, name);
    new_node->score = score;

    //리스트가 비어있으면 head와 포인터가 새로 만들어진 노드
    if (ds->head == NULL) {
        ds->head = new_node;
        new_node->prev = ds->head;
        new_node->next = ds->head;
        *p = new_node;
    }
    else {
        //새 노드를 리스트 끝에 연결
        new_node->next = ds->head;
        new_node->prev = *p;
        ds->head->prev = new_node;
        (*p)->next = new_node;

        //점수가 낮으면 계속 포인터를 거슬러 올라감
        for (int i = 0; i<ds->length; i++) {
            if ((*p)->score < new_node->score) {
                swap_node(ds, *p, new_node);
                *p = new_node->prev;
                
                //새 노드가 제일 크면 head는 새 노드를 가리킴
                if (i == ds->length - 1) {
                    ds->head = new_node;
                }
            }
            else break;
        }
        *p = ds->head->prev;
    }
    ds->length++;
}


/*
두 노드의 순서를 바꾸는 함수
a가 head에 더 가까운 노드
*/
void swap_node(DStype *ds, Student *a, Student *b) {
    Student *temp = (Student *)malloc(sizeof(Student));

    //a가 첫 번째 노드이면, head가 b를 가리킴
    if (ds->length <= 1) {
        ds->head = b;
        b->prev = a;
    }

    //a가 첫 번째 노드가 아니면, a 이전의 노드가 b를 가리킴
    else {
        temp = a->prev;
        temp->next = b;
        b->prev = temp;
        temp = b->next;
        temp->prev = a;
    }
    b->next = a;
    a->prev = b;
    a->next = temp;

    temp = NULL;
    free(temp);
}

/*
리스트 목록에 있는 pos번째 리스트를 반환
*/
Student *get_node(DStype *ds, int pos) {
    //pos가 범위를 벗어나면 NULL을 반환
    if (pos < 0 || pos >= ds->length) {
        return NULL;
    }

    Student *node = (Student *)malloc(sizeof(Student));
    node = ds->head;
    
    //앞에 가까우면 앞으로 탐색
    if (pos <= ds->length / 2) {
        for (int i = 0; i < pos; i++) {
            node = node->next;
        }
    }
    //뒤에 가까우면 뒤로 탐색
    else {
        for (int i = 0; i < ds->length - pos; i++) {
            node = node->prev;
        }
    }

    return node;
}