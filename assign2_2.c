#include <stdio.h>
#include <stdlib.h>
#define MAZESIZE 10 //미로의 크기

/*
행과 열, 다음 스택을 가리키는 link를 가지는
Stack 구조체
*/
typedef struct stack {
    int row;
    int column;
    struct Stack *link;
}Stack;

/*
스택의 가장 위를 가리키는 top
*/
typedef struct linkedStackList {
    Stack *top;
}SLink;

/*
이때까지 탐색한 좌표를 저장하는 구조체
*/
typedef struct point {
    int row[MAZESIZE*MAZESIZE];
    int col[MAZESIZE*MAZESIZE];
}point;

void init_linkedList(SLink *sl);
void push(SLink *sl, int row, int col, point *p);
void pop(SLink *sl);
int is_valid(SLink *sl, int row, int col);
void is_exit(SLink *sl, int row, int col, point *p);
void display(SLink *sl, point *p);

//미로
int maze[MAZESIZE][MAZESIZE] = {    { 1,1,1,1,1,1,1,1,1,1 },
                                    { 0,0,0,0,1,0,0,0,0,1 },
                                    { 1,0,0,0,1,0,0,0,0,1 },
                                    { 1,0,1,1,1,0,0,1,0,1 },
                                    { 1,0,0,0,1,0,0,1,0,1 },
                                    { 1,0,1,0,1,0,0,1,0,1 },
                                    { 1,0,1,0,1,0,0,1,0,1 },
                                    { 1,0,1,0,1,0,0,1,0,1 },
                                    { 1,0,1,0,0,0,0,1,0,0 },
                                    { 1,1,1,1,1,1,1,1,1,1 } };

int end_row = 8;        //출구 행
int end_col = 9;        //출구 열
int point_pointer = 0;  //point 구조체에 담긴 좌표의 끝을 가리키는 변수


int main()
{
    SLink *sl = (SLink*)malloc(sizeof(SLink));
    point *p = (point*)malloc(sizeof(point));
    init_linkedList(sl);
    int start_row = 1;  //시작 행
    int start_col = 0;  //시작 열

    push(sl, start_row, start_col, p);  //탐색 시작
    return 0;
}


/*
스택 초기화
*/
void init_linkedList(SLink *sl) {
    sl->top = NULL;
}


/*
스택에 좌표를 입력
*/
void push(SLink *sl, int row, int col, point *p) {
    //입력한 좌표가 타당한지 확인
    if (!is_valid(sl, row, col)) {
        return -1;
    }

    Stack *new_stack = (Stack*)malloc(sizeof(Stack));

    //스택이 비어있으면 new_stack은 NULL을 가리킴
    if (sl->top == NULL) {
        new_stack->link = NULL;
    }
    //스택이 비어있지 않으면 new_stack은 바로 밑을 가리킴
    else {
        new_stack->link = sl->top;
    }

    //새 스택이 top이 되고, 새 스택의 좌표를 입력
    sl->top = new_stack;
    new_stack->row = row;
    new_stack->column = col;

    printf("push :\t%d, %d\n", new_stack->row, new_stack->column);   //push되는 좌표를 출력

    pop(sl);                //좌표로 이동
    maze[row][col] = -1;    //탐색한 곳을 표시
    
    //이동한 좌표를 저장
    p->row[point_pointer] = row;
    p->col[point_pointer++] = col;

    //출구에 도착하면 종료
    is_exit(sl, row, col, p);

    push(sl, row + 1, col, p);  //아래
    push(sl, row - 1, col, p);  //위
    push(sl, row, col - 1, p);  //왼쪽
    push(sl, row, col + 1, p);  //오른쪽
}

/*
스택의 가장 위에 있는 좌표를 출력하고 삭제
*/
void pop(SLink *sl) {
    //스택이 비어있으면 -1을 반환
    if (sl->top == NULL) {
        return -1;
    }

    Stack *temp = (Stack*)malloc(sizeof(Stack));        //pop할 임시 변수
    temp = sl->top;
    sl->top = temp->link;

    printf("pop :\t%d, %d\n", temp->row, temp->column);  //pop되는 좌표를 출력

    free(temp);
}

/*
이동하려는 좌표가 타당한지 확인
*/
int is_valid(SLink *sl, int row, int col) {
    //좌표의 범위가 잘못됨
    if (row < 0 || col < 0 || row >= MAZESIZE || col >= MAZESIZE) {
        return 0;
    }

    //탐색하려는 좌표가 벽이거나, 이미 탐색함
    if (maze[row][col] != 0) {
        return 0;
    }

    return 1;
}

/*
출구에 도착했는지 확인
*/
void is_exit(SLink *sl, int row, int col, point *p) {
    if (row == end_row && col == end_col) {
        display(sl, p);
        exit(1);
    }
}

/*
이 때까지 이동한 좌표를 출력함
*/
void display(SLink *sl, point *p) {
    printf("-----------------------------\n");
    for (int i = 0; i < point_pointer; i++) {
        printf("%d, %d\n", p->row[i], p->col[i]);
    }
}
