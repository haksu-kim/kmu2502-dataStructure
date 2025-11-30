#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 구조체 정의
typedef struct {
    int id;
    char name[50];
    char gender;
    int korea_grade;
    int english_grade;
    int math_grade;
} Student;

// 정렬 통계 구조체
typedef struct {
    long long comparisons;
    size_t memory_used;
} SortStats;

// 전역 변수
Student *original_data = NULL;
int data_count = 0;
long long compare_count = 0;

// ============================================
// 비교 함수들
// ============================================

int compare_id_asc(const void *a, const void *b) {
    compare_count++;
    return ((Student*)a)->id - ((Student*)b)->id;
}

int compare_id_desc(const void *a, const void *b) {
    compare_count++;
    return ((Student*)b)->id - ((Student*)a)->id;
}

int compare_name_asc(const void *a, const void *b) {
    compare_count++;
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

int compare_name_desc(const void *a, const void *b) {
    compare_count++;
    return strcmp(((Student*)b)->name, ((Student*)a)->name);
}

int compare_gender_asc(const void *a, const void *b) {
    compare_count++;
    return ((Student*)a)->gender - ((Student*)b)->gender;
}

int compare_gender_desc(const void *a, const void *b) {
    compare_count++;
    return ((Student*)b)->gender - ((Student*)a)->gender;
}

int compare_grade_asc(const void *a, const void *b) {
    compare_count++;
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    int sum1 = s1->korea_grade + s1->english_grade + s1->math_grade;
    int sum2 = s2->korea_grade + s2->english_grade + s2->math_grade;

    if (sum1 != sum2) return sum1 - sum2;
    if (s1->korea_grade != s2->korea_grade) return s1->korea_grade - s2->korea_grade;
    if (s1->english_grade != s2->english_grade) return s1->english_grade - s2->english_grade;
    return s1->math_grade - s2->math_grade;
}

int compare_grade_desc(const void *a, const void *b) {
    compare_count++;
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    int sum1 = s1->korea_grade + s1->english_grade + s1->math_grade;
    int sum2 = s2->korea_grade + s2->english_grade + s2->math_grade;

    if (sum1 != sum2) return sum2 - sum1;
    if (s1->korea_grade != s2->korea_grade) return s2->korea_grade - s1->korea_grade;
    if (s1->english_grade != s2->english_grade) return s2->english_grade - s1->english_grade;
    return s2->math_grade - s1->math_grade;
}

// ✨ 새로 추가된 비교 함수들
int compare_total_grade_asc(const void *a, const void *b) {
    compare_count++;
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    int sum1 = s1->korea_grade + s1->english_grade + s1->math_grade;
    int sum2 = s2->korea_grade + s2->english_grade + s2->math_grade;

    if (sum1 != sum2) return sum1 - sum2;
    // 동일한 경우 국어, 영어, 수학 순으로 더 큰 사람 우선 (오름차순이므로 역순)
    if (s1->korea_grade != s2->korea_grade) return s2->korea_grade - s1->korea_grade;
    if (s1->english_grade != s2->english_grade) return s2->english_grade - s1->english_grade;
    return s2->math_grade - s1->math_grade;
}

int compare_total_grade_desc(const void *a, const void *b) {
    compare_count++;
    Student *s1 = (Student*)a;
    Student *s2 = (Student*)b;
    int sum1 = s1->korea_grade + s1->english_grade + s1->math_grade;
    int sum2 = s2->korea_grade + s2->english_grade + s2->math_grade;

    if (sum1 != sum2) return sum2 - sum1;
    // 동일한 경우 국어, 영어, 수학 순으로 더 큰 사람 우선
    if (s1->korea_grade != s2->korea_grade) return s2->korea_grade - s1->korea_grade;
    if (s1->english_grade != s2->english_grade) return s2->english_grade - s1->english_grade;
    return s2->math_grade - s1->math_grade;
}

// ============================================
// 정렬 알고리즘
// ============================================

void bubble_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(&arr[j], &arr[j + 1]) > 0) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selection_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (cmp(&arr[j], &arr[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Student temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

void insertion_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(&arr[j], &key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void shell_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && cmp(&arr[j - gap], &temp) > 0; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void quick_sort_helper(Student *arr, int low, int high, int (*cmp)(const void*, const void*)) {
    if (low < high) {
        Student pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (cmp(&arr[j], &pivot) <= 0) {
                i++;
                Student temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        Student temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quick_sort_helper(arr, low, pi - 1, cmp);
        quick_sort_helper(arr, pi + 1, high, cmp);
    }
}

void quick_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    if (n > 0) quick_sort_helper(arr, 0, n - 1, cmp);
}

void heapify(Student *arr, int n, int i, int (*cmp)(const void*, const void*)) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && cmp(&arr[left], &arr[largest]) > 0)
        largest = left;
    if (right < n && cmp(&arr[right], &arr[largest]) > 0)
        largest = right;

    if (largest != i) {
        Student temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest, cmp);
    }
}

void heap_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, cmp);
    for (int i = n - 1; i > 0; i--) {
        Student temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0, cmp);
    }
}

