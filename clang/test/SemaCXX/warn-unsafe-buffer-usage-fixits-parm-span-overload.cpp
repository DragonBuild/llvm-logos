// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck %s

// CHECK-NOT: fix-it:

// We cannot deal with overload conflicts for now so NO fix-it to
// function parameters will be emitted if there are overloads for that
// function.

void foo(int *p, int * q);

void foo(int *p);

void foo(int *p) {
  int tmp;
  tmp = p[5];
}
