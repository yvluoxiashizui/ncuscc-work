#include "format.h"
#include "stats.h"

int main(void) {
    Stats stats;

    if (stats_read(&stats) != 0) {
        return 1;
    }

    print_report(stats.count, stats.total, stats.smallest, stats.largest);
    return 0;
}
