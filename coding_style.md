# Coding Style and Format

## clang-format

VCLib uses [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for formatting. The format settings are listed in the `.clang-format` file at the root of this repository. Please make sure to run clang-format on the modified files before pushing new code.

General rules are the following:

- 4 spaces for indentation;
- open brackets at the end of the line (with the exception of the beginning of
classes and functions);
- 80 characters limit per line;

## Coding Style

VCLib uses the following coding style rules:

- The order of inclusion of headers should be the following (each subsection in alphabetical order):
  1. h file corresponding to this cpp file (if applicable);
  2. headers from the same component;
  3. headers from other components;
  4. system headers.
- Generally, the code of the library uses CamelCase: use upper case letters as word separators
- Class names and type names start with an upper case letter;
- Functions, member functions, non-const variables and namespaces start with a lower case letter;
- The private member attribute name in class should be prepended with the character `m`;
- Class member attributes should be declared before the member functions;
- The global or class constants and enumeration values should be all capital letters separated with `_`;
    - The only exception are the view constants, which are all lower camel case and reside in the `vcl::views` namespace;

Example:

```cpp
class MyClass {
    int mMyAttribute = 0;

public:
    const int MY_CONSTANT = 42;

    enum class MyEnum {
        VALUE_1,
        VALUE_2
    };

    void myFunction(int myParameter) 
    {
        mMyAttribute = myParameter;
    }

};

void anotherFunction() 
{
    MyClass myObject;
    myObject.myFunction(42);
}

```

## File naming 

- File names should be lower case, and should use `_` as word separator;
- Header file names should end with the `.h` extension;
- Source file names should end with the `.cpp` extension;

