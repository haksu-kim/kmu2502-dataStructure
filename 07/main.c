#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define V 10
#define E 20

void createRandomGraph(int graph[V][V]) {
    int i, j, edges = 0;
    srand(time(NULL));

    for(i = 0; i < V; i++) {
        for(j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    while(edges < E) {
        int u = rand() % V;
        int v = rand() % V;
        if(u != v && graph[u][v] == 0) {
            graph[u][v] = 1;
            graph[v][u] = 1;  // 무방향 연결
            edges++;
        }
    }
}

void printGraph(int graph[V][V]) {
    printf("=== 인접 행렬 ===\n");
    for(int i = 0; i < V; i++) {
        for(int j = 0; j < V; j++) {
            printf(" %d ", graph[i][j]);
        }
        printf("\n");
    }
}

void bfs(int graph[V][V], int start, int dist[V]) {
    int visited[V] = {0};
    int queue[V];
    int front = 0, rear = 0;

    // 초기화
    for(int i = 0; i < V; i++)
        dist[i] = -1;

    dist[start] = 0;
    visited[start] = 1;
    queue[rear++] = start;

    while(front < rear) {
        int u = queue[front++];
        for(int v = 0; v < V; v++) {
            if(graph[u][v] == 1 && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                queue[rear++] = v;
            }
        }
    }
}

void printShortest(int graph[V][V]) {
    int dist[V];

    printf("\n=== 모든 노드 쌍의 최단 거리 ===\n");
    for(int i = 0; i < V; i++) {
        bfs(graph, i, dist);
        for(int j = i + 1; j < V; j++) {
            printf("(%c - %c): ", i + 65, j + 65);
            if(dist[j] == -1)
                printf("연결X\n");
            else
                printf("%d\n", dist[j]);
        }
    }
}

int main(void) {
    int graph[V][V];

    createRandomGraph(graph);
    printGraph(graph);
    printShortest(graph);

    return 0;
}
