#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef float ElemType;

// 多维信号节点结构
typedef struct SignalNode {
    ElemType value;                // 节点存储的数值
    struct SignalNode** children;  // 子节点指针数组
    int childCount;                // 子节点数量
    int childCapacity;             // 子节点数组容量
} SignalNode;

// 创建新节点
SignalNode* createNode(ElemType value) {
    SignalNode* node = (SignalNode*)malloc(sizeof(SignalNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->value = value;
    node->childCount = 0;
    node->childCapacity = 4;  // 初始容量为4
    node->children = (SignalNode**)malloc(node->childCapacity * sizeof(SignalNode*));
    
    if (node->children == NULL) {
        free(node);
        return NULL;
    }
    
    return node;
}

// 释放节点及其所有子节点
void freeNode(SignalNode* node) {
    if (node == NULL) {
        return;
    }
    
    // 递归释放所有子节点
    for (int i = 0; i < node->childCount; i++) {
        freeNode(node->children[i]);
    }
    
    // 释放子节点数组和节点本身
    free(node->children);
    free(node);
}

// 初始化多维信号（返回根节点）
SignalNode* initList(ElemType rootValue) {
    return createNode(rootValue);
}

// 判断信号是否为空
bool isEmpty(const SignalNode* root) {
    return root == NULL;
}

// 计算信号中的元素总数
int getElemCount(const SignalNode* root) {
    if (root == NULL) {
        return 0;
    }
    
    // 使用队列进行层次遍历
    typedef struct QueueNode {
        const SignalNode* data;
        struct QueueNode* next;
    } QueueNode;
    
    QueueNode* front = NULL;
    QueueNode* rear = NULL;
    int count = 0;
    
    // 将根节点入队
    QueueNode* rootQNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (rootQNode == NULL) {
        return -1;  // 内存分配失败
    }
    rootQNode->data = root;
    rootQNode->next = NULL;
    front = rear = rootQNode;
    
    // 层次遍历
    while (front != NULL) {
        // 出队
        QueueNode* current = front;
        const SignalNode* node = current->data;
        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }
        
        count++;  // 计数
        
        // 将所有子节点入队
        for (int i = 0; i < node->childCount; i++) {
            QueueNode* childQNode = (QueueNode*)malloc(sizeof(QueueNode));
            if (childQNode == NULL) {
                // 内存分配失败，清理队列并返回错误
                while (front != NULL) {
                    current = front;
                    front = front->next;
                    free(current);
                }
                free(current);
                return -1;
            }
            
            childQNode->data = node->children[i];
            childQNode->next = NULL;
            
            if (rear == NULL) {
                front = rear = childQNode;
            } else {
                rear->next = childQNode;
                rear = childQNode;
            }
        }
        
        free(current);
    }
    
    return count;
}

// 在节点中添加子节点
bool addChildToNode(SignalNode* node, ElemType value) {
    if (node == NULL) {
        return false;
    }
    
    // 检查是否需要扩容
    if (node->childCount >= node->childCapacity) {
        int newCapacity = node->childCapacity * 2;
        SignalNode** newChildren = (SignalNode**)realloc(node->children, 
                                   newCapacity * sizeof(SignalNode*));
        if (newChildren == NULL) {
            return false;  // 内存分配失败
        }
        
        node->children = newChildren;
        node->childCapacity = newCapacity;
    }
    
    // 创建新节点
    SignalNode* newNode = createNode(value);
    if (newNode == NULL) {
        return false;
    }
    
    // 添加到子节点数组
    node->children[node->childCount++] = newNode;
    return true;
}

// 在指定路径的节点下插入新节点
/*
root: 根节点
path: 节点路径（从根节点开始，逐级向下）
pathLength: 路径下的子节点的数量
value: 要插入的元素
*/
bool insertNode(SignalNode* root, const int* path, int pathLength, ElemType value) {
    if (root == NULL) {
        return false;
    }
    
    SignalNode* current = root;
    
    // 遍历路径找到目标节点
    for (int i = 0; i < pathLength; i++) {
        int index = path[i];
        if (index < 0 || index >= current->childCount) {
            return false;  // 路径无效
        }
        current = current->children[index];
    }
    
    // 在目标节点下添加新节点
    return addChildToNode(current, value);
}

// 删除指定路径的节点及其所有子节点，返回新的根节点（如果根节点被删除）
SignalNode* deleteNode(SignalNode* root, const int* path, int pathLength) {
    if (root == NULL) {
        return NULL;
    }
    
    // 如果要删除根节点
    if (pathLength == 0) {
        freeNode(root);
        return NULL;
    }
    
    SignalNode* parent = root;
    
    // 找到要删除节点的父节点
    for (int i = 0; i < pathLength - 1; i++) {
        int index = path[i];
        if (index < 0 || index >= parent->childCount) {
            return root;  // 路径无效，返回原根节点
        }
        parent = parent->children[index];
    }
    
    // 获取要删除的节点索引
    int deleteIndex = path[pathLength - 1];
    if (deleteIndex < 0 || deleteIndex >= parent->childCount) {
        return root;  // 路径无效，返回原根节点
    }
    
    // 删除节点
    SignalNode* nodeToDelete = parent->children[deleteIndex];
    
    // 将后面的节点前移
    for (int i = deleteIndex; i < parent->childCount - 1; i++) {
        parent->children[i] = parent->children[i + 1];
    }
    
    parent->childCount--;
    freeNode(nodeToDelete);
    
    return root;
}

// 获取指定路径节点的值
bool getValue(const SignalNode* root, const int* path, int pathLength, ElemType* result) {
    if (root == NULL) {
        return false;
    }
    
    const SignalNode* current = root;
    
    // 如果要获取根节点的值
    if (pathLength == 0) {
        *result = current->value;
        return true;
    }
    
    // 遍历路径找到目标节点
    for (int i = 0; i < pathLength; i++) {
        int index = path[i];
        if (index < 0 || index >= current->childCount) {
            return false;  // 路径无效
        }
        current = current->children[index];
    }
    
    *result = current->value;
    return true;
}

// 设置指定路径节点的值
bool setValue(SignalNode* root, const int* path, int pathLength, ElemType value) {
    if (root == NULL) {
        return false;
    }
    
    SignalNode* current = root;
    
    // 如果要设置根节点的值
    if (pathLength == 0) {
        current->value = value;
        return true;
    }
    
    // 遍历路径找到目标节点
    for (int i = 0; i < pathLength; i++) {
        int index = path[i];
        if (index < 0 || index >= current->childCount) {
            return false;  // 路径无效
        }
        current = current->children[index];
    }
    
    current->value = value;
    return true;
}

// 递归打印节点及其子节点
void printNode(const SignalNode* node, int depth) {
    if (node == NULL) return;
    
    // 打印缩进
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    // 打印节点值
    printf("└─ %.2f\n", node->value);
    
    // 打印子节点
    for (int i = 0; i < node->childCount; i++) {
        printNode(node->children[i], depth + 1);
    }
}

// 打印整个信号树（用于调试）
void printSignalTree(const SignalNode* root) {
    if (root == NULL) {
        printf("信号树为空\n");
        return;
    }
    
    printf("信号树结构：\n");
    printNode(root, 0);
}


/*均值滤波器*/
#pragma region 均值滤波器{
void meanFilterSameLevel(SignalNode* root) {
    //边界1：根节点为空 或者 根节点的子节点数小于3
    if (root == NULL || root->childCount < 3) {
        return; // 至少需要3个节点才能进行滤波
    }
    
    // 检查子节点是否都有子节点
    int maxGrandchildCount = 0;
    for (int i = 0; i < root->childCount; i++) {
        if (root->children[i]->childCount > maxGrandchildCount) {
            maxGrandchildCount = root->children[i]->childCount;
        }
    }
    
    //边界2：没有孙子节点
    if (maxGrandchildCount == 0) {
        return; // 没有孙子节点，就不用滤波
    }
    
    // 对每个位置的孙子节点进行滤波
    for (int pos = 0; pos < maxGrandchildCount; pos++) {
        // 检查每个子节点在此位置是否有孙子节点
        int validCount = 0;
        for (int i = 0; i < root->childCount; i++) {
            if (pos < root->children[i]->childCount) {
                validCount++;
            }
        }
        
        //边界3：有效节点数小于3
        if (validCount >= 3) { // 至少需要3个有效节点才能滤波
            // 保存原始值
            float* originalValues = (float*)malloc(root->childCount * sizeof(float));
            //原始值为空，返回
            if (originalValues == NULL) {
                return; 
            }
            
            // 收集原始值
            for (int i = 0; i < root->childCount; i++) {
                if (pos < root->children[i]->childCount) {
                    originalValues[i] = root->children[i]->children[pos]->value;
                } else {
                    originalValues[i] = 0.0f; // 对于没有此位置孙子节点的子节点，设为0
                }
            }
            
            // 对中间节点应用均值滤波（跳过第一个和最后一个）
            for (int i = 1; i < root->childCount - 1; i++) {
                if (pos < root->children[i]->childCount) {
                    float sum = originalValues[i-1] + originalValues[i] + originalValues[i+1];
                    root->children[i]->children[pos]->value = sum / 3.0f;
                }
            }
            
            free(originalValues);
        }
    }
    
    // 递归处理每个子节点
    for (int i = 0; i < root->childCount; i++) {
        meanFilterSameLevel(root->children[i]);
    }
}

// 对指定路径节点的子节点进行均值滤波
bool applyMeanFilterAtPath(SignalNode* root, const int* path, int pathLength) {
    if (root == NULL) {
        return false;
    }
    
    SignalNode* current = root;
    
    // 遍历路径找到目标节点
    for (int i = 0; i < pathLength; i++) {
        int index = path[i];
        if (index < 0 || index >= current->childCount) {
            return false; // 路径无效
        }
        current = current->children[index];
    }
    
    // 对目标节点的子节点进行均值滤波
    meanFilterSameLevel(current);
    return true;
}

#pragma endregion}

/*上采样滤波器*/
#pragma region 上采样滤波器{
// 创建两个节点之间的插值节点
SignalNode* createInterpolatedNode(SignalNode* node1, SignalNode* node2) {
    //边界1：两个节点都为空
    if (node1 == NULL || node2 == NULL) {
        return NULL;
    }
    
    // 创建新的父节点，值为两个节点值的均值
    ElemType newValue = (node1->value + node2->value) / 2.0f;
    SignalNode* newNode = createNode(newValue);
    
    //边界2：新的父节点为空
    if (newNode == NULL) {
        return NULL;
    }
    
    // 子节点的最大数量
    int maxChildCount = (node1->childCount > node2->childCount) ? 
                         node1->childCount : node2->childCount;
    
    // 为每对对应的子节点创建插值子节点
    for (int i = 0; i < maxChildCount; i++) {
        if (i < node1->childCount && i < node2->childCount) {
            // 两个节点都有对应位置的子节点
            ElemType value1 = node1->children[i]->value;
            ElemType value2 = node2->children[i]->value;
            
            // 计算均值
            ElemType avgValue = (value1 + value2) / 2.0f;
            //添加为新节点的子节点
            addChildToNode(newNode, avgValue);
        } 
        else if (i < node1->childCount) {
            // 只有第一个节点有这个位置的子节点
            addChildToNode(newNode, node1->children[i]->value);
        } 
        else {
            // 只有第二个节点有这个位置的子节点
            addChildToNode(newNode, node2->children[i]->value);
        }
    }
    
    return newNode;
}

// 对信号树进行上采样，增加节点数目为原来的2倍
SignalNode* upsample(SignalNode* root) {
    //边界1：根节点为空
    if (root == NULL) {
        return NULL;
    }
    
    // 处理子节点
    int originalChildCount = root->childCount;
    //边界3 ：只有一个子节点，直接返回根节点
    if (originalChildCount <= 1) {
        return root;
    }
    
    // 为新的子节点分配空间（原来的两倍减1，因为n个节点之间有n-1个间隔）
    int newChildCount = originalChildCount * 2 - 1;
    SignalNode** newChildren = (SignalNode**)malloc(newChildCount * sizeof(SignalNode*));
    //边界4:新的子节点为空
    if (newChildren == NULL) {
        return root; // 内存分配失败，返回原始树
    }
    
    // 遍历复制原始子节点,然后在它们之间插入新节点
    for (int i = 0; i < originalChildCount; i++) {
        // 复制原始子节点
        newChildren[i*2] = root->children[i];
        
        // 在相邻节点之间插入新节点（除了最后一对节点）
        if (i < originalChildCount - 1) {
            // 创建相邻两个节点的插值节点
            newChildren[i*2 + 1] = createInterpolatedNode(
                root->children[i], root->children[i+1]);
        }
    }
    
    // 更新根节点的子节点数组和计数
    free(root->children);
    root->children = newChildren;
    root->childCount = newChildCount;
    
    return root;
}
#pragma endregion}


// 从文本文件读取信号树
SignalNode* readSignalTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return NULL;
    }
    
    // 创建根节点（默认值为0，后面会更新）
    SignalNode* root = createNode(0.0f);
    if (root == NULL) {
        fclose(file);
        return NULL;
    }
    
    char line[256];
    int lineNum = 0;
    
    // 读取第一行（根节点）
    if (fgets(line, sizeof(line), file)) {
        // 跳过"信号树结构："这样的标题行
        if (strstr(line, "信号树结构") != NULL || strstr(line, "└─") == NULL) {
            if (!fgets(line, sizeof(line), file)) {
                fclose(file);
                freeNode(root);
                return NULL;
            }
        }
        
        // 提取根节点的值
        float value;
        if (strstr(line, "└─") && sscanf(strstr(line, "└─") + 3, "%f", &value) == 1) {
            root->value = value;
        }
    } else {
        fclose(file);
        freeNode(root);
        return NULL;
    }
    
    // 用于跟踪当前路径的数组
    int path[100] = {0};  // 假设最大深度为100
    int pathLen = 0;
    int prevDepth = 0;
    
    // 读取剩余行
    while (fgets(line, sizeof(line), file)) {
        // 计算当前行的深度（缩进级别）
        int depth = 0;
        int i = 0;
        while (line[i] == ' ') {
            depth++;
            i++;
        }
        depth = depth / 2;  // 每两个空格表示一级深度
        
        // 跳过非数据行
        if (strstr(line, "└─") == NULL) {
            continue;
        }
        
        // 提取数值
        float value;
        if (sscanf(strstr(line, "└─") + 3, "%f", &value) != 1) {
            continue;  // 如果无法解析数值，跳过该行
        }
        
        // 根据深度调整路径
        if (depth > prevDepth) {
            // 深度增加，进入子节点
            pathLen = depth - 1;
        } else if (depth < prevDepth) {
            // 深度减少，回到上层
            pathLen = depth - 1;
        } else {
            // 同级，增加索引
            if (pathLen >= 0) {
                path[pathLen]++;
            }
        }
        
        // 插入节点
        insertNode(root, path, pathLen, value);
        prevDepth = depth;
    }
    
    fclose(file);
    return root;
}

