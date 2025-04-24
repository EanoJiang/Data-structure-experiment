#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int dimension = 3;  // 维度为3
    int signalCount = 5000;  // 生成5000条数据
    
    // 打开文件
    FILE *file = fopen("data_距离.txt", "w");
    if (file == NULL) {
        printf("无法创建文件\n");
        return 1;
    }
    
    // 初始化随机数生成器
    srand(time(NULL));
    
    // 写入维度和信号数量
    fprintf(file, "%d %d\n", dimension, signalCount);
    
    // 生成并写入参考点数据（随机生成）
    for (int i = 0; i < dimension; i++) {
        double value = (double)(rand() % 100) / 10.0;  // 生成0到10之间的随机浮点数
        fprintf(file, "%.1f ", value);
    }
    fprintf(file, "\n");
    
    // 生成并写入信号数据
    for (int i = 0; i < signalCount; i++) {
        for (int j = 0; j < dimension; j++) {
            double value = (double)(rand() % 100) / 10.0;  // 生成0到10之间的随机浮点数
            fprintf(file, "%.1f ", value);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("已成功生成5000条随机数据并保存到data_距离.txt文件\n");
    
    return 0;
} 