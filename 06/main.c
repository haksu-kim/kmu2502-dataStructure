#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 100 //정점

//인접리스트 연결 구조체
typedef struct Node {
    int v;
    struct Node* next;
} Node;

int cmp_insert=0, cmp_check=0, cmp_print=0;

//인접행렬
void matrixTest(int edges, int targetNode) {
    int i, j;
    int adj[V][V] = {0};
    
    //그래프 생성
    int count = 0;
    while(count < edges) {
        int a = rand() % V;
        int b = rand() % V;
        if(a != b && adj[a][b] == 0) {
            adj[a][b] = adj[b][a] = 1;
            count++;
        }
    }
    //삽입/삭제
    cmp_insert = 0;
    for(i=0; i<1000; i++) {
        int a = rand() % V;
        int b = rand() % V;
        cmp_insert++;
        if(adj[a][b] == 1) {
            adj[a][b] = adj[b][a] = 0;
        } else {
            adj[a][b] = adj[b][a] = 1;
        }
    }
    //연결
    cmp_check=0;
    for(i=0; i<1000; i++){
        int a = rand() % V;
        int b = rand() % V;
        cmp_check++;
        if(adj[a][b] == 1) {
            
            }
        }
    //인접
    //printf("노드 %d의 인접 노드: ", targetNode);
    cmp_print=0;
        for(j=0; j<V; j++){
            cmp_print++;
            if(adj[targetNode][j] == 1) {
            //printf("%d ", j);
            }
        }
        printf("\n");
    
    //메모리
    size_t mem = sizeof(adj);
    
    printf("인접행렬>>\n");
    printf("  메모리: %zu Bytes\n", mem);
    printf("  간선 삽입/삭제 비교: %d번\n", cmp_insert);
    printf("  두 정점의 연결 확인 비교: %d번\n",cmp_check);
    printf("  한 노드의 연결 노드 출력 비교: %d번\n",cmp_print);
}

//인접리스트
void listTest(int edges, int targetNode) {
    int i;
    Node* adj[V] = {NULL};
    
    int count=0;
    while(count<edges) {
        int a = rand() % V;
        int b = rand() % V;
        
        int exists=0; //중복간선방지
        Node* cur = adj[a];
        while(cur) {
            cmp_insert++;
            if(cur->v == b) {exists = 1; break; }
            cur = cur->next;
        }
        
        if(!exists && a != b) {
            Node* newA = (Node*)malloc(sizeof(Node));
            newA->v = b;
            newA->next = adj[a];
            adj[a] = newA;
            
            Node* newB = (Node*)malloc(sizeof(Node));
            newB->v = a;
            newB->next = adj[b];
            adj[b] = newB;
            
            count++;
        }
    }
    //삽입/삭제
    for(i=0; i<1000; i++) {
        int a = rand() % V;
        int b = rand() % V;
        
        Node* cur = adj[a];
        Node* prev = NULL;
        int found=0;
        
        while(cur) {
            cmp_insert++;
            if(cur->v == b) {
                found=1;
                if(prev) prev->next = cur->next;
                else adj[a] = cur->next;
                free(cur);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        //없으면 삽입
        if(!found && a != b) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->v = b;
            newNode->next = adj[a];
            adj[a] = newNode;
        }
    }
    //연결
    for(i=0; i<1000; i++) {
        int a = rand() % V;
        int b = rand() % V;
        
        Node* cur = adj[a];
        while(cur) {
            cmp_check++;
            if(cur->v == b) break;
            cur = cur->next;
        }
    }
    //인접
        Node* cur = adj[targetNode];
        while(cur) {
            cmp_print++;
            //printf("%d ", cur->v);
            cur = cur->next;
        }
        printf("\n");
    
    //메모리
    size_t mem = sizeof(adj);
    for(i=0; i<V; i++) {
        cur= adj[i];
        while(cur) {
            mem += sizeof(Node);
            Node* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    
    printf("인접리스트>>\n");
    printf("  메모리: %zu Bytes\n", mem);
    printf("  삽입/삭제 비교: %d번\n", cmp_insert);
    printf("  연결 확인 비교: %d번\n", cmp_check);
    printf("  인접 노드 출력 비교: %d번\n", cmp_print);
}

int main(void) {
    srand(time(NULL));
    int target = 15; //특정 노드 지정
    
    printf("===== 희소 그래프 (100 edges) =====\n");
    cmp_insert = cmp_check = cmp_print = 0;
    matrixTest(100, target);

    cmp_insert = cmp_check = cmp_print = 0;
    listTest(100, target);

    printf("\n===== 밀집 그래프 (4000 edges) =====\n");
    cmp_insert = cmp_check = cmp_print = 0;
    matrixTest(4000, target);

    cmp_insert = cmp_check = cmp_print = 0;
    listTest(4000, target);

    return 0;
}
