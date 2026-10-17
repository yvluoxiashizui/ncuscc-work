# ncuscc-work

考核实践仓库。各阶段任务的源码按题目分目录存放在 `src/` 下，每个目录自成一个可独立构建运行的程序。

> **材料来源与局限**
>
> 本仓库的题目来自课程平台。完成期间平台发生故障，容器与初始工程无法获取：题目一仅拿到 `c/main.c` 模板，题目二至题目五的初始工程、越界缺陷版本、`data.txt` 与全部 `samples/` 文件均未能下载。
>
> 因此题目二至题目五的实现，以及前两题的测试用例，均仅依据题面 `README.md` 与 `hints.md` 从公开契约出发编写或重建，并未在初始工程上做"修复"。每题的验证方式与结果见对应小节。

## 目录结构

```text
.
├── README.md                    仓库总说明
├── REPORT.md                    技术报告（流程、语法、文件组织、命令行记录）
└── src/
    ├── reading-stats/           题目一：读数统计
    │   └── main.c
    ├── device-inspection/       题目二：设备巡检记录
    │   └── main.c
    ├── reading-window/          题目三：固定容量读数窗口
    │   └── main.c
    ├── multifile-stats/         题目四：多文件统计工具
    │   ├── CMakeLists.txt
    │   ├── main.c
    │   ├── stats.h / stats.c
    │   └── format.h / format.c
    └── parallel-qsort/          题目五（可选报告）：四线程快速排序
        ├── qsort_threads.c      源码，留档用（平台不需要）
        └── report.md            报告本体，提交时复制到工作区根目录
```

前三题是单文件程序，用 `gcc` 直接编译；题目四是多文件工程，用 CMake 构建（可执行目标名固定为 `main`，产物路径 `build/main`）；题目五是可选报告，只提交 `report.md`，源码不提交。

## 环境与工具链

在 Windows 上通过 WSL2 使用 Ubuntu，工具链如下（版本经实测记录，便于复现）：

| 用途 | 工具 | 版本 |
| --- | --- | --- |
| 操作系统 | Ubuntu (WSL2) | 26.04 LTS，内核 6.18.33.2-microsoft-standard-WSL2 |
| 编译器 | gcc / g++ | 15.2.0 (Ubuntu 15.2.0-16ubuntu1) |
| 调试器 | gdb | 17.1 |
| 构建 | GNU Make / CMake | 4.4.1 / 4.2.3 |
| 版本控制 | git | 2.53.0 |
| 编辑器 | vim / VS Code (WSL Remote) | 9.1 / - |

## 题目一：读数统计

### 题目描述

**输入**

- 第一行：整数 `n`（`1 <= n <= 10000`），读数个数。
- 第二行：`n` 个整数，用空格分隔，每个在闭区间 `[-1000, 1000]`。

**输出**

向标准输出恰好输出 4 行（`=` 两边不要有空格）：

```text
count=<n>
sum=<所有读数之和>
min=<最小读数>
max=<最大读数>
```

程序必须以退出码 0 结束。`sum` 在 32 位 `int` 范围内。

### 构建与运行

```bash
gcc -std=c11 -Wall -Wextra -o main src/reading-stats/main.c
./main
```

`-Wall -Wextra` 用于提前发现问题；判题机使用不带告警开关的 `gcc -std=c11`。

### 实现要点

- **流式读入，不使用数组**：读一个数就更新一次统计量，空间开销与 `n` 无关。
- **`min` / `max` 用第一个读到的数初始化**，而不是 0。0 未必出现在输入里：若用 0 初始化，全正数输入会让 `min` 一直卡在 0，全负数输入会让 `max` 一直卡在 0。因此先读第一个读数并同时赋给 `sum`、`min`、`max`，循环从第二个读数开始。
- **每次 `scanf` 都检查返回值**：返回值不等于 1 表示没有成功读入，此时以非零退出码退出，不拿未读入的值参与统计。
- **输出严格对齐契约**：恰好 4 行、`=` 两边无空格、每行以换行结尾、退出码 0。

### 验证

