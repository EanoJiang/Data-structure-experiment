#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 10000
#define HASH_SIZE 10007  // 选择一个较大的质数作为哈希表大小

typedef int ElementType;

// 顺序表结构
typedef struct {
    ElementType data[MAX_SIZE];
    int length;
} SeqList;

// 二叉排序树节点结构
typedef struct BSTNode {
    ElementType data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode, *BSTree;

// 哈希表节点结构
typedef struct HashNode {
    ElementType data;
    struct HashNode *next;  // 拉链法解决冲突
} HashNode;

// 哈希表结构
typedef struct {
    HashNode *table[HASH_SIZE];
    int count;  // 记录哈希表中的元素数量
} HashTable;

// 初始化顺序表
void InitSeqList(SeqList *L) {
    L->length = 0;
}

// 从文件加载数据到顺序表
int LoadSeqListFromFile(SeqList *L, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    int value;
    while (fscanf(fp, "%d", &value) == 1 && L->length < MAX_SIZE) {
        L->data[L->length++] = value;
    }
    
    fclose(fp);
    return 1;
}
//静态表：顺序查找、二分查找

// 顺序查找并在未找到时插入
// 在顺序表中逐个比较元素，查找关键字key，若未找到则将key插入到表尾
// 参数：L - 顺序表结构指针，key - 要查找的关键字
// 返回值：若找到则返回元素下标，未找到但插入成功返回-插入位置-1，插入失败返回-1
// 时间复杂度：O(n)，n为表长
int SequentialSearchAndInsert(SeqList *L, ElementType key) {
    int i;
    // 先进行顺序查找
    for (i = 0; i < L->length; i++) {
        if (L->data[i] == key) {
            return i;  // 找到匹配元素，返回下标 (0或正数)
        }
    }
    
    // 未找到元素，尝试插入到表尾
    if (L->length < MAX_SIZE) {
        L->data[L->length] = key;
        int pos = L->length;
        L->length++;
        return -pos-1;  // 返回插入位置的负值减1，以区分查找成功的情况
    }
    
    return -1;  // 表已满，插入失败
}

// 对顺序表进行排序（用于二分查找前的准备）
//使用快速排序提高效率
void Swap(ElementType *a, ElementType *b) {
    ElementType temp = *a;
    *a = *b;
    *b = temp;
}

int Partition(ElementType arr[], int low, int high) {
    ElementType pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            Swap(&arr[i], &arr[j]);
        }
    }
    Swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void QuickSort(ElementType arr[], int low, int high) {
    if (low < high) {
        int pi = Partition(arr, low, high);
        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}

void SortSeqList(SeqList *L) {
    QuickSort(L->data, 0, L->length - 1);
}

// 将排序后的表内容保存到文件
int SaveSortedListToFile(SeqList L, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("无法创建文件 %s\n", filename);
        return 0;
    }
    
    for (int i = 0; i < L.length; i++) {
        fprintf(fp, "%d\n", L.data[i]);
    }
    
    fclose(fp);
    return 1;
}

// 二分查找并在未找到时插入
// 在已按升序排列的顺序表中使用二分策略查找关键字key，若未找到则将key插入适当位置
// 参数：L - 已排序的顺序表结构指针，key - 要查找的关键字
// 返回值：若找到则返回元素下标，未找到但插入成功返回-插入位置-1，插入失败返回-1
// 时间复杂度：O(log n) + O(n)，查找O(log n)，插入最坏需要O(n)移动元素
int BinarySearchAndInsert(SeqList *L, ElementType key) {
    int low = 0;
    int high = L->length - 1;
    int mid;
    int insert_pos = 0;
    
    // 二分查找过程
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (L->data[mid] == key) {
            return mid;  // 找到匹配元素，返回正数下标
        } else if (L->data[mid] > key) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    // 未找到元素，计算应该插入的位置
    insert_pos = low;  // low为应插入的位置
    
    // 检查表是否已满
    if (L->length >= MAX_SIZE) {
        return -1;  // 表已满，插入失败
    }
    
    // 从后向前移动元素，为新元素腾出位置
    for (int i = L->length; i > insert_pos; i--) {
        L->data[i] = L->data[i-1];
    }
    
    // 插入新元素
    L->data[insert_pos] = key;
    L->length++;
    
    return -insert_pos-1;  // 返回插入位置的负值减1，以区分查找成功
}

