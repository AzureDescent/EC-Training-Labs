# EC Training Labs

上海交通大学 · 交龙战队 · RoboMaster 电控组 —— **培训作业仓库**。

这里放的是培训期间要动手做的东西。**不要只读代码**，作业是要你在上面改、提交、推上来的。

---

## 怎么用

**学生：**

```text
1. Fork 这个仓库到你自己的 GitHub 账号（保持 public）
2. git clone git@github.com:<你的用户名>/EC-Training-Labs.git
3. 打开对应那一天的 README，从「开始之前」往下做
```

> 保持 public 是必须的——验收工具靠 `git clone` 拿仓库，私有仓库没法验收。

**内容按天组织：**

| 目录 | 内容 |
| --- | --- |
| [`day0/`](day0/) | 先导课：环境、Keil、CLion、Git、clang-format |

以后会有 `day1/`、`day2/`……

---

## 仓库结构约定

```text
EC-Training-Labs/
├── README.md            你正在看的这份
├── day0/
│   ├── README.md        这一天怎么走：有哪些作业、按什么顺序
│   ├── project/         ★ 作业用的代码工程
│   └── homework/        ★ 每份作业的任务书
└── (day1/ …)
```

**为什么每份作业不单独建仓库**：Day 0 的几份作业是**一条链**，都在同一个工程上做——
先能编译、再修 bug、再格式化、最后用 Git 把前面这些改动**有组织地提交上去**。
分开建仓库的话，Git 那份作业就只能提交老师编的假数据了。

---

## 遇到问题

1. 先看那份作业任务书里的**「常见错误」**一节
2. 再跑 `git status`，把它输出的原文看一遍
3. 还不行，带着 `git status` 和 `git log --oneline --graph -8` 的输出去问
