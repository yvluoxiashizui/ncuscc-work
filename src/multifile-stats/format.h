#ifndef FORMAT_H
#define FORMAT_H

/* 报表输出：参数顺序为 count、total、smallest、largest，
 * 输出顺序固定为 count、sum、min、max 四行。 */
void print_report(long long count, long long total, long long smallest, long long largest);

#endif
