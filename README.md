# sylva-lang

Sylva is a simple yet powerful object-oriented programming language.

# Why another new language

Just because I want it, I want a new language with thease features:

* Light-weight as Lua, but with built-in class system

* Elegant as Ruby, but without a heavy stack of standard libraries

* Easy to build, no need to import files around, just like Swift, but easier to create native C bindings

# Roadmap

## 1.0 - In Progress

I've created several Ruby-DSLs successfully, but I don't have much knowledge about compilers. At the very first stage, I'll try to create a language which could be compiled to ANSI C codes, this will make Sylva fast, strong and easy to bind existed libraries.

### Components

`sylva` - a cli program compiles multiple `.sylva` files into C source code, inject any C bindings and then compile C source code with GCC / Clang, it also generate `.h` files from sylva `native class`

`sylva-runtime` - provides the standard libraries for sylva, including data structures and more. Will be composed by several `.sylva` files with `native class` and a `libsylvaruntime.a`.

`libsylva` API for `sylva` cli

## 2.0 - Planning

self lift

## 3.0 - Maybe

bypass C code, use LLVM directly

# Grammar

See `docs` folder