void merge(Student *arr, int l, int m, int r, int (*cmp)(const void*, const void*)) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Student *L = malloc(n1 * sizeof(Student));
    Student *R = malloc(n2 * sizeof(Student));
    if (!L || !R) {
        free(L); free(R);
        return;
    }

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (cmp(&L[i], &R[j]) <= 0) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

void merge_sort_helper(Student *arr, int l, int r, int (*cmp)(const void*, const void*)) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_helper(arr, l, m, cmp);
        merge_sort_helper(arr, m + 1, r, cmp);
        merge(arr, l, m, r, cmp);
    }
}

void merge_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    if (n > 0) merge_sort_helper(arr, 0, n - 1, cmp);
}

void tree_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    merge_sort(arr, n, cmp);
}

// ============================================
// 기수 정렬 전용 구현
// ============================================

// ID용 기수 정렬
void counting_sort_radix_id(Student *arr, int n, int exp, int asc) {
    Student *output = malloc(n * sizeof(Student));
    if (!output) return;

    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i].id / exp) % 10]++;

    if (asc) {
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    } else {
        for (int i = 8; i >= 0; i--) count[i] += count[i + 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i].id / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    memcpy(arr, output, n * sizeof(Student));
    free(output);
}

// 문자열(NAME)용 기수 정렬
void counting_sort_radix_name(Student *arr, int n, int pos, int asc) {
    Student *output = malloc(n * sizeof(Student));
    if (!output) return;

    int count[256] = {0};

    for (int i = 0; i < n; i++) {
        unsigned char ch = (pos < strlen(arr[i].name)) ?
                           (unsigned char)arr[i].name[pos] : 0;
        count[ch]++;
    }

    if (asc) {
        for (int i = 1; i < 256; i++)
            count[i] += count[i-1];
    } else {
        for (int i = 254; i >= 0; i--)
            count[i] += count[i+1];
    }

    for (int i = n - 1; i >= 0; i--) {
        unsigned char ch = (pos < strlen(arr[i].name)) ?
                           (unsigned char)arr[i].name[pos] : 0;
        output[count[ch] - 1] = arr[i];
        count[ch]--;
    }

    memcpy(arr, output, n * sizeof(Student));
    free(output);
}

// 성별(GENDER)용 기수 정렬
void counting_sort_radix_gender(Student *arr, int n, int asc) {
    Student *output = malloc(n * sizeof(Student));
    if (!output) return;

    int count[256] = {0};

    for (int i = 0; i < n; i++)
        count[(unsigned char)arr[i].gender]++;

    if (asc) {
        for (int i = 1; i < 256; i++)
            count[i] += count[i-1];
    } else {
        for (int i = 254; i >= 0; i--)
            count[i] += count[i+1];
    }

    for (int i = n - 1; i >= 0; i--) {
        unsigned char ch = (unsigned char)arr[i].gender;
        output[count[ch] - 1] = arr[i];
        count[ch]--;
    }

    memcpy(arr, output, n * sizeof(Student));
    free(output);
}

