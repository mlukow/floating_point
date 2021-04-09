#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  float u = 2.0f;
  float v = -4.0f;

  for (size_t i = 0; i < 90; i++) {
    float w = 111.0f - 1130.f / v + 3000.0f / (u * v);
    u = v;
    v = w;
  }

  printf("%-16.12f\n", v);

  return EXIT_SUCCESS;
}
