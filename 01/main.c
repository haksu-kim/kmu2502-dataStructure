#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool check_node(const char* str, int* pos);

void skip_whitespace(const char* str, int* p) {
    while (str[*p] != '\0' && isspace(str[*p])) {
        (*p)++;
    }
}

bool is_valid_tree(const char* str) {
    int p = 0;
    
    bool result = check_node(str, &p);
    
    skip_whitespace(str, &p);
    if (str[p] != '\0') {
        return false;
    }
    
    return result;
}

bool check_node(const char* str, int* p) {
    skip_whitespace(str, p);
    
    if (str[*p] != '(') {
        return false;
    }
    (*p)++;
    
    skip_whitespace(str, p);
    
    if (!isalpha(str[*p])) {
        return false;
    }
    (*p)++;
    
    while (true) {
        skip_whitespace(str, p);
        
        if (str[*p] == '(') {
            if (!check_node(str, p)) {
                return false;
            }
        } else if (isalpha(str[*p])) {
            (*p)++;
        } else {
            break;
        }
    }
    
    skip_whitespace(str, p);
    
    if (str[*p] != ')') {
        return false;
    }
    (*p)++;
    
    return true;
}

int main() {
    char buffer[512];

    printf("Input : ");

    while (true) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (buffer[0] == '\0') {
            break;
        }

        if (is_valid_tree(buffer)) {
            printf("결과: TRUE\n");
        } else {
            printf("결과: ERROR\n");
        }
    }
    return 0;
}
