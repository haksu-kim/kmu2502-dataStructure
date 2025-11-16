#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 10000
#define MAX_VALUE 1000000
#define TEST_RUNS 100

void copy_array(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

void generate_random_data(int arr[], int size, int seed) {
    srand(seed);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (MAX_VALUE + 1);
    }
}

long long insertion_sort(int arr[], int size) {
    long long comparisons = 0;
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (comparisons++, arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

long long shell_sort_basic(int arr[], int size) {
    long long comparisons = 0;
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && (comparisons++, arr[j - gap] > temp); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    return comparisons;
}

//최적 쉘 정렬 - ciura
long long shell_sort_ciura(int arr[], int size) {
    long long comparisons = 0;
    int gaps[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750, 4376, 10941, 27353, 68383, 170958, 427396};
    int gap_count = sizeof(gaps) / sizeof(gaps[0]);
    
    // 시작 간격 찾기
    int start = 0;
    for (int i = gap_count - 1; i >= 0; i--) {
        if (gaps[i] < size) {
            start = i;
            break;
        }
    }
    
    // 정렬 수행
    for (int i = start; i >= 0; i--) {
        int gap = gaps[i];
        for (int j = gap; j < size; j++) {
            int temp = arr[j];
            int k;
            for (k = j; k >= gap && (comparisons++, arr[k - gap] > temp); k -= gap) {
                arr[k] = arr[k - gap];
            }
            arr[k] = temp;
        }
    }
    return comparisons;
}

int main() {
    int original[DATA_SIZE], temp[DATA_SIZE];
    long long total_insertion = 0, total_basic = 0, total_ciura = 0;
    
    for (int run = 1; run <= TEST_RUNS; run++) {
        generate_random_data(original, DATA_SIZE, time(NULL) + run * 1000);
        
        copy_array(original, temp, DATA_SIZE);
        total_insertion += insertion_sort(temp, DATA_SIZE);
        
        copy_array(original, temp, DATA_SIZE);
        total_basic += shell_sort_basic(temp, DATA_SIZE);
        
        copy_array(original, temp, DATA_SIZE);
        total_ciura += shell_sort_ciura(temp, DATA_SIZE);
    }
    
    printf("삽입 정렬 평균 비교 횟수: %.0f회\n", (double)total_insertion / TEST_RUNS);
    printf("기본 쉘 정렬 평균 비교 횟수: %.0f회\n", (double)total_basic / TEST_RUNS);
    printf("최적화 쉘 정렬 평균 비교 횟수: %.0f회\n", (double)total_ciura / TEST_RUNS);
    
    return 0;
}
