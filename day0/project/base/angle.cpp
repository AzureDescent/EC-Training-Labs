//
// Created by DrownFish on 2026/9/12.
//

#include "base/math.h"

float wrap_angle_deg(float deg) {
  if (deg > 180.0f) {
    deg -= 360.0f;
  } else if (deg <= -180.0f) {
    deg += 360.0f;
  }
  return deg;
}
