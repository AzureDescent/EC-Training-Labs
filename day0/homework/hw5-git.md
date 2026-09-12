# HW5 · Git 工作流

这是 Day 0 先导课的课下作业 **HW5**。你将在一个小型 C++ 工程上，完整走一遍
团队协作的闭环：**看清状态 → 整理提交 → 开分支 → 修 bug → 推送 → 同步上游 → 解决冲突 → 验证**。

> ### ⚠️ 工作目录
>
> **下面所有命令，都在 `day0/project/` 这个目录里执行。**
>
> ```text
> EC-Training-Labs/          ← 仓库根
> └── day0/
>     ├── project/           ★ 你在这里面敲命令
>     └── homework/
>         └── hw5-git.md     ← 你正在看的这份
> ```
>
> `app/` `base/` `tests/` `tools/` 这些路径都是相对 `day0/project/` 的。
> 走错目录是这份作业最常见的卡点。

---

## 学习目标

做完这份作业，你应该能够：

1. 拿到一个不熟悉的仓库时，**先看清状态再动手**（`git status` / `git diff`）
2. 把一堆杂乱的修改**整理成多个 atomic commit**，并写出别人看得懂的 message
3. 用**分支**隔离自己的工作，理解"分支只是一个指针"
4. 面对一个真实的**合并冲突**，读懂双方意图并写出一致的结果
5. 在提交前用 `grade.py` **自查**，而不是等别人告诉你哪里不合格

---

## 开始之前

逐条确认。**有一条没打勾就先别往下做**，否则后面一定会卡住。

- [ ] `git --version` 有输出
- [ ] `git config user.name` 和 `git config user.email` 都有输出（见 Day 0 课件 PreClass）
- [ ] `ssh -T git@github.com` 返回 `Hi <你的用户名>!`（见 Day 0 课件 PreClass）
- [ ] 已经 **Fork** 了这个仓库（`EC-Training-Labs`）到你自己的账号
- [ ] 你的 fork 是 **public**（不是 private）——私有仓库老师没法验收
- [ ] `cmake --version` 和 `g++ --version` 都有输出
- [ ] 完成了 HW1：`python tools/build.py` 能跑通，`test_clamp` 通过

---

## 获取代码

```bash
# 1. 先 Fork（在 GitHub 网页上点 Fork 按钮），然后：
git clone git@github.com:<你的用户名>/EC-Training-Labs.git

# 2. 进到工程目录 —— 后面所有命令都在这里
cd EC-Training-Labs/day0/project

# 3. 把老师的仓库加成第二个远端，后面 Task 9 要用
git remote add upstream git@github.com:SJTU-RoboMaster-Team/EC-Training-Labs.git
git remote -v          # 应该看到 origin（你的）和 upstream（老师的）
```

---

## 任务

一共 12 个 Task。**每个 Task 都写了「怎么验证」——做完就验，不要攒到最后。**

### Task 1 · 看清当前状态

**先铺出"你接手时"的工作区。**

你 clone 下来的仓库是**干净**的——因为 `git clone` 只会给你**已提交**的内容。
但真实情况里，你接手一个目录时它往往是脏的：同事做到一半、构建产物散落一地。

```bash
python tools/setup_worktree.py
```

这个脚本把那一刻复现出来。**跑完之后，先不要改任何东西，只看：**

```bash
git status
git diff
```

**你应该看到：** 有两个文件被修改（`app/control.cpp` 和 `app/clamp.cpp`），
还有几个 Git 不认识的文件（在 `mcu/stm32f407/MDK-ARM/` 下面）。

**怎么验证：** 你能回答这三个问题——
① 改了哪些文件？② 每个文件改了什么？③ 哪些文件 Git 根本不认识？

> **顺带记住这一条**：`git clone` 拿到的是**提交历史**，不是某个人的工作区。
> 正在改、还没提交的东西，不会跟着仓库走。这也是为什么"我本地明明改了"经常是个误会。

---

### Task 2 · 把不该提交的东西挡在外面

`mcu/stm32f407/MDK-ARM/` 下面那几个文件，和 `build/` 目录，**都不该进版本库**。
它们是工具生成的，每次打开工程都会变。

打开 `.gitignore`，把缺的规则补上。Keil 生成文件对应的规则是：

```gitignore
*DebugConfig*
**/RTE/**
*uvguix*
```

`build/` 目录也补一条。

**怎么验证：**

```bash
git status
```

**期望：** `mcu/` 下面那几个文件从 untracked 列表里**消失**了，只剩下两个 `modified`。

