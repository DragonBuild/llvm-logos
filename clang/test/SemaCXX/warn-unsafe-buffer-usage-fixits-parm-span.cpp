// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fdiagnostics-parseable-fixits -include %s %s 2>&1 | FileCheck %s

// TODO test if there's not a single character in the file after a decl or def

#ifndef INCLUDE_ME
#define INCLUDE_ME

void simple(int *p);
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:1-[[@LINE-1]]:1}:"#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:20-[[@LINE-2]]:20}:"\nvoid simple(std::span<int>);\n"

#else

void simple(int *);
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:1-[[@LINE-1]]:1}:"#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:19-[[@LINE-2]]:19}:"\nvoid simple(std::span<int>);\n"

void simple(int *p) {
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:13-[[@LINE-1]]:19}:"std::span<int> p"
  int tmp;
  tmp = p[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid simple(int *p) {return simple(std::span<int>(p, <# size #>));}\n"


void twoParms(int *p, int * q) {
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:15-[[@LINE-1]]:21}:"std::span<int> p"
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:23-[[@LINE-2]]:30}:"std::span<int> q"
  int tmp;
  tmp = p[5] + q[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid twoParms(int *p, int * q) {return twoParms(std::span<int>(p, <# size #>), q);}\n"
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-2]]:2-[[@LINE-2]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid twoParms(int *p, int * q) {return twoParms(p, std::span<int>(q, <# size #>));}\n"

void decayedArrayParm(int arr[]) {
  // CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:23-[[@LINE-1]]:32}:"std::span<int> arr"
  int tmp;
  tmp = arr[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid decayedArrayParm(int arr[]) {return decayedArrayParm(std::span<int>(arr, <# size #>));}\n"


void ptrToConst(const int * x) {
  // CHECK: fix-it:{{.*}}:{[[@LINE-1]]:17-[[@LINE-1]]:30}:"std::span<const int> x"
  int tmp = x[5];
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid ptrToConst(const int * x) {return ptrToConst(std::span<const int>(x, <# size #>));}\n"

#endif
