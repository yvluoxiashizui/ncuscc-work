#include <stdio.h>

#include "format.h"

void print_report(long long count, long long total, long long smallest, long long largest) {
    printf("count=%lld\n", count);
    printf("sum=%lld\n", total);
    printf("min=%lld\n", smallest);
    printf("max=%lld\n", largest);
}
