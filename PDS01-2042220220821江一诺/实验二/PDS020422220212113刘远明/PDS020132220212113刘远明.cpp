// ��׼���������
#include <stdio.h> 
// ��׼��
#include <stdlib.h> 
// �ڴ�����
#include <malloc.h> 
// �ַ���������
#include <string.h> 
// �����������
#include <iostream> 
// ���� OK Ϊ 0
#define OK 0 
// ���� ERROR Ϊ -1
#define ERROR -1 
// ������������СΪ 100
#define MAXSIZE 100 

// ʹ�ñ�׼�����ռ�
using namespace std; 

// ������������ڵ�ṹ�� HTNode
typedef struct {
    int data; // �ڵ�����
    int weight; // �ڵ�Ȩ��
    int parent, lch, rch; // ���ڵ㡢���ӽڵ㡢���ӽڵ�����
} HTNode, *HuffmanTree; // �����������ָ������ HuffmanTree

// ��������������ָ������ HuffmanCode
typedef char* *HuffmanCode;

// ��ʼ��������������
void InitHuffmanTree(HuffmanTree& HT, int n) {
    if (n <= 1) return; // ��� n С�ڵ��� 1���򷵻�
    int m = 2 * n - 1; // ������������ܽڵ���
    HT = new HTNode[m + 1]; // ����������������ڴ�
    // ��ʼ������������ÿ���ڵ�
    for (int i = 1; i <= m; ++i) {
        HT[i].parent = 0; // ���ڵ�������ʼ��Ϊ 0
        HT[i].lch = 0; // ���ӽڵ�������ʼ��Ϊ 0
        HT[i].rch = 0; // ���ӽڵ�������ʼ��Ϊ 0
    }
}

// ѡ����������е�������С�ڵ㺯��
void select(HuffmanTree& HT, int n, int& s1, int& s2) {
    int min1 = MAXSIZE, min2 = MAXSIZE; // ��ʼ�� min1 �� min2 Ϊ���ֵ MAXSIZE
    // ���ҹ��������е�������СȨ�ؽڵ�
    for (int i = 1; i <= n; i++) {
        if (HT[i].weight < min1 && HT[i].parent == 0) {
            min1 = HT[i].weight;
            s1 = i;
        }
    }
    int temp = HT[s1].weight; // �ݴ� s1 ��Ȩ��
    HT[s1].weight = MAXSIZE; // �� s1 ��Ȩ����Ϊ���ֵ����������ѡ��
    // ���ҹ��������еĵڶ�����СȨ�ؽڵ�
    for (int i = 1; i <= n; i++) {
        if (HT[i].weight < min2 && HT[i].parent == 0) {
            min2 = HT[i].weight;
            s2 = i;
        }
    }
    HT[s1].weight = temp; // �ָ� s1 ��ԭʼȨ��ֵ
}

// ����������������
void CreatHuffmanTree(HuffmanTree& HT, int n) {
    int m, s1, s2;
    if (n <= 1) return; // ��� n С�ڵ��� 1���򷵻�
    m = 2 * n - 1; // ������������ܽڵ���
    // ͨ�� n-1 ��ѡ��ͺϲ�������������������
    for (int i = n + 1; i <= m; ++i) {
        // ѡ��ǰ���������е�������С�ڵ�
        select(HT, i - 1, s1, s2);
        HT[s1].parent = i; // ���� s1 �ĸ��ڵ�Ϊ i
        HT[s2].parent = i; // ���� s2 �ĸ��ڵ�Ϊ i
        HT[i].lch = s1; // ���� i �����ӽڵ�Ϊ s1
        HT[i].rch = s2; // ���� i �����ӽڵ�Ϊ s2
        HT[i].weight = HT[s1].weight + HT[s2].weight; // ���� i ��Ȩ��Ϊ�����ӽڵ�Ȩ��֮��
    }
    HT[0].data = m; // �����ڵ�ı�Ŵ��� HT[0].data ��
}

// �����������������
void CreatHuffmanCode(HuffmanTree HT, HuffmanCode& HC, int n) {
    int i, start, c, f;
    char *cd;
    HC = (HuffmanCode)malloc((n + 1) * sizeof(char*)); // ���� n ���ַ������ͷָ�����
    cd = (char*)malloc(n * sizeof(char)); // ���������Ĺ����ռ�
    cd[n - 1] = '\0'; // ���ñ��������λ��
    // ����ַ������������
    for (i = 1; i <= n; i++) {
        start = n - 1; // ������ʼλ��
        for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) {
            // ��Ҷ�ӵ���������ÿ���ַ��Ĺ���������
            if (HT[f].lch == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
        }
        HC[i] = (char*)malloc((n - start) * sizeof(char)); // ����洢����Ŀռ�
        strcpy(HC[i], &cd[start]); // �����뿽�����������������
    }
    free(cd); // �ͷű��빤���ռ�
}

// ��ʾ���������벢�洢����
void showHuffmanTree(HuffmanTree HT, HuffmanCode HC, int length, int* data, char* show) {
    int m = 0, index = 0;
    for (int i = 0; i < length; i++) {
        int j = 1;
        // �������ݶ�Ӧ�Ĺ���������
        while (data[i] != HT[j].data) {
            j++;
        }
        // ������������洢�� show ��


        for (int count = 0; count < sizeof(HC[j]) / sizeof(char) && HC[j][count]; count++) {
            show[index++] = HC[j][count];
        }
        m++;
    }
    show[index] = '#'; // ��ӱ��������
    // ��ӡ����������
    for (int x = 0; x < index; x++) {
        printf("%c", show[x]);
    }
}

