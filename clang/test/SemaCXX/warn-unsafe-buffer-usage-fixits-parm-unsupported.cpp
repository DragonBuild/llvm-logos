// RUN: %clang_cc1 -std=c++20 -Wno-all -Wunsafe-buffer-usage -fcxx-exceptions -verify %s
// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fcxx-exceptions -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck %s

// We handle cv-qualifiers poorly:

void const_ptr(int * const x) { // expected-warning{{'x' is an unsafe pointer used for buffer access}} expected-note{{change type of 'x' to 'std::span' to preserve bounds information}}  
  // CHECK: fix-it:{{.*}}:{[[@LINE-1]]:16-[[@LINE-1]]:29}:"std::span<int> x"
  // FIXME: the fix-it above is incorrect
  int tmp = x[5]; // expected-note{{used in buffer access here}}
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid const_ptr(int * const x) {return const_ptr(std::span<int>(x, <# size #>));}\n"
// FIXME: the fix-it above is incorrect

void const_ptr_to_const(const int * const x) {// expected-warning{{'x' is an unsafe pointer used for buffer access}} expected-note{{change type of 'x' to 'std::span' to preserve bounds information}}  
  // CHECK: fix-it:{{.*}}:{[[@LINE-1]]:25-[[@LINE-1]]:44}:"std::span<const int> x"
  // FIXME: the fix-it above is incorrect  
  int tmp = x[5]; // expected-note{{used in buffer access here}}
}
// CHECK-DAG: fix-it:{{.*}}:{[[@LINE-1]]:2-[[@LINE-1]]:2}:"\n#if __has_cpp_attribute(clang::unsafe_buffer_usage)\n{{\[}}{{\[}}clang::unsafe_buffer_usage{{\]}}{{\]}}\n#endif\nvoid const_ptr_to_const(const int * const x) {return const_ptr_to_const(std::span<const int>(x, <# size #>));}\n"
// FIXME: the fix-it above is incorrect

// We do not fix parameters participating unsafe operations for the
// following functions/methods or function-like expressions:

// CHECK-NOT: fix-it:
class A {
  // constructor & descructor
  A(int * p) {  // expected-warning{{'p' is an unsafe pointer used for buffer access}}
    int tmp;
    tmp = p[5]; // expected-note{{used in buffer access here}}
  }

  // class member methods
  void foo(int *p) { // expected-warning{{'p' is an unsafe pointer used for buffer access}}
    int tmp;
    tmp = p[5];      // expected-note{{used in buffer access here}}
  }

  // overload operator
  int operator+(int * p) { // expected-warning{{'p' is an unsafe pointer used for buffer access}}
    int tmp;
    tmp = p[5];            // expected-note{{used in buffer access here}}
    return tmp;
  }
};

// lambdas
void foo() {
  auto Lamb = [&](int *p) // expected-warning{{'p' is an unsafe pointer used for buffer access}}
    -> int {
    int tmp;
    tmp = p[5];           // expected-note{{used in buffer access here}}
    return tmp;
  };
}

// template
template<typename T>
void template_foo(T * p) { // expected-warning{{'p' is an unsafe pointer used for buffer access}}
  T tmp;
  tmp = p[5];              // expected-note{{used in buffer access here}}
}

void instantiate_template_foo() {
  int * p;
  template_foo(p);        // FIXME expected note {{in instantiation of function template specialization 'template_foo<int>' requested here}}
}

// variadic function
void vararg_foo(int * p...) { // expected-warning{{'p' is an unsafe pointer used for buffer access}}
  int tmp;
  tmp = p[5];                 // expected-note{{used in buffer access here}}
}

// constexpr functions
constexpr int constexpr_foo(int * p) { // expected-warning{{'p' is an unsafe pointer used for buffer access}}
  return p[5];                         // expected-note{{used in buffer access here}}
}

// function body is a try-block
void fn_with_try_block(int* p)    // expected-warning{{'p' is an unsafe pointer used for buffer access}}
  try {
    int tmp;

    if (p == nullptr)
      throw 42;
    tmp = p[5];                   // expected-note{{used in buffer access here}}
  }
  catch (int) {
    *p = 0;
  }

// The following two unsupported cases are not specific to
// parm-fixits. Adding them here in case they get forgotten.
void isArrayDecayToPointerUPC(int a[][10], int (*b)[10]) {
// expected-warning@-1{{'a' is an unsafe pointer used for buffer access}}
// expected-warning@-2{{'b' is an unsafe pointer used for buffer access}} 
  int tmp;
  
  tmp = a[5][5] + b[5][5];  // expected-warning2{{unsafe buffer access}}  expected-note2{{used in buffer access here}}
}




