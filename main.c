#include "exceptions.h"
#include <stdio.h>

int foo() {
  puts("some operation that definitely fails");
  return RES_IO_ERR;
}

void onerr(int err) {
  printf("**OOPS** caught: %d\n", err);
}

void bar() {
  puts("Phew. I handled my business.");
}

int main() {
  handler(foo, onerr, bar);
  return 0;
}
