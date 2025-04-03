#include <iostream>
#include <cstdio>
#include <cstring>
#include<queue>
using namespace std;
#define MaxInt -1
#define MVNum 100

int count1 = 0;
int a3 = 1;
typedef char VerTexType;
typedef int ArcType;
bool visited[MVNum];

typedef struct {
    VerTexType vexs[MVNum];
    ArcType arcs[MVNum][MVNum];
    int vexnum, arcnum;
} AMGraph;

// 初始化图
void InitGraph(AMGraph* G) {
    memset(G->arcs, MaxInt ,sizeof(G->arcs)); // 初始化邻接矩阵，全部置为0
    G->vexnum = 0;
    G->arcnum = 0;
}

// 添加顶点
void AddVertex(AMGraph* G, VerTexType vertex) {
    int i = 0;
    if (G->vexnum == MVNum) {
        printf("图已满，无法添加新顶点！\n");
        return;
    }
    for (i = 0; i < G->vexnum; i++)
    {
        if (G->vexs[i] == vertex)
        {
            printf("顶点重复\n");
            return;
        }
    }
    G->vexs[G->vexnum++] = vertex;
}

// 添加边
void AddEdge(AMGraph* G, VerTexType start, VerTexType end, ArcType weight) {
    int i, j;
    for (i = 0; i < G->vexnum; i++) {
        if (G->vexs[i] == start) {
            break;
        }
    }
    for (j = 0; j < G->vexnum; j++) {
        if (G->vexs[j] == end) {
            break;
        }
    }
    if (i == G->vexnum || j == G->vexnum) {
        printf("顶点不存在，无法添加边！\n");
        return;
    }
    int temp = G->arcs[i][j];
    G->arcs[i][j] = weight;
    if(weight != 0 && temp == 0){ G->arcnum++; }
}

// 输出邻接矩阵
void PrintGraph(AMGraph* G) {
    printf("顶点数：%d，边数：%d\n", G->vexnum, G->arcnum);
    printf("   ");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%c  ", G->vexs[i]);
    }
    printf("\n");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%c  ", G->vexs[i]);
        for (int j = 0; j < G->vexnum; j++) {
        	if (G->arcs[i][j]==MaxInt)
        	printf("∞ ");
        	else
            printf("%d  ", G->arcs[i][j]);
        }
        printf("\n");
    }
}


int NextAdjVex(AMGraph* G, int v, int w) {
    for (int j = w + 1; j < G->vexnum; j++) {
        if (G->arcs[v][j] != 0) { // 邻接矩阵中的非零元素表示存在边
            return j; // 返回下一个邻接结点的序号
        }
    }
    return -1; // 没有下一个邻接结点
}

int FirstAdjVex(AMGraph* G, int v) {
    if (v < 0 || v >= G->vexnum) {
        printf("结点不存在！\n");
        a3 = 0;
        return -1;
    }
    else { a3 = 1; }
    for (int j = 0; j < G->vexnum; j++) {
        if (G->arcs[v][j] != 0) { // 邻接矩阵中的非零元素表示存在边
            return j; // 返回邻接结点的序号
        }
    }
    return -1; // 该结点没有邻接结点
}

void DFS(AMGraph* G, int v)
{     	//图G为邻接矩阵类型 
    count1++;
    if (v > G->vexnum - 1) { printf("输入错误"); return; }
    if ((count1 - G->vexnum)) { std::cout << G->vexs[v] << "->"; }
    else { std::cout << G->vexs[v]; }
    visited[v] = true;  		//访问第v个顶点
    //依次检查邻接矩阵中v所在的行，即每个与v相邻的点  
    for (int w = 0; w < G->vexnum; w++)
        if ((G->arcs[v][w] != MaxInt) && (!visited[w]))
            DFS(G, w);
    //G.arcs[v][w]!=MaxInt表示w是v的邻接点，如果w未访问，则递归调用DFS
}

void BFS(AMGraph* G, int v) {
    cout << G->vexs[v] << "->";
    visited[v] = true;
    queue<int> Q;
    Q.push(v);
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop(); // 取出队头元素并删除
        for (int w = FirstAdjVex(G, u); w >= 0; w = NextAdjVex(G, u, w)) {
            if (!visited[w]) {
                count1++;
                cout << G->vexs[w];
                if (count1 < G->vexnum-1) { cout << "->"; }
                visited[w] = true;
                Q.push(w);
            }
        }
    }
}

int main() {
    char input0;
    char input1;
    char input2_0[2];
    int input2_1;
    int input3;
    int input4;
    int input5;
    AMGraph G;
    InitGraph(&G);

    // 添加顶点
    AddVertex(&G, 'A');
    AddVertex(&G, 'B');
    AddVertex(&G, 'C');
    AddVertex(&G, 'D');

    // 添加边
    AddEdge(&G, 'A', 'B', 3);
    AddEdge(&G, 'B', 'C', 5);
    AddEdge(&G, 'C', 'A', 2);
    AddEdge(&G, 'C', 'D', 4);

    // 输出邻接矩阵
    PrintGraph(&G);
    printf("\n***************************************\n");
    printf("1.插入顶点\n2.插入边\n3.寻找序号为v的第一个邻接点\n4.图的深度优先遍历\n5.图的广度优先遍历\n6.打印图\n7.退出\n");
    printf("***************************************\n");
    while (true)
    {
        std::cout << "请输入要进行的操作:\n";
        std::cin >> input0;
        if (!(int(input0) - 55)) { break; }
        switch (int(input0)) {
        case 49:
        {
            std::cout << "请输入要插入的顶点:\n";
            std::cin >> input1;
            AddVertex(&G, input1);
            PrintGraph(&G);
        }
        break;
        case 50:
        {
            std::cout << "请输入要插入的边的横坐标(字符串):\n";
            std::cin >> input2_0[0];
            std::cout << "请输入要插入的边的纵坐标(字符串):\n";
            std::cin >> input2_0[1];
            std::cout << "请输入要插入的边的权重:\n";
            std::cin >> input2_1;
            AddEdge(&G, input2_0[0], input2_0[1], input2_1);
            PrintGraph(&G);
        }
        break;
        case 51:
        {
            std::cout << "请输入要查找的节点的序号:\n";
            std::cin >> input3;
            int adj = FirstAdjVex(&G, input3 - 1); // 结点 A 对应的序号是 0
            if (a3)
            {
                if (adj != -1) {
                    printf("结点 %c 的第一个邻接结点是：%c\n", G.vexs[input3 - 1], G.vexs[adj]);
                }
                else {
                    printf("结点 %c 没有邻接结点！\n", G.vexs[input3 - 1]);
                }
            }
        }
        break;
        case 52:
        {
            std::cout << "请输入深度优先搜索序号:\n";
            std::cin >> input4;
            DFS(&G, input4 - 1);
            std::cout << "\n";
            count1 = 0;
            for (int i = 0; i <= G.vexnum; i++)
            {
                visited[i] = false;
            }
        }
        break;
        case 53:
        {
            std::cout << "请输入广度优先搜索序号:\n";
            std::cin >> input5;
            BFS(&G, input5 - 1);
            std::cout << "\n";
            count1 = 0;
            for (int i = 0; i <= G.vexnum; i++)
            {
                visited[i] = false;
            }
        }
        break;
        case 54:
        {
            PrintGraph(&G);
        }
        break;
        default:
            printf("输入错误！\n");
            continue;
        }
    }
    return 0;
}
