#include <stdio.h>

/* 读数统计（C11）
 * 从标准输入读入 n 个整数读数，按题面格式输出 count / sum / min / max。
 */

int main(void) {
    int n = 0;
    int i = 0;
    int value = 0;
    int sum = 0;
    int min;   /* 不给初值：紧接着就用第一个读数赋值 */
    int max;   /* 同上 */

    if (scanf("%d", &n) != 1) {
        return 1;   /* 没读到 n：非零退出码 */
    }

    /* 第一个读数：它同时是 sum、min、max 的初值。
     * 这里不能用 0 初始化 min/max —— 0 未必出现在输入里：
     * 全正数输入会让 min 卡在 0，全负数输入会让 max 卡在 0。 */
    if (scanf("%d", &value) != 1) {
        return 1;
    }
    sum = value;
    min = value;
    max = value;

    /* 从第二个读数开始，逐个与当前的最小/最大值比较 */
    for (i = 1; i < n; i++) {
        if (scanf("%d", &value) != 1) {
            return 1;   /* 该读到数却没读到：不拿未读入的值参与统计 */
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
