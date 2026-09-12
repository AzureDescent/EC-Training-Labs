#ifndef DAY0_APP_CLAMP_H
#define DAY0_APP_CLAMP_H

#include "app/arm.h"

// 夹爪
class Clamp {
public:
    void beginCalibration();
    void endCalibration();
    bool isCalibrating() const {
        return is_calibrating_;
    }

    // 每周期调用一次，返回本次判定出的状态
    ClampState update(float force, float angle);

private:
    // 阻力阈值：超过它认为夹到东西了
    static constexpr float kResistThreshold = 1400.0f;
    // 打开到位角
    static constexpr float kOpenAngle = 900.0f;

    bool is_calibrating_ = false;
    ClampState state_ = ClampState::kIdle;
};

#endif // DAY0_APP_CLAMP_H
