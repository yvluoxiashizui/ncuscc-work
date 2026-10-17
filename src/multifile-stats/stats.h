#ifndef STATS_H
#define STATS_H

/* 总和最大约 1e13，超出 32 位范围 */
typedef struct {
    long long count;
    long long total;
    long long smallest;
    long long largest;
} Stats;

/* 读入 n 与 n 个整数；成功返回 0，失败返回非 0 */
int stats_read(Stats *out);

#endif