// ���������뺯��
void decodeHuffmanTree(HuffmanTree HT, char* show) {
    int i = HT[0].data;
    int j = 0;
    char endflag = '#';
    // ����ַ�����
    while (show[j] != '#') {
        if (show[j] == '0')
            i = HT[i].lch;
        else if (show[j] == '1') {
            i = HT[i].rch;
        }
        // �ж��Ƿ�ΪҶ�ӽڵ㣬��������������
        if (HT[i].lch == 0) {
            printf("%d ", HT[i].data);
            i = HT[0].data;
        }
        j++;
    }
    printf("\n");
}

// ��ȡ�ļ�������Ƶ��ͳ�ƺ���
int read(char* name, int n, int* data, int* metaData, int* weight, int& length) {
    for (int x = 0; x < n; x++) {
        data[x] = -1; // ��ʼ����������Ϊ -1
    }
    int i = 0, x = 0;
    int result = 0;
    FILE* fp = fopen(name, "r"); // ���ļ�
    char c[50];
    if (fp == NULL) {
        puts("���ܴ��ļ���\n"); // ���������Ϣ
        return ERROR;
    }
    if (fgets(c, sizeof(c), fp)) {
        char* p = strtok(c, ",");
        if (p) {
            result = atoi(p); // ���ַ���ת��Ϊ����
            data[i] = result;
            metaData[x] = result;
            length++;
            weight[i] = 1;
        }
        while (p = strtok(NULL, ",")) {
            int flag = 0;
            result = atoi(p); // ���ַ���ת��Ϊ����
            metaData[++x] = result;
            length++;
            // ͳ��Ƶ��
            for (int j = 0; j < n; j++) {
                if (result == data[j]) {
                    weight[j]++;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                i++;
                data[i] = result;
                weight[i] = 1;
            }
        }
    }
    fclose(fp); // �ر��ļ�
    return OK;
}

// ���ַ���ת��Ϊ��������
int atoi(char* str) {
    int value = 0;
    while (*str >= '0' && *str <= '9') {
        value *= 10;
        value += *str - '0';
        str++;
    }
    return value;
}

// ð���������������ݺ�Ƶ����������
void bubble_sort(int* data, int* weight, int size) {
    int i = 0;
    int j = 0;
    int temp = 0;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - 1 - i; j++) {
            if (data[j] > data[j + 1]) {
                // �������ݺ�Ƶ��
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
                temp = weight[j];
                weight[j] = weight[j + 1];
                weight[j + 1] = temp;
            }
        }
    }
}

// ������
int main() {
    int n = 5; // ���������С
    int length = 0; // ���峤�ȱ���
    int input;
    char c[50];
    int* data = (int*)malloc(n * sizeof(int)); // �������������ڴ�
    int* weight = (int*)malloc(n * sizeof(int)); // ����Ƶ�������ڴ�
    int* metaData = (int*)malloc(20 * sizeof(int)); // ����Ԫ���������ڴ�
    HuffmanTree HT;
    InitHuffmanTree(HT, n); // ��ʼ����������
    read("E:\\zuoye\\2source.txt", n, data, metaData, weight, length); // ��ȡ�ļ�������Ƶ��ͳ��

    // �����Դ����
    printf("��Դ�������£�\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", metaData[i]);
    }

    // �����ݽ���ð�����򲢽���ֱ��ͼͳ��
    bubble_sort(data, weight, n);
    printf("\n\nһά����ֱ��ͼͳ��+ð������:\n");
    for (int i = 1; i <= n; i++) {
        HT[i].data = data[i - 1];
        HT[i].weight = weight[i - 1];
        printf("��Դ����Ϊ��%d, Ȩ��Ϊ��%d\n", HT[i].data, HT[i].weight);
    }

    // �������
    printf("\n����Ϊ��\n");
    HuffmanCode HC;
    CreatHuffmanTree(HT, n);
    CreatHuffmanCode(HT, HC, n);
    for (int i = 1; i <= n; i++)
        printf("��Դ����Ϊ��%d������Ϊ��%s\n", HT[i].data, HC[i]);

    // ����
    printf("��������Ҫ�������Դ����Χ0~4,�Զ��ŷָ���,������Ҫ������-1��ϵͳĬ�������Դ�ļ��ı��뼰ת����\n");
    gets(c);
    char* show = (char*)malloc(200);
    if (strcmp(c, "-1") != 0) {
        length = 0;
        char* p = strtok(c, ",");
        int num = 0;
        if (p) {
            if (atoi(p) < 0 || atoi(p) > 4) {
                printf("������Դ��Χ���󣬷�ΧӦ��0~4\n");
                return ERROR;
            }
            metaData[num] = atoi(p);
            length++;
        }
        while (p = strtok(NULL, ",")) {
            if (atoi(p) < 0 || atoi(p) > 4) {
                printf("������Դ��Χ���󣬷�ΧӦ��0~4\n");
               

 return ERROR;
            }
            metaData[++num] = atoi(p);
            length++;
        }
    }
    printf("\n����Ϊ��\n");
    showHuffmanTree(HT, HC, length, metaData, show);
    // ����
    printf("\n\n�����\n");
    decodeHuffmanTree(HT, show);

    return OK;
}

