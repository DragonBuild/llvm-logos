// RUN: %clang_cc1 -std=c++20 -Wno-all -Wunsafe-buffer-usage -fcxx-exceptions -verify %s
// RUN: %clang_cc1 -std=c++20 -Wunsafe-buffer-usage -fcxx-exceptions -fdiagnostics-parseable-fixits %s 2>&1 | FileCheck %s

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
  template_foo(p);        // expected-note{{in instantiation of function template specialization 'template_foo<int>' requested here}}
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

// main function
int main(int argc, char *argv[]) { // expected-warning{{'argv' is an unsafe pointer used for buffer access}}
  char tmp;
  tmp = argv[5][5];                // expected-note{{used in buffer access here}} \
				      expected-warning{{unsafe buffer access}}
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
