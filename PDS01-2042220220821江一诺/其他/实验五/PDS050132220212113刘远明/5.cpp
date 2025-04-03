#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include<time.h>
#define MAX_LENGTH 100
#define NIL 0
#define MAX_DATA 1000
clock_t start, finish;     //�����һ�ε���CPUʱ�ӵ�λ��ʵ�ʣ��������Ϊ����һ��������
double Total_time;        //����һ��double���͵ı��������ڴ洢ʱ�䵥λ
int k = 0;
int sz = 1000;
int left = 0;//���±�
int right = sz - 1;//���±�
char str1[] = "AAAA";
int a = 0;
typedef struct {
    char letters[5];  // 4����ĸ��һ����β����
    int nums[3];
	int id;
} DataEntry;

typedef struct {
	int id0;
	int dis;
} distance;

DataEntry data[MAX_DATA];
char line[MAX_LENGTH];
int index = 0;

int read_data() {
    FILE *file = fopen("E:\\ƽʱ��ҵ\\���ݽṹʵ��\\ʵ����\\data.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%4s,%d,%d,%d", data[index].letters, &data[index].nums[0], &data[index].nums[1], &data[index].nums[2]);
		data[index].id = index;
        index++;
    }

    fclose(file);

    // ��ӡ��ȡ��������
    for (int i = 0; i < index; i++) {
        printf("%d,%s,%d,%d,%d\n", data[i].id, data[i].letters, data[i].nums[0], data[i].nums[1], data[i].nums[2]);
    }
}
int Division(distance R[], int low, int high)  //�ָ
{
    distance base = R[low];  //��׼Ԫ�أ�
    while (low < high)
    {
        //�ȴӺ���ǰ�ң��ٴ�ǰ�����
        while (low < high && R[high].dis >= base.dis)
            high--;   //���������ҵ�һ���Ȼ�׼ֵС��Ԫ�أ�
        R[low] = R[high];
        while (low < high && R[low].dis < base.dis)
            low++;   //���������ҵ�һ���Ȼ�׼ֵ���Ԫ�أ�
        R[high] = R[low];
    }
    R[low] = base;
    return low;
}

void QuickSort(distance R[], int low, int high)
{
    if (low < high)
    {
        int i = Division(R, low, high);  //�ָ
        QuickSort(R, low, i - 1);  //�������ֱַ�����
        QuickSort(R, i + 1, high);
    }
}

