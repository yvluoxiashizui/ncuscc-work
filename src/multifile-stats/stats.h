#ifndef STATS_H
#define STATS_H

/* 统计结果：count 为读入个数，total 为总和，smallest / largest 为极值。
 * 读数范围可到 ±1000000000，n 可到 10000，因此总和必须用 64 位整数。 */
typedef struct {
    long long count;
    long long total;
    long long smallest;
    long long largest;
} Stats;

/* 从标准输入读入 n 及其后的 n 个整数并完成统计。
 * 成功返回 0，读取失败返回非 0。 */
int stats_read(Stats *out);

#endif
