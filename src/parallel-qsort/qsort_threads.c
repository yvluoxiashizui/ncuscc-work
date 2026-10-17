#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 四线程快速排序：工作线程上限 4，区间长度 <= CUTOFF 时转插入排序 */

#define MAX_WORKERS 4
#define CUTOFF 4

typedef struct {
    int *a;
    int lo;
    int hi;
} Task;

static pthread_mutex_t slot_lock = PTHREAD_MUTEX_INITIALIZER;
static int slots_used = 0;   /* 已创建工作线程数 */

/* 题面给出的 16 个数 */
static const int DATA[16] = {42, 7, 19, 42, 3, 88, 15, 7, 61, 0, 54, 23, 91, 15, 36, 100};

static int take_slot(void) {
    int ok = 0;
    pthread_mutex_lock(&slot_lock);
    if (slots_used < MAX_WORKERS) {
        slots_used = slots_used + 1;
        ok = 1;
    }
    pthread_mutex_unlock(&slot_lock);
    return ok;
}

static void swap_int(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

static void insertion_sort(int *a, int lo, int hi) {
    int i = 0;
    int j = 0;
    int key = 0;
    for (i = lo + 1; i <= hi; i++) {
        key = a[i];
        j = i - 1;
        while (j >= lo && a[j] > key) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = key;
    }
}

/* Lomuto 划分：取区间最右元素为基准，返回基准的最终下标 */
static int partition(int *a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo;
    int j = 0;
    for (j = lo; j < hi; j++) {
        if (a[j] < pivot) {
            swap_int(&a[i], &a[j]);
            i = i + 1;
        }
    }
    swap_int(&a[i], &a[hi]);
    return i;
}

static void sort_range(int *a, int lo, int hi);

static void *worker_main(void *arg) {
    Task *t = (Task *)arg;
    sort_range(t->a, t->lo, t->hi);
    free(t);
    return NULL;
}

static void sort_range(int *a, int lo, int hi) {
    int p = 0;
    int plo = 0;
    int phi = 0;
    int slo = 0;
    int shi = 0;

    if (lo >= hi) {
        return;
    }
    if (hi - lo + 1 <= CUTOFF) {
        insertion_sort(a, lo, hi);
        return;
    }

    p = partition(a, lo, hi);

    /* 较大一侧交给新线程，另一侧留在当前线程 */
    if ((p - lo) >= (hi - p)) {
        plo = lo;
        phi = p - 1;
        slo = p + 1;
        shi = hi;
    } else {
        plo = p + 1;
        phi = hi;
        slo = lo;
        shi = p - 1;
    }

    if ((phi - plo + 1) > CUTOFF && take_slot()) {
        Task *t = (Task *)malloc(sizeof *t);
        pthread_t tid;
        if (t != NULL) {
            t->a = a;
            t->lo = plo;
            t->hi = phi;
            if (pthread_create(&tid, NULL, worker_main, t) == 0) {
                sort_range(a, slo, shi);   /* 当前线程做另一侧 */
                pthread_join(tid, NULL);   /* 汇合 */
                return;
            }
            free(t);
        }
    }

    /* 名额用尽或区间太小：串行处理两侧 */
    sort_range(a, slo, shi);
    sort_range(a, plo, phi);
}

static int cmp_int(const void *x, const void *y) {
    int a = *(const int *)x;
    int b = *(const int *)y;
    return (a > b) - (a < b);
}

int main(int argc, char **argv) {
    int data[4096];
    int expect[4096];
    int n = 16;
    int i = 0;
    int inversions = 0;
    int same = 0;
    struct timespec t0;
    struct timespec t1;
    double ms = 0.0;

    if (argc >= 3 && strcmp(argv[1], "rand") == 0) {
        n = atoi(argv[2]);
        if (n < 1) {
            n = 1;
        }
        if (n > 4096) {
            n = 4096;
        }
        srand(argc >= 4 ? (unsigned)atoi(argv[3]) : 1u);
        for (i = 0; i < n; i++) {
            data[i] = rand() % 201 - 100;
        }
    } else {
        n = 16;
        for (i = 0; i < n; i++) {
            data[i] = DATA[i];
        }
    }

    memcpy(expect, data, (size_t)n * sizeof data[0]);
    qsort(expect, (size_t)n, sizeof expect[0], cmp_int);

    clock_gettime(CLOCK_MONOTONIC, &t0);
    sort_range(data, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (i = 1; i < n; i++) {
        if (data[i - 1] > data[i]) {
            inversions = inversions + 1;
        }
    }
    same = (memcmp(data, expect, (size_t)n * sizeof data[0]) == 0);

    printf("n=%d\n", n);
    printf("排序结果:");
    for (i = 0; i < n && i < 64; i++) {
        printf(" %d", data[i]);
    }
    if (n > 64) {
        printf(" ...");
    }
    printf("\n");
    printf("相邻逆序对=%d\n", inversions);
    printf("与 qsort 结果一致=%s\n", same ? "是" : "否");
    printf("创建工作线程=%d（上限 %d）\n", slots_used, MAX_WORKERS);

    ms = (double)(t1.tv_sec - t0.tv_sec) * 1000.0
       + (double)(t1.tv_nsec - t0.tv_nsec) / 1000000.0;
    printf("耗时=%.3f ms\n", ms);

    return (inversions == 0 && same && slots_used <= MAX_WORKERS) ? 0 : 1;
}
