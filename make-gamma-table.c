/*
  Calculate a gamma correction table given a gamma value and an input range.
*/

#include <stdio.h>
#include <math.h>

int main() {
  const double gamma = 2.8;
  const unsigned int max_in = 255;
  const unsigned int max_out = 255;

  printf("const uint8_t gamma8[] = {");
  for (unsigned int i=0; i<=max_in; ++i) {
    if (i > 0) printf(",");
    if (i % 16 == 0) printf("\n ");
    printf("%3d", (int) (pow((double) i / max_in, gamma) * max_out + 0.5));
  }
  printf(" };\n");
  return 0;
}
