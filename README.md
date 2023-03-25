# llvm-ObjCS

_Please note that this toolchain is still in very active, early development, and there may be statements or things noted in specifications / this README
that are not yet available or public._

llvm-ObjCS is a modified build of apple-llvm, allowing compilation of Objective-CS code directly to LLVM IR.

The Objective-CS Language Specification can be viewed here: https://github.com/eswick/Objective-CS

The toolchain has direct integration and companion tooling with the [dragon](https://github.com/DragonBuild/dragon) build system,
and drop-in compatibility with [theos](https://github.com/theos/theos). 

It can be used to compile Logos projects, and both Logos and Objective-CS files can be mixed and built within the same project.

---

Implementing it directly in LLVM as opposed to, in logos' case, via a preprocessor allows:
* Using @ directives more in line with regular Objective-C
* Utilizing clangd / other development tools. Yes, this includes autocomplete/similar features in any editor supporting clangd
* Better error/warning output

And from a development standpoint:
* Far more flexibility, ease of maintenance, from a language standpoint. 
* Not having to write/read perl

This project is still in active development, and has a ways to go before reaching feature parity with modern Logos.

I'll update this README.md in the future with better info.

---

This is a modern continuation of Evan Swick's https://github.com/eswick/clang-logos and Objective-CS projects, who has unfortunately since passed away. 

Its maintenance has since been continued by cynder.




