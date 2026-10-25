this is the readme for the parser covering design choices and logic

This parser is a recursive descent parser (top to bottom) with LL(1).

NOTE: though not a parser issue, the lexer creates identifiers through a very lax rule, any non-keyword seperated by space is considered an identifier,
so stuff like !-!helloreturn is considered an identifier

Recursive descent parsing involves recursive function calls, with lower precedent calls occuring first, and higher precedent
calls occuring down the call chain.

in C++, lvalues are long lived values, think values that are allocated memory, have named variables, etc
rvalues are temp values, they only exist for a brief moment, usually only to init an lvalue

int x = 50; <-- x is an lvalue, 50 is an rvalue. x will continue to exist after this, but 50 will not



std::move() does not actually move anything. Its only job is to perform a compile-time cast that converts
an lvalue into an rvalue reference (T&&).