// 점수(0~100)용 기수 정렬
void counting_sort_radix_grade(Student *arr, int n, int grade_type, int asc) {
    Student *output = malloc(n * sizeof(Student));
    if (!output) return;

    int count[301] = {0};

    for (int i = 0; i < n; i++) {
        int value;
        switch(grade_type) {
            case 0:
                value = arr[i].korea_grade;
                break;
            case 1:
                value = arr[i].english_grade;
                break;
            case 2:
                value = arr[i].math_grade;
                break;
            case 3:
                value = arr[i].korea_grade +
                        arr[i].english_grade +
                        arr[i].math_grade;
                break;
            default:
                value = 0;
        }
        count[value]++;
    }

    if (asc) {
        for (int i = 1; i < 301; i++)
            count[i] += count[i-1];
    } else {
        for (int i = 299; i >= 0; i--)
            count[i] += count[i+1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int value;
        switch(grade_type) {
            case 0:
                value = arr[i].korea_grade;
                break;
            case 1:
                value = arr[i].english_grade;
                break;
            case 2:
                value = arr[i].math_grade;
                break;
            case 3:
                value = arr[i].korea_grade +
                        arr[i].english_grade +
                        arr[i].math_grade;
                break;
            default:
                value = 0;
        }
        output[count[value] - 1] = arr[i];
        count[value]--;
    }

    memcpy(arr, output, n * sizeof(Student));
    free(output);
}

// ============================================
// 통합 기수 정렬 함수
// ============================================

void radix_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    if (n <= 1) return;

    if (cmp == compare_id_asc || cmp == compare_id_desc) {
        int max = arr[0].id;
        for (int i = 1; i < n; i++)
            if (arr[i].id > max) max = arr[i].id;

        int asc = (cmp == compare_id_asc);
        for (int exp = 1; max / exp > 0; exp *= 10)
            counting_sort_radix_id(arr, n, exp, asc);
    }
    else if (cmp == compare_name_asc || cmp == compare_name_desc) {
        int max_len = 0;
        for (int i = 0; i < n; i++) {
            int len = strlen(arr[i].name);
            if (len > max_len) max_len = len;
        }

        int asc = (cmp == compare_name_asc);
        for (int pos = max_len - 1; pos >= 0; pos--) {
            counting_sort_radix_name(arr, n, pos, asc);
        }
    }
    else if (cmp == compare_gender_asc || cmp == compare_gender_desc) {
        int asc = (cmp == compare_gender_asc);
        counting_sort_radix_gender(arr, n, asc);
    }
    else if (cmp == compare_grade_asc || cmp == compare_grade_desc) {
        int asc = (cmp == compare_grade_asc);

        counting_sort_radix_grade(arr, n, 2, asc);
        counting_sort_radix_grade(arr, n, 1, asc);
        counting_sort_radix_grade(arr, n, 0, asc);
        counting_sort_radix_grade(arr, n, 3, asc);
    }
    // ✨ 새로운 total_grade 비교 함수를 위한 기수 정렬
    else if (cmp == compare_total_grade_asc || cmp == compare_total_grade_desc) {
        // total_grade는 복잡한 타이브레이크 룰이 있으므로 비교 기반 정렬 사용
        merge_sort(arr, n, cmp);
    }
}

// ============================================
// 데이터 로딩
// ============================================

int load_data(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File open failed: %s\n", filename);
        printf("Check if the file exists in current directory.\n");
        return 0;
    }

    char line[2048];

    if (!fgets(line, sizeof(line), fp)) {
        printf("File is empty.\n");
        fclose(fp);
        return 0;
    }

    printf("File opened successfully\n");
    printf("Header: %s", line);

    data_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) > 5) data_count++;
    }


    if (data_count == 0) {
        printf("No data found.\n");
        fclose(fp);
        return 0;
    }

    original_data = (Student*)calloc(data_count, sizeof(Student));
    if (!original_data) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return 0;
    }


    rewind(fp);
    fgets(line, sizeof(line), fp);

    int idx = 0;
    int line_num = 1;

    while (fgets(line, sizeof(line), fp) && idx < data_count) {
        line_num++;
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) < 5) continue;

        int read = sscanf(line, "%d,%49[^,],%c,%d,%d,%d",
                          &original_data[idx].id,
                          original_data[idx].name,
                          &original_data[idx].gender,
                          &original_data[idx].korea_grade,
                          &original_data[idx].english_grade,
                          &original_data[idx].math_grade);

        if (read == 6) {
            idx++;
        }
    }

    fclose(fp);
    data_count = idx;

    return 1;
}

