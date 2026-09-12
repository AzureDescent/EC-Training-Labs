#include "app/clamp.h"

void Clamp::beginCalibration() {
  is_calibrating_ = true;
  state_ = ClampState::kIdle;
}

void Clamp::endCalibration() {
  is_calibrating_ = false;
}

ClampState Clamp::update(float force, float angle) {
  if (force > kResistThreshold) {
    state_ = ClampState::kBlocked;
    return state_;
  }

  if (angle > kOpenAngle) {
    state_ = ClampState::kHolding;
  } else {
    state_ = ClampState::kMoving;
  }
  return state_;
}
