#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100000
#define TRUE 1
#define FALSE 0

typedef struct {
    int data[MAX_SIZE];
    int size;
} UnsortedArray;

void unsorted_init(UnsortedArray* arr) {
    arr->size = 0;
}

int unsorted_insert(UnsortedArray* arr, int value) {
    if (arr->size >= MAX_SIZE) return -1;
    arr->data[arr->size++] = value;
    return 0;
}

int unsorted_search(UnsortedArray* arr, int target, int* found) {
    int comparisons = 0;
    *found = FALSE;
    for (int i = 0; i < arr->size; i++) {
        comparisons++;
        if (arr->data[i] == target) {
            *found = TRUE;
            return comparisons;
        }
    }
    return comparisons;
}

int unsorted_delete(UnsortedArray* arr, int target, int* deleted) {
    int comparisons = 0;
    *deleted = FALSE;
    for (int i = 0; i < arr->size; i++) {
        comparisons++;
        if (arr->data[i] == target) {
            arr->data[i] = arr->data[arr->size - 1];
            arr->size--;
            *deleted = TRUE;
            return comparisons;
        }
    }
    return comparisons;
}

typedef struct {
    int data[MAX_SIZE];
    int size;
} SortedArray;

void sorted_init(SortedArray* arr) {
    arr->size = 0;
}

