this is the readme for the parser covering design choices and logic

what the parser is
a recursive descent parser (top down) with one token of lookahead, so LL(1). it reads the token stream from the lexer and builds an AST. it only checks syntax. undeclared variables, type mismatches, missing returns and so on are handled later by the semantic analysis pass, not here. later stages (semantic analysis, and eventually codegen) walk the AST with visitors.

how recursive descent works here
there is one function per grammar rule and they call each other. lower precedence rules are called first, and higher precedence rules sit further down the call chain. so in a + b * c the multiplication ends up deeper in the tree, which means it gets evaluated first.

[TODO: list the precedence levels from lowest to highest, and say which operators are left associative]

lexer note (deliberate choice for now)
though not a parser issue, the lexer creates identifiers through a very lax rule: any non-keyword separated by whitespace is an identifier, so stuff like !-!helloreturn is considered an identifier. the knock-on effect is that unspaced operators (a-b, !x, a!=b) can turn into one identifier instead of several tokens. for v1 the rule is to put spaces around every operator. fixing it properly means making the lexer operator aware, which i am leaving for later to keep the lexer simple.

line numbers
tokens carry their line number. [TODO: confirm that AST nodes copy it from the tokens, since that is what lets the DumpVisitor print line ranges]

known loose ends (also deliberate for now)
- the BNF does not yet cover the initializer and for loop changes
- IfStatementType is redundant. the plan is to drop it and check whether the else branch is null instead

[TODO: error handling. does the parser stop at the first syntax error, or try to recover?]
[TODO: who owns the AST nodes, and why]

c++ notes
in C++, lvalues are long lived values, think values that are allocated memory, have named variables, etc
rvalues are temp values, they only exist for a brief moment, usually only to init an lvalue

int x = 50; <-- x is an lvalue, 50 is an rvalue. x will continue to exist after this, but 50 will not

std::move() does not actually move anything. Its only job is to perform a compile-time cast that converts
an lvalue into an rvalue reference (T&&).