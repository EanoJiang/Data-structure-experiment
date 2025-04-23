#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100

typedef struct ArcNode {
    int adjvex; // 邻接点序号
    int weight; // 权重
    struct ArcNode *next;
} ArcNode;

typedef struct VertexNode {
    int data; // 顶点值
    ArcNode *first;
} VertexNode;

typedef struct {
    VertexNode adjList[MAX_VERTICES];
    int numVertices, numEdges;
} Graph;

// 初始化图
void initGraph(Graph *G) {
    G->numVertices = 0;
    G->numEdges = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        G->adjList[i].data = -1;
        G->adjList[i].first = NULL;
    }
}

// 查找顶点在图中的位置（返回索引）
int findVertex(Graph *G, int data) {
    for (int i = 0; i < G->numVertices; i++) {
        if (G->adjList[i].data == data)
            return i;
    }
    return -1; // 边界：未找到对应顶点，返回 -1
}

// 插入一个顶点（如果不存在）
void insertVertex(Graph *G, int data) {
    if (G->numVertices >= MAX_VERTICES) return; // 边界：图已满
    if (findVertex(G, data) == -1) {
        G->adjList[G->numVertices].data = data;
        G->adjList[G->numVertices].first = NULL;
        G->numVertices++;
    }
}

// 插入一条边（有向 + 权重）
void insertEdge(Graph *G, int from, int to, int weight) {
    int fromIdx = findVertex(G, from);
    int toIdx = findVertex(G, to);
    if (fromIdx == -1 || toIdx == -1) return; // 边界：from 或 to 顶点不存在

    ArcNode *newArc = (ArcNode *)malloc(sizeof(ArcNode));
    newArc->adjvex = toIdx;
    newArc->weight = weight;
    newArc->next = G->adjList[fromIdx].first;
    G->adjList[fromIdx].first = newArc;

    G->numEdges++;
}

// 寻找结点 v 的第一个邻接结点
int firstNeighbor(Graph *G, int v) {
    if (v < 0 || v >= G->numVertices) return -1; // 边界：非法索引
    if (G->adjList[v].first != NULL)
        return G->adjList[v].first->adjvex;
    return -1; // 边界：无邻接点
}

// 寻找序号为v1结点的邻接结点v2的下一个邻接结点
int nextNeighbor(Graph *G, int v, int currentNeighbor) {
    if (v < 0 || v >= G->numVertices) return -1; // 边界：非法索引
    ArcNode *p = G->adjList[v].first;
    while (p && p->adjvex != currentNeighbor) p = p->next;
    if (p && p->next)
        return p->next->adjvex;
    return -1; // 边界：未找到当前邻接点或其后无邻接点
}

// 打印图
void PrintGraph(Graph *G) {
    for (int i = 0; i < G->numVertices; i++) {
        printf("%d:", G->adjList[i].data);
        ArcNode *p = G->adjList[i].first;
        while (p != NULL) {
            printf(" -> %d(权重:%d)", G->adjList[p->adjvex].data, p->weight);
            p = p->next;
        }
        printf("\n");
    }
}

// 打印邻接矩阵
void PrintAdjMatrix(Graph *G) {
    int matrix[MAX_VERTICES][MAX_VERTICES];

    // 初始化矩阵
    for (int i = 0; i < G->numVertices; i++) {
        for (int j = 0; j < G->numVertices; j++) {
            matrix[i][j] = 0; // 或者用 -1 表示无连接
        }
    }

    // 填充矩阵
    for (int i = 0; i < G->numVertices; i++) {
        ArcNode *p = G->adjList[i].first;
        while (p) {
            matrix[i][p->adjvex] = p->weight;
            p = p->next;
        }
    }

    // 打印矩阵
    printf("邻接矩阵:\n   ");
    for (int j = 0; j < G->numVertices; j++) {
        printf("%3d", G->adjList[j].data);
    }
    printf("\n");

    for (int i = 0; i < G->numVertices; i++) {
        printf("%3d", G->adjList[i].data);
        for (int j = 0; j < G->numVertices; j++) {
            printf("%3d", matrix[i][j]);
        }
        printf("\n");
    }
}


// DFS遍历,深度优先搜索
//v：起始顶点的索引
int visited[MAX_VERTICES];

void DFS(Graph *G, int v) {
    if (v < 0 || v >= G->numVertices) return; // 边界：非法起始点
    printf("%d ", G->adjList[v].data);
    visited[v] = 1;
    //从当前顶点v出发，访问所有与v相邻的顶点，但不访问已访问过的顶点
    for (ArcNode *p = G->adjList[v].first; p != NULL; p = p->next) {
        if (!visited[p->adjvex])
            DFS(G, p->adjvex);
    }
}

// BFS遍历，广度优先搜索
void BFS(Graph *G, int start) {
    if (start < 0 || start >= G->numVertices) return; // 边界：起始点为非法顶点

    int queue[MAX_VERTICES], front = 0, rear = 0;
    memset(visited, 0, sizeof(visited));

    printf("%d ", G->adjList[start].data);
    visited[start] = 1;
    queue[rear++] = start;

    // 广度优先搜索
    //从起始顶点开始，依次访问其邻接点，然后将邻接点入队，直到队列为空。
    while (front < rear) {
        int v = queue[front++];
        // 遍历该顶点的邻接点
        for (ArcNode *p = G->adjList[v].first; p != NULL; p = p->next) {
            // 未访问过的邻接点入队
            if (!visited[p->adjvex]) {
                printf("%d ", G->adjList[p->adjvex].data);
                visited[p->adjvex] = 1;
                queue[rear++] = p->adjvex;
            }
        }
    }
}

// 从文件读取图（顶点1,顶点2,权重）
void loadGraphFromFile(Graph *G, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("文件读取失败");
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        int v1, v2, w;
        if (sscanf(line, "%d,%d,%d", &v1, &v2, &w) != 3) continue; // 边界：格式不正确
        insertVertex(G, v1);
        insertVertex(G, v2);
        insertEdge(G, v1, v2, w);
    }
    fclose(fp);
}


int main() {
    Graph G;
    initGraph(&G);
    
    loadGraphFromFile(&G, "data.txt"); 
    printf("Graph:\n");
    PrintGraph(&G);
    PrintAdjMatrix(&G);
    
    //DFS和BFS：
    // 起始顶点
    int i = 5;
    printf("DFS:\n");
    memset(visited, 0, sizeof(visited));
    //从顶点i开始深度
    DFS(&G, i-1);
    printf("\n");

    printf("BFS:\n");
    BFS(&G, i-1); // 同上
    printf("\n");

    // 插入新顶点和边
    int newV1 = 10, newV2 = 11, newW = 1;
    insertVertex(&G, newV1);
    insertVertex(&G, newV2);
    insertEdge(&G, newV1, newV2, newW);

    printf("插入后:\n");
    PrintGraph(&G);
    PrintAdjMatrix(&G);
    printf("DFS:\n");
    memset(visited, 0, sizeof(visited));
    DFS(&G, i-1);
    printf("\n");
    printf("BFS:\n");
    BFS(&G, i-1);
    printf("\n");

    return 0;
}
