#ifndef DAY0_BASE_MOTOR_H
#define DAY0_BASE_MOTOR_H

// 电机反馈数据
// 角度单位 deg，转速单位 dps，电流单位 A，温度单位 C
struct MotorData {
    float angle = 0.0f;          // 累计角度
    float ecd_angle = 0.0f;      // 本次编码器角度
    float last_raw_angle = 0.0f; // 上次编码器角度
    float speed = 0.0f;          // 转速
    float current = 0.0f;        // 转矩电流
    float temp = 0.0f;           // 温度
};

#endif // DAY0_BASE_MOTOR_H
