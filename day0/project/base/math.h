#ifndef DAY0_BASE_MATH_H
#define DAY0_BASE_MATH_H

// 把角度差归一化到 (-180, 180]
//
// 编码器原始角在 [0, 360) 之间回绕，直接用 current - last 求增量的话，
// 跨零时会算出 ±350 这种数，累计角度就飞了。所以增量必须先过这里。
inline float deg_normalize_180(float d) {
  if (d > 180.0f) {
    d -= 360.0f;
  }
  return d;
}

inline float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

#endif  // DAY0_BASE_MATH_H
