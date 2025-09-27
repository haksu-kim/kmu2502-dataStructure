#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX 100

typedef struct {
    int childCount;
} Stack;
/*
void trim_spaces(char *input) {
    
    char* writer = input;
    char *reader = input;
    
    if (reader[0] == '(') {
        reader++;
    }
    while(*reader) {
        if(*reader != ' ') {
            *writer = *reader;
            writer++;
        }
        reader++;
    }
    *writer = '\0';
}
*/

void trim_spaces(char* input) {
    char* writer = input;
    char* reader = input;
    
    while(*reader) {
        if(*reader != ' ') {
            *writer = *reader;
            writer++;
        }
        reader++;
    }
    *writer = '\0';
}

int check_Tree(const char *str) {
    Stack stack[MAX];
    int top = -1;

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (isalpha(c)) {

            if (top >= 0) {
                stack[top].childCount++;
                if (stack[top].childCount > 2) return FALSE;
            }
        }
        else if (c == '(') {
            stack[++top].childCount = 0;
        }
        else if (c == ')') {
            if (top < 0) return FALSE;
            top--;
        }
    }
    return (top == -1) ? TRUE : FALSE;
}

int main(void) {
    char input[MAX];

    scanf("%[^\n]s", input);
    trim_spaces(input);
    

    if (check_Tree(input))
        printf("TRUE\n");
    else
        printf("FALSE\n");

    return 0;
}
