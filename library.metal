#include <metal_stdlib>

using namespace metal;

kernel void iter(device float *data) {
  for (uint i = 2; i < 32; i++) {
    data[i] = 111.0f - 1130.0f / data[i - 1] + 3000.0f / (data[i - 2] * data[i - 1]);
  }
}
