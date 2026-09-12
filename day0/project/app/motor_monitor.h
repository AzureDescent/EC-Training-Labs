#ifndef DAY0_APP_MOTOR_MONITOR_H
#define DAY0_APP_MOTOR_MONITOR_H

#include "base/motor.h"

// 单电机反馈监视器
class MotorMonitor {
public:
    // 归零：累计角度清零，基准角也清零
    void reset();

    // 归零并把基准角设为当前角度（上电时用，避免第一帧产生巨大增量）
    void reset(float raw_angle_deg);

    // 每收到一帧反馈调用一次
    void update(float raw_angle_deg, float speed_dps, float current_a);

    const MotorData& data() const {
        return data_;
    }

private:
    MotorData data_{};
};

#endif // DAY0_APP_MOTOR_MONITOR_H