// 创建二叉排序树节点
BSTNode* CreateBSTNode(ElementType data) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 向二叉排序树中插入节点
BSTree InsertBST(BSTree T, ElementType key) {
    if (T == NULL) {
        // 创建新节点
        return CreateBSTNode(key);
    }
    
    if (key < T->data) {
        // 插入左子树
        T->left = InsertBST(T->left, key);
    } else if (key > T->data) {
        // 插入右子树
        T->right = InsertBST(T->right, key);
    }
    
    return T;
}

// 从文件构建二叉排序树
BSTree BuildBSTFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        return NULL;
    }
    
    BSTree root = NULL;
    int value;
    
    while (fscanf(fp, "%d", &value) == 1) {
        root = InsertBST(root, value);
    }
    
    fclose(fp);
    return root;
}

//动态表：二叉排序树查找
// 在二叉排序树中查找并在未找到时插入
// 参数：T - 二叉排序树根节点指针的指针，key - 要查找的关键字
// 返回值：1表示找到元素，0表示未找到但已插入新元素
int SearchBSTAndInsert(BSTree *T, ElementType key) {
    // 树为空或找到目标节点
    if (*T == NULL) {
        // 未找到，创建新节点并插入
        *T = CreateBSTNode(key);
        return 0;  // 表示插入了新节点
    }
    
    if (key == (*T)->data) {
        return 1;  // 找到匹配元素
    } else if (key < (*T)->data) {
        // 在左子树中继续查找
        return SearchBSTAndInsert(&((*T)->left), key);
    } else {
        // 在右子树中继续查找
        return SearchBSTAndInsert(&((*T)->right), key);
    }
}

// 释放二叉排序树
void FreeBST(BSTree T) {
    if (T != NULL) {
        FreeBST(T->left);
        FreeBST(T->right);
        free(T);
    }
}

// 统计二叉排序树高度
int BSTHeight(BSTree T) {
    if (T == NULL) {
        return 0;
    }
    
    int leftHeight = BSTHeight(T->left);
    int rightHeight = BSTHeight(T->right);
    
    //取较大值
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// 统计二叉排序树节点数
int BSTNodeCount(BSTree T) {
    if (T == NULL) {
        return 0;
    }
    
    return BSTNodeCount(T->left) + BSTNodeCount(T->right) + 1;
}

// 打印顺序表前几个元素和总数
void PrintSeqListSummary(SeqList L) {
    int i, display_count = 20;  // 只显示前20个元素
    
    printf("顺序表数据总数: %d\n", L.length);
    printf("前%d个数据: ", display_count < L.length ? display_count : L.length);
    
    for (i = 0; i < L.length && i < display_count; i++) {
        printf("%d ", L.data[i]);
    }
    printf("...\n");
}

// 哈希函数
int HashFunction(ElementType key) {
    return key % HASH_SIZE;
}

// 初始化哈希表
void InitHashTable(HashTable *H) {
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        H->table[i] = NULL;
    }
    H->count = 0;
}

// 在哈希表中插入元素
void InsertHash(HashTable *H, ElementType key) {
    int pos = HashFunction(key);
    
    // 创建新节点
    HashNode *newNode = (HashNode*)malloc(sizeof(HashNode));
    if (newNode == NULL) {
        printf("内存分配失败\n");
        return;
    }
    
    newNode->data = key;
    
    // 头插法插入链表
    newNode->next = H->table[pos];
    H->table[pos] = newNode;
    H->count++;
}

// 从文件构建哈希表
HashTable* BuildHashFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        return NULL;
    }
    
    HashTable *H = (HashTable*)malloc(sizeof(HashTable));
    if (H == NULL) {
        printf("内存分配失败\n");
        fclose(fp);
        return NULL;
    }
    
    InitHashTable(H);
    
    int value;
    while (fscanf(fp, "%d", &value) == 1) {
        InsertHash(H, value);
    }
    
    fclose(fp);
    return H;
}

