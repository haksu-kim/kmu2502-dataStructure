#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void trim_left(char input) {
    char*a = input;

    while(*input != 0) {
        if(*input != ' ') {
            if(a != input) *a = *input;
            a++;
        }
        input++;
    }
    *a = '\0';
    }

int check_parent(char* a) {
    int check_p=0;
    while(*a != 0) {
        if(*a != '(' || *a != ')') {
            printf("ERROR!");
            return FALSE;
        }
        if(*a == '(') {
            check_p++;
        }
        if(*a == ')') {
            check_p--;
        }
        a++;
    }
    if(check_p == 0) {
        return 0; // TRUE
    } else {
        return 1; // FALSE
    }
}

int check_binary(char* a) {
    int count=0;
    int first_count, second_count=0;
    for(int i=0; i<=strlen(a); i++) {
        if(*a == '(') {
            while(*a != '(' || *a != ')') {
                    count++;
                }
                if(count >= 2) {
                    printf("FALSE");
                    return 1;
                }d
            i+=count;
            }
        if(i == strlen(a)) {
            printf("TRUE");
            }
        }
    }



int main(void) {
    char input[1000];
    scanf("%s", &input);

    trim_left(*input);
    check_parent(*input);
    check_binary(*input);

    return 0;

}