```bash
printf '5\n3 -1 4 -1 5\n' | ./main
# count=5 sum=10 min=-1 max=5   （退出码 0）

printf '1\n-7\n' | ./main
# count=1 sum=-7 min=-7 max=-7

printf '10\n1000 -1000 0 999 -999 1 0 -1 500 -500\n' | ./main
# count=10 sum=0 min=-1000 max=1000
```

| 用例 | 覆盖点 | 结果 |
| --- | --- | --- |
| `5 / 3 -1 4 -1 5` | 正负混合 | 通过 |
| `1 / -7` | `n = 1` 且为负数 | 通过 |
| `10 / 1000 -1000 0 999 -999 1 0 -1 500 -500` | 取值上下界 | 通过 |
| `3 / 7 3 9` | 全正数，`min` 不被 0 卡住 | 通过 |
| `4 / -5 -2 -9 -1` | 全负数，`max` 不被 0 卡住 | 通过 |
| `5 / 4 4 4 4 4` | 全部相同 | 通过 |
| `n = 10000` | 规模上界 | 通过 |

## 题目二：设备巡检记录

### 题目描述

**输入**

- 第一行：记录数 `n`（`1 <= n <= 1000`）。
- 随后 `n` 行，每行三个字段，用单个空格分隔：`<type> <status> <hours>`
  - `type` 取值：`printer`、`scanner`、`router`、`server`
  - `status` 取值：`ok`、`repair`
  - `hours`：整数，`0 <= hours <= 100000`

**输出**

按固定顺序输出四行，每行是「类型、记录数、总小时数」：

```text
printer <count> <totalHours>
scanner <count> <totalHours>
router  <count> <totalHours>
server  <count> <totalHours>
```

即使某类没有记录也必须输出，计数和总小时数为 0。最后一行输出总小时数最大的类型：

```text
busiest <type>
```

若多个类型总小时数相同，取固定顺序中最靠前的类型。退出码必须为 0。

### 构建与运行

```bash
gcc -std=c11 -Wall -Wextra -o main src/device-inspection/main.c
./main
```

### 实现要点

- **按下标聚合**：用长度为 4 的 `count[]` 与 `hours[]`，下标顺序与固定输出顺序一致（`printer`、`scanner`、`router`、`server`），读入时把类型名映射成下标。
- **类型名整串比较**：用 `strcmp` 完整比对，不能只比较首字符（`scanner` 与 `server` 首字母相同）。
- **零记录类型照常输出**：四行由数组循环生成，而不是只输出出现过的类型。
- **并列取靠前**：从下标 0 向 3 扫描，判断条件用严格大于 `>`，相等时不替换，于是并列时保留顺序更靠前的类型。
- **字符串读取限定宽度**：`scanf("%15s %15s %d", ...)`，缓冲区为 `char type[16]`；`%s` 本身不检查长度，不限定宽度在输入超长时会写越界。

### 验证

```bash
printf '6\nprinter ok 12\nscanner repair 30\nrouter ok 45\nprinter repair 8\nserver ok 100\nrouter repair 15\n' | ./main
# printer 2 20 / scanner 1 30 / router 2 60 / server 1 100 / busiest server

printf '3\nscanner ok 0\nprinter repair 5\nserver ok 5\n' | ./main
# printer 1 5 / scanner 1 0 / router 0 0 / server 1 5 / busiest printer

printf '2\nrouter repair 5\nrouter repair 7\n' | ./main
# printer 0 0 / scanner 0 0 / router 2 12 / server 0 0 / busiest router
```

| 用例 | 覆盖点 | 结果 |
| --- | --- | --- |
| 样例 1 | 多类型混合、按类型累加 | 通过 |
| 样例 2 | `printer` 与 `server` 总小时数并列，取靠前者 | 通过 |
| 样例 3 | 只有一种类型，其余输出 0 0 | 通过 |
| `1 / printer ok 0` | 四类均为 0，并列取第一个 | 通过 |
| `1 / server ok 100000` | 单条记录取到小时数上界 | 通过 |
| `n = 1000` 全部 `server ok 100000` | 计数与总小时数上界（100000000） | 通过 |

## 题目三：固定容量读数窗口

### 题目描述

**输入**

