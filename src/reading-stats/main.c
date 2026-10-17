#include <stdio.h>

/* 读数统计：输出 count / sum / min / max */

int main(void) {
    int n = 0;
    int i = 0;
    int value = 0;
    int sum = 0;
    int min;   /* 由第一个读数赋值 */
    int max;

    if (scanf("%d", &n) != 1) {
        return 1;
    }

    /* 第一个读数作为 sum、min、max 的初值；极值不能用 0 初始化 */
    if (scanf("%d", &value) != 1) {
        return 1;
    }
    sum = value;
    min = value;
    max = value;

    for (i = 1; i < n; i++) {
        if (scanf("%d", &value) != 1) {
            return 1;
        }
        if (value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
        sum = sum + value;
    }

    printf("count=%d\n", n);
    printf("sum=%d\n", sum);
    printf("min=%d\n", min);
    printf("max=%d\n", max);
    return 0;
}
