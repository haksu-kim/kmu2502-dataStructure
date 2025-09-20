#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void trim_left(char *input) {
    char* writer = input;
    char* reader = input;
    
    while(*reader != 0) {
        if(*reader != ' ') {
            *writer = *reader;
            writer++;
        }
        reader++;
    }
    *writer = '\0';
    }

int check_parent(char* str) {
    int check_p=0;
    
    for(int i=0; str[i] != '\0'; i++) {
        if(str[i] == '(') {
            check_p++;
        } else if(str[i] == ')') {
            check_p--;
        }
        if(check_p < 0) {
            return FALSE;
        }
    }
    return (check_p == 0) ? TRUE : FALSE;
}

int check_binary(char* str) {
    for(int i=0; str[i] != '0'; i++) {
        if(str[i] == '(') {
            int child_count = 0;
            int balance = 1;
            int j = i + 1;
            
            if(isalpha(str[j])) {
                j++;
            }
            
            while(str[j] != '\0' && balance > 0) {
                if(isalpha(str[j])) {
                    child_count++;
                    while(isalpha(str[j])) j++;
                }
                else if(str[j] == '(') { //자식노드
                    child_count++;
                    int sub_balance = 1;
                    j++;
                    
                    while(sub_balance > 0 && str[j] != '\0') {
                        if(str[j] == '(') sub_balance++;
                        if(str[j] == ')') sub_balance--;
                        j++;
                    }
                }
                else if(str[j] == ')') {
                    balance--;
                    j++;
                }
                else {
                    j++;
                }
            }
            
            if(child_count > 2) {
                return FALSE;
            }
        }
    }
    return TRUE;
}


int main(void) {
    char input[1000];
    scanf("%s", input);

    trim_left(input);
    
    if(check_parent(input) && check_binary(input)) {
        printf("TRUE");
    } else {
        printf("FALSE");
    }

    return 0;

}
