#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 定义信号元素结构体
typedef struct {
    double distance;  // 计算的距离
} SignalElement;

// 从文件读取距离数据
int readDistancesFromFile(const char* filename, double** values) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return -1;
    }
    
    // 先统计文件中的数据行数
    int count = 0;
    char line[5000];
    double temp;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%lf", &temp) == 1) {
            count++;
        }
    }
    
    if (count <= 0) {
        printf("文件中没有有效数据\n");
        fclose(file);
        return -1;
    }
    
    // 重新定位到文件开头
    rewind(file);
    
    // 分配内存
    *values = (double*)malloc(count * sizeof(double));
    if (*values == NULL) {
        printf("内存分配失败\n");
        fclose(file);
        return -1;
    }
    
    // 读取数据
    int i = 0;
    while (i < count && fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%lf", &(*values)[i]) == 1) {
            i++;
        }
    }
    
    fclose(file);
    return count;
}

// 1. 插入排序 (Insertion Sort)
void insertionSort(SignalElement* array, int size) {
    int i, j;
    SignalElement key;
    
    for (i = 1; i < size; i++) {
        key = array[i];
        j = i - 1;
        //边界条件，如果当前元素比前一个元素小，则直接插入，否则把当前元素向后移动一位
        while (j >= 0 && array[j].distance > key.distance) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

// 2. 希尔排序 (Shell Sort) - 插入排序的改进版
void shellSort(SignalElement* array, int size) {
    // 使用希尔增量序列
    for (int gap = size / 2; gap > 0; gap /= 2) {
        // 对每个增量执行插入排序
        for (int i = gap; i < size; i++) {
            SignalElement temp = array[i];
            int j;
            
            for (j = i; j >= gap && array[j - gap].distance > temp.distance; j -= gap) {
                array[j] = array[j - gap];
            }
            array[j] = temp;
        }
    }
}

// 3. 快速排序 (Quick Sort) - 交换排序的一种
void swap(SignalElement* a, SignalElement* b) {
    SignalElement temp = *a;
    *a = *b;
    *b = temp;
}

int partition(SignalElement* array, int low, int high) {
    // 选择最后一个元素作为基准
    double pivot = array[high].distance;
    int i = low - 1;
    
    for (int j = low; j <= high - 1; j++) {
        if (array[j].distance < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return i + 1;
}

void quickSortRecursive(SignalElement* array, int low, int high) {
    // 递归终止条件：起始索引<结束索引
    if (low < high) {
        int pi = partition(array, low, high);
        quickSortRecursive(array, low, pi - 1);
        quickSortRecursive(array, pi + 1, high);
    }
}

void quickSort(SignalElement* array, int size) {
    quickSortRecursive(array, 0, size - 1);
}

// 4. 选择排序 (Selection Sort)
void selectionSort(SignalElement* array, int size) {
    int i, j, min_idx;
    
    for (i = 0; i < size - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < size; j++) {
            if (array[j].distance < array[min_idx].distance) {
                min_idx = j;
            }
        }
        // 交换找到的最小元素与第一个元素
        if (min_idx != i) {
            SignalElement temp = array[i];
            array[i] = array[min_idx];
            array[min_idx] = temp;
        }
    }
}

// 打印信号元素数组
void printArray(SignalElement* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("距离: %.6f\n", array[i].distance);
    }
    printf("\n");
}


// 复制数组
void copyArray(SignalElement* dest, SignalElement* src, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

// 测试排序算法性能并输出到文件
void testSortingPerformance() {
    FILE* outputFile = fopen("运行时间对比.txt", "w");
    if (outputFile == NULL) {
        printf("无法创建输出文件\n");
        return;
    }
    
    fprintf(outputFile, "排序算法性能测试报告\n");
    fprintf(outputFile, "==========================\n\n");
    
    // 完整文件路径
    const char* filepath = "./计算得出的距离.txt";
    
    // 读取实际数据
    double* distanceValues;
    int size = readDistancesFromFile(filepath, &distanceValues);
    
    if (size <= 0) {
        printf("性能测试读取数据失败或数据为空，尝试使用绝对路径\n");
        filepath = "/Users/User/Desktop/Data structure experiment/PDS01-2042220220821江一诺/实验四/计算得出的距离.txt";
        size = readDistancesFromFile(filepath, &distanceValues);
        
        if (size <= 0) {
            printf("性能测试使用绝对路径仍然失败\n");
            fprintf(outputFile, "无法读取数据文件\n");
            fclose(outputFile);
            return;
        }
    }
    
    printf("\n成功从文件读取了%d个数据用于性能测试\n数据来源: %s\n\n", size, filepath);
    
    // 创建信号元素数组
    SignalElement* originalArray = (SignalElement*)malloc(size * sizeof(SignalElement));
    if (originalArray == NULL) {
        printf("内存分配失败\n");
        free(distanceValues);
        fclose(outputFile);
        return;
    }
    
    // 初始化信号元素数组
    for (int i = 0; i < size; i++) {
        originalArray[i].distance = distanceValues[i];  // 距离值作为距离
    }
    
    fprintf(outputFile, "数据量: %d 个元素\n\n", size);
    fprintf(outputFile, "| 算法名称 | 运行时间 (ms) |\n");
    fprintf(outputFile, "|----------|---------------|\n");
    
    // 为每种排序算法创建副本
    SignalElement* shellArray = (SignalElement*)malloc(size * sizeof(SignalElement));
    SignalElement* quickArray = (SignalElement*)malloc(size * sizeof(SignalElement));
    SignalElement* selectArray = (SignalElement*)malloc(size * sizeof(SignalElement));
    
    if (shellArray == NULL || quickArray == NULL || selectArray == NULL) {
        printf("内存分配失败\n");
        free(originalArray);
        if (shellArray) free(shellArray);
        if (quickArray) free(quickArray);
        if (selectArray) free(selectArray);
        free(distanceValues);
        fclose(outputFile);
        return;
    }
    
    copyArray(shellArray, originalArray, size);
    copyArray(quickArray, originalArray, size);
    copyArray(selectArray, originalArray, size);
    
    // 测试希尔排序
    clock_t start = clock();
    shellSort(shellArray, size);
    clock_t end = clock();
    double shellTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    
    // 测试快速排序
    start = clock();
    quickSort(quickArray, size);
    end = clock();
    double quickTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    
    // 测试选择排序
    start = clock();
    selectionSort(selectArray, size);
    end = clock();
    double selectTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    
    // 输出结果
    fprintf(outputFile, "| 希尔排序 | %13.2f |\n", shellTime);
    fprintf(outputFile, "| 快速排序 | %13.2f |\n", quickTime);
    fprintf(outputFile, "| 选择排序 | %13.2f |\n", selectTime);
    
    // 释放内存
    free(originalArray);
    free(shellArray);
    free(quickArray);
    free(selectArray);
    free(distanceValues);
    
    fprintf(outputFile, "\n注：时间单位为毫秒 (ms)。测试环境：MacOS。\n");
    fprintf(outputFile, "数据来源：%s（%d个数据点）\n", filepath, size);
    fclose(outputFile);
    printf("性能测试完成，结果已保存到运行时间对比.txt文件中\n");
}

int main() {
    // 设置随机数种子
    srand((unsigned int)time(NULL));
    
    // 完整文件路径
    const char* filepath = "./计算得出的距离.txt";
    
    // 读取距离数据
    double* distanceValues;
    int size = readDistancesFromFile(filepath, &distanceValues);
    
    if (size <= 0) {
        printf("读取数据失败或数据为空，请检查文件路径: %s\n", filepath);
        // 尝试绝对路径
        filepath = "/Users/User/Desktop/Data structure experiment/PDS01-2042220220821江一诺/实验四/计算得出的距离.txt";
        printf("尝试使用绝对路径: %s\n", filepath);
        size = readDistancesFromFile(filepath, &distanceValues);
        
        if (size <= 0) {
            printf("使用绝对路径仍然失败，请确认文件存在并有读取权限\n");
            return -1;
        }
    }
    
    printf("\n成功读取了%d个数据\n\n", size);
    
    // 创建信号元素数组
    SignalElement* signals = (SignalElement*)malloc(size * sizeof(SignalElement));
    if (signals == NULL) {
        printf("内存分配失败\n");
        free(distanceValues);
        return -1;
    }
    
    // 初始化信号元素数组
    for (int i = 0; i < size; i++) {
        signals[i].distance = distanceValues[i];  // 距离值作为距离
        // 打印少量示例，避免输出过多
        if (i < 10 || i > size - 10) {
            printf("信号[%d]: 距离=%.6f\n", i, signals[i].distance);
            if (i == 10) printf("... (省略中间数据) ...\n");
        }
    }
    
    // 复制数组以便使用不同排序算法
    SignalElement* signalsShell = (SignalElement*)malloc(size * sizeof(SignalElement));
    SignalElement* signalsQuick = (SignalElement*)malloc(size * sizeof(SignalElement));
    SignalElement* signalsSelect = (SignalElement*)malloc(size * sizeof(SignalElement));
    
    if (signalsShell == NULL || signalsQuick == NULL || signalsSelect == NULL) {
        printf("内存分配失败\n");
        free(signals);
        free(distanceValues);
        return -1;
    }
    
    // 复制数组
    for (int i = 0; i < size; i++) {
        signalsShell[i] = signals[i];
        signalsQuick[i] = signals[i];
        signalsSelect[i] = signals[i];
    }
    
    printf("\n排序前 (仅显示前10个元素):\n");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("距离: %.6f\n", signalsShell[i].distance);
    }
    printf("... (总共 %d 个元素) ...\n\n", size);

    // 使用希尔排序
    printf("开始希尔排序...\n");
    clock_t start = clock();
    shellSort(signalsShell, size);
    clock_t end = clock();
    double shellTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    printf("希尔排序完成，用时 %.2f 毫秒\n", shellTime);
    
    printf("希尔排序后 (仅显示前10个元素):\n");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("距离: %.6f\n", signalsShell[i].distance);
    }
    printf("... (总共 %d 个元素) ...\n\n", size);
    
    // 使用快速排序
    printf("开始快速排序...\n");
    start = clock();
    quickSort(signalsQuick, size);
    end = clock();
    double quickTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    printf("快速排序完成，用时 %.2f 毫秒\n", quickTime);
    
    printf("快速排序后 (仅显示前10个元素):\n");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("距离: %.6f\n", signalsQuick[i].distance);
    }
    printf("... (总共 %d 个元素) ...\n\n", size);
    
    // 使用选择排序
    printf("开始选择排序...\n");
    start = clock();
    selectionSort(signalsSelect, size);
    end = clock();
    double selectTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // 毫秒
    printf("选择排序完成，用时 %.2f 毫秒\n", selectTime);
    
    printf("选择排序后 (仅显示前10个元素):\n");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("距离: %.6f\n", signalsSelect[i].distance);
    }
    printf("... (总共 %d 个元素) ...\n\n", size);
    
    // 释放内存
    free(signals);
    free(signalsShell);
    free(signalsQuick);
    free(signalsSelect);
    free(distanceValues);
    
    // 执行性能测试并输出到文件
    printf("\n开始排序算法性能测试...\n");
    testSortingPerformance();
    
    return 0;
} 