int main() {
    // 创建一个示例信号树并保存到文件
    SignalNode* exampleTree = initList(0.0f);
    
    // 添加第一级子节点
    addChildToNode(exampleTree, 1.0f);
    addChildToNode(exampleTree, 2.0f);
    addChildToNode(exampleTree, 3.0f);
    
    // 添加第二级子节点
    addChildToNode(exampleTree->children[0], 1.0f);
    addChildToNode(exampleTree->children[0], 2.0f);
    addChildToNode(exampleTree->children[0], 3.0f);
    
    addChildToNode(exampleTree->children[1], 2.0f);
    addChildToNode(exampleTree->children[1], 1.0f);
    addChildToNode(exampleTree->children[1], 10.0f);
    
    addChildToNode(exampleTree->children[2], -5.0f);
    addChildToNode(exampleTree->children[2], 2.0f);
    addChildToNode(exampleTree->children[2], 8.0f);

    // 打印示例树并保存到文件
    FILE* file = fopen("data.txt", "w");
    if (file != NULL) {
        fprintf(file, "信号树结构：\n");
        // 手动写入树结构到文件
        fprintf(file, "└─ %.2f\n", exampleTree->value);
        for (int i = 0; i < exampleTree->childCount; i++) {
            fprintf(file, "  └─ %.2f\n", exampleTree->children[i]->value);
            for (int j = 0; j < exampleTree->children[i]->childCount; j++) {
                fprintf(file, "    └─ %.2f\n", exampleTree->children[i]->children[j]->value);
            }
        }
        fclose(file);
    }
    
    // 打印原始信号树
    printf("\n原始信号树:\n");
    printSignalTree(exampleTree);
    
    // 应用均值滤波
    int path[] = {};  // 空路径表示从根节点开始
    applyMeanFilterAtPath(exampleTree, path, 0);
    
    // 打印均值滤波后的信号树
    printf("\n均值滤波后的信号树:\n");
    printSignalTree(exampleTree);
    
    // 创建树的副本用于上采样
    SignalNode* upsampledTree = initList(exampleTree->value);
    for (int i = 0; i < exampleTree->childCount; i++) {
        addChildToNode(upsampledTree, exampleTree->children[i]->value);
        for (int j = 0; j < exampleTree->children[i]->childCount; j++) {
            addChildToNode(upsampledTree->children[i], exampleTree->children[i]->children[j]->value);
        }
    }
    
    // 应用上采样
    upsampledTree = upsample(upsampledTree);
    
    // 打印上采样后的信号树
    printf("\n上采样后的信号树:\n");
    printSignalTree(upsampledTree);

    
    // 释放内存
    freeNode(exampleTree);
    freeNode(upsampledTree); 
    
    printf("释放后的信号树是否为空:%d\n",isEmpty(upsampledTree));

    return 0;
}