int sorted_insert(SortedArray* arr, int value) {
    if (arr->size >= MAX_SIZE) return -1;

    int comparisons = 0;
    int lo = 0, hi = arr->size;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        comparisons++;
        if (arr->data[mid] < value) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    for (int i = arr->size; i > lo; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[lo] = value;
    arr->size++;

    return comparisons;
}

int sorted_search(SortedArray* arr, int target, int* found) {
    int comparisons = 0;
    *found = FALSE;
    int lo = 0, hi = arr->size;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        comparisons++;
        if (arr->data[mid] == target) {
            *found = TRUE;
            return comparisons;
        } else if (arr->data[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return comparisons;
}

int sorted_delete(SortedArray* arr, int target, int* deleted) {
    int comparisons = 0;
    *deleted = FALSE;
    int lo = 0, hi = arr->size;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        comparisons++;
        if (arr->data[mid] == target) {
            for (int i = mid; i < arr->size - 1; i++) {
                arr->data[i] = arr->data[i + 1];
            }
            arr->size--;
            *deleted = TRUE;
            return comparisons;
        } else if (arr->data[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return comparisons;
}

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct {
    AVLNode* root;
    long long insert_comparisons;
    long long delete_comparisons;
} AVLTree;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_height(AVLNode* node) {
    return node ? node->height : 0;
}

int get_balance(AVLNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

AVLNode* create_node(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

AVLNode* avl_insert_helper(AVLNode* node, int key, long long* comparisons) {
    if (!node) return create_node(key);

    (*comparisons)++;
    if (key < node->key) {
        node->left = avl_insert_helper(node->left, key, comparisons);
    } else if (key > node->key) {
        node->right = avl_insert_helper(node->right, key, comparisons);
    } else {
        return node;
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    if (balance > 1 && key < node->left->key) {
        (*comparisons)++;
        return rotate_right(node);
    }

    if (balance < -1 && key > node->right->key) {
        (*comparisons)++;
        return rotate_left(node);
    }

    if (balance > 1 && key > node->left->key) {
        (*comparisons) += 2;
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && key < node->right->key) {
        (*comparisons) += 2;
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void avl_insert(AVLTree* tree, int key) {
    tree->root = avl_insert_helper(tree->root, key, &tree->insert_comparisons);
}

int avl_search(AVLNode* root, int target, int* found) {
    int comparisons = 0;
    *found = FALSE;
    AVLNode* current = root;

    while (current) {
        comparisons++;
        if (target == current->key) {
            *found = TRUE;
            return comparisons;
        } else if (target < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return comparisons;
}

AVLNode* min_value_node(AVLNode* node) {
    AVLNode* current = node;
    while (current->left) {
        current = current->left;
    }
    return current;
}

AVLNode* avl_delete_helper(AVLNode* root, int key, long long* comparisons, int* deleted) {
    if (!root) return root;

    (*comparisons)++;
    if (key < root->key) {
        root->left = avl_delete_helper(root->left, key, comparisons, deleted);
    } else if (key > root->key) {
        root->right = avl_delete_helper(root->right, key, comparisons, deleted);
    } else {
        *deleted = TRUE;

        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            AVLNode* temp = min_value_node(root->right);
            root->key = temp->key;
            root->right = avl_delete_helper(root->right, temp->key, comparisons, deleted);
        }
    }

    if (!root) return root;

    root->height = 1 + max(get_height(root->left), get_height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0) {
        (*comparisons)++;
        return rotate_right(root);
    }

    if (balance > 1 && get_balance(root->left) < 0) {
        (*comparisons) += 2;
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) {
        (*comparisons)++;
        return rotate_left(root);
    }

    if (balance < -1 && get_balance(root->right) > 0) {
        (*comparisons) += 2;
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

void avl_delete(AVLTree* tree, int key, int* deleted) {
    tree->delete_comparisons = 0;
    tree->root = avl_delete_helper(tree->root, key, &tree->delete_comparisons, deleted);
}

void avl_init(AVLTree* tree) {
    tree->root = NULL;
    tree->insert_comparisons = 0;
    tree->delete_comparisons = 0;
}

void avl_free(AVLNode* node) {
    if (node) {
        avl_free(node->left);
        avl_free(node->right);
        free(node);
    }
}

int read_csv(const char* filename, int* data, int max_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return -1;
    }

    char buffer[256];
    int count = 0;

    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file) && count < max_size) {
        data[count++] = atoi(buffer);
    }

    fclose(file);
    return count;
}

int generate_non_existing_id(int* dataset, int data_count) {
    int candidate;
    int found;
    int max_attempts = 1000;

    int max_value = dataset[0];
    for (int i = 1; i < data_count; i++) {
        if (dataset[i] > max_value) {
            max_value = dataset[i];
        }
    }

    for (int attempt = 0; attempt < max_attempts; attempt++) {
        candidate = rand() % (max_value * 2) + 1;

        found = FALSE;
        for (int i = 0; i < data_count; i++) {
            if (dataset[i] == candidate) {
                found = TRUE;
                break;
            }
        }

        if (!found) {
            return candidate;
        }
    }

    return max_value + rand() % 100000 + 1;
}

void print_separator() {
    printf("================================================================================\n");
}

void print_results(const char* structure_name,
                  long long insert_comps,
                  int search_exist_comps,
                  int search_not_exist_comps,
                  int delete_exist_comps,
                  int delete_not_exist_comps) {
    printf("\n[%s]\n", structure_name);
    print_separator();
    printf("%-30s: %15lld 회\n", "전체 삽입 비교", insert_comps);
    printf("%-30s: %15d 회\n", "검색(존재) 비교", search_exist_comps);
    printf("%-30s: %15d 회\n", "검색(미존재) 비교", search_not_exist_comps);
    printf("%-30s: %15d 회\n", "삭제(존재) 비교", delete_exist_comps);
    printf("%-30s: %15d 회\n", "삭제(미존재) 비교", delete_not_exist_comps);
    print_separator();
}

int main() {
    clock_t start, end;
    double cpu_time;

    int* dataset = (int*)malloc(MAX_SIZE * sizeof(int));
    if (!dataset) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    printf("\n");
    print_separator();
    printf("    데이터 구조 성능 비교 프로그램\n");
    print_separator();

    int data_count = read_csv("dataset_id_ascending.csv", dataset, MAX_SIZE);
    if (data_count <= 0) {
        printf("데이터를 읽을 수 없습니다.\n");
        free(dataset);
        return 1;
    }

    printf("데이터 개수: %d\n", data_count);

    srand((unsigned int)time(NULL));

    int test_id_exist = dataset[rand() % data_count];
    int test_id_not_exist = generate_non_existing_id(dataset, data_count);

    printf("테스트 ID (존재): %d\n", test_id_exist);
    printf("테스트 ID (랜덤): %d\n", test_id_not_exist);
    print_separator();

    printf("\n[1] 비정렬 배열 처리 중...\n");
    start = clock();

    UnsortedArray unsorted;
    unsorted_init(&unsorted);

    long long unsorted_insert_comps = 0;
    for (int i = 0; i < data_count; i++) {
        unsorted_insert_comps += unsorted_insert(&unsorted, dataset[i]);
    }

    int found, deleted;
    int unsorted_search_exist = unsorted_search(&unsorted, test_id_exist, &found);
    int unsorted_search_not_exist = unsorted_search(&unsorted, test_id_not_exist, &found);
    int unsorted_delete_exist = unsorted_delete(&unsorted, test_id_exist, &deleted);
    int unsorted_delete_not_exist = unsorted_delete(&unsorted, test_id_not_exist, &deleted);

    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    print_results("비정렬 배열 (순차 탐색)",
                  unsorted_insert_comps,
                  unsorted_search_exist,
                  unsorted_search_not_exist,
                  unsorted_delete_exist,
                  unsorted_delete_not_exist);

    printf("\n[2] 정렬 배열 처리 중...\n");
    start = clock();

    SortedArray sorted;
    sorted_init(&sorted);

    long long sorted_insert_comps = 0;
    for (int i = 0; i < data_count; i++) {
        sorted_insert_comps += sorted_insert(&sorted, dataset[i]);
    }

    int sorted_search_exist = sorted_search(&sorted, test_id_exist, &found);
    int sorted_search_not_exist = sorted_search(&sorted, test_id_not_exist, &found);
    int sorted_delete_exist = sorted_delete(&sorted, test_id_exist, &deleted);
    int sorted_delete_not_exist = sorted_delete(&sorted, test_id_not_exist, &deleted);

    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    print_results("정렬 배열 (이진 탐색)",
                  sorted_insert_comps,
                  sorted_search_exist,
                  sorted_search_not_exist,
                  sorted_delete_exist,
                  sorted_delete_not_exist);

    printf("\n[3] AVL Tree 처리 중...\n");
    start = clock();

    AVLTree avl;
    avl_init(&avl);

    for (int i = 0; i < data_count; i++) {
        avl_insert(&avl, dataset[i]);
    }

    int avl_search_exist = avl_search(avl.root, test_id_exist, &found);
    int avl_search_not_exist = avl_search(avl.root, test_id_not_exist, &found);

    long long avl_delete_exist_comps = 0;
    avl_delete(&avl, test_id_exist, &deleted);
    avl_delete_exist_comps = avl.delete_comparisons;

    long long avl_delete_not_exist_comps = 0;
    avl_delete(&avl, test_id_not_exist, &deleted);
    avl_delete_not_exist_comps = avl.delete_comparisons;

    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    print_results("AVL Tree",
                  avl.insert_comparisons,
                  avl_search_exist,
                  avl_search_not_exist,
                  (int)avl_delete_exist_comps,
                  (int)avl_delete_not_exist_comps);

    avl_free(avl.root);
    free(dataset);

    return 0;
}