// 在哈希表中查找并在未找到时插入
// 参数：H - 哈希表，key - 要查找的关键字
// 返回值：1表示找到元素，0表示未找到但已插入新元素
int SearchHashAndInsert(HashTable *H, ElementType key) {
    int pos = HashFunction(key);
    HashNode *p = H->table[pos];
    
    // 遍历链表查找
    while (p != NULL) {
        if (p->data == key) {
            return 1;  // 找到元素
        }
        p = p->next;
    }
    
    // 未找到元素，执行插入操作
    InsertHash(H, key);
    return 0;  // 表示插入了新元素
}

// 释放哈希表内存
void FreeHashTable(HashTable *H) {
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        HashNode *current = H->table[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
        H->table[i] = NULL;
    }
    free(H);
}

// 计算哈希表中的冲突次数和平均查找长度
void HashTableStats(HashTable *H, int *conflicts, float *asl) {
    int i;
    int total_conflicts = 0;
    int total_length = 0;
    int chains = 0;  // 实际有数据的链表数
    
    for (i = 0; i < HASH_SIZE; i++) {
        int chain_length = 0;
        HashNode *p = H->table[i];
        
        // 计算链长
        while (p != NULL) {
            chain_length++;
            p = p->next;
        }
        
        if (chain_length > 0) {
            chains++;
            if (chain_length > 1) {
                total_conflicts += (chain_length - 1);
            }
            total_length += chain_length;
        }
    }
    
    *conflicts = total_conflicts;
    // 平均查找长度计算
    if (chains > 0) {
        *asl = (float)total_length / chains;
    } else {
        *asl = 0.0;
    }
}

// 为保持兼容性，保留原始的搜索函数
// 在二叉排序树中查找
// 递归地在二叉排序树中查找关键字key
// 参数：T - 二叉排序树根节点指针，key - 要查找的关键字
// 返回值：若找到则返回目标节点指针，未找到返回NULL
// 时间复杂度：平均O(log n)，最坏O(n)，n为树中节点数
BSTNode* SearchBST(BSTree T, ElementType key) {
    // 递归终止条件：树为空（未找到）或找到关键字
    if (T == NULL || T->data == key) {
        return T;
    }
    
    // 二叉排序树的特性：小值在左，大值在右
    if (key < T->data) {
        return SearchBST(T->left, key);  // 目标值小于当前节点值，在左子树中查找
    } else {
        return SearchBST(T->right, key); // 目标值大于当前节点值，在右子树中查找
    }
}

