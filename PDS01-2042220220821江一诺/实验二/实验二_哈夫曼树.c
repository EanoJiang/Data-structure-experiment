#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_NODES 100 // 树的最大节点数
#define MAX_LEAFS 50  // 最大叶子节点数

typedef struct {
    char data; 
    int weight; 
    int parent; 
    int lchild; 
    int rchild; 
} HuffmanNode;

typedef char** HuffmanCode;

// 初始化哈夫曼树节点
void initHuffmanTree(HuffmanNode* nodes, int leafCount) {
    int totalNodes = 2 * leafCount - 1;
    for (int i = leafCount; i < totalNodes; ++i) {
        nodes[i].parent = -1;
        nodes[i].lchild = -1;
        nodes[i].rchild = -1;
    }
}

// 选择两个权重最小的节点
void select(HuffmanNode* nodes, int count, int* s1, int* s2) {
    int min1 = INT_MAX, min2 = INT_MAX;
    *s1 = *s2 = -1;
    for (int i = 0; i < count; ++i) {
        // 选择未连接的节点
        if (nodes[i].parent == -1) { 
            //选权重最小的两个节点
            if (nodes[i].weight < min1) {
                //更新最小值
                min2 = min1;
                //更新最小值节点指针
                *s2 = *s1;
                min1 = nodes[i].weight;
                *s1 = i;
            } else if (nodes[i].weight < min2) {
                min2 = nodes[i].weight;
                *s2 = i;
            }
        }
    }
}

// 创建哈夫曼树
void createHuffmanTree(HuffmanNode* nodes, int leafCount) {
    initHuffmanTree(nodes, leafCount);
    int totalNodes = 2 * leafCount - 1;

    //遍历每个叶子节点，连接到两个权重最小的节点
    for (int i = leafCount; i < totalNodes; ++i) {
        int s1, s2;
        select(nodes, i, &s1, &s2);

        nodes[s1].parent = i;
        nodes[s2].parent = i;

        nodes[i].lchild = s1;
        nodes[i].rchild = s2;
        nodes[i].weight = nodes[s1].weight + nodes[s2].weight;
    }
}

// 哈夫曼编码
void createHuffmanCode(HuffmanNode* nodes, HuffmanCode* hc, int leafCount) {
    *hc = (HuffmanCode)malloc((leafCount + 1) * sizeof(char*));
    // 编码结束符
    char* cd = (char*)malloc(leafCount * sizeof(char));
    cd[leafCount - 1] = '\0';

    //遍历每个叶子节点，生成编码
    for (int i = 0; i < leafCount; ++i) {
        int start = leafCount - 1;
        int c = i;
        int f = nodes[i].parent;
        // 回溯到根节点
        //只要不是-1就有父节点，就一直回溯，直到根节点
        while (f != -1) {
            start--;
            // 左子节点0，右子节点1
            if (nodes[f].lchild == c) {
                cd[start] = '0';
            } else {
                cd[start] = '1';
            }
            //向上更新节点
            c = f;
            f = nodes[f].parent;
        }
        (*hc)[i] = (char*)malloc(leafCount - start);
        strcpy((*hc)[i], &cd[start]);
    }
    free(cd);  // 释放临时空间
}

// 哈夫曼解码：将编码的字符串解码为原始信号
void decodeSignal(HuffmanNode* nodes, int *signal, char* encodedStr, int leafCount) {
    //根节点索引=(叶子数-1)*2
    int root = 2 * leafCount - 2;
    int current = root; 
    int length = 0;
    signal[length] = '\0';

    while (*encodedStr) { 
        // 左子节点0，右子节点1
        if (*encodedStr == '0') {
            current = nodes[current].lchild;
        } else {
            current = nodes[current].rchild;
        }
        // 到达叶子末端，重置当前索引到根节点
        if (nodes[current].lchild == -1 && nodes[current].rchild == -1) {
            signal[length++] = nodes[current].data;
            current = root; 
        }
        // 移动到下一个字符
        encodedStr++;
    }
    signal[length] = '\0';
}

// 打印哈夫曼树节点信息
void printNodes(HuffmanNode* nodes, int count) {
    for (int i = 0; i < count; ++i) {
        printf("Node %d: [data='%c', weight=%d, parent=%d, lchild=%d, rchild=%d]\n",
               i, nodes[i].data, nodes[i].weight, nodes[i].parent, nodes[i].lchild, nodes[i].rchild);
    }
}

// 打印哈夫曼编码
void printHuffmanCodes(HuffmanCode hc, HuffmanNode* nodes, int leafCount) {
    printf("哈夫曼编码：\n");
    for (int i = 0; i < leafCount; ++i) {
        printf("Data '%c': Code = %s\n", nodes[i].data, hc[i]);
    }
}

int main() {
    HuffmanNode nodes[MAX_NODES];
    int leafCount = 0;

    // 打开文件并读取数据
    FILE *file = fopen("huffmantree.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // 读取文件中的字符和对应频率
    while (fscanf(file, " %c %d", &nodes[leafCount].data, &nodes[leafCount].weight) == 2) {
        nodes[leafCount].parent = -1;
        nodes[leafCount].lchild = -1;
        nodes[leafCount].rchild = -1;
        leafCount++;
    }
    fclose(file);

    // 确保叶子节点数量没有超过定义的上限
    if (leafCount > MAX_LEAFS) {
        fprintf(stderr, "叶子节点数量超出最大限制。\n");
        return EXIT_FAILURE;
    }

    // 创建哈夫曼树
    createHuffmanTree(nodes, leafCount);

    // 打印节点信息
    printf("哈夫曼树节点信息:\n");
    printNodes(nodes, 2 * leafCount - 1);

    // 生成和打印哈夫曼编码
    HuffmanCode hc;
    createHuffmanCode(nodes, &hc, leafCount);
    printHuffmanCodes(hc, nodes, leafCount);

    int decodedSignal[MAX_LEAFS];
    for(int i = 0; i < leafCount; i++) {
    decodeSignal(nodes, decodedSignal, hc[i], leafCount);
    printf("%s 解码后: %c\n", hc[i],decodedSignal[0]);
    }

    
    // 释放哈夫曼编码存储空间
    for (int i = 0; i < leafCount; ++i) {
        free(hc[i]);
    }
    free(hc);
    
    return 0;
}