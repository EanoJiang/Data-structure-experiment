#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 计算欧几里得距离
//两个点在多维空间中各维度差值的平方和的平方根
double euclideanDistance(double *point1, double *point2, int dimension) {
    double sum = 0.0;
    for (int i = 0; i < dimension; i++) {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// 计算街区距离（曼哈顿距离）
//两个点在多维空间中各维度差值的绝对值之和。
double manhattanDistance(double *point1, double *point2, int dimension) {
    double sum = 0.0;
    for (int i = 0; i < dimension; i++) {
        sum += fabs(point1[i] - point2[i]);
    }
    return sum;
}

// 计算余弦距离
//两个向量之间的余弦相似度，并将其转换为距离1 - cos(θ)
double cosineDistance(double *point1, double *point2, int dimension) {
    double dotProduct = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;
    
    for (int i = 0; i < dimension; i++) {
        dotProduct += point1[i] * point2[i];
        norm1 += point1[i] * point1[i];
        norm2 += point2[i] * point2[i];
    }
    // 边界条件：避免除以零
    if (norm1 == 0.0 || norm2 == 0.0) {
        return 1.0; // 最大距离
    }
    
    // 余弦相似度转换为余弦距离：1 - cos(θ)
    return 1.0 - (dotProduct / (sqrt(norm1) * sqrt(norm2)));
}

// 计算多维信号中各元素到参考点的距离
//distancesType: 0-欧几里得距离，1-曼哈顿距离，2-余弦距离
void calculateDistances(double **signals, int signalCount, int dimension, 
                        double *referencePoint, double *results, int distanceType) {
    for (int i = 0; i < signalCount; i++) {
        switch (distanceType) {
            case 0: // 欧几里得距离
                results[i] = euclideanDistance(signals[i], referencePoint, dimension);
                break;
            case 1: // 街区距离
                results[i] = manhattanDistance(signals[i], referencePoint, dimension);
                break;
            case 2: // 余弦距离
                results[i] = cosineDistance(signals[i], referencePoint, dimension);
                break;
            default:
                printf("不支持的距离类型\n");
                return;
        }
    }
}

// 打印距离结果
void printDistances(double *results, int count) {
    printf("各元素到参考点的距离：\n");
    for (int i = 0; i < count; i++) {
        printf("元素%d: %.4f\n", i + 1, results[i]);
    }
}

// 保存距离结果到文件
void saveDistancesToFile(double *results, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建文件: %s\n", filename);
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%.6f\n", results[i]);
    }
    
    fclose(file);
    printf("距离数据已保存到文件: %s\n", filename);
}

// 逐层释放分配的内存
void freeMemory(double **signals, int count) {
    for (int i = 0; i < count; i++) {
        free(signals[i]);
    }
    free(signals);
}

// 从文件读取信号数据
int readSignalData(const char *filename, double ***signals, int *signalCount, int *dimension, double **referencePoint) {
    FILE *file = fopen(filename, "r");
    //边界条件：文件不存在
    if (file == NULL) {
        printf("无法打开文件: %s\n", filename);
        return 0;
    }
    
    // 读取维度和信号数量
    //边界条件：维度和信号数量不是两个整数
    if (fscanf(file, "%d %d", dimension, signalCount) != 2) {
        printf("读取维度和信号数量失败\n");
        fclose(file);
        return 0;
    }
    
    // 分配参考点内存
    *referencePoint = (double *)malloc(*dimension * sizeof(double));
    // 边界条件：参考点不存在
    if (*referencePoint == NULL) {
        printf("内存分配失败\n");
        fclose(file);
        return 0;
    }
    
    // 读取参考点数据
    for (int i = 0; i < *dimension; i++) {
        // 边界条件：参考点数据不是一个浮点数
        if (fscanf(file, "%lf", &(*referencePoint)[i]) != 1) {
            printf("读取参考点数据失败\n");
            free(*referencePoint);
            fclose(file);
            return 0;
        }
    }
    
    // 分配信号数据内存
    *signals = (double **)malloc(*signalCount * sizeof(double *));
    // 边界条件：信号数据不存在
    if (*signals == NULL) {
        printf("内存分配失败\n");
        free(*referencePoint);
        fclose(file);
        return 0;
    }
    
    for (int i = 0; i < *signalCount; i++) {
        (*signals)[i] = (double *)malloc(*dimension * sizeof(double));
        // 边界条件：信号数据不存在
        if ((*signals)[i] == NULL) {
            printf("内存分配失败\n");
            for (int j = 0; j < i; j++) {
                free((*signals)[j]);
            }
            free(*signals);
            free(*referencePoint);
            fclose(file);
            return 0;
        }
        
        // 读取每个信号的数据
        for (int j = 0; j < *dimension; j++) {
            // 边界条件：信号数据不是一个浮点数
            if (fscanf(file, "%lf", &(*signals)[i][j]) != 1) {
                printf("读取信号数据失败\n");
                for (int k = 0; k <= i; k++) {
                    free((*signals)[k]);
                }
                free(*signals);
                free(*referencePoint);
                fclose(file);
                return 0;
            }
        }
    }
    
    fclose(file);
    return 1;
}

// 主函数
int main() {
    double **signals = NULL;
    double *referencePoint = NULL;
    int dimension = 0;
    int signalCount = 0;
    
    // 从文件读取数据
    if (!readSignalData("data_距离.txt", &signals, &signalCount, &dimension, &referencePoint)) {
        return 1;
    }
    printf("\n数据读取成功\n");
    
    // 存储结果的数组
    double *results = (double *)malloc(signalCount * sizeof(double));
    if (results == NULL) {
        printf("内存分配失败\n");
        free(referencePoint);
        freeMemory(signals, signalCount);
        return 1;
    }
    
    // 显示读取的数据
    printf("维度: %d\n", dimension);
    printf("信号数量: %d\n", signalCount);
    printf("参考点: ");
    for (int i = 0; i < dimension; i++) {
        printf("%.1f ", referencePoint[i]);
    }
    printf("\n\n信号数据:\n");
    for (int i = 0; i < signalCount; i++) {
        printf("信号 %d: ", i+1);
        for (int j = 0; j < dimension; j++) {
            printf("%.1f ", signals[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    int distanceType;
    printf("请选择距离类型（0：欧几里得距离，1：街区距离，2：余弦距离）：");
    scanf("%d", &distanceType);
    
    // 计算距离
    calculateDistances(signals, signalCount, dimension, referencePoint, results, distanceType);
    
    // 打印结果
    printDistances(results, signalCount);
    
    // 保存距离结果到文件
    saveDistancesToFile(results, signalCount, "计算得出的距离.txt");
    
    // 释放内存
    freeMemory(signals, signalCount);
    free(referencePoint);
    free(results);
    
    return 0;
} 