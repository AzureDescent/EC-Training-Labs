#!/usr/bin/env python3
"""铺出「你接手时」的工作区状态。

为什么需要这一步：`git clone` 只会给你**已提交**的内容，工作区永远是干净的。
但真实情况里，你接手一个目录时它往往是脏的——同事做到一半、构建产物散落一地。
这个脚本把那一刻复现出来，让你练习"先看清状态"。

用法（在仓库根目录下）：

    python tools/setup_worktree.py

可以重复运行，不会叠加。
"""

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def edit_control() -> None:
    """改动 A：调夹爪默认速度。"""
    path = ROOT / "app" / "control.cpp"
    text = path.read_text(encoding="utf-8")
    old = "constexpr float kDefaultClampSpeed = 4.0f;"
    new = "constexpr float kDefaultClampSpeed = 6.0f;"
    if old in text:
        path.write_text(text.replace(old, new, 1), encoding="utf-8", newline="")
        print("  app/control.cpp  默认夹爪速度 4.0 -> 6.0")
    elif new in text:
        print("  app/control.cpp  已经是铺开状态，跳过")
    else:
        raise SystemExit("!! app/control.cpp 里找不到 kDefaultClampSpeed，仓库状态不对")


def edit_clamp() -> None:
    """改动 B：标定期间不判阻力。"""
    path = ROOT / "app" / "clamp.cpp"
    text = path.read_text(encoding="utf-8")
    old = "  if (force > kResistThreshold) {"
    new = "  if (force > kResistThreshold && !is_calibrating_) {"
    if old in text:
        path.write_text(text.replace(old, new, 1), encoding="utf-8", newline="")
        print("  app/clamp.cpp    标定期间跳过阻力判断")
    elif new in text:
        print("  app/clamp.cpp    已经是铺开状态，跳过")
    else:
        raise SystemExit("!! app/clamp.cpp 里找不到阻力判断，仓库状态不对")


# Keil 打开工程时会生成这些文件。它们每次都变，不该进版本库。
KEIL_FILES = {
    "mcu/stm32f407/MDK-ARM/DebugConfig/day0_STM32F407VG.dbgconf": """\
# Keil 调试配置。由 IDE 生成，每次换调试器都会变。
Dp_Init = 1
Dp_Reset = 1
Dp_Halt = 0
""",
    "mcu/stm32f407/MDK-ARM/RTE/_day0/RTE_Components.h": """\
/* 由 Keil Run-Time Environment 自动生成，不要手改 */
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

#define RTE_DEVICE_STARTUP_STM32F4XX

#endif /* RTE_COMPONENTS_H */
""",
    "mcu/stm32f407/MDK-ARM/day0.uvguix.zhangsan": """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<!-- Keil 保存的窗口布局。每个人打开过的窗口都不一样，所以它总是变。 -->
<Layout>
  <Window Name="Project" Left="0" Top="0" Width="280" Height="600"/>
  <Window Name="Editor" Left="280" Top="0" Width="900" Height="600"/>
</Layout>
""",
}


def main() -> int:
    print("[1/3] 铺开同事改到一半的两处修改")
    edit_control()
    edit_clamp()

    print()
    print("[2/3] 生成 Keil 打开工程时产生的文件（不该进版本库的那几个）")
    for rel, content in KEIL_FILES.items():
        path = ROOT / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8", newline="")
        print(f"  {rel}")

    print()
    print("[3/3] 完成。现在跑：")
    print()
    print("    git status")
    print("    git diff")
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
