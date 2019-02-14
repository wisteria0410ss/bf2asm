#include <stdio.h>
#include <stdlib.h>

void err(char *msg){
    fprintf(stderr, "%s", msg);
    fprintf(stderr, "\ncompilation terminated.\n");
    exit(1);
}

typedef struct{
    int len, count;
    int *buf;
} Stack;

Stack *stack_init(){
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->len = 8;
    s->count = 0;
    s->buf = (int *)malloc(sizeof(int) * s->len);

    return s;
}
int stack_count(Stack *s){
    return s->count;
}
void stack_push(Stack *s, int x){
    s->buf[s->count] = x;
    s->count++;
    if(s->count == s->len){
        s->len *= 2;
        s->buf = (int *)realloc(s->buf, s->len);
    }
}
void stack_pop(Stack *s){
    if(s->count > 0) s->count--;
    else err("stack is empty.");
}
int stack_top(Stack *s){
    if(s->count == 0) err("stack is empty.");
    return s->buf[s->count-1];
}

int main(int argc, char **argv){
    if(argc < 2) err("bfc: no input file");
    FILE *fp;
    fp = fopen(argv[1], "r");
    if(fp == NULL) err("bfc: couldn't open file");

    printf("\tglobal _start\nsection .data\nbuf_io:\n\tdb 0\nbuf:\n\ttimes 65536 db 0\nsection .text\n");
    printf("getchar:\n\tmov rax, 0\n\tmov rdi, 1\n\tmov rsi, buf_io\n\tmov rdx, 1\n\tsyscall\n\tmov byte al, [buf_io]\n\tmov byte [buf+rbx], al\n\tret\n");
    printf("putchar:\n\tmov byte al, [buf+rbx]\n\tmov byte [buf_io], al\n\tmov rax, 1\n\tmov rdi, 1\n\tmov rsi, buf_io\n\tmov rdx, 1\n\tsyscall\n\tret\n");
    printf("fin:\n\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n");
    printf("_start:\n\tmov rbx, 0\n");

    int c, count = 0, top;
    Stack *stack;
    stack = stack_init();

    while((c=getc(fp)) != EOF){
        switch(c){
            case '>':
                printf("\tadd rbx, 1\n");
                break;
            case '<':
                printf("\tsub rbx, 1\n");
                break;
            case '+':
                printf("\tadd byte [buf+rbx], 1\n");
                break;
            case '-':
                printf("\tsub byte [buf+rbx], 1\n");
                break;
            case '.':
                printf("\tcall putchar\n");
                break;
            case ',':
                printf("\tcall getchar\n");
                break;
            case '[':
                printf("op%d:\n", count);
                stack_push(stack, count);
                printf("\tcmp byte [buf+rbx], 0\n\tje cl%d\n", count);
                count++;
                break;
            case ']':
                if(stack_count(stack) == 0) err("too many \']\'");
                top = stack_top(stack);
                stack_pop(stack);
                printf("\tjmp op%d\n", top);
                printf("cl%d:\n", top);
                break;
        }
    }

    printf("\tjmp fin\n");

    if(stack_count(stack) > 0){
        err("too many \'[\'");
    }
    return 0;
}