- 第一行：整数 `n`（`1 <= n <= 100000`）。
- 第二行：`n` 个整数，每个在 `[-1000, 1000]`。

**正确行为**

程序只保留最后 8 个读数；若不足 8 个，则全部保留。按输入顺序，最早保留的是 `oldest`，最后读到的是 `last`。

**输出**

恰好四行：

```text
kept=<保留个数>
oldest=<最早的保留值>
last=<最后的读数值>
sum=<保留值之和>
```

退出码必须为 0。`kept` 最大为 8，因此 `sum` 在普通 `int` 范围内。

### 构建与运行

```bash
gcc -std=c11 -Wall -Wextra -o main src/reading-window/main.c
./main
```

### 实现要点

- **环形缓冲**：容量固定为 `CAPACITY = 8`，第 `i` 个读数写入 `window[i % CAPACITY]`，下标恒在 `0..7`，容量用尽后自动覆盖最早写入的位置；不使用随输入增长的数组。
- **保留计数有上限**：`kept` 增加到 `CAPACITY` 后不再增加，因此 `kept` 最大为 8。
- **`oldest` 分两种情况**：未填满时最早的值在 `window[0]`；绕回后，最早的值位于**下一个将被覆盖的位置** `window[n % CAPACITY]`，而不是固定的 `window[0]`。
- **求和范围与保留内容一致**：从 `oldest` 起按环形顺序累加 `kept` 个值，只对实际保留的读数求和。

### 验证

```bash
printf '5\n3 1 4 1 5\n' | ./main
# kept=5 oldest=3 last=5 sum=14

printf '12\n1 2 3 4 5 6 7 8 9 10 11 12\n' | ./main
# kept=8 oldest=5 last=12 sum=68   （保留 5 6 7 8 9 10 11 12）

printf '8\n8 7 6 5 4 3 2 1\n' | ./main
# kept=8 oldest=8 last=1 sum=36
```

| 用例 | 覆盖点 | 结果 |
| --- | --- | --- |
| 样例 1（`n = 5`） | 容量未满 | 通过 |
| 样例 2（`n = 12`） | 超过容量，发生绕回 | 通过 |
| 样例 3（`n = 8`） | 刚好填满，`oldest` 为 `window[0]` | 通过 |
| `n = 1` | 最少记录 | 通过 |
| `n = 9` | 绕回一位 | 通过 |
| `n = 16` | 绕回两轮 | 通过 |
| `n = 100000` 末 8 个为 `1..8` | 规模上界与窗口顺序 | 通过 |
| 末尾 8 个全为 `-1000` | 负值、求和下界 `-8000` | 通过 |
| AddressSanitizer / UndefinedBehaviorSanitizer（样例 2 与 n=100000） | 无越界写、无未定义行为 | 通过 |

## 题目四：多文件统计工具

### 题目描述

**输入**

- 第一行：整数 `n`（`1 <= n <= 10000`）。
- 第二行：`n` 个整数，每个在 `[-1000000000, 1000000000]`。

**输出**

恰好四行：

```text
count=<n>
sum=<所有整数之和>
min=<最小值>
max=<最大值>
```

退出码必须为 0。`sum` 可能超过 32 位整数范围，必须使用 64 位整数。

### 工程结构

```text
src/multifile-stats/
├── CMakeLists.txt        构建入口，可执行目标名 main
├── main.c                入口：调用统计与格式化
├── stats.h / stats.c     统计：读入并计算 count / total / smallest / largest
└── format.h / format.c   格式化：按 count / sum / min / max 输出四行
```

### 构建与运行

```bash
cd src/multifile-stats
cmake -S . -B build
cmake --build build
./build/main
```

### 实现要点