typedef struct SortTree {
    int data;//������ݵ�������
    struct SortTree* left;//ָ���� ��ָ��
    struct SortTree* right;//ָ���� ��ָ��
}Node;
/*ȫ�ֱ���*/
Node* root;//���ڵ�
void Init(int key)
{
    root = (Node*)malloc(sizeof(Node));
    root->data = key;
    root->left = NULL;
    root->right = NULL;
}
void insert(int key)
{
    //����һ����ʱָ�� �����ƶ�
    Node* temp = root;//�����ƶ� �Լ� ����ѭ��
    Node* prev = NULL;//��λ��������λ�õ�ǰһ�����
    while (temp != NULL)
    {
        prev = temp;
        if (key < temp->data)
        {
            temp = temp->left;
        }
        else if (key > temp->data)
        {
            temp = temp->right;
        }
        else
        {
            return;
        }
    }
    if (key < prev->data)
    {
        prev->left = (Node*)malloc(sizeof(Node));
        prev->left->data = key;
        prev->left->left = NULL;
        prev->left->right = NULL;
    }
    else
    {
        prev->right = (Node*)malloc(sizeof(Node));
        prev->right->data = key;
        prev->right->left = NULL;
        prev->right->right = NULL;
    }
}
void show(Node* root)
{
    if (root == NULL)
    {
        return;
    }
    show(root->left);
    printf("%d ", root->data);
    show(root->right);
}
int search(Node* root, int key)
{
    while (root != NULL)
    {
        if (key == root->data)
            return root->data;
        else if (key < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return 0;
}
typedef struct {
    int* Table;//�����ϣ�ڵ���������ַ
    int size;//��ϣ����
}HashTable;

//��ʼ����ϣ��
HashTable* InitHashTabel(int size) {
    HashTable* H = (HashTable*)malloc(sizeof(HashTable));
    H->size = size;
    H->Table = (int*)malloc(sizeof(int) * size);
    //�����Բ�λ��ʼ��Ϊ����״̬
    while (size > 0) H->Table[--size] = NIL;
    return H;
}
//��ϣ����
int Hash(int data, int size) {
    return data % size;//����������
}
//����̽�ⷨ�����ϣ��ͻ
int LinearProbe(HashTable* H, int data) {
    int Pos = Hash(data, H->size);//ͨ����ϣ��������õ����ϣ��ַ
    //����ǰλ�ñ�ռ��
    while (H->Table[Pos] != NIL) {
        //���Ѵ��ڵ�ǰ��
        if (H->Table[Pos] == data) {
            return Pos;//������λ��
        }
        Pos = (Pos + 1) % H->size;//����̽����һ����λ
    }
    return Pos;//���ؿ���λ��
}
//�����ϣ�ڵ�
int Insert(HashTable* H, int key) {
    int Pos = LinearProbe(H, key);//��ȡ�ùؼ���Ӧ����λ��
    //�жϸùؼ����Ƿ��ڹ�ϣ����
    if (H->Table[Pos] != key) {
        H->Table[Pos] = key;
        return 1;//����ɹ�
    }
    return 0;//����ʧ��
}
//��ѯ��ϣ�ڵ�
int Search(HashTable* H, int key) {
    //����̽�����key�Ƿ��ڹ�ϣ����
    int Pos = LinearProbe(H, key);
    if (H->Table[Pos] != NIL)
        return Pos;
    return -1;//����Ԫ�ز�����
}
//ɾ����ϣ�ڵ�
int Delete(HashTable* H, int key) {
    int Pos = Search(H, key);//���Ҹùؼ���
    if (Pos != -1) {
        H->Table[Pos] = NIL;//ֱ�ӽ���λ�ÿ�
        return 1;//ɾ���ɹ�������1
    }
    return 0;//ɾ��ʧ�ܣ�����0
}
//��ӡ��ϣ��ڵ�
void print(HashTable* H) {
    for (int i = 0; i < H->size; i++) {
        printf("%d  ", H->Table[i]);
    }
}
int get_charnum() {
    printf("����������Ҫ���ҵ��ַ�\n");
    scanf("%s", &str1);
    for (int i = 1; i < 1000; i++) {
        if (!strcmp(str1, data[i].letters))
        {
            k = data[i].id;
            break;
        }
    }
    return k;
}

/**/
int main()
{
    read_data();
    distance distance[1000];
    for (int i = 1; i < 1000; i++) {
        distance[i].dis = sqrt(data[i].nums[0] * data[i].nums[0] + data[i].nums[1] * data[i].nums[1] + data[i].nums[2] * data[i].nums[2]);
        distance[i].id0 = i;
    }
    Init(500);
    for (int i = 1; i < 999; i++)
    {
        insert(distance[i].id0);
    }
    show(root);
    HashTable* H = InitHashTabel(1000);
    for (int i = 1; i < 1000; i++) {
        Insert(H, distance[i].id0);
    }
    print(H);
    while (1) 
    {
        printf("\n**********************************\n");
        printf("*    1.���ֲ���                  *\n");
        printf("*    2.�����������              *\n");
        printf("*    3.Hash����                  *\n");
        printf("*    4.�˳�                      *\n");
        printf("**********************************\n");
        printf("��ʼ������ɣ������������\n");
        int input;
        scanf("%d", &input);
        switch (input) {
        case 1:
            start = clock();
            k = get_charnum();
            while (left <= right)
            {
                int mid = (left + right) / 2;//�м�Ԫ���±�;mid = left+(right-left)/2
                if (distance[mid].id0 < k)
                {
                    left = mid + 1;
                }
                else if (distance[mid].id0 > k)
                {
                    right = mid - 1;
                }
                else
                {
                    printf("�ҵ����±��ǣ�%d\n", mid);
                    break;
                }
                if (left > right)
                    printf("�����ڸ�����\n");
            }
            finish = clock();
            Total_time = finish - start;
            break;
        case 2: {
            start = clock();
            k = get_charnum();
            int a = search(root, k);
            printf("\n%d\n", a);
            finish = clock();
            Total_time = finish - start;
            break;
        }
        case 3:
            start = clock();
            k = get_charnum();
            a = Search(H, k);
            printf("\n%d\n", a);
            finish = clock();
            Total_time = finish - start;
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("�����������������������\n");
            break;
        }
    }
}
