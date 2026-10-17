#include <stdio.h>

#include "stats.h"

int stats_read(Stats *out) {
    long long n = 0;
    long long i = 0;
    long long value = 0;

    if (scanf("%lld", &n) != 1) {
        return 1;
    }
    if (n < 1) {
        return 1;
    }

    /* 第一个读数作为 total/smallest/largest 的初值（极值不能用 0） */
    if (scanf("%lld", &value) != 1) {
        return 1;
    }
    out->count = 1;
    out->total = value;
    out->smallest = value;
    out->largest = value;

    for (i = 1; i < n; i++) {
        if (scanf("%lld", &value) != 1) {
            return 1;
        }
        out->count = out->count + 1;
        out->total = out->total + value;
        if (value < out->smallest) {
            out->smallest = value;
        }
        if (value > out->largest) {
            out->largest = value;
        }
    }

    return 0;
}