- **构建入口列出全部实现文件**：`add_executable(main main.c stats.c format.c)` 漏掉实现文件会在链接阶段报 `undefined reference`。
- **声明与定义保持一致**：`print_report` 在 `format.h` 中声明为 `(long long count, long long total, long long smallest, long long largest)`，`format.c` 的定义必须逐字相同。C 会在同一源文件内检查声明与定义的类型冲突；C++ 会把不同参数类型视为重载，随后因该签名没有定义而链接失败。
- **64 位求和**：读数绝对值可到 `1000000000`，`n` 可到 `10000`，总和最大 `1e13`，超出 32 位 `int` 范围，因此统计量统一使用 `long long`：输入用 `%lld` 读取，输出用 `%lld` 打印。
- **极值从第一个读数初始化**：`smallest` / `largest` 不用 0 作初值。
- **标准版本显式声明**：`CMakeLists.txt` 中设置 `CMAKE_C_STANDARD 11`、`CMAKE_C_STANDARD_REQUIRED ON` 与 `CMAKE_C_EXTENSIONS OFF`，编译器实际收到的是 `-std=c11`（而不是默认的 `-std=gnu11`），与判题命令一致。

### 验证

```bash
cd src/multifile-stats
cmake -S . -B build && cmake --build build

printf '3\n1000000000 -1000000000 1\n' | ./build/main
# count=3 sum=1 min=-1000000000 max=1000000000

printf '5\n1000000000 1000000000 1000000000 1000000000 1000000000\n' | ./build/main
# count=5 sum=5000000000 min=1000000000 max=1000000000   （sum 超过 32 位）

printf '1\n0\n' | ./build/main
# count=1 sum=0 min=0 max=0
```

| 用例 | 覆盖点 | 结果 |
| --- | --- | --- |
| 样例 1 | 正负混合、取值上下界 | 通过 |
| 样例 2 | `sum = 5000000000` 超出 32 位 | 通过 |
| 样例 3 | `n = 1`，读数为 0 | 通过 |
| `n = 1 / -1000000000` | 取值下界 | 通过 |
| `n = 2 / 1000000000 -1000000000` | 求和为 0 | 通过 |
| `n = 10000` 全为 `1000000000` | 求和上界 `10000000000000` | 通过 |

## 题目五（可选报告）：四线程快速排序

### 题目说明

可选报告任务：用不超过 4 个工作线程实现快速排序，并提交 `report.md`。平台不编译、不运行、不判对错，只做人工审阅；报告须为 UTF-8 Markdown 常规文件，且不超过 1 MiB。

### 提交物

| 文件 | 说明 |
| --- | --- |
| `src/parallel-qsort/report.md` | 报告本体，提交时复制到工作区根目录 |
| `src/parallel-qsort/qsort_threads.c` | 源码，留档用（平台不要求提交） |

### 本地实现与验证

```bash
gcc -std=c11 -Wall -Wextra -pthread -o qs src/parallel-qsort/qsort_threads.c
./qs                 # 题面给出的 16 个整数
./qs rand 500 42     # 随机数据复核：n=500，随机种子 42
```

实现方式：C11 + `pthread`；Lomuto 划分，取区间最右元素为基准；划分后较大的一侧交给新线程、较小的一侧留在当前线程；用互斥锁保护的计数器把累计创建的工作线程数限制在 4 以内；区间长度不超过 `CUTOFF = 4` 时改用插入排序；创建线程的那一帧内调用 `pthread_join` 汇合。

| 检查 | 结果 |
| --- | --- |
| 题面 16 个整数 | 排序结果 `0 3 7 7 15 15 19 23 36 42 42 54 61 88 91 100`，退出码 0 |
| 相邻元素无逆序 | 逆序对数为 0 |
| 与标准库 `qsort` 对照 | `memcmp` 完全一致 |
| 工作线程数 | 题面数据用 2 个；各规模下最多 4 个，未超过上限 |
| ThreadSanitizer（`-fsanitize=thread`） | 无数据竞争报告 |
| 随机压力：200 组种子、`n=500` | 全部通过 |

### 报告结构

`report.md` 按题面要求分四节：线程、划分与汇合、正确性、环境限制。环境一节写明实验发生在单 CPU 沙盒中，耗时只作观察记录，不作为并行加速成绩。

## 关于 AI 参与

本仓库的代码与文档在 AI 辅助下完成，所用模型为 DeepSeek 的 `deepseek-flash`（通过 DeepSeek Harness 运行）。所有结论都用本地编译与测试复核过：各题的编译命令、样例逐字节比对、退出码检查与边界用例结果见对应小节。
