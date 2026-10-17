#include <stdio.h>

/* 固定容量读数窗口：只保留最后 CAPACITY 个读数 */

#define CAPACITY 8

int main(void) {
    int n = 0;
    int i = 0;
    int value = 0;
    int window[CAPACITY];
    int kept = 0;
    int sum = 0;
    int oldest = 0;

    if (scanf("%d", &n) != 1) {
        return 1;
    }

    for (i = 0; i < n; i++) {
        if (scanf("%d", &value) != 1) {
            return 1;
        }
        /* 下标恒在 0..CAPACITY-1，满了覆盖最早的位置 */
        window[i % CAPACITY] = value;
        if (kept < CAPACITY) {
            kept = kept + 1;
        }
    }

    /* 未满时最早在 window[0]；绕回后在下一个将被覆盖的位置 */
    if (kept < CAPACITY) {
        oldest = 0;
    } else {
        oldest = n % CAPACITY;
    }

    sum = 0;
    for (i = 0; i < kept; i++) {
        sum = sum + window[(oldest + i) % CAPACITY];
    }

    printf("kept=%d\n", kept);
    printf("oldest=%d\n", window[oldest]);
    printf("last=%d\n", value);
    printf("sum=%d\n", sum);
    return 0;
}
