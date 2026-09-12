#!/usr/bin/env python3
"""配置 + 构建 + 跑测试。Windows / Linux / macOS 通用。

用法（在仓库根目录下）：

    python tools/build.py

只构建不跑测试：

    python tools/build.py --no-test

清理后重新构建：

    python tools/build.py --clean
"""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "build"


def run(cmd: list[str]) -> int:
    """跑一条命令，实时打印输出，返回退出码。"""
    print("$ " + " ".join(str(c) for c in cmd))
    try:
        # 不要 capture_output：让学生实时看到编译器报错
        return subprocess.run([str(c) for c in cmd], cwd=str(ROOT)).returncode
    except FileNotFoundError:
        print(f"\n!! 找不到命令：{cmd[0]}")
        return 127


def main() -> int:
    parser = argparse.ArgumentParser(description="构建并测试 day0/project")
    parser.add_argument("--no-test", action="store_true", help="只构建，不跑测试")
    parser.add_argument("--clean", action="store_true", help="先删掉 build/ 再构建")
    args = parser.parse_args()

    # 环境检查：先给出人话提示，而不是让 cmake 抛一堆看不懂的错
    if shutil.which("cmake") is None:
        print("!! 找不到 cmake。")
        print("   请先完成 Day 0 的环境配置，确认 `cmake --version` 有输出。")
        return 1
    if shutil.which("g++") is None and shutil.which("clang++") is None:
        print("!! 找不到 C++ 编译器（g++ 或 clang++）。")
        print("   请先完成 Day 0 的环境配置。")
        return 1

    if args.clean and BUILD_DIR.exists():
        print(f"删除 {BUILD_DIR}")
        shutil.rmtree(BUILD_DIR)

    print("=" * 60)
    print("配置 (cmake configure)")
    print("=" * 60)
    rc = run(["cmake", "-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Debug"])
    if rc != 0:
        return rc

    print()
    print("=" * 60)
    print("构建 (cmake build)")
    print("=" * 60)
    rc = run(["cmake", "--build", "build", "--config", "Debug"])
    if rc != 0:
        return rc

    if args.no_test:
        return 0

    print()
    print("=" * 60)
    print("测试 (ctest)")
    print("=" * 60)
    # ctest 在有测试失败时返回非 0 —— 这是我们要看到的，所以原样返回
    return run(["ctest", "--test-dir", "build", "--output-on-failure", "-C", "Debug"])


if __name__ == "__main__":
    sys.exit(main())
