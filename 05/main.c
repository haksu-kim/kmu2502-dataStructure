#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
    int height;
}TreeNode;

int searchArray(int num[], int size, int key) {
    int compa = 0;
    for(int i=0; i<size; i++) {
        compa++;
        if(num[i] == key) {
            return compa;
        }
    }
    return compa;
}

int searchTree(TreeNode* root, int key) {
    int compa = 0;
    TreeNode* current = root;
    while(current != NULL) {
        compa++;
        if(key == current->data) {
            return compa;
        } else if(key < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return compa;
}

int my_max(int a, int b) {
    return (a > b) ? a : b;
}

int height(TreeNode *N) {
    if(N == NULL) {
        return 0;
    }
    return N->height;
}

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    
    if(newNode == NULL) {
        exit(EXIT_FAILURE);
    }
    
    newNode -> data = data;
    newNode -> left = newNode -> right = NULL;
    newNode -> height = 1;
    return newNode;
}

TreeNode* insertBST(TreeNode* root, int data) {
    if(root == NULL) {
        return createNode(data);
    }
    
    if(data < root->data) {
        root->left = insertBST(root->left, data);
    } else if(data > root->data) {
        root->right = insertBST(root->right, data);
    }
    return root;
}

TreeNode* rightRotate(TreeNode *y) {
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = my_max(height(y->left), height(y->right)) + 1;
    x->height = my_max(height(x->left), height(x->right)) + 1;
    
    return x;
}//LL

TreeNode* leftRotate(TreeNode *x) {
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = my_max(height(x->left), height(x->right)) + 1;
    y->height = my_max(height(y->left), height(y->right)) + 1;
    
    return y;
}//RR

int getBalance(TreeNode *N) {
    if(N == NULL) {
        return 0;
    }
    return height(N->left) - height(N->right);
}//균형인수

TreeNode* insertAVL(TreeNode* node, int data) {
    if(node == NULL) {
        return (createNode(data));
    }
    
    if(data < node->data) {
        node->left = insertAVL(node->left, data);
    } else if(data > node->data) {
        node->right = insertAVL(node->right, data);
    } else {
        return node;
    }
    
    node->height = 1 + my_max(height(node->left), height(node->right));
    
    int balance = getBalance(node);
    
    if(balance > 1 && data < node->left->data) return rightRotate(node); // LL
    if(balance < -1 && data > node->right->data) return leftRotate(node); // RR
    if(balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node); }// LR
    if(balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node); }// RL
        
    return node;
}

TreeNode* buildBST(int num[], int size) {
    TreeNode *root = NULL;
    for(int i=0; i<size; i++) {
        root = insertBST(root, num[i]);
    }
    return root;
}

TreeNode* buildAVL(int num[], int size) {
    TreeNode *root = NULL;
    for(int i=0; i<size; i++) {
        root = insertAVL(root, num[i]);
    }
    return root;
}

void isData_1(int num[]) {
    for(int i=0; i<MAX; i++) {
        int temp;
        int isDuplicate;
        
        do{
            isDuplicate = 0;
            temp = rand() % 10001;
            
            for(int k=0; k<i; k++) {
                if(num[k] == temp) {
                    isDuplicate = 1;
                    break;
                }
            }
        } while(isDuplicate); //중복이면 다시 뽑기
        num[i] = temp;
    }
}//0~10000사이의 무작위 정수 100개 (중복X)

void isData_2(int num[]) {
    for(int i=0; i<MAX; i++) {
        num[i] = i;
    }
}//0~999까지 정렬된 정수 100개

void isData_3(int num[]) {
    for(int i=0; i<MAX; i++) {
        num[i] = 999 - i;
    }
}//999~0까지 정렬된 정수 100개

void isData_4(int value[]) {
    for(int i = 0; i < 1000; i++) {
        value[i] = i * (i % 2 + 2);
    }
}//제시된 정수 1000개

void freeTree(TreeNode* root) {
    if(root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void generate_search_keys(int keys[]) {
    for(int i=0; i<MAX; i++) {
        keys[i] = rand() % 10001;
    }
}

void run_exp(void (*data_func)(int[]), const char* data_name, int search_keys[]) {
    int data[MAX];
    data_func(data);
    
    printf("--- %s ---\n", data_name);
    
    TreeNode* bstRoot = buildBST(data, MAX);
    TreeNode* avlRoot = buildAVL(data, MAX);
    
    long long total_array_searches = 0;
    long long total_bst_searches = 0;
    long long total_avl_searches = 0;
    
    for(int i=0; i<MAX; i++) {
        int key_to_find = search_keys[i];
        total_array_searches += searchArray(data, MAX, key_to_find);
        total_bst_searches += searchTree(bstRoot, key_to_find);
        total_avl_searches += searchTree(avlRoot, key_to_find);
    }
    
    printf("Array: %s에서 평균 %.2f회 탐색\n", data_name, (double)total_array_searches / MAX);
    printf("BST:   %s에서 평균 %.2f회 탐색\n", data_name, (double)total_bst_searches / MAX);
    printf("AVL:   %s에서 평균 %.2f회 탐색\n", data_name, (double)total_avl_searches / MAX);
    printf("\n");
    
    freeTree(bstRoot);
    freeTree(avlRoot);
}



int main(void) {
    srand(time(NULL));
    
    int search_keys[MAX];
    generate_search_keys(search_keys);
    
    run_exp(isData_1, "데이터 (1): 무작위", search_keys);
    run_exp(isData_2, "데이터 (2): 정방향 정렬", search_keys);
    run_exp(isData_3, "데이터 (3): 역방향 정렬", search_keys);
    run_exp(isData_4, "데이터 (4): 지정된 패턴", search_keys);
    
    return 0;
}
