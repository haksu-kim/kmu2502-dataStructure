#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode* items[MAX];
    int top;
} Stack;

void initStack(Stack* s) {
    s->top = -1;
}

int isStackEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, TreeNode* item) {
    if (s->top >= MAX - 1) {
        fprintf(stderr, "Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    s->items[++(s->top)] = item;
}

TreeNode* pop(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return s->items[(s->top)--];
}

TreeNode* peek(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }
    return s->items[s->top];
}

void trim_spaces(char* input) {
    char* writer = input;
    char* reader = input;
    while(*reader) {
        if(*reader != ' ') {
            *writer++ = *reader;
        }
        reader++;
    }
    *writer = '\0';
}

TreeNode* buildTreeIterative(char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    Stack stack;
    initStack(&stack);

    TreeNode* root = NULL;
    TreeNode* currentNode = NULL;

    for (int i = 0; str[i] != '\0'; ++i) {
        char ch = str[i];

        if (isalpha(ch)) {
            
            currentNode = (TreeNode*)malloc(sizeof(TreeNode));
            currentNode->data = ch;
            currentNode->left = currentNode->right = NULL;

            if (root == NULL) {
                root = currentNode;
            } else {
                TreeNode* parent = peek(&stack);
                if (parent == NULL) {
                    fprintf(stderr, "Error'%c'\n", ch);
                    exit(EXIT_FAILURE);
                }

                if (parent->left == NULL) {
                    parent->left = currentNode; // 첫 번째 자식은 왼쪽
                } else if (parent->right == NULL) {
                    parent->right = currentNode; // 두 번째 자식은 오른쪽
                } else {
                    // 세 번째 자식은 허용되지 않음
                    fprintf(stderr, "Error'%c'\n", parent->data);
                    exit(EXIT_FAILURE);
                }
            }
        } else if (ch == '(') {
            // '('는 현재 노드가 부모가 됨을 의미 -> 스택에 push
            push(&stack, currentNode);
        } else if (ch == ')') {
            // ')'는 현재 서브트리 완료를 의미 -> 스택에서 pop
            pop(&stack);
        }
    }//for
    return root;
}

//전위순회
void preorder(TreeNode* root) {
    if (root == NULL) return;

      Stack stack;
      initStack(&stack);
      push(&stack, root);

      while (!isStackEmpty(&stack)) {
          TreeNode* current = pop(&stack);
          printf("%c ", current->data);

          if (current->right != NULL) {
              push(&stack, current->right);
          }
          if (current->left != NULL) {
              push(&stack, current->left);
          }
      }
}

//중위순회
void inorder(TreeNode* root) {
      Stack stack;
      initStack(&stack);
      TreeNode* current = root;

      while (current != NULL || !isStackEmpty(&stack)) {
          while (current != NULL) {
              push(&stack, current);
              current = current->left;
          }

          current = pop(&stack);
          printf("%c ", current->data);
          current = current->right;
      }
}

//후위순회
void postorder(TreeNode* root) {
    if (root == NULL) return;

       Stack s1, s2;
       initStack(&s1);
       initStack(&s2);

       push(&s1, root);

       while (!isStackEmpty(&s1)) {
           TreeNode* current = pop(&s1);
           push(&s2, current);

           if (current->left != NULL) {
               push(&s1, current->left);
           }
           if (current->right != NULL) {
               push(&s1, current->right);
           }
       }

       while (!isStackEmpty(&s2)) {
           TreeNode* current = pop(&s2);
           printf("%c ", current->data);
       }
}

void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main(void) {
    char input[200];

    scanf("%[^\n]s", input);
    trim_spaces(input);
    
    TreeNode* root = buildTreeIterative(input);
    
    printf("pre-order:  ");
    preorder(root);
    printf("\n");
    
    printf("in-order:   ");
    inorder(root);
    printf("\n");
    
    printf("post-order: ");
    postorder(root);
    printf("\n");

    freeTree(root);

    return 0;
}
