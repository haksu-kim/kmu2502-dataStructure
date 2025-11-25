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

// 상세 진행도 표시 (시간 예측 포함)
void print_detailed_progress(int current, int total, const char *sort_name,
                             const char *criteria_name, double elapsed_sec) {
    int bar_width = 35;
    float progress = (float)current / total;
    int pos = bar_width * progress;

    // 예상 남은 시간 계산
    double est_total = (current > 0) ? (elapsed_sec / current * total) : 0;
    double est_remaining = est_total - elapsed_sec;

    printf("\r    ⏳ [");
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) printf("█");
        else if (i == pos) printf(">");
        else printf("·");
    }
    printf("] %4d/%d (%5.1f%%) | 경과: %6.1fs | 남은시간: %6.1fs ",
           current, total, progress * 100, elapsed_sec, est_remaining);
    fflush(stdout);
}

// 간단한 진행도 표시
void print_simple_progress(int current, int total, const char *msg) {
    int bar_width = 40;
    float progress = (float)current / total;
    int pos = bar_width * progress;

    printf("\r  [");
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) printf("█");
        else if (i == pos) printf(">");
        else printf("·");
    }
    printf("] %d/%d (%.1f%%) %s", current, total, progress * 100, msg);
    fflush(stdout);
}

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

void counting_sort_radix(Student *arr, int n, int exp, int asc) {
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

void radix_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    if (cmp == compare_id_asc || cmp == compare_id_desc) {
        int max = arr[0].id;
        for (int i = 1; i < n; i++)
            if (arr[i].id > max) max = arr[i].id;

        for (int exp = 1; max / exp > 0; exp *= 10)
            counting_sort_radix(arr, n, exp, cmp == compare_id_asc);
    } else {
        merge_sort(arr, n, cmp);
    }
}

void tree_sort(Student *arr, int n, int (*cmp)(const void*, const void*)) {
    merge_sort(arr, n, cmp);
}

// ============================================
// 데이터 로딩
// ============================================

int load_data(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("❌ 파일 열기 실패: %s\n", filename);
        printf("   현재 디렉토리에 파일이 있는지 확인하세요.\n");
        return 0;
    }

    char line[2048];

    // 헤더 읽기
    if (!fgets(line, sizeof(line), fp)) {
        printf("❌ 파일이 비어있습니다.\n");
        fclose(fp);
        return 0;
    }

    printf("✅ 파일 열기 성공\n");
    printf("   헤더: %s", line);

    // 데이터 개수 세기
    printf("⏳ 데이터 개수 확인 중...\n");
    data_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) > 5) data_count++;

        if (data_count % 1000 == 0) {
            print_simple_progress(data_count, data_count, "");
        }
    }

    printf("\r   총 %d줄 발견                    \n", data_count);

    if (data_count == 0) {
        printf("❌ 데이터가 없습니다.\n");
        fclose(fp);
        return 0;
    }

    // 메모리 할당
    printf("⏳ 메모리 할당 중... (%zu bytes)\n", data_count * sizeof(Student));
    original_data = (Student*)calloc(data_count, sizeof(Student));
    if (!original_data) {
        printf("❌ 메모리 할당 실패\n");
        fclose(fp);
        return 0;
    }

    printf("✅ 메모리 할당 성공\n");

    // 파일 처음으로
    rewind(fp);
    fgets(line, sizeof(line), fp); // 헤더 스킵

    // 데이터 읽기
    printf("⏳ 데이터 읽기 중...\n");
    int idx = 0;
    int line_num = 1;

    while (fgets(line, sizeof(line), fp) && idx < data_count) {
        line_num++;

        // 개행 제거
        line[strcspn(line, "\r\n")] = 0;

        // 빈 줄 스킵
        if (strlen(line) < 5) continue;

        // sscanf로 안전하게 파싱
        int read = sscanf(line, "%d,%49[^,],%c,%d,%d,%d",
                          &original_data[idx].id,
                          original_data[idx].name,
                          &original_data[idx].gender,
                          &original_data[idx].korea_grade,
                          &original_data[idx].english_grade,
                          &original_data[idx].math_grade);

        if (read == 6) {
            idx++;
            if (idx % 500 == 0) {
                print_simple_progress(idx, data_count, "읽는 중...");
            }
        }
    }

    fclose(fp);
    data_count = idx;

    printf("\r✅ 데이터 로딩 완료: %d개                              \n\n", data_count);

    // 샘플 데이터 출력
    if (data_count > 0) {
        printf("📋 샘플 데이터 (첫 3개):\n");
        for (int i = 0; i < 3 && i < data_count; i++) {
            printf("   [%d] ID:%d, Name:%s, Gender:%c, 국:%d, 영:%d, 수:%d\n",
                   i+1, original_data[i].id, original_data[i].name,
                   original_data[i].gender, original_data[i].korea_grade,
                   original_data[i].english_grade, original_data[i].math_grade);
        }
        printf("\n");
    }

    if (data_count > 5000) {
        printf("⚠️  데이터 %d개 - 버블/선택 정렬은 시간이 오래 걸립니다.\n\n", data_count);
    }

    return 1;
}

