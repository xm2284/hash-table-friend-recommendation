# 哈希表实现智能推荐好友系统

[![C++](https://img.shields.io/badge/C++-00599C?logo=c%2B%2B&logoColor=white&style=flat-square)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-1F1F1F?style=flat-square)](LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-00C853?style=flat-square)]()

本系统是一个 C++ 控制台程序，使用哈希表存储用户信息，并根据兴趣标签推荐相似好友。本项目为数据结构课程设计，采用菜单交互方式，演示了哈希表存储、冲突处理、遍历与排序在完整系统中的应用。

---

## 目录

- [功能特性](#功能特性)
- [系统架构](#系统架构)
- [数据结构设计](#数据结构设计)
- [推荐算法](#推荐算法)
- [环境与运行](#环境与运行)
- [文件结构](#文件结构)

---

## 功能特性

- 添加用户：输入编号、姓名、年龄和兴趣标签，保存到哈希表中
- 查询用户：根据编号快速查找并显示用户信息
- 删除用户：根据编号删除指定用户
- 显示全部用户：按桶位置展示所有已存用户
- 推荐好友：根据兴趣标签计算相似度，推荐相似好友
- 排序输出：推荐结果按相似度从高到低排列
- 内置测试数据：一键加载示例用户，方便演示

---

## 系统架构

系统由五个主要模块组成：

| 模块 | 功能说明 |
|------|----------|
| 菜单交互模块 | 显示功能菜单，根据用户选择调用对应功能 |
| 用户管理模块 | 完成用户的添加、查询、删除与显示 |
| 哈希表存储模块 | 通过 `id % table_size` 将用户分配到对应桶中 |
| 好友推荐模块 | 遍历其他用户，比较兴趣标签，统计共同兴趣 |
| 排序输出模块 | 按相似度从高到低输出推荐结果 |

![系统架构](screenshots/02-system-structure.png)

---

## 数据结构设计

### 哈希表

系统的核心数据结构是长度为 10 的数组，每个位置称为一个"桶"，桶内使用 `vector<User>` 保存用户：

```cpp
const int table_size = 10;
vector<User> hashTable[table_size];
```

用户通过以下哈希函数映射到对应桶中：

```cpp
int hashFunction(int id) {
    return id % table_size;
}
```

当多个用户被映射到同一个桶时，它们会一起存放在同一个 vector 中，这就是哈希冲突的处理方式。

![哈希表桶数组](screenshots/03-hash-table-buckets.png)

![哈希冲突](screenshots/05-hash-collision.png)

### 用户结构体

```cpp
struct User {
    int id;
    string name;
    int age;
    vector<string> interests;
};

struct RecommendResult {
    User user;
    int sameInterestCount;
    double similarity;
};
```

`User` 用于保存一个用户的基本信息；`RecommendResult` 用于保存推荐结果，包括候选好友、共同兴趣数量和相似度。

![用户信息结构](screenshots/06-user-structure.png)

---

## 推荐算法

推荐好友的过程如下：

1. 根据用户编号找到目标用户
2. 遍历哈希表中的其他用户
3. 比较兴趣标签，统计共同兴趣数量
4. 计算相似度
5. 按相似度从高到低排序输出推荐结果

**相似度公式：**

```
相似度 = 共同兴趣数量 / 目标用户兴趣数量
```

![推荐流程图](screenshots/07-recommend-flow.png)

![兴趣标签比较](screenshots/08-interest-compare.png)

![相似度公式](screenshots/09-similarity-formula.png)

![结果排序](screenshots/10-result-ranking.png)

---

## 环境与运行

### 环境要求

- 支持 C++17 的编译器（如 g++、MinGW、MSVC）

### 编译

```bash
g++ -std=c++17 src/main.cpp -o FriendRecommendSystem
```

### 运行

```bash
./FriendRecommendSystem
```

### 快速体验

1. 选择 `6` 加载内置测试数据
2. 选择 `4` 查看全部用户
3. 选择 `5` 并输入用户编号 `101`，查看好友推荐结果

![控制台菜单](screenshots/11-console-menu.png)

---

## 文件结构

```
.
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   └── main.cpp               # 源代码
├── screenshots/               # 项目配图
│   ├── 01-topic-overview.png
│   ├── 02-system-structure.png
│   ├── 03-hash-table-buckets.png
│   ├── 04-hash-function.png
│   ├── 05-hash-collision.png
│   ├── 06-user-structure.png
│   ├── 07-recommend-flow.png
│   ├── 08-interest-compare.png
│   ├── 09-similarity-formula.png
│   ├── 10-result-ranking.png
│   ├── 11-console-menu.png
│   └── 12-program-flow.png
└── docs/
    └── sample_input.txt        # 测试数据说明
```