int check_duplicates() {

    Student *temp = malloc(data_count * sizeof(Student));
    if (!temp) return 0;

    memcpy(temp, original_data, data_count * sizeof(Student));

    for (int i = 0; i < data_count - 1; i++) {
        for (int j = 0; j < data_count - i - 1; j++) {
            if (temp[j].id > temp[j + 1].id) {
                Student t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    int has_dup = 0;
    for (int i = 0; i < data_count - 1; i++) {
        if (temp[i].id == temp[i + 1].id) {
            has_dup = 1;
            break;
        }
    }

    free(temp);
    return has_dup;
}

// ============================================
// 정렬 실행
// ============================================

SortStats run_sort_test(void (*sort_func)(Student*, int, int (*)(const void*, const void*)),
                        int (*cmp)(const void*, const void*),
                        const char *sort_name,
                        const char *criteria_name,
                        int iterations) {
    SortStats stats = {0, 0};
    long long total = 0;

    clock_t start = clock();

    for (int i = 0; i < iterations; i++) {
        Student *temp = malloc(data_count * sizeof(Student));
        if (!temp) {
            printf("\nMemory allocation failed (iter %d)\n", i);
            break;
        }

        memcpy(temp, original_data, data_count * sizeof(Student));
        compare_count = 0;

        sort_func(temp, data_count, cmp);

        total += compare_count;
        free(temp);
    }

    double total_elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("    전체 시간: %.2fs \n", total_elapsed);

    stats.comparisons = total / iterations;
    stats.memory_used = data_count * sizeof(Student);
    return stats;
}

// ============================================
// 메인
// ============================================

int main() {

    if (!load_data("dataset_id_ascending.csv")) {
        return 1;
    }

    int has_dup = check_duplicates();

    const int ITER = 10;

    printf("  * Data count: %d\n", data_count);

    typedef struct {
        void (*func)(Student*, int, int (*)(const void*, const void*));
        const char *name;
        int skip_dup;
    } Sort;

    Sort sorts[] = {
        {bubble_sort, "버블 정렬", 0},
        {selection_sort, "선택 정렬", 0},
        {insertion_sort, "삽입", 0},
        {shell_sort, "셸 정렬", 0},
        {quick_sort, "퀵 정렬", 0},
        {heap_sort, "힙 정렬", 1},
        {merge_sort, "병합 정렬", 0},
        {radix_sort, "기수 정렬", 0},
        {tree_sort, "트리 정렬", 1}
    };

    typedef struct {
        int (*func)(const void*, const void*);
        const char *name;
        int stable;
    } Criteria;

    // ✨ 새로운 정렬 기준 추가
    Criteria crit[] = {
        {compare_id_asc, "ID 오름차순", 0},
        {compare_id_desc, "ID 내림차순", 0},
        {compare_name_asc, "NAME 오름차순", 0},
        {compare_name_desc, "NAME 내림차순", 0},
        {compare_gender_asc, "GENDER 오름차순", 1},
        {compare_gender_desc, "GENDER 내림차순", 1},
        {compare_grade_asc, "GRADE 오름차순", 0},
        {compare_grade_desc, "GRADE 내림차순", 0},
        {compare_total_grade_asc, "TOTAL GRADE 오름차순 (동점시 국영수 순)", 0},
        {compare_total_grade_desc, "TOTAL GRADE 내림차순 (동점시 국영수 순)", 0}
    };

    int total_tasks = 0;
    for (int c = 0; c < 10; c++) {  // ✨ 8에서 10으로 변경
        for (int s = 0; s < 9; s++) {
            if (has_dup && sorts[s].skip_dup) continue;
            if (crit[c].stable && s != 0 && s != 2 && s != 6 && s != 7) continue;
            total_tasks++;
        }
    }

    int completed = 0;
    clock_t program_start = clock();

    for (int c = 0; c < 10; c++) {  // ✨ 8에서 10으로 변경
        printf("----------------------------------------------------\n");
        printf("    정렬  기준: %s\n", crit[c].name);
        printf("----------------------------------------------------\n\n");

        for (int s = 0; s < 9; s++) {
            if (has_dup && sorts[s].skip_dup) {
                printf("  [X] %s - SKIPPED (Duplicate data)\n\n", sorts[s].name);
                continue;
            }

            if (crit[c].stable && s != 0 && s != 2 && s != 6 && s != 7) {
                printf("  [X] %s - SKIPPED (Not stable sort)\n\n", sorts[s].name);
                continue;
            }

            completed++;
            printf("  [작업 %d/%d] %s - %s\n", completed, total_tasks, sorts[s].name, crit[c].name);

            SortStats st = run_sort_test(sorts[s].func, crit[c].func,
                                        sorts[s].name, crit[c].name, ITER);
            printf("    평균 비교 횟수: %lld\n", st.comparisons);
            printf("    메모리 사용량: %zu bytes\n\n", st.memory_used);
        }
    }

    double total_time = (double)(clock() - program_start) / CLOCKS_PER_SEC;

    printf("\n");
    printf("====================================================\n");
    printf("\n");
    printf("Total execution time: %.2f seconds (%.2f minutes)\n", total_time, total_time / 60.0);
    printf("Total tasks completed: %d\n\n", completed);

    free(original_data);
    return 0;
}