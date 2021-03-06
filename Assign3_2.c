#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int student_number;
    int score;
}Student;

typedef struct {
    Student node[100];
}StudentNode;

typedef struct {
    Student key;
    struct TreeNode *left, *right;
}TreeNode;

int size;
void init_student(Student s[]);
void display(StudentNode *s);
void connect_node(StudentNode *s, Student node[]);
void init_tree(TreeNode *t);
void mix_node(StudentNode *s);
void insert_node(TreeNode *t, Student s);

int main()
{
    Student student[100];
    StudentNode *s = (StudentNode*)malloc(sizeof(StudentNode));
    TreeNode *tree = (TreeNode*)malloc(sizeof(TreeNode));
    size = sizeof(student) / sizeof(student[0]);

    init_student(student);
    insert_node(s, student);
    display(s);
    init_tree(tree);
    mix_node(s);
    display(s);
    return 0;
}

void init_student(Student s[]) {
    FILE *fp;
    fopen_s(&fp, "Student_info.txt", "r");
    
    if (fp == NULL) {
        printf("failed to open file.\n");
        return;
    }
    printf("Success to open file!\n");

    for (int i = 0; i < 100 && !feof(fp); i++) {
        fscanf_s(fp, "%d\t", &s[i].student_number);
        fscanf_s(fp, "%[^\n]", &s[i].name, _countof(s[i].name));
        s[i].score = i;
    }
}

void display(StudentNode *s) {
    for (int i = 0; i < size; i++) {
        printf("%d\t %-20s\t %d\n", s->node[i].student_number, s->node[i].name, s->node[i].score);
    }
    printf("-------------------------------------------------\n");
}

void connect_node(StudentNode *s, Student node[]) {
    for (int i = 0; i < size; i++) {
        s->node[i] = node[i];
    }
}

void init_tree(TreeNode *t) {
    t = NULL;
}

void mix_node(StudentNode *s) {
    Student temp;
    int count = 1000;
    
    for (int i = 0; i < count; i++) {
        int val = rand() % 100;
        temp = s->node[0];
        s->node[0] = s->node[val];
        s->node[val] = temp;
    }
}

void insert_node(TreeNode *t, Student s) {
    if(t == NULL) 
}

