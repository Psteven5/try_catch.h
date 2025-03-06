# try_catch.h
Simple jmp_buf-based try-catch in C

# Install
This is an STB-style single-header library, so simply include the header file, and in a single source file do
```c
#define TRY_CATCH_IMPLEMENTATION
```
before including to include the implementation as well (mostly internal state).

# Usage
try-catch.h provides a simple try-catch block in C using macros. The general structure of such a block is provided below.
```c
TRY // Begin try block
        THROW(Foo, {}); // Throw exception of type Foo
CATCH(Foo, foo) // Catch only Foo and store Foo-typed data
        ...
CATCH(bar) // Catch all types and store typename
        ...
END_TRY // End try block
```
Any uncaught exception or exception thrown in a catch block will get propagated to an earlier try-catch block.
If this does not exist, it will be treated as an uncaught exception and abort.
