#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define FIND_NUM 10     //출력하려는 학생 수

/*
학생의 정보를 담고 있는 구조체
next는 그 다음으로 점수가 같거나 낮은 학생
back은 그 다음으로 점수가 같거나 높은 학생
*/
typedef struct student {
    char name[100];
    int student_number;
    int score;
    struct Student *next;
    struct Student *back;
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
	for (int i = 0; i < 100; i++) {
		s = get_node(ds, i);
		printf("%d\t%-20s\t%d\n", s->student_number, s->name, s->score);
	}

    free(ds);
    free(s);
    return 0;
}

/*
학생들의 정보가 담긴 파일을 불러와
점수에 따라 내림차순으로 삽입
*/
void init_student(DStype *ds) {
    FILE *fp;
    fopen_s(&fp, "Student_info(1).txt", "r");
    Student **p = (Student *)malloc(sizeof(Student));

    int std_num, score;
    char name[100];

    
    srand(time(NULL));

    if (fp == NULL) {
        printf("Fail to open the file.\n");
        return -1;
    }

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
Linked List 초기화
*/
void init_linkedList(DStype *ds) {
    if (ds == NULL) return;
    ds->head = NULL;
    ds->length = 0;
}

/*
리스트의 정보를 담는 DStype,
리스트의 끝을 가리키는 포인터 p,
새 노드의 정보를 인자로 받는 노드 삽입 함수
*/
void dinsert_node(DStype *ds, Student **p, int student_number, char *name, int score) {
    Student *new_node = (Student *)malloc(sizeof(Student));
    new_node->student_number = student_number;
    strcpy_s(new_node->name, 100, name);
    new_node->score = score;

    //리스트가 비어있으면 head와 포인터가 새로 만들어진 노드
    if (ds->head == NULL) {
        ds->head = new_node;
        (*p) = new_node;
        new_node->back = NULL;
        new_node->next = NULL;
    }
    else {
        Student *pos = (Student *)malloc(sizeof(Student));  //원래의 포인터를 저장할 임시 변수
        pos = (*p);

        (*p)->next = new_node;      // 새 노드를 리스트 끝에 연결
        new_node->next = NULL;
        new_node->back = (*p);

        //점수가 낮으면 계속 포인터를 거슬러 올라감
        while ((*p)->score < new_node->score) {
            swap_node(ds, *p, new_node);
            *p = new_node->back;
            if ((*p) == NULL) break;
        }

        if ((*p) != pos) (*p) = pos;    //swap이 발생하면 리스트의 끝은 원래 포인터
        else *p = new_node;             //swap이 발생 안 하면 리스트의 끝은 새 노드

        pos = NULL;
        free(pos);
    }
    ds->length++;
}


/*
두 노드의 순서를 바꾸는 함수
a가 head에 더 가까운 노드
*/
void swap_node(DStype *ds, Student *a, Student *b) {
    Student *temp = (Student *)malloc(sizeof(Student));

    //a가 첫 노드이면, head가 b를 가리킴
    if (a->back == NULL) {
        ds->head = b;
        b->back = NULL;
    }

    //그렇지 않으면, a 이전의 노드가 b를 가리킴
    else {
        temp = a->back;
        temp->next = b;
        b->back = temp;
    }

    //b가 a를 가리키고, a는 b가 원래 가리키고 있는 노드를 가리킴
    //back -> b -> a -> next
    temp = b->next;
    b->next = a;
    a->back = b;
    a->next = temp;
    if (temp != NULL) temp->back = a;

    temp = NULL;
    free(temp);
}

/*
리스트 목록에 있는 pos번째 리스트를 반환
*/
Student *get_node(DStype *ds, int pos) {
    if (pos < 0 || pos >= ds->length) {
        return NULL;
    }

    Student *node = (Student *)malloc(sizeof(Student));
    node = ds->head;
    for (int i = 0; i < pos; i++) {
        node = node->next;
    }

    return node;
}