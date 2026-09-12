// 夹爪阈值逻辑回归测试
//
// 这个用例保护 clamp 的既有行为：改动夹爪相关代码时，不应该把这几条弄坏。
#include "app/clamp.h"
#include "tests/test_util.h"

namespace {

const char* name(ClampState s) {
  switch (s) {
    case ClampState::kIdle:
      return "kIdle";
    case ClampState::kMoving:
      return "kMoving";
    case ClampState::kBlocked:
      return "kBlocked";
    case ClampState::kHolding:
      return "kHolding";
  }
  return "?";
}

}  // namespace

int main() {
  Clamp c;

  // 阻力超过阈值 -> 卡住
  testutil::check_eq("blocked when force high", name(c.update(1500.0f, 0.0f)), "kBlocked");
  // 阻力正常、角度没到位 -> 运动中
  testutil::check_eq("moving when angle low", name(c.update(100.0f, 100.0f)), "kMoving");
  // 阻力正常、角度到位 -> 夹持
  testutil::check_eq("holding when angle high", name(c.update(100.0f, 950.0f)), "kHolding");
  // 走一遍标定流程之后，阈值行为不变
  c.beginCalibration();
  c.endCalibration();
  testutil::check_eq("blocked after calibration", name(c.update(1500.0f, 0.0f)), "kBlocked");

  return testutil::report("test_clamp");
}
