#include <stdio.h>
#include <string.h>

/* 设备巡检记录：按固定顺序汇总每类记录数与小时数，并输出最忙的类型 */

int main(void) {
    /* 下标与输出顺序一致 */
    const char *names[4] = {"printer", "scanner", "router", "server"};
    int count[4] = {0, 0, 0, 0};
    int hours[4] = {0, 0, 0, 0};

    char type[16];     /* 最长类型名 7 个字符 */
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
        /* 限定宽度，避免输入超长时写越界 */
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

    /* 严格大于：并列时保留靠前的类型 */
    for (i = 1; i < 4; i++) {
        if (hours[i] > hours[best]) {
            best = i;
        }
    }
    printf("busiest %s\n", names[best]);

    return 0;
}
