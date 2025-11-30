#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MAX_NAME 100
#define MAX_LINE 1024
#define ITERATIONS 100

typedef struct {
    int id;
    char name[MAX_NAME];
    char gender[10];
    int korean, english, math, total;
} Student;

long long comp_count = 0;

// ============================================
// 비교 함수
// ============================================
int cmp_id_asc(const void* a, const void* b) {
    comp_count++;
    return ((Student*)a)->id - ((Student*)b)->id;
}

int cmp_id_desc(const void* a, const void* b) {
    comp_count++;
    return ((Student*)b)->id - ((Student*)a)->id;
}

int cmp_name_asc(const void* a, const void* b) {
    comp_count++;
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

int cmp_name_desc(const void* a, const void* b) {
    comp_count++;
    return strcmp(((Student*)b)->name, ((Student*)a)->name);
}

int cmp_grade_asc(const void* a, const void* b) {
    comp_count++;
    Student* s1 = (Student*)a, *s2 = (Student*)b;
    if (s1->total != s2->total) return s1->total - s2->total;
    if (s1->korean != s2->korean) return s2->korean - s1->korean;
    if (s1->english != s2->english) return s2->english - s1->english;
    return s2->math - s1->math;
}

int cmp_grade_desc(const void* a, const void* b) {
    comp_count++;
    Student* s1 = (Student*)a, *s2 = (Student*)b;
    if (s1->total != s2->total) return s2->total - s1->total;
    if (s1->korean != s2->korean) return s2->korean - s1->korean;
    if (s1->english != s2->english) return s2->english - s1->english;
    return s2->math - s1->math;
}

// ============================================
// 1. Shell Sort - Ciura Sequence
// ============================================
void shell_sort(Student* arr, int n, int (*cmp)(const void*, const void*)) {
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gap_count = 8;

    int start_idx = 0;
    while (start_idx < gap_count && gaps[start_idx] >= n) {
        start_idx++;
    }

    for (int g = start_idx; g < gap_count; g++) {
        int gap = gaps[g];

        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j = i;

            while (j >= gap && cmp(&arr[j - gap], &temp) > 0) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

// ============================================
// 2. Dual-Pivot Quick Sort
// ============================================
void swap(Student* arr, int i, int j) {
    Student temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void insertion_sort(Student* arr, int lo, int hi, int (*cmp)(const void*, const void*)) {
    for (int i = lo + 1; i <= hi; i++) {
        Student key = arr[i];
        int j = i - 1;

        while (j >= lo && cmp(&arr[j], &key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void single_pivot_partition(Student* arr, int lo, int hi,
                            int (*cmp)(const void*, const void*), int depth) {
    if (hi <= lo) return;

    int len = hi - lo + 1;
    if (len < 27) {
        insertion_sort(arr, lo, hi, cmp);
        return;
    }

    if (depth == 0) {
        for (int i = lo + (hi - lo) / 2; i >= lo; i--) {
            int parent = i;
            while (true) {
                int largest = parent;
                int left = lo + 2 * (parent - lo) + 1;
                int right = lo + 2 * (parent - lo) + 2;

                if (left <= hi && cmp(&arr[left], &arr[largest]) > 0) largest = left;
                if (right <= hi && cmp(&arr[right], &arr[largest]) > 0) largest = right;

                if (largest == parent) break;
                swap(arr, parent, largest);
                parent = largest;
            }
        }

        for (int i = hi; i > lo; i--) {
            swap(arr, lo, i);
            int parent = lo;
            while (true) {
                int largest = parent;
                int left = lo + 2 * (parent - lo) + 1;
                int right = lo + 2 * (parent - lo) + 2;

                if (left < i && cmp(&arr[left], &arr[largest]) > 0) largest = left;
                if (right < i && cmp(&arr[right], &arr[largest]) > 0) largest = right;

                if (largest == parent) break;
                swap(arr, parent, largest);
                parent = largest;
            }
        }
        return;
    }

    int mid = lo + (hi - lo) / 2;
    if (cmp(&arr[mid], &arr[lo]) < 0) swap(arr, mid, lo);
    if (cmp(&arr[hi], &arr[mid]) < 0) swap(arr, hi, mid);
    if (cmp(&arr[mid], &arr[lo]) < 0) swap(arr, mid, lo);

    Student pivot = arr[mid];
    swap(arr, mid, hi);

    int i = lo - 1;
    int j = hi;

    while (true) {
        while (cmp(&arr[++i], &pivot) < 0);
        while (j > lo && cmp(&arr[--j], &pivot) > 0);
        if (i >= j) break;
        swap(arr, i, j);
    }
    swap(arr, i, hi);

    single_pivot_partition(arr, lo, i - 1, cmp, depth - 1);
    single_pivot_partition(arr, i + 1, hi, cmp, depth - 1);
}

void dual_pivot_quick_sort_helper(Student* arr, int lo, int hi,
                                   int (*cmp)(const void*, const void*), int depth) {
    int len = hi - lo + 1;

    if (len < 27) {
        insertion_sort(arr, lo, hi, cmp);
        return;
    }

    if (depth == 0) {
        for (int i = lo + (hi - lo) / 2; i >= lo; i--) {
            int parent = i;
            while (true) {
                int largest = parent;
                int left = lo + 2 * (parent - lo) + 1;
                int right = lo + 2 * (parent - lo) + 2;

                if (left <= hi && cmp(&arr[left], &arr[largest]) > 0) largest = left;
                if (right <= hi && cmp(&arr[right], &arr[largest]) > 0) largest = right;

                if (largest == parent) break;
                swap(arr, parent, largest);
                parent = largest;
            }
        }

        for (int i = hi; i > lo; i--) {
            swap(arr, lo, i);
            int parent = lo;
            while (true) {
                int largest = parent;
                int left = lo + 2 * (parent - lo) + 1;
                int right = lo + 2 * (parent - lo) + 2;

                if (left < i && cmp(&arr[left], &arr[largest]) > 0) largest = left;
                if (right < i && cmp(&arr[right], &arr[largest]) > 0) largest = right;

                if (largest == parent) break;
                swap(arr, parent, largest);
                parent = largest;
            }
        }
        return;
    }

    int third = len / 3;
    int m1 = lo + third;
    int m2 = hi - third;

    if (m1 <= lo) m1 = lo + 1;
    if (m2 >= hi) m2 = hi - 1;

    if (cmp(&arr[m1], &arr[lo]) < 0) swap(arr, m1, lo);
    if (cmp(&arr[hi], &arr[m2]) < 0) swap(arr, hi, m2);
    if (cmp(&arr[m2], &arr[m1]) < 0) swap(arr, m2, m1);
    if (cmp(&arr[m1], &arr[lo]) < 0) swap(arr, m1, lo);
    if (cmp(&arr[hi], &arr[m2]) < 0) swap(arr, hi, m2);

    Student pivot1 = arr[m1];
    Student pivot2 = arr[m2];

    int pivot_cmp = cmp(&pivot1, &pivot2);
    if (pivot_cmp == 0) {
        comp_count--;
        single_pivot_partition(arr, lo, hi, cmp, depth);
        return;
    }

    swap(arr, m1, lo + 1);
    swap(arr, m2, hi - 1);

    int less = lo + 2;
    int great = hi - 2;

    for (int k = less; k <= great; k++) {
        int c1 = cmp(&arr[k], &pivot1);

        if (c1 < 0) {
            swap(arr, k, less);
            less++;
        } else {
            int c2 = cmp(&arr[k], &pivot2);
            if (c2 > 0) {
                while (k < great && cmp(&arr[great], &pivot2) > 0) {
                    great--;
                }
                swap(arr, k, great);
                great--;

                if (cmp(&arr[k], &pivot1) < 0) {
                    swap(arr, k, less);
                    less++;
                }
            }
        }
    }

    swap(arr, lo + 1, less - 1);
    swap(arr, hi - 1, great + 1);

    dual_pivot_quick_sort_helper(arr, lo, less - 2, cmp, depth - 1);
    dual_pivot_quick_sort_helper(arr, less, great, cmp, depth - 1);
    dual_pivot_quick_sort_helper(arr, great + 2, hi, cmp, depth - 1);
}

void dual_pivot_quick_sort(Student* arr, int n, int (*cmp)(const void*, const void*)) {
    if (n < 2) return;
    int depth = 2 * (int)log2(n);
    dual_pivot_quick_sort_helper(arr, 0, n - 1, cmp, depth);
}

// ============================================
// 3. Red-Black Tree
// ============================================
typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    Student data;
    struct RBNode *left, *right, *parent;
    Color color;
} RBNode;

RBNode* rb_create_node(Student data) {
    RBNode* node = malloc(sizeof(RBNode));
    node->data = data;
    node->left = node->right = node->parent = NULL;
    node->color = RED;
    return node;
}

void rb_rotate_left(RBNode** root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left) y->left->parent = x;
    y->parent = x->parent;

    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rb_rotate_right(RBNode** root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right) x->right->parent = y;
    x->parent = y->parent;

    if (!y->parent) *root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void rb_fix_insert(RBNode** root, RBNode* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;

            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rb_rotate_left(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_rotate_right(root, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;

            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rb_rotate_right(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_rotate_left(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void rb_insert(RBNode** root, Student data, int (*cmp)(const void*, const void*)) {
    RBNode* z = rb_create_node(data);
    RBNode* y = NULL;
    RBNode* x = *root;

    while (x) {
        y = x;
        int c = cmp(&data, &x->data);
        if (c < 0) x = x->left;
        else if (c > 0) x = x->right;
        else {
            free(z);
            comp_count--;
            return;
        }
    }

    z->parent = y;

    if (!y) *root = z;
    else {
        int c = cmp(&data, &y->data);
        if (c < 0) y->left = z;
        else y->right = z;
    }

    rb_fix_insert(root, z);
}

void rb_inorder(RBNode* node, Student* arr, int* idx) {
    if (node) {
        rb_inorder(node->left, arr, idx);
        arr[(*idx)++] = node->data;
        rb_inorder(node->right, arr, idx);
    }
}

void rb_free(RBNode* node) {
    if (node) {
        rb_free(node->left);
        rb_free(node->right);
        free(node);
    }
}

void rb_tree_sort(Student* arr, int n, int (*cmp)(const void*, const void*)) {
    RBNode* root = NULL;

    for (int i = 0; i < n; i++) {
        rb_insert(&root, arr[i], cmp);
    }

    int idx = 0;
    rb_inorder(root, arr, &idx);
    rb_free(root);
}

// ============================================
// 4. AVL Tree
// ============================================
typedef struct AVLNode {
    Student data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

int avl_height(AVLNode* n) { return n ? n->height : 0; }
int avl_max(int a, int b) { return a > b ? a : b; }

void avl_update(AVLNode* n) {
    if (n) n->height = 1 + avl_max(avl_height(n->left), avl_height(n->right));
}

int avl_balance(AVLNode* n) {
    return n ? avl_height(n->left) - avl_height(n->right) : 0;
}

AVLNode* avl_create_node(Student data) {
    AVLNode* n = malloc(sizeof(AVLNode));
    n->data = data;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

AVLNode* avl_rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    y->left = x->right;
    x->right = y;
    avl_update(y);
    avl_update(x);
    return x;
}

AVLNode* avl_rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;
    avl_update(x);
    avl_update(y);
    return y;
}

AVLNode* avl_insert(AVLNode* node, Student data, int (*cmp)(const void*, const void*)) {
    if (!node) return avl_create_node(data);

    int c = cmp(&data, &node->data);

    if (c < 0) {
        node->left = avl_insert(node->left, data, cmp);
    } else if (c > 0) {
        node->right = avl_insert(node->right, data, cmp);
    } else {
        comp_count--;
        return node;
    }

    avl_update(node);
    int bal = avl_balance(node);

    if (bal > 1 && cmp(&data, &node->left->data) < 0) {
        return avl_rotate_right(node);
    }

    if (bal < -1 && cmp(&data, &node->right->data) > 0) {
        return avl_rotate_left(node);
    }

    if (bal > 1 && cmp(&data, &node->left->data) > 0) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }

    if (bal < -1 && cmp(&data, &node->right->data) < 0) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    return node;
}

void avl_inorder(AVLNode* node, Student* arr, int* idx) {
    if (node) {
        avl_inorder(node->left, arr, idx);
        arr[(*idx)++] = node->data;
        avl_inorder(node->right, arr, idx);
    }
}

void avl_free(AVLNode* node) {
    if (node) {
        avl_free(node->left);
        avl_free(node->right);
        free(node);
    }
}

void avl_tree_sort(Student* arr, int n, int (*cmp)(const void*, const void*)) {
    AVLNode* root = NULL;

    for (int i = 0; i < n; i++) {
        root = avl_insert(root, arr[i], cmp);
    }

    int idx = 0;
    avl_inorder(root, arr, &idx);
    avl_free(root);
}

// ============================================
// 5. Timsort
// ============================================
#define MIN_MERGE 32
#define MIN_GALLOP 7

int min_run_length(int n) {
    int r = 0;
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

void tim_binary_insertion_sort(Student* arr, int left, int right,
                               int (*cmp)(const void*, const void*)) {
    for (int i = left + 1; i <= right; i++) {
        Student key = arr[i];

        int l = left, r = i - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (cmp(&arr[mid], &key) > 0) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }

        int pos = l;
        for (int j = i; j > pos; j--) {
            arr[j] = arr[j - 1];
        }
        arr[pos] = key;
    }
}

void tim_merge(Student* arr, int l, int m, int r,
               int (*cmp)(const void*, const void*)) {
    int len1 = m - l + 1, len2 = r - m;
    Student* left = malloc(len1 * sizeof(Student));
    Student* right = malloc(len2 * sizeof(Student));

    memcpy(left, arr + l, len1 * sizeof(Student));
    memcpy(right, arr + m + 1, len2 * sizeof(Student));

    int i = 0, j = 0, k = l;
    int min_gallop = MIN_GALLOP;
    int left_wins = 0;
    int right_wins = 0;

    while (i < len1 && j < len2) {
        if (cmp(&left[i], &right[j]) <= 0) {
            arr[k++] = left[i++];
            left_wins++;
            right_wins = 0;

            if (left_wins >= min_gallop) {
                while (i < len1 && j < len2 && cmp(&left[i], &right[j]) <= 0) {
                    arr[k++] = left[i++];
                }
                min_gallop++;
                left_wins = 0;
            }
        } else {
            arr[k++] = right[j++];
            right_wins++;
            left_wins = 0;

            if (right_wins >= min_gallop) {
                while (i < len1 && j < len2 && cmp(&right[j], &left[i]) < 0) {
                    arr[k++] = right[j++];
                }
                min_gallop++;
                right_wins = 0;
            }
        }
    }

    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];

    free(left);
    free(right);
}

void timsort(Student* arr, int n, int (*cmp)(const void*, const void*)) {
    if (n < 2) return;

    int min_run = min_run_length(n);

    for (int start = 0; start < n; start += min_run) {
        int end = start + min_run - 1;
        if (end >= n) end = n - 1;
        tim_binary_insertion_sort(arr, start, end, cmp);
    }

    for (int size = min_run; size < n; size = 2 * size) {
        for (int start = 0; start < n; start += 2 * size) {
            int mid = start + size - 1;
            int end = start + 2 * size - 1;

            if (mid >= n) continue;
            if (end >= n) end = n - 1;

            if (mid < end) {
                tim_merge(arr, start, mid, end, cmp);
            }
        }
    }
}

// ============================================
// CSV
// ============================================
int read_csv(const char* file, Student** students) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        printf("Error: Cannot open file: %s\n", file);
        return 0;
    }

    char line[MAX_LINE];
    int cnt = 0, cap = 100;
    *students = malloc(cap * sizeof(Student));

    fgets(line, MAX_LINE, fp);

    while (fgets(line, MAX_LINE, fp)) {
        if (cnt >= cap) {
            cap *= 2;
            *students = realloc(*students, cap * sizeof(Student));
        }

        sscanf(line, "%d,%[^,],%[^,],%d,%d,%d",
               &(*students)[cnt].id,
               (*students)[cnt].name,
               (*students)[cnt].gender,
               &(*students)[cnt].korean,
               &(*students)[cnt].english,
               &(*students)[cnt].math);

        (*students)[cnt].total = (*students)[cnt].korean +
                                 (*students)[cnt].english +
                                 (*students)[cnt].math;
        cnt++;
    }

    fclose(fp);
    return cnt;
}

// ============================================
// Run Sort
// ============================================
void run_sort(const char* name, Student* orig, int n,
              void (*sort)(Student*, int, int (*)(const void*, const void*)),
              int (*cmp)(const void*, const void*)) {

    printf("%-30s | ", name);
    fflush(stdout);

    double avg_comp = 0;
    clock_t start = clock();

    for (int iter = 0; iter < ITERATIONS; iter++) {
        Student* arr = malloc(n * sizeof(Student));
        memcpy(arr, orig, n * sizeof(Student));

        comp_count = 0;
        sort(arr, n, cmp);
        avg_comp += comp_count;

        free(arr);

        if ((iter + 1) % 25 == 0) {
            printf(".");
            fflush(stdout);
        }
    }

    avg_comp /= ITERATIONS;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf(" %6.2fs | %15.0f comparisons\n", elapsed, avg_comp);
}

// ============================================
// Main
// ============================================
int main() {
    Student* students = NULL;

    int n = read_csv("dataset_id_ascending.csv", &students);
    if (n == 0) {
        printf("Error: Please check CSV file!\n\n");
        return 1;
    }

    printf("Data size: %d\n", n);
    printf("Iterations: %d\n", ITERATIONS);
    printf("================================================================\n");

    printf("\n[ ID Ascending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_id_asc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_id_asc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_id_asc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_id_asc);
    run_sort("5. Timsort", students, n, timsort, cmp_id_asc);

    printf("\n[ ID Descending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_id_desc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_id_desc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_id_desc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_id_desc);
    run_sort("5. Timsort", students, n, timsort, cmp_id_desc);

    printf("\n[ Name Ascending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_name_asc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_name_asc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_name_asc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_name_asc);
    run_sort("5. Timsort", students, n, timsort, cmp_name_asc);

    printf("\n[ Name Descending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_name_desc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_name_desc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_name_desc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_name_desc);
    run_sort("5. Timsort", students, n, timsort, cmp_name_desc);

    printf("\n[ Grade Total Ascending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_grade_asc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_grade_asc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_grade_asc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_grade_asc);
    run_sort("5. Timsort", students, n, timsort, cmp_grade_asc);

    printf("\n[ Grade Total Descending ]\n");
    printf("----------------------------------------------------------------\n");
    run_sort("1. Shell Sort (Ciura)", students, n, shell_sort, cmp_grade_desc);
    run_sort("2. Dual-Pivot Quick Sort", students, n, dual_pivot_quick_sort, cmp_grade_desc);
    run_sort("3. Red-Black Tree Sort", students, n, rb_tree_sort, cmp_grade_desc);
    run_sort("4. AVL Tree Sort", students, n, avl_tree_sort, cmp_grade_desc);
    run_sort("5. Timsort", students, n, timsort, cmp_grade_desc);

    printf("\n================================================================\n");

    free(students);
    return 0;
}