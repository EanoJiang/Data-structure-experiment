#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ASCII码256个，所以长度设置为256
#define MAX_WORD_LENGTH 256
#define NUM_CHARS 256

typedef struct {
    char data;
    int weight;
    int parent;
    int lch;
    int rch;
} Histogram;

int main() {
    char srcFile[MAX_WORD_LENGTH];
    strcpy(srcFile, "data.txt");

    FILE* fp = fopen(srcFile, "r");
    if (fp == NULL) {
        printf("打开失败\n");
        exit(1);
    }

    Histogram HT[NUM_CHARS] = {0}; // 初始化结构体数组
    for (int i = 0; i < NUM_CHARS; i++) {
        HT[i].data = (char)i;
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lch = 0;
        HT[i].rch = 0;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        HT[ch].weight++; // 增加字符的出现次数
    }

    fclose(fp);

    // 打印输出结果
    for (int i = 0; i < NUM_CHARS; i++) {
        if (HT[i].weight > 0) {
            printf("字符: %c 出现的频率: %d\n", HT[i].data, HT[i].weight);
        }
    }

    return 0;
}