int main() {
    SeqList L;
    InitSeqList(&L);
    
    printf("正在从data.txt加载数据...\n");
    
    // 加载数据
    if (!LoadSeqListFromFile(&L, "data.txt")) {
        printf("加载数据失败，创建测试数据...\n");
        // 如果文件不存在，创建一些测试数据
        int i;
        for (i = 0; i < 100; i++) {  
            L.data[L.length++] = rand() % 10000;
        }
        
        // 保存测试数据到文件
        FILE *fp = fopen("data.txt", "w");
        if (fp != NULL) {
            for (i = 0; i < L.length; i++) {
                fprintf(fp, "%d\n", L.data[i]);
            }
            fclose(fp);
            printf("已创建测试数据并保存到data.txt\n");
        }
    }
    
    // 打印顺序表内容摘要
    PrintSeqListSummary(L);
    
    // 1. 顺序查找
    int key = 0;
    printf("\n请输入要查找的关键字: ");
    scanf("%d", &key);
    
    clock_t start, end;
    double cpu_time_used;
    double seq_time, bin_time, bst_time, sort_time, hash_time;
    int seq_pos, bin_pos;
    BSTNode* bst_result;
    int hash_result;
    
    // 顺序查找测试
    start = clock();
    seq_pos = SequentialSearchAndInsert(&L, key);
    end = clock();
    seq_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    
    if (seq_pos >= 0) {
        printf("\n1. 顺序查找：找到关键字 %d，位置为 %d\n", key, seq_pos);
    } else if (seq_pos != -1) {
        // 插入成功，计算真实位置
        int insert_pos = -seq_pos - 1;
        printf("\n1. 顺序查找：未找到关键字 %d，已插入到位置 %d\n", key, insert_pos);
    } else {
        printf("\n1. 顺序查找：未找到关键字 %d，表已满无法插入\n", key);
    }
    printf("   顺序查找耗时: %.3f 毫秒\n", seq_time);
    
    // 2. 二分查找测试
    // 先排序
    printf("\n对数据进行排序中...\n");
    start = clock();
    SortSeqList(&L);
    end = clock();
    sort_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    printf("排序耗时: %.3f 毫秒\n", sort_time);
    
    // 将排序后的内容保存到文件
    if (SaveSortedListToFile(L, "sorted_data.txt")) {
        printf("排序后的数据已保存到 sorted_data.txt\n");
    } else {
        printf("保存排序后的数据失败\n");
    }
    
    start = clock();
    bin_pos = BinarySearchAndInsert(&L, key);
    end = clock();
    bin_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    
    if (bin_pos >= 0) {
        printf("\n2. 二分查找：找到关键字 %d，位置为 %d\n", key, bin_pos);
    } else if (bin_pos != -1) {
        // 插入成功，计算真实位置
        int insert_pos = -bin_pos - 1;
        printf("\n2. 二分查找：未找到关键字 %d，已插入到位置 %d\n", key, insert_pos);
    } else {
        printf("\n2. 二分查找：未找到关键字 %d，表已满无法插入\n", key);
    }
    printf("   二分查找耗时: %.3f 毫秒\n", bin_time);
    
    // 3. 二叉排序树查找
    // 构建二叉排序树
    printf("\n正在构建二叉排序树...\n");
    start = clock();
    BSTree bst = BuildBSTFromFile("data.txt");
    end = clock();
    double bst_build_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    printf("构建二叉排序树耗时: %.3f 毫秒\n", bst_build_time);
    
    int bst_height = BSTHeight(bst);
    int bst_node_count = BSTNodeCount(bst);
    printf("二叉排序树高度: %d, 节点数: %d\n", bst_height, bst_node_count);
    
    start = clock();
    int bst_insert_result = SearchBSTAndInsert(&bst, key);
    end = clock();
    bst_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    
    if (bst_insert_result == 1) {
        printf("\n3. 二叉排序树查找：找到关键字 %d\n", key);
    } else {
        printf("\n3. 二叉排序树查找：未找到关键字 %d，已成功插入\n", key);
        // 更新树的高度和节点数
        bst_height = BSTHeight(bst);
        bst_node_count = BSTNodeCount(bst);
        printf("   插入后二叉排序树高度: %d, 节点数: %d\n", bst_height, bst_node_count);
    }
    printf("   二叉排序树查找耗时: %.3f 毫秒\n", bst_time);
    
    // 释放二叉排序树
    FreeBST(bst);
    
    // 4. 哈希表查找
    printf("\n正在构建哈希表...\n");
    start = clock();
    HashTable *hashTable = BuildHashFromFile("data.txt");
    end = clock();
    double hash_build_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    printf("构建哈希表耗时: %.3f 毫秒\n", hash_build_time);
    
    // 计算冲突次数和平均查找长度
    int conflicts;
    float asl;
    HashTableStats(hashTable, &conflicts, &asl);
    printf("哈希表冲突次数: %d, 平均查找长度: %.2f\n", conflicts, asl);
    
    start = clock();
    int hash_insert_result = SearchHashAndInsert(hashTable, key);
    end = clock();
    hash_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // 转换为毫秒
    
    if (hash_insert_result == 1) {
        printf("\n4. 哈希表查找：找到关键字 %d\n", key);
    } else {
        printf("\n4. 哈希表查找：未找到关键字 %d，已成功插入\n", key);
        // 重新计算哈希表统计信息
        HashTableStats(hashTable, &conflicts, &asl);
        printf("   插入后哈希表冲突次数: %d, 平均查找长度: %.2f\n", conflicts, asl);
    }
    printf("   哈希表查找耗时: %.3f 毫秒\n", hash_time);
    
    // 释放哈希表
    FreeHashTable(hashTable);
    
    printf("\n性能分析：\n");
    printf("1. 顺序查找 - 时间复杂度O(n)，适合于数据量小的情况\n");
    printf("2. 二分查找 - 时间复杂度O(log n)，但需要先对数据排序\n");
    printf("3. 二叉排序树 - 平均查找时间复杂度O(log n)，最坏情况O(n)\n");
    printf("4. 哈希表查找 - 平均时间复杂度O(1)，最坏情况O(n)\n");
    
    // 将查找时间和性能分析保存到文件
    FILE *perf_file = fopen("性能分析.txt", "w");
    if (perf_file != NULL) {
        fprintf(perf_file, "查找算法性能测试报告\n");
        fprintf(perf_file, "==========================\n\n");
        fprintf(perf_file, "数据量: %d 个元素\n", L.length);
        fprintf(perf_file, "查找关键字: %d\n\n", key);
        
        fprintf(perf_file, "| 算法名称         | 运行时间 (毫秒) |\n");
        fprintf(perf_file, "|-----------------|---------------|\n");
        fprintf(perf_file, "| 顺序查找         | %13.3f |\n", seq_time);
        fprintf(perf_file, "| 二分查找         | %13.3f |\n", bin_time);
        fprintf(perf_file, "| 二叉排序树查找    | %13.3f |\n", bst_time);
        fprintf(perf_file, "| 哈希表查找       | %13.3f |\n", hash_time);
        fprintf(perf_file, "\n");
        
        fprintf(perf_file, "预处理时间:\n");
        fprintf(perf_file, "| 预处理步骤       | 运行时间 (毫秒) |\n");
        fprintf(perf_file, "|-----------------|---------------|\n");
        fprintf(perf_file, "| 排序操作         | %13.3f |\n", sort_time);
        fprintf(perf_file, "| 构建二叉排序树    | %13.3f |\n", bst_build_time);
        fprintf(perf_file, "| 构建哈希表       | %13.3f |\n\n", hash_build_time);
        
        fprintf(perf_file, "查找结果:\n");
        if (seq_pos >= 0) {
            fprintf(perf_file, "- 顺序查找：找到关键字 %d，位置为 %d\n", key, seq_pos);
        } else if (seq_pos != -1) {
            int insert_pos = -seq_pos - 1;
            fprintf(perf_file, "- 顺序查找：未找到关键字 %d，已插入到位置 %d\n", key, insert_pos);
        } else {
            fprintf(perf_file, "- 顺序查找：未找到关键字 %d，表已满无法插入\n", key);
        }
        
        if (bin_pos >= 0) {
            fprintf(perf_file, "- 二分查找：找到关键字 %d，位置为 %d\n", key, bin_pos);
        } else if (bin_pos != -1) {
            int insert_pos = -bin_pos - 1;
            fprintf(perf_file, "- 二分查找：未找到关键字 %d，已插入到位置 %d\n", key, insert_pos);
        } else {
            fprintf(perf_file, "- 二分查找：未找到关键字 %d，表已满无法插入\n", key);
        }
        
        if (bst_insert_result == 1) {
            fprintf(perf_file, "- 二叉排序树查找：找到关键字 %d\n", key);
        } else {
            fprintf(perf_file, "- 二叉排序树查找：未找到关键字 %d，已成功插入\n", key);
        }
        
        if (hash_insert_result == 1) {
            fprintf(perf_file, "- 哈希表查找：找到关键字 %d\n", key);
        } else {
            fprintf(perf_file, "- 哈希表查找：未找到关键字 %d，已成功插入\n", key);
        }
        
        fprintf(perf_file, "\n二叉排序树信息: 高度: %d, 节点数: %d\n", bst_height, bst_node_count);
        fprintf(perf_file, "哈希表信息: 冲突次数: %d, 平均查找长度: %.2f\n\n", conflicts, asl);
        
        fprintf(perf_file, "算法复杂度分析:\n");
        fprintf(perf_file, "- 顺序查找: O(n)\n");
        fprintf(perf_file, "- 二分查找: O(log n), 但需要先排序 O(n log n)\n");
        fprintf(perf_file, "- 二叉排序树查找: 平均O(log n), 最坏O(n)\n");
        fprintf(perf_file, "- 哈希表查找: 平均O(1), 最坏O(n)\n\n");
        
        fprintf(perf_file, "注：时间单位为毫秒。测试环境：%s。\n", "MacOS");
        fprintf(perf_file, "数据来源：data.txt（%d个数据点）\n", L.length);
        
        fclose(perf_file);
        printf("\n性能分析结果已保存到 性能分析.txt 文件\n");
    } else {
        printf("\n无法创建性能分析结果文件\n");
    }
    
    return 0;
} 