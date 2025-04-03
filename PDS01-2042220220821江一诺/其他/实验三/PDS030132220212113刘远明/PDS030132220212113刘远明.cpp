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

// ��ʼ��ͼ
void InitGraph(AMGraph* G) {
    memset(G->arcs, MaxInt ,sizeof(G->arcs)); // ��ʼ���ڽӾ���ȫ����Ϊ0
    G->vexnum = 0;
    G->arcnum = 0;
}

// ��Ӷ���
void AddVertex(AMGraph* G, VerTexType vertex) {
    int i = 0;
    if (G->vexnum == MVNum) {
        printf("ͼ�������޷�����¶��㣡\n");
        return;
    }
    for (i = 0; i < G->vexnum; i++)
    {
        if (G->vexs[i] == vertex)
        {
            printf("�����ظ�\n");
            return;
        }
    }
    G->vexs[G->vexnum++] = vertex;
}

// ��ӱ�
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
        printf("���㲻���ڣ��޷���ӱߣ�\n");
        return;
    }
    int temp = G->arcs[i][j];
    G->arcs[i][j] = weight;
    if(weight != 0 && temp == 0){ G->arcnum++; }
}

// ����ڽӾ���
void PrintGraph(AMGraph* G) {
    printf("��������%d��������%d\n", G->vexnum, G->arcnum);
    printf("   ");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%c  ", G->vexs[i]);
    }
    printf("\n");
    for (int i = 0; i < G->vexnum; i++) {
        printf("%c  ", G->vexs[i]);
        for (int j = 0; j < G->vexnum; j++) {
        	if (G->arcs[i][j]==MaxInt)
        	printf("�� ");
        	else
            printf("%d  ", G->arcs[i][j]);
        }
        printf("\n");
    }
}


int NextAdjVex(AMGraph* G, int v, int w) {
    for (int j = w + 1; j < G->vexnum; j++) {
        if (G->arcs[v][j] != 0) { // �ڽӾ����еķ���Ԫ�ر�ʾ���ڱ�
            return j; // ������һ���ڽӽ������
        }
    }
    return -1; // û����һ���ڽӽ��
}

int FirstAdjVex(AMGraph* G, int v) {
    if (v < 0 || v >= G->vexnum) {
        printf("��㲻���ڣ�\n");
        a3 = 0;
        return -1;
    }
    else { a3 = 1; }
    for (int j = 0; j < G->vexnum; j++) {
        if (G->arcs[v][j] != 0) { // �ڽӾ����еķ���Ԫ�ر�ʾ���ڱ�
            return j; // �����ڽӽ������
        }
    }
    return -1; // �ý��û���ڽӽ��
}

void DFS(AMGraph* G, int v)
{     	//ͼGΪ�ڽӾ������� 
    count1++;
    if (v > G->vexnum - 1) { printf("�������"); return; }
    if ((count1 - G->vexnum)) { std::cout << G->vexs[v] << "->"; }
    else { std::cout << G->vexs[v]; }
    visited[v] = true;  		//���ʵ�v������
    //���μ���ڽӾ�����v���ڵ��У���ÿ����v���ڵĵ�  
    for (int w = 0; w < G->vexnum; w++)
        if ((G->arcs[v][w] != MaxInt) && (!visited[w]))
            DFS(G, w);
    //G.arcs[v][w]!=MaxInt��ʾw��v���ڽӵ㣬���wδ���ʣ���ݹ����DFS
}

void BFS(AMGraph* G, int v) {
    cout << G->vexs[v] << "->";
    visited[v] = true;
    queue<int> Q;
    Q.push(v);
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop(); // ȡ����ͷԪ�ز�ɾ��
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

    // ��Ӷ���
    AddVertex(&G, 'A');
    AddVertex(&G, 'B');
    AddVertex(&G, 'C');
    AddVertex(&G, 'D');

    // ��ӱ�
    AddEdge(&G, 'A', 'B', 3);
    AddEdge(&G, 'B', 'C', 5);
    AddEdge(&G, 'C', 'A', 2);
    AddEdge(&G, 'C', 'D', 4);

    // ����ڽӾ���
    PrintGraph(&G);
    printf("\n***************************************\n");
    printf("1.���붥��\n2.�����\n3.Ѱ�����Ϊv�ĵ�һ���ڽӵ�\n4.ͼ��������ȱ���\n5.ͼ�Ĺ�����ȱ���\n6.��ӡͼ\n7.�˳�\n");
    printf("***************************************\n");
    while (true)
    {
        std::cout << "������Ҫ���еĲ���:\n";
        std::cin >> input0;
        if (!(int(input0) - 55)) { break; }
        switch (int(input0)) {
        case 49:
        {
            std::cout << "������Ҫ����Ķ���:\n";
            std::cin >> input1;
            AddVertex(&G, input1);
            PrintGraph(&G);
        }
        break;
        case 50:
        {
            std::cout << "������Ҫ����ıߵĺ�����(�ַ���):\n";
            std::cin >> input2_0[0];
            std::cout << "������Ҫ����ıߵ�������(�ַ���):\n";
            std::cin >> input2_0[1];
            std::cout << "������Ҫ����ıߵ�Ȩ��:\n";
            std::cin >> input2_1;
            AddEdge(&G, input2_0[0], input2_0[1], input2_1);
            PrintGraph(&G);
        }
        break;
        case 51:
        {
            std::cout << "������Ҫ���ҵĽڵ�����:\n";
            std::cin >> input3;
            int adj = FirstAdjVex(&G, input3 - 1); // ��� A ��Ӧ������� 0
            if (a3)
            {
                if (adj != -1) {
                    printf("��� %c �ĵ�һ���ڽӽ���ǣ�%c\n", G.vexs[input3 - 1], G.vexs[adj]);
                }
                else {
                    printf("��� %c û���ڽӽ�㣡\n", G.vexs[input3 - 1]);
                }
            }
        }
        break;
        case 52:
        {
            std::cout << "��������������������:\n";
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
            std::cout << "�������������������:\n";
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
            printf("�������\n");
            continue;
        }
    }
    return 0;
}
