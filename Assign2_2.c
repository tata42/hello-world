#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAZESIZE 10

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

typedef struct point {
    int row[MAZESIZE*MAZESIZE];
    int col[MAZESIZE*MAZESIZE];
}point;

void init_linkedList(SLink *sl);
void push(SLink *sl, int row, int col, point p);
void pop(SLink *sl);
int check(SLink *sl, int row, int col, point p);
void display(SLink *sl, point p);

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

int end_row = 8;
int end_col = 9;
int point_pointer = 0;

int main()
{
    SLink *sl = (SLink*)malloc(sizeof(SLink));
    point p;
    init_linkedList(sl);
    memset(&p, 0, sizeof(point));

    int start_row = 1;
    int start_col = 0;

    push(sl, start_row, start_col, p);
    return 0;
}

/*
스택 초기화
*/
void init_linkedList(SLink *sl) {
    sl->top = NULL;
}

/*
스택에 행과 열을 넣는 함수
*/
void push(SLink *sl, int row, int col, point p) {
    //입력한 좌표가 타당한지 확인
    if (!check(sl, row, col, p)) {
        return -1;
    }
    //좌표로 이동
    pop(sl);
    Stack *new_stack = (Stack*)malloc(sizeof(Stack));

    //스택이 비어있으면 new_stack은 NULL을 가리킴
    if (sl->top == NULL) {
        new_stack->link = NULL;
    }
    //스택이 비어있지 않으면 new_stack은 바로 밑을 가리킴
    else {
        new_stack->link = sl->top;
    }
    sl->top = new_stack;
    new_stack->row = row;
    new_stack->column = col;
    printf("push : %d, %d\n", new_stack->row, new_stack->column);   //push되는 좌표를 출력

    maze[row][col] = -1;    //탐색한 곳을 표시
    p.row[point_pointer-1] = row;
    p.col[point_pointer-1] = col;
    

    push(sl, row + 1, col, p); //아래
    push(sl, row - 1, col, p); //위
    push(sl, row, col - 1, p); //왼쪽
    push(sl, row, col + 1, p); //오른쪽
}

/*
스택의 가장 위에 있는 행과 열을 출력하고 삭제
*/
void pop(SLink *sl) {
    if (sl->top == NULL) {
        return -1;
    }
    Stack *temp = (Stack*)malloc(sizeof(Stack));        //pop할 임시 변수
    temp = sl->top;
    sl->top = temp->link;
    printf("pop : %d, %d\n", temp->row, temp->column);  //pop되는 좌표를 출력

    free(temp);
}

int check(SLink *sl, int row, int col, point p) {
    //행과 열의 범위가 잘못됨
    if (row < 0 || col < 0 || row >= MAZESIZE || col >= MAZESIZE) {
        return 0;
    }
    //탐색하려는 좌표가 벽이거나, 이미 탐색함
    if (maze[row][col] != 0) {
        point_pointer--;
        return 0;
    }
    //출구를 찾으면 종료
    if (row == end_row && col == end_col) {
        display(sl, p);
        exit(1);
    }

    return 1;
}

void display(SLink *sl, point p) {
    printf("-----------------------------\n");
    for (int i = 0; i < point_pointer; i++) {
        printf("%d, %d\n", p.row[i], p.col[i]);
    }
}
