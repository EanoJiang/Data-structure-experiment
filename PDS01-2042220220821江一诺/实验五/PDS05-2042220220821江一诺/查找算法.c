#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 10000  // 增加容量以支持5000个数据

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

// 顺序查找
// 在顺序表中逐个比较元素，查找关键字key
// 参数：L - 顺序表结构，key - 要查找的关键字
// 返回值：若找到则返回元素下标，未找到返回-1
// 时间复杂度：O(n)，n为表长
int SequentialSearch(SeqList L, ElementType key) {
    int i;
    for (i = 0; i < L.length; i++) {  // 从表头开始，逐个元素进行比较
        if (L.data[i] == key) {       // 找到匹配的元素
            return i;  // 返回元素位置（下标）
        }
    }
    return -1;  // 遍历完整个表未找到，返回-1表示失败
}

// 对顺序表进行排序（用于二分查找前的准备）- 使用快速排序提高效率
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

// 二分查找
// 在已按升序排列的顺序表中使用二分策略查找关键字key
// 参数：L - 已排序的顺序表结构，key - 要查找的关键字
// 返回值：若找到则返回元素下标，未找到返回-1
// 时间复杂度：O(log n)，n为表长
int BinarySearch(SeqList L, ElementType key) {
    int low = 0;                // 初始化查找区间的下界
    int high = L.length - 1;    // 初始化查找区间的上界
    int mid;
    
    while (low <= high) {       // 当查找区间有效时继续查找
        mid = low + (high - low) / 2;  // 计算中间位置（避免整数溢出）
        if (L.data[mid] == key) {
            return mid;          // 找到元素，返回位置
        } else if (L.data[mid] > key) {
            high = mid - 1;      // 中间值大于目标值，在左半区间查找
        } else {
            low = mid + 1;       // 中间值小于目标值，在右半区间查找
        }
    }
    
    return -1;  // 查找区间缩小到空仍未找到，返回-1表示失败
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
    // 如果key已存在，不做任何操作
    
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

// 中序遍历打印二叉排序树的部分内容
void InOrderTraversalPartial(BSTree T, int *count, int max_display) {
    if (T != NULL && *count < max_display) {
        InOrderTraversalPartial(T->left, count, max_display);
        if (*count < max_display) {
            printf("%d ", T->data);
            (*count)++;
        }
        InOrderTraversalPartial(T->right, count, max_display);
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
        for (i = 0; i < 100; i++) {  // 创建100个数据作为示例
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
    double seq_time, bin_time, bst_time, sort_time;
    int seq_pos, bin_pos;
    BSTNode* bst_result;
    
    // 顺序查找测试
    start = clock();
    seq_pos = SequentialSearch(L, key);
    end = clock();
    seq_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    if (seq_pos != -1) {
        printf("\n1. 顺序查找：找到关键字 %d，位置为 %d\n", key, seq_pos);
    } else {
        printf("\n1. 顺序查找：未找到关键字 %d\n", key);
    }
    printf("   顺序查找耗时: %f 秒\n", seq_time);
    
    // 2. 二分查找测试
    // 先排序
    printf("\n对数据进行排序中...\n");
    start = clock();
    SortSeqList(&L);
    end = clock();
    sort_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("排序耗时: %f 秒\n", sort_time);
    
    // 将排序后的内容保存到文件
    if (SaveSortedListToFile(L, "sorted_data.txt")) {
        printf("排序后的数据已保存到 sorted_data.txt\n");
    } else {
        printf("保存排序后的数据失败\n");
    }
    
    start = clock();
    bin_pos = BinarySearch(L, key);
    end = clock();
    bin_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    if (bin_pos != -1) {
        printf("\n2. 二分查找：找到关键字 %d，位置为 %d\n", key, bin_pos);
    } else {
        printf("\n2. 二分查找：未找到关键字 %d\n", key);
    }
    printf("   二分查找耗时: %f 秒\n", bin_time);
    
    // 3. 二叉排序树查找
    // 构建二叉排序树
    printf("\n正在构建二叉排序树...\n");
    start = clock();
    BSTree bst = BuildBSTFromFile("data.txt");
    end = clock();
    double bst_build_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("构建二叉排序树耗时: %f 秒\n", bst_build_time);
    
    int bst_height = BSTHeight(bst);
    int bst_node_count = BSTNodeCount(bst);
    printf("二叉排序树高度: %d, 节点数: %d\n", bst_height, bst_node_count);
    
    start = clock();
    bst_result = SearchBST(bst, key);
    end = clock();
    bst_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    if (bst_result != NULL) {
        printf("\n3. 二叉排序树查找：找到关键字 %d\n", key);
    } else {
        printf("\n3. 二叉排序树查找：未找到关键字 %d\n", key);
    }
    printf("   二叉排序树查找耗时: %f 秒\n", bst_time);
    
    // 释放二叉排序树
    FreeBST(bst);
    
    printf("\n性能分析：\n");
    printf("1. 顺序查找 - 时间复杂度O(n)，适合于数据量小的情况\n");
    printf("2. 二分查找 - 时间复杂度O(log n)，但需要先对数据排序\n");
    printf("3. 二叉排序树 - 平均查找时间复杂度O(log n)，最坏情况O(n)\n");
    
    // 将查找时间和性能分析保存到文件
    FILE *perf_file = fopen("性能分析.txt", "w");
    if (perf_file != NULL) {
        fprintf(perf_file, "查找算法性能测试报告\n");
        fprintf(perf_file, "==========================\n\n");
        fprintf(perf_file, "数据量: %d 个元素\n", L.length);
        fprintf(perf_file, "查找关键字: %d\n\n", key);
        
        fprintf(perf_file, "| 算法名称         | 运行时间 (秒) |\n");
        fprintf(perf_file, "|-----------------|---------------|\n");
        fprintf(perf_file, "| 顺序查找         | %13.6f |\n", seq_time);
        fprintf(perf_file, "| 二分查找         | %13.6f |\n", bin_time);
        fprintf(perf_file, "| 二叉排序树查找    | %13.6f |\n", bst_time);
        fprintf(perf_file, "\n");
        
        fprintf(perf_file, "预处理时间:\n");
        fprintf(perf_file, "| 预处理步骤       | 运行时间 (秒) |\n");
        fprintf(perf_file, "|-----------------|---------------|\n");
        fprintf(perf_file, "| 排序操作         | %13.6f |\n", sort_time);
        fprintf(perf_file, "| 构建二叉排序树    | %13.6f |\n\n", bst_build_time);
        
        fprintf(perf_file, "查找结果:\n");
        if (seq_pos != -1) {
            fprintf(perf_file, "- 顺序查找：找到关键字 %d，位置为 %d\n", key, seq_pos);
        } else {
            fprintf(perf_file, "- 顺序查找：未找到关键字 %d\n", key);
        }
        
        if (bin_pos != -1) {
            fprintf(perf_file, "- 二分查找：找到关键字 %d，位置为 %d\n", key, bin_pos);
        } else {
            fprintf(perf_file, "- 二分查找：未找到关键字 %d\n", key);
        }
        
        if (bst_result != NULL) {
            fprintf(perf_file, "- 二叉排序树查找：找到关键字 %d\n", key);
        } else {
            fprintf(perf_file, "- 二叉排序树查找：未找到关键字 %d\n", key);
        }
        
        fprintf(perf_file, "\n二叉排序树信息: 高度: %d, 节点数: %d\n\n", bst_height, bst_node_count);
        
        fprintf(perf_file, "算法复杂度分析:\n");
        fprintf(perf_file, "- 顺序查找: O(n)\n");
        fprintf(perf_file, "- 二分查找: O(log n), 但需要先排序 O(n log n)\n");
        fprintf(perf_file, "- 二叉排序树查找: 平均O(log n), 最坏O(n)\n\n");
        
        fprintf(perf_file, "注：时间单位为秒。测试环境：%s。\n", "MacOS");
        fprintf(perf_file, "数据来源：data.txt（%d个数据点）\n", L.length);
        
        fclose(perf_file);
        printf("\n性能分析结果已保存到 性能分析.txt 文件\n");
    } else {
        printf("\n无法创建性能分析结果文件\n");
    }
    
    return 0;
} 