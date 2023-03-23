// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fdiagnostics-parseable-fixits -include %s %s 2>&1 | FileCheck %s

// CHECK-NOT: fix-it:

// We cannot deal with overload conflicts for now so NO fix-it to
// function parameters will be emitted if there are overloads for that
// function.

#ifndef INCLUDE_ME
#define INCLUDE_ME

void baz();

#else


void foo(int *p, int * q);

void foo(int *p);

void foo(int *p) {
  int tmp;
  tmp = p[5];
}

// an overload declaration of `bar(int)` appears after it
void bar(int *p) {
  int tmp;
  tmp = p[5];
}

void bar(); 

// an overload declaration of `baz(int)` appears is included
void baz(int *p) {
  int tmp;
  tmp = p[5];
}


#endif
