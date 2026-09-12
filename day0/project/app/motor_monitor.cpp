#include "app/motor_monitor.h"

#include "base/math.h"

void MotorMonitor::reset() {
  data_ = MotorData{};
}

void MotorMonitor::reset(float raw_angle_deg) {
  data_ = MotorData{};
  data_.ecd_angle = raw_angle_deg;
  data_.last_raw_angle = raw_angle_deg;
}

void MotorMonitor::update(float raw_angle_deg, float speed_dps, float current_a) {
  // 累计角度 += 本次增量。增量必须先归一化，见 base/math.h
  data_.angle += wrap_angle_deg(raw_angle_deg - data_.last_raw_angle);
  data_.last_raw_angle = raw_angle_deg;
  data_.ecd_angle = raw_angle_deg;
  data_.speed = speed_dps;
  data_.current = current_a;
}
