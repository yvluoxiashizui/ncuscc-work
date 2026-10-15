#include <stdio.h>

/* 读数统计（C11）
 * 读入 n 和 n 个整数，按题面格式输出 count / sum / min / max。
 */

int main(void) {
    int n = 0;
    int i = 0;
    int value = 0;
    int sum = 0;
    int min = 0;
    int max = 0;

    if (scanf("%d", &n) != 1) {
        return 1;   /* 没读到 n：非零退出码 */
    }

    for (i = 0; i < n; i++) {
        scanf("%d", &value);
        sum = sum + value;
    }

    printf("count=%d\n", n);
    printf("sum=%d\n", sum);
    printf("min=%d\n", min);
    printf("max=%d\n", max);
    return 0;
}