int check_duplicates() {
    printf("⏳ 중복 검사 중...\n");

    Student *temp = malloc(data_count * sizeof(Student));
    if (!temp) return 0;

    memcpy(temp, original_data, data_count * sizeof(Student));

    // 간단한 버블 정렬로 중복 검사
    for (int i = 0; i < data_count - 1; i++) {
        if (i % 100 == 0) {
            print_simple_progress(i, data_count, "정렬 중...");
        }
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
    printf("\r%s                              \n\n", has_dup ? "⚠️  중복 있음" : "✅ 중복 없음");
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
    clock_t last_update = start;

    for (int i = 0; i < iterations; i++) {
        Student *temp = malloc(data_count * sizeof(Student));
        if (!temp) {
            printf("\n❌ 메모리 할당 실패 (iter %d)\n", i);
            break;
        }

        memcpy(temp, original_data, data_count * sizeof(Student));
        compare_count = 0;

        sort_func(temp, data_count, cmp);

        total += compare_count;
        free(temp);

        // 진행도 업데이트 (1초마다 또는 10회마다)
        clock_t now = clock();
        double elapsed = (double)(now - start) / CLOCKS_PER_SEC;

        if ((now - last_update) / CLOCKS_PER_SEC >= 1.0 ||
            (i + 1) % 10 == 0 ||
            i == iterations - 1) {
            print_detailed_progress(i + 1, iterations, sort_name, criteria_name, elapsed);
            last_update = now;
        }
    }

    double total_elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf(" ✓\n");
    printf("       └─ 완료! 총 소요시간: %.2fs (%.2f분)\n", total_elapsed, total_elapsed / 60.0);

    stats.comparisons = total / iterations;
    stats.memory_used = data_count * sizeof(Student);
    return stats;
}

// ============================================
// 메인
// ============================================

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║      🎓 정렬 알고리즘 성능 비교 프로그램 v2.0      ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    printf("\n");

    if (!load_data("dataset_id_ascending.csv")) {
        return 1;
    }

    int has_dup = check_duplicates();

    const int ITER = 100;

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("⚙️  설정\n");
    printf("   • 데이터 개수: %d개\n", data_count);
    printf("   • 반복 횟수: %d회 (각 정렬마다)\n", ITER);
    printf("   • 결과: 1000회 평균값 출력\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    typedef struct {
        void (*func)(Student*, int, int (*)(const void*, const void*));
        const char *name;
        int skip_dup;
    } Sort;

    Sort sorts[] = {
        {bubble_sort, "버블 정렬", 0},
        {selection_sort, "선택 정렬", 0},
        {insertion_sort, "삽입 정렬", 0},
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

    Criteria crit[] = {
        {compare_id_asc, "ID 오름차순", 0},
        {compare_id_desc, "ID 내림차순", 0},
        {compare_name_asc, "NAME 오름차순", 0},
        {compare_name_desc, "NAME 내림차순", 0},
        {compare_gender_asc, "GENDER 오름차순", 1},
        {compare_gender_desc, "GENDER 내림차순", 1},
        {compare_grade_asc, "성적 오름차순", 0},
        {compare_grade_desc, "성적 내림차순", 0}
    };

    // 전체 작업 수 계산
    int total_tasks = 0;
    for (int c = 0; c < 8; c++) {
        for (int s = 0; s < 9; s++) {
            if (has_dup && sorts[s].skip_dup) continue;
            if (crit[c].stable && s != 0 && s != 2 && s != 6) continue;
            total_tasks++;
        }
    }

    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║         📊 정렬 시작 (총 %d개 작업)                 ║\n", total_tasks);
    printf("╚════════════════════════════════════════════════════╝\n\n");

    int completed = 0;
    clock_t program_start = clock();

    for (int c = 0; c < 8; c++) {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("📌 정렬 기준: %s\n", crit[c].name);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

        for (int s = 0; s < 9; s++) {
            if (has_dup && sorts[s].skip_dup) {
                printf("  ⊗ %s - SKIPPED (중복 데이터)\n\n", sorts[s].name);
                continue;
            }

            if (crit[c].stable && s != 0 && s != 2 && s != 6) {
                printf("  ⊗ %s - SKIPPED (Stable 정렬 아님)\n\n", sorts[s].name);
                continue;
            }

            completed++;
            printf("  🔹 [작업 %d/%d] %s - %s\n", completed, total_tasks, sorts[s].name, crit[c].name);
            printf("     1000회 반복 평균 측정 중...\n");

            SortStats st = run_sort_test(sorts[s].func, crit[c].func,
                                        sorts[s].name, crit[c].name, ITER);
            printf("       ├─ 평균 비교 횟수: %lld회\n", st.comparisons);
            printf("       └─ 메모리 사용량: %zu bytes\n\n", st.memory_used);
        }
    }

    double total_time = (double)(clock() - program_start) / CLOCKS_PER_SEC;

    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║              ✅ 모든 정렬 완료!                     ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("⏱️  전체 소요 시간: %.2f초 (%.2f분)\n", total_time, total_time / 60.0);
    printf("📊 총 %d개 작업 완료\n\n", completed);

    free(original_data);
    return 0;
}