> **这三条规则不是我编的**，是真实仓库 `.gitignore` 里的原文。
> 真实项目里你也会遇到同样的事：先看到一堆陌生的 untracked 文件，
> 然后判断"哪些是生成的、哪些是我该提交的"。
> 这次我直接把规则给你，是为了让你把注意力放在**做**上，而不是猜。

---

### Task 3 · 第一次提交（只提交 control.cpp）

`app/control.cpp` 的修改是**一件事**：有人把夹爪默认速度从 4 调到了 6。

只提交它。**不要 `git add .`**——`clamp.cpp` 是另一件事，下次再提交。

```bash
git add app/control.cpp
git diff --staged          # ★ 提交前必须看一遍
git commit
```

commit message 要符合 `type(scope): subject`，比如：

```text
tune(clamp): raise default clamp speed from 4 to 6
```

**怎么验证：**

```bash
git log --oneline -1       # 看到你的 message
git status                 # control.cpp 不在 modified 列表里了，clamp.cpp 还在
```

---

### Task 4 · 第二次提交（只提交 clamp.cpp）

`app/clamp.cpp` 是**另一件事**：标定过程中不应该触发阻力判断。

```bash
git add app/clamp.cpp
git diff --staged
git commit
```

message 形如：

```text
fix(clamp): ignore resistance threshold while calibrating
```

**怎么验证：** `git status` 显示 `nothing to commit, working tree clean`。

> **为什么分两次提交？** 这就是 **atomic commit**：
> 一个 commit 只表达一个逻辑变化。如果你把这两件事合成一次提交，
> 以后想单独回退"调速度"就会连带回退"标定逻辑"。
> 这两个修改是我**故意放在一起**的——就是为了让你练这个。

---

### Task 5 · 开一个任务分支

**从现在开始，所有工作都在分支上做。**

```bash
git switch -c fix/encoder-wrap
```

分支名格式：`<type>/<what>`，`<what>` 用小写加短横线。
`<type>` 从这几个里选：`feat` `fix` `refactor` `tune` `exp`。

**怎么验证：**

```bash
git branch                 # 当前分支前面有 *
git status                 # On branch fix/encoder-wrap
```

> **为什么叫这个名字？** 两个月后有人看到 `fix/encoder-wrap`，
> 能立刻知道"这个分支在修编码器回绕，修完就能删"。
> 对比一下 `0429` 或者 `temp`——那种名字两个月后没人知道能不能删。

---

### Task 6 · 修 bug

#### 6.1 先看它怎么坏的

```bash
python tools/build.py
```

**你应该看到** `test_encoder` 失败，类似：

```text
  [ ok ] plain forward                got    10.00
  [ ok ] plain backward               got   -10.00
  [FAIL] forward across zero          got  -348.00  want    12.00
  [ ok ] backward across zero         got   -12.00
test_encoder: 3/4 passed
```

**注意：4 个用例里只有 1 个失败。** 这不是巧合，是你定位问题的线索。

#### 6.2 想清楚为什么只有一个方向坏

编码器原始角在 `[0, 360)` 之间回绕。求"这一帧走了多少度"时，
不能直接用 `current - last`——跨零的时候会算出 `±350` 这种数。

代码里有一个函数专门干这件事，在 `base/math.h`。

**先自己想，再看下面。**

<details>
<summary>想不出来？点开看提示（只说思路，不给答案）</summary>

拿纸算一下这两条路径：

```text
负向跨零：  5° → 1° → 357° → 353°      实际一共走了 -12°
正向跨零：  355° → 359° → 3° → 7°      实际一共走了 +12°
```

看 `1° → 357°` 这一步：`357 - 1 = +356`。
再看 `359° → 3°` 这一步：`3 - 359 = -356`。

**两个都是 ±356，但一个要变成 -4，另一个要变成 +4。**
现在的代码只处理了其中一种情况。

**为什么会这样？** 因为 `+356` 和 `-356` 会走进**不同的分支**。
</details>

#### 6.3 改代码

改 `base/math.h`。改动应该很小（2 行左右）。

**怎么验证：**

```bash
python tools/build.py
```

**期望：**

```text
test_encoder: 4/4 passed
test_clamp: 4/4 passed
```

> ⚠️ **`test_clamp` 必须仍然通过。** 它是回归保护——修 bug 不应该弄坏别的功能。

---

### Task 7 · 提交修复

```bash
git diff                    # 先看一遍自己改了什么
git add base/math.h
git diff --staged
git commit
```

message 要**说清症状和原因**，比如：

```text
fix(motor): normalize encoder delta in both directions

deg_normalize_180 只处理了 d > 180 的情况，导致正向跨零时
累计角度一次跳 -350 度。补上 d < -180 的分支。
```

**怎么验证：** `git log --oneline` 能看到 3 个提交，且 message 都能看懂。

---

### Task 8 · 推送

```bash
git push -u origin fix/encoder-wrap
```

**怎么验证：** 打开 `https://github.com/<你的用户名>/EC-Training-Labs`，
在分支下拉框里能看到 `fix/encoder-wrap`。

---

### Task 9 · 同步上游（制造一次真实冲突）

**场景**：老师（相当于"另一个队友"）在上游仓库开了一个功能分支 `drop-mode`，
给机械臂加了一个"存取矿"模式。这个功能和你的标定功能**改到了同一个枚举的末尾**，
你要把它合进来。

**先做一件事：在你的分支上，给 `app/arm.h` 的模式枚举末尾加一个自己的值。**

```cpp
enum Mode_e : uint8_t {
  FOLD,
  CRUISE,
  TWIST,
  EXCHANGE,
  STORAGE_FRONT,
  STORAGE_BACK,
  CALIBRATE,        // ← 你加的，标定模式
};
```

提交它：

```text
feat(arm): add CALIBRATE mode for joint calibration
```

然后同步上游：

```bash
git fetch upstream
git log --oneline --graph upstream/drop-mode -5   # 看看老师加了什么
git merge upstream/drop-mode
```

**你应该会看到冲突**（CONFLICT），在 `app/arm.h`，长得像这样：

```cpp
  STORAGE_BACK,
<<<<<<< HEAD
  CALIBRATE,
=======
  DROP_MODE,
>>>>>>> upstream/main
};
```

**怎么验证（这一步之后 `git status` 会显示 unmerged paths）：**

```bash
git status
```

---

### Task 10 · 解决冲突

**关键认知：冲突不是"选一边"，而是"想清楚两边各自要什么"。**

- 你加 `CALIBRATE` 是为了标定功能
- 老师加 `DROP_MODE` 是为了存取矿

**这两个都要保留。** 冲突的原因是它们加在了同一行位置，不是它们互相矛盾。

编辑 `app/arm.h`，把两边都留下（顺序不限）：

```cpp
  STORAGE_BACK,
  CALIBRATE,        // 你的
  DROP_MODE,        // 上游的
};
```

然后：

```bash
git diff                                    # ★ 确认没有残留的冲突标记
python tools/build.py                                    # ★ 必须重新验证
git add app/arm.h
git commit
```

merge commit 的 message 要写清**你做了什么决定**。

**怎么验证：**

```bash
git log --oneline --graph -6
```

**期望：** 能看到一个 merge commit（有两条线汇进来）。

> ⚠️ **合并成功 ≠ 程序正确。** Git 只能告诉你"文本层面合并完了"，
> 它不知道你的枚举值对不对。**所以第 5 步的 `python tools/build.py` 不能省。**

---

### Task 11 · 全量验证

```bash
python tools/build.py
```

**期望：**

```text
test_encoder: 4/4 passed
test_clamp: 4/4 passed
100% tests passed, 0 tests failed out of 2
```

---

### Task 12 · 收尾

```bash
git push
git status                    # working tree clean
git log --oneline --graph -8  # 回头看一遍自己做了什么
```

---

## 自查

**提交前，先自己跑一遍 grader。** 它和老师用的是同一份代码，结果完全一样。

```bash
# 在培训材料目录下跑（不是在你的仓库里，也不是在 day0/project 里）
python grade.py hw5 https://github.com/<你的用户名>/EC-Training-Labs
```

**期望看到：**

```text
┌────────────────────────────────────────────────────────────┐
│ HW5 · Git 工作流                                           │
│ 仓库 https://github.com/<你的用户名>/EC-Training-Labs      │
│ 分支 fix/encoder-wrap → main                               │
├────────────────────────────────────────────────────────────┤
│ ✅ 分支命名               存在合规分支：fix/encoder-wrap   │
│ ✅ 分支上有提交           fix/encoder-wrap 上有 4 个提交   │
│ ✅ message 格式           19/19 条合规                     │
│ ✅ message 黑名单         没有低信息量 message             │
│ ✅ 无构建产物入库         历史中没有构建产物               │
│ ✅ 无冲突标记残留         工作区与历史中都没有冲突标记     │
│ ✅ 测试文件未被修改       day0/project/tests 与最初一致    │
│ ✅ 冲突保留双方意图       CALIBRATE 和 DROP_MODE 都在      │
│ ✅ 发生过合并             1 个 merge commit                │
│ ✅ 测试全部通过           clamp 4/4 · encoder 4/4          │
│ ✅ 提交数                 你自己加了 4 个提交（上限 15）   │
│ ✅ message 长度           都在 10–72 字符内                │
│ ✅ atomic 启发式          没有明显跨模块的巨型提交         │
│ ✅ 工作区干净             没有未提交的改动                 │
│ ✅ 已推送到远端           origin/fix/encoder-wrap 一致     │
├────────────────────────────────────────────────────────────┤
│ 结论   PASS    全部必修项通过                              │
└────────────────────────────────────────────────────────────┘
```

**如果 FAIL**，明细里会写清楚是哪一项没过——**照着改，然后重跑**，
不要直接提交。

---

## 提交

把自查输出的**截图**发到飞书群。

> 截图里要能看到：**仓库地址、分支名、每一行的 ✅/❌、以及最后的结论**。
> 没有结论那一行的截图不算。

---

## 评分

**本作业只有 PASS / FAIL 两个结果。** 下面这些**必须全部通过**
（就是自查输出里的前 10 行）：

- [ ] 存在命名为 `fix/<what>` 或 `tune/<what>` 等合规格式的任务分支
- [ ] 分支上有实质提交（不是空分支）
- [ ] 所有 commit message 符合 `type(scope): subject` 或 `type: subject`
- [ ] 没有任何 commit message 属于低信息量黑名单（`update` / `modify` / `tmp` / `tuning` / `改` …）
- [ ] 历史中没有构建产物（`build/` / `*.o` / Keil 生成文件）
- [ ] 工作区与历史中没有冲突标记残留（`<<<<<<<` / `>>>>>>>`）
- [ ] `tests/` 下的测试文件**没有被修改**（改测试让它通过 = 任务没完成）
- [ ] 合并后 `CALIBRATE` 和 `DROP_MODE` 两个枚举值**都存在**
- [ ] 历史里有 **merge commit**（合并确实发生过）
- [ ] `test_encoder` 与 `test_clamp` **全部通过**

**提示项（不影响结论，但会显示出来）：**

- 你自己加的提交数超过 15 个
- 某条 message 长度不在 10–72 字符
- 某个 commit 一次跨了太多模块
- 工作区还有未提交的改动
- 任务分支还没推送

> **自查输出里的顺序就是上面的顺序** —— 前 10 行全绿才是 PASS。
> 你不需要记这些，跑一次 `grade.py` 就知道了。

---

## 常见错误

| 症状 | 原因 | 怎么办 |
| --- | --- | --- |
| `git push` 报 `Permission denied (publickey)` | SSH key 没配好 | 回 Day 0 课件的 PreClass 章节 |
| `git push` 报 `rejected` / `fetch first` | 远端有你本地没有的提交 | `git pull` 之后再 push。**不要用 `--force`** |
| `git status` 里看不到 Keil 那几个文件 | 你已经把它们加进 `.gitignore` 了 | 正常，Task 2 的目标就是这个 |
| 编译报 `deg_normalize_180` 找不到 | 没 `#include "base/math.h"` | 补上 include |
| 合并后编译不过 | 冲突标记没删干净 | `git diff` 找出来 |
| `git switch -c` 说分支已存在 | 之前建过没删 | `git switch fix/encoder-wrap` 切过去 |
| `git merge upstream/drop-mode` 说 no such ref | 没 `git fetch upstream` | 先 fetch |
| `cmake` 报找不到编译器 | 工具链没装好 | 回 Day 0 课件的环境章节 |
| 跑了 `git add .`，把构建产物也加进去了 | —— | `git restore --staged build/`，然后确认 `.gitignore` |

---

## 参考

| 内容 | 在哪 |
| --- | --- |
| Git 课件（`status` / `diff` / `add` / `commit`） | Day 0 课件 Loop 1–2 |
| 分支与 HEAD | Day 0 课件 Loop 3 |
| 远端与同步 | Day 0 课件 Loop 4 |
| 合并与冲突 | Day 0 课件 Loop 5 |
| Commit message 与分支名的写法 | Day 0 课件「书写约定」一节 |
| 撤销与恢复 | Day 0 课件 Loop 6 |
| 完整的命令速查 | Day 0 课件附录 C |
| 本作业的测评范围与判定项 | 讲师手里的 `lab/DESIGN.md` |
| Pro Git（中文） | https://git-scm.com/book/zh/v2 |
| Learn Git Branching（可视化练分支） | https://learngitbranching.js.org/ |

---

## 卡住了怎么办

1. **先跑 `git status`**，把它输出的原文看一遍——大部分问题它会直接告诉你
2. **再跑 `git log --oneline --graph -8`**，看看自己现在在哪
3. 翻上面的「常见错误」
4. 还不行就带着**这两条命令的输出**去问，不要只说"我 Git 坏了"
