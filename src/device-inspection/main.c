#include <stdio.h>
#include <string.h>

/* 设备巡检记录（C11）
 * 读入 n 条记录（类型 状态 小时数），按固定顺序输出每类的记录数与总小时数，
 * 最后输出总小时数最大的类型。
 */

int main(void) {
    /* 下标与题面要求的固定输出顺序一致 */
    const char *names[4] = {"printer", "scanner", "router", "server"};
    int count[4] = {0, 0, 0, 0};
    int hours[4] = {0, 0, 0, 0};

    char type[16];     /* 最长的类型名 "printer" / "scanner" 为 7 个字符 */
    char status[16];
    int value = 0;
    int n = 0;
    int i = 0;
    int k = 0;
    int best = 0;

    if (scanf("%d", &n) != 1) {
        return 1;
    }

    for (i = 0; i < n; i++) {
        /* 限定字符串宽度，避免输入超长时写越界 */
        if (scanf("%15s %15s %d", type, status, &value) != 3) {
            return 1;
        }

        for (k = 0; k < 4; k++) {
            if (strcmp(type, names[k]) == 0) {
                break;
            }
        }
        if (k == 4) {
            return 1;
        }

        count[k] = count[k] + 1;
        hours[k] = hours[k] + value;
    }

    for (i = 0; i < 4; i++) {
        printf("%s %d %d\n", names[i], count[i], hours[i]);
    }

    /* 用严格大于比较：并列时保留固定顺序中更靠前的类型 */
    best = 0;
    for (i = 1; i < 4; i++) {
        if (hours[i] > hours[best]) {
            best = i;
        }
    }
    printf("busiest %s\n", names[best]);

    return 0;
}
