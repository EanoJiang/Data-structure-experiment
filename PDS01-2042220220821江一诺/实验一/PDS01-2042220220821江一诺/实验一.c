#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义数据类型
typedef float ElemType;

// 三维信号节点结构  
typedef struct Node {
    ElemType x, y, z;   // 三维坐标
    struct Node *prev;  // 前驱指针
    struct Node *next;  // 后继指针
} Node;

// 带虚拟头尾节点的双向链表
typedef struct {
    Node *dummyHead;
    Node *dummyTail;
    int size;
} SignalList;

/* 初始化链表 */
void init(SignalList *list) {
    list->dummyHead = (Node*)malloc(sizeof(Node));//动态分配内存
    list->dummyTail = (Node*)malloc(sizeof(Node));
    
    list->dummyHead->prev = NULL;
    list->dummyHead->next = list->dummyTail;
    list->dummyTail->prev = list->dummyHead;
    list->dummyTail->next = NULL;
    list->size = 0;
}

/* 判断链表是否为空 */
bool isEmpty(SignalList *list) {
    return list->size == 0;
}

/* 创建新节点 */
Node* createNode(ElemType x, ElemType y, ElemType z) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    node->z = z;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* 在指定位置插入节点 */
void insert(SignalList *list, int index, ElemType x, ElemType y, ElemType z) {
    if(index < 0 || index > list->size) return;

    Node *prev = list->dummyHead;
    for(int i=0; i<index; i++)
        prev = prev->next;

    Node *newNode = createNode(x, y, z);



    // 双向指针连接
    newNode->next = prev->next;
    newNode->prev = prev;
    prev->next->prev = newNode;
    prev->next = newNode;
    list->size++;
}

/* 删除指定位置节点 */
void removeAt(SignalList *list, int index) {
    if(index <0 || index >= list->size) return;

    Node *prev = list->dummyHead;
    for(int i=0; i<index; i++)
        prev = prev->next;

    Node *toDelete = prev->next;
    // 调整链表连接
    prev->next = toDelete->next;
    toDelete->next->prev = prev;
    
    free(toDelete);
    list->size--;
}

/* 获取节点数据 */
void get(SignalList *list, int index, ElemType *x, ElemType *y, ElemType *z) {
    if(index <0 || index >= list->size) return;

    Node *current = list->dummyHead->next;
    for(int i=0; i<index; i++)
        current = current->next;
    
    *x = current->x;
    *y = current->y;
    *z = current->z;
}

/* 均值滤波（窗口大小3）*/
void smooth(SignalList *list) {
    if(list->size < 3) return;

    Node *current = list->dummyHead->next->next; // 从第二个节点开始
    //边界条件：跳过第一个和最后一个
    for(int i=1; i<list->size-1; i++) {
        Node *prev = current->prev;
        Node *next = current->next;
        // 三维坐标分别求平均
        current->x = (prev->x + current->x + next->x)/3;
        current->y = (prev->y + current->y + next->y)/3;
        current->z = (prev->z + current->z + next->z)/3;
        current = current->next;
    }
}

/* 上采样（线性插值）*/
void upsample(SignalList *list) {
    Node *current = list->dummyHead->next;
    while(current != list->dummyTail && current->next != list->dummyTail) {
        Node *next = current->next;
        // 在两个节点间插入插值节点
        insert(list, current->next->prev->x+1, 
              (current->x + next->x)/2,
              (current->y + next->y)/2,
              (current->z + next->z)/2);
        current = next;
    }
}

/* 下采样（隔点采样）*/
void downsample(SignalList *list) {
    int count = 0;
    Node *current = list->dummyHead->next;
    while(current != list->dummyTail) {
        Node *next = current->next;
        if(count % 2 == 1) {
            // 删除奇数位置节点
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            list->size--;
        }
        count++;
        current = next;
    }
}

/* 打印信号 */
void printList(SignalList *list) {
    Node *current = list->dummyHead->next;
    printf("Signal List (%d points):\n", list->size);
    int index = 1;
    while(current != list->dummyTail) {
        printf("└─ %d\n", index);
        printf("    └─ %.2f\n", current->x);
        printf("    └─ %.2f\n", current->y);
        printf("    └─ %.2f\n", current->z);
        current = current->next;
        index++;
    }
}

/* 从文件读取信号数据 */
bool loadFromFile(SignalList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return false;
    }
    
    // 清空现有列表
    while (!isEmpty(list)) {
        removeAt(list, 0);
    }
    
    char line[100];
    int index;
    ElemType x, y, z;
    
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "└─ %d", &index) == 1) {
            // 读取坐标
            fgets(line, sizeof(line), file);
            sscanf(line, "    └─ %f", &x);
            
            fgets(line, sizeof(line), file);
            sscanf(line, "    └─ %f", &y);
            
            fgets(line, sizeof(line), file);
            sscanf(line, "    └─ %f", &z);
            
            // 添加到列表末尾
            insert(list, list->size, x, y, z);
        }
    }
    
    fclose(file);
    return true;
}

/* 保存信号数据到文件 */
bool saveToFile(SignalList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建文件 %s\n", filename);
        return false;
    }
    
    Node *current = list->dummyHead->next;
    int index = 1;
    
    while (current != list->dummyTail) {
        fprintf(file, "└─ %d\n", index);
        fprintf(file, "    └─ %.2f\n", current->x);
        fprintf(file, "    └─ %.2f\n", current->y);
        fprintf(file, "    └─ %.2f\n", current->z);
        current = current->next;
        index++;
    }
    
    fclose(file);
    return true;
}


int main() {
    SignalList signal;
    init(&signal);

    // 从文件加载信号数据
    if (loadFromFile(&signal, "data.txt")) {
        printf("\n######从文件加载的原始信号######\n");
        printList(&signal);
        
        // 均值滤波
        smooth(&signal);
        printf("\n######滤波后######\n");
        printList(&signal);

        
        // 保存处理后的数据
        saveToFile(&signal, "filtered_signal.txt");
        printf("\n滤波后的数据已保存到 filtered_signal.txt\n");

        // 上采样
        upsample(&signal);
        printf("\n##########上采样后##########\n");
        printList(&signal);
        
        // 保存上采样后的数据
        saveToFile(&signal, "upsampled_signal.txt");
        printf("\n上采样后的数据已保存到 upsampled_signal.txt\n");

        // 下采样
        downsample(&signal);
        printf("\n##########下采样后###########\n");
        printList(&signal);
        
        // 保存下采样后的数据
        saveToFile(&signal, "downsampled_signal.txt");
        printf("\n下采样后的数据已保存到 downsampled_signal.txt\n");
    } 

    //插入新节点
    insert(&signal, 3, 1.0, 2.0, 3.0);
    printf("\n########插入新节点后#######\n");
    printList(&signal);
    printf("是否为空:\t");
    isEmpty(&signal)? printf("为空\n") : printf("不为空\n");

    //删除所有节点
    while (!isEmpty(&signal)) {
        removeAt(&signal, 0);
    }
    printf("\n######清空后的信号######\n");
    printList(&signal);
    printf("是否为空:\t");
    isEmpty(&signal)? printf("为空\n") : printf("不为空\n");
    
    // 释放资源
    free(signal.dummyHead);
    free(signal.dummyTail);

    return 0;
}

