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
clock_t start, finish;     //定义第一次调用CPU时钟单位的实际，可以理解为定义一个计数器
double Total_time;        //定义一个double类型的变量，用于存储时间单位
int k = 0;
int sz = 1000;
int left = 0;//左下标
int right = sz - 1;//右下标
char str1[] = "AAAA";
int a = 0;
typedef struct {
    char letters[5];  // 4个字母和一个结尾符号
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
    FILE *file = fopen("E:\\平时作业\\数据结构实验\\实验五\\data.txt", "r");
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

    // 打印读取到的数据
    for (int i = 0; i < index; i++) {
        printf("%d,%s,%d,%d,%d\n", data[i].id, data[i].letters, data[i].nums[0], data[i].nums[1], data[i].nums[2]);
    }
}
int Division(distance R[], int low, int high)  //分割；
{
    distance base = R[low];  //基准元素；
    while (low < high)
    {
        //先从后向前找，再从前向后找
        while (low < high && R[high].dis >= base.dis)
            high--;   //从右向左找第一个比基准值小的元素；
        R[low] = R[high];
        while (low < high && R[low].dis < base.dis)
            low++;   //从左向右找第一个比基准值大的元素；
        R[high] = R[low];
    }
    R[low] = base;
    return low;
}

void QuickSort(distance R[], int low, int high)
{
    if (low < high)
    {
        int i = Division(R, low, high);  //分割；
        QuickSort(R, low, i - 1);  //将两部分分别排序；
        QuickSort(R, i + 1, high);
    }
}

typedef struct SortTree {
    int data;//存放数据的数据域
    struct SortTree* left;//指针域 左指针
    struct SortTree* right;//指针域 右指针
}Node;
/*全局变量*/
Node* root;//根节点
void Init(int key)
{
    root = (Node*)malloc(sizeof(Node));
    root->data = key;
    root->left = NULL;
    root->right = NULL;
}
void insert(int key)
{
    //定义一个临时指针 用于移动
    Node* temp = root;//方便移动 以及 跳出循环
    Node* prev = NULL;//定位到待插入位置的前一个结点
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
    int* Table;//储存哈希节点的数组基地址
    int size;//哈希表长度
}HashTable;

//初始化哈希表
HashTable* InitHashTabel(int size) {
    HashTable* H = (HashTable*)malloc(sizeof(HashTable));
    H->size = size;
    H->Table = (int*)malloc(sizeof(int) * size);
    //将所以槽位初始化为空闲状态
    while (size > 0) H->Table[--size] = NIL;
    return H;
}
//哈希函数
int Hash(int data, int size) {
    return data % size;//除留余数法
}
//线性探测法解决哈希冲突
int LinearProbe(HashTable* H, int data) {
    int Pos = Hash(data, H->size);//通过哈希函数计算得到其哈希地址
    //若当前位置被占用
    while (H->Table[Pos] != NIL) {
        //若已存在当前键
        if (H->Table[Pos] == data) {
            return Pos;//返回其位置
        }
        Pos = (Pos + 1) % H->size;//线性探测下一个槽位
    }
    return Pos;//返回空闲位置
}
//插入哈希节点
int Insert(HashTable* H, int key) {
    int Pos = LinearProbe(H, key);//获取该关键字应所在位置
    //判断该关键字是否在哈希表中
    if (H->Table[Pos] != key) {
        H->Table[Pos] = key;
        return 1;//插入成功
    }
    return 0;//插入失败
}
//查询哈希节点
int Search(HashTable* H, int key) {
    //线性探测查找key是否在哈希表中
    int Pos = LinearProbe(H, key);
    if (H->Table[Pos] != NIL)
        return Pos;
    return -1;//所查元素不存在
}
//删除哈希节点
int Delete(HashTable* H, int key) {
    int Pos = Search(H, key);//查找该关键字
    if (Pos != -1) {
        H->Table[Pos] = NIL;//直接将槽位置空
        return 1;//删除成功，返回1
    }
    return 0;//删除失败，返回0
}
//打印哈希表节点
void print(HashTable* H) {
    for (int i = 0; i < H->size; i++) {
        printf("%d  ", H->Table[i]);
    }
}
int get_charnum() {
    printf("请输入你需要查找的字符\n");
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
        printf("*    1.二分查找                  *\n");
        printf("*    2.二叉排序查找              *\n");
        printf("*    3.Hash查找                  *\n");
        printf("*    4.退出                      *\n");
        printf("**********************************\n");
        printf("初始化已完成，请输入操作：\n");
        int input;
        scanf("%d", &input);
        switch (input) {
        case 1:
            start = clock();
            k = get_charnum();
            while (left <= right)
            {
                int mid = (left + right) / 2;//中间元素下标;mid = left+(right-left)/2
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
                    printf("找到了下标是：%d\n", mid);
                    break;
                }
                if (left > right)
                    printf("不存在该数字\n");
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
            printf("输入操作项有误，请重新输入\n");
            break;
        }
    }
}
