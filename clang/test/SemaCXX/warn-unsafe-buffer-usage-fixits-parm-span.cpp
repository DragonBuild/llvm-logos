// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fdiagnostics-parseable-fixits -include %s %s 2>&1 | FileCheck %s

// TODO test if there's not a single character in the file after a decl or def
// TODO: Need to resolve this issue: "#include <span>\n" and
//       "[[clang::unsafe_buffer_usage]] " may be inserted at the same
//        beginning location.

#ifndef INCLUDE_ME
#define INCLUDE_ME
void f();
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:1-[[@LINE-1]]:1}:"#include <span>\n"

void simple(int *p);
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:1-[[@LINE-1]]:1}:"#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:20-[[@LINE-2]]:20}:"\nvoid simple(std::span<int>);\n"

#else

void simple(int *);
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:1-[[@LINE-1]]:1}:"#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:19-[[@LINE-2]]:19}:"\nvoid simple(std::span<int>);\n"

void simple(int *p) {
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:13-[[@LINE-1]]:18}:"std::span<int>"
  int tmp;
  tmp = p[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid simple(int *p) {return simple(std::span<int>(p, <# size #>));}\n"


void twoParms(int *p, int * q) {
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:15-[[@LINE-1]]:20}:"std::span<int>"
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:23-[[@LINE-2]]:28}:"std::span<int>"
  int tmp;
  tmp = p[5] + q[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid twoParms(int *p, int * q) {return twoParms(std::span<int>(p, <# size #>), q);}\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:2-[[@LINE-2]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid twoParms(int *p, int * q) {return twoParms(p, std::span<int>(q, <# size #>));}\n"
#endif
