#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("无法创建文件\n");
        return 1;
    }
    
    // 设置随机数种子
    srand(time(NULL));
    
    // 生成5000个随机数并写入文件
    for (int i = 0; i < 5000; i++) {
        fprintf(fp, "%d\n", rand() % 10000);
    }
    
    fclose(fp);
    printf("已成功生成5000个随机数据并写入data.txt\n");
    return 0;
} 