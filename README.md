# ncuscc-work

考核实践仓库。各阶段任务的源码按题目分目录存放在 `src/` 下，每个目录自成一个可独立编译运行的程序。

## 目录结构

```text
.
├── README.md                    仓库总说明
└── src/
    ├── reading-stats/           题目一：读数统计
    │   └── main.c               C11 实现
    └── device-inspection/       题目二：设备巡检记录
        └── main.c               C11 实现
```

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
