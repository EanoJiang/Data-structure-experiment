#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATH_LENGTH 256
#define NUM_CHARS 256

typedef struct {
    char data;
    int weight;
} CharCount;

// 显示字符的函数，处理不可打印字符
void displayChar(char ch) {
    if (isprint(ch)) {
        printf("'%c'", ch);
    } else {
        switch (ch) {
            case '\n': printf("'\\n'"); break;
            case '\t': printf("'\\t'"); break;
            case '\r': printf("'\\r'"); break;
            case ' ':  printf("' '"); break;
            default:   printf("ASCII码: %d", (unsigned char)ch);
        }
    }
}

int main(int argc, char* argv[]) {
    char srcFile[MAX_PATH_LENGTH];
    
    // 处理命令行参数
    if (argc > 1) {
        strcpy(srcFile, argv[1]);
    } else {
        strcpy(srcFile, "data.txt"); // 默认文件名
        printf("未指定文件名，将使用默认文件: data.txt\n");
    }

    FILE* fp = fopen(srcFile, "r");
    if (fp == NULL) {
        printf("错误: 无法打开文件 '%s'\n", srcFile);
        return 1;
    }

    CharCount charStats[NUM_CHARS] = {0}; // 初始化结构体数组
    for (int i = 0; i < NUM_CHARS; i++) {
        charStats[i].data = (char)i;
        charStats[i].weight = 0;
    }

    int ch;
    long totalChars = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch >= 0 && ch < NUM_CHARS) {
            charStats[ch].weight++; // 增加字符的出现次数
            totalChars++;
        }
    }

    fclose(fp);

    if (totalChars == 0) {
        printf("文件为空或没有可读取的字符\n");
        return 0;
    }

    // 打印汇总信息
    printf("文件: %s\n", srcFile);
    printf("总字符数: %ld\n", totalChars);
    printf("字符频率统计:\n");
    printf("-----------------------------\n");
    printf("| 字符  | 出现次数 | 频率(％) |\n");
    printf("-----------------------------\n");

    int distinctChars = 0;
    for (int i = 0; i < NUM_CHARS; i++) {
        if (charStats[i].weight > 0) {
            distinctChars++;
            printf("| ");
            displayChar(charStats[i].data);
            printf(" | %-8d | %6.2f%% |\n", 
                   charStats[i].weight, 
                   (charStats[i].weight * 100.0) / totalChars);
        }
    }
    printf("-----------------------------\n");
    printf("不同字符总数: %d\n", distinctChars);

    return 0;
} 