// 编码器累计角度测试
//
// 编码器原始角在 [0, 360) 之间回绕，累计角度的算法必须能正确处理跨零。
#include "app/motor_monitor.h"
#include "tests/test_util.h"

namespace {

    // 从 seq[0] 建立基准，依次喂入其余角度，返回累计角度
    float run(const float* seq, int n) {
        MotorMonitor m;
        m.reset(seq[0]);
        for (int i = 1; i < n; ++i) {
            m.update(seq[i], 0.0f, 0.0f);
        }
        return m.data().angle;
    }

} // namespace

int main() {
    const float fwd[] = {10.0f, 15.0f, 20.0f};
    testutil::check_near("plain forward", run(fwd, 3), 10.0f, 0.01f);
    // 10 -> 15 -> 20，每步 +5，共 +10

    const float back[] = {20.0f, 15.0f, 10.0f};
    testutil::check_near("plain backward", run(back, 3), -10.0f, 0.01f);
    // 20 -> 15 -> 10，每步 -5，共 -10

    const float fwd_wrap[] = {355.0f, 359.0f, 3.0f, 7.0f};
    testutil::check_near("forward across zero", run(fwd_wrap, 4), 12.0f, 0.01f);
    // 355 -> 359 -> 3 -> 7：穿过 360/0，实际只走了 +12

    const float back_wrap[] = {5.0f, 1.0f, 357.0f, 353.0f};
    testutil::check_near("backward across zero", run(back_wrap, 4), -12.0f, 0.01f);
    // 5 -> 1 -> 357 -> 353：穿过 0/360，实际只走了 -12

    return testutil::report("test_encoder");
}
