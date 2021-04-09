#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  long double u = 2.0L;
  long double v = -4.0L;

  for (size_t i = 0; i < 90; i++) {
    long double w = 111.0L - 1130.0L / v + 3000.0L / (u * v);
    u = v;
    v = w;
  }

  printf("%-16.12Lf\n", v);

  return EXIT_SUCCESS;
}
