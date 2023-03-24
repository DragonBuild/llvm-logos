# llvm-logos

clang-logos is a modified build of apple-llvm, allowing compilation of Logos code directly to LLVM IR.

The syntax for the logos we compile here is slightly different than DHowett's original Logos syntax. 

Implementing it directly in LLVM as opposed to via a preprocessor allows:
* Using @ directives more in line with regular Objective-C
* Utilizing clangd / other development tools. Yes, this includes autocomplete/similar features in any editor supporting clangd
* Better error/warning output

And from a development standpoint:
* Far more flexibility, ease of maintenance, from a language standpoint. 
* Not having to write/read perl

This project is still in active development, and has a ways to go before reaching feature parity with modern Logos.

I'll update this README.md in the future with better info.

This is a modern continuation of Evan Swick's https://github.com/eswick/clang-logos, continued in his memory. 




