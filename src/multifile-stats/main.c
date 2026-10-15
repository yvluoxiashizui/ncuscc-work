#include "format.h"
#include "stats.h"

/* 入口：读入并统计，然后按契约输出报表 */
int main(void) {
    Stats stats;

    if (stats_read(&stats) != 0) {
        return 1;
    }

    print_report(stats.count, stats.total, stats.smallest, stats.largest);
    return 0;
}
