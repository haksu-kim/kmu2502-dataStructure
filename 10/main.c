#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_STUDENTS 35000

typedef struct {
    int id;
    char name[MAX_NAME];
    char gender;
    int korean, english, math;
    int product;
} Student;

int comp_count = 0;

// 순차 탐색
int sequential_search(Student *arr, int n, int target) {
    comp_count = 0;
    for (int i = 0; i < n; i++) {
        comp_count++;
        if (arr[i].product == target) return i;
    }
    return -1;
}

// 힙 정렬
void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Student *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n) {
        comp_count++;
        if (arr[left].product > arr[largest].product)
            largest = left;
    }
    
    if (right < n) {
        comp_count++;
        if (arr[right].product > arr[largest].product)
            largest = right;
    }
    
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(Student *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// 이진 탐색
int binary_search(Student *arr, int n, int target) {
    int l = 0, r = n - 1;
    comp_count = 0;
    
    while (l <= r) {
        int m = (l + r) / 2;
        comp_count++;
        if (arr[m].product == target) return m;
        if (arr[m].product < target) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

// CSV 파일 읽기
int load_csv(const char *filename, Student **students) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("파일 열기 실패\n");
        return -1;
    }
    
    *students = malloc(MAX_STUDENTS * sizeof(Student));
    char line[256];
    int count = 0;
    
    fgets(line, sizeof(line), fp);
    
    while (fgets(line, sizeof(line), fp) && count < MAX_STUDENTS) {
        Student *s = &(*students)[count];
        if (sscanf(line, "%d,%[^,],%c,%d,%d,%d", 
                   &s->id, s->name, &s->gender, 
                   &s->korean, &s->english, &s->math) == 6) {
            s->product = s->korean * s->english * s->math;
            count++;
        }
    }
    
    fclose(fp);
    return count;
}

int main() {
    Student *students = NULL;
    
    int n = load_csv("dataset_id_ascending.csv", &students);
    if (n <= 0) return 1;
    
    srand(time(NULL));
    int target = rand() % 1000001;
    printf("==== 탐색 값: %d ====\n\n", target);
    
    // 1) 순차 탐색
    printf("[순차 탐색]\n");
    int result1 = sequential_search(students, n, target);
    int seq_comp = comp_count;
    printf("결과: %s\n", result1 != -1 ? "찾음" : "못찾음");
    printf("비교 횟수: %d\n\n", seq_comp);
    
    // 2) 정렬 + 이진 탐색
    printf("[힙 정렬 + 이진 탐색]\n");
    Student *sorted = malloc(n * sizeof(Student));
    memcpy(sorted, students, n * sizeof(Student));
    
    comp_count = 0;
    heap_sort(sorted, n);
    int sort_comp = comp_count;
    
    int result2 = binary_search(sorted, n, target);
    int bin_comp = comp_count;
    int comp = sort_comp + bin_comp;
    
    printf("결과: %s\n", result2 != -1 ? "찾음" : "못찾음");
    printf("정렬 + 탐색 비교 횟수: %d\n", comp);
    
    free(students);
    free(sorted);
    return 0;
}