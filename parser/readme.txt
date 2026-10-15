this is the readme for the parser covering design choices and logic

This parser is a recursive descent parser (top to bottom) with LL(1).

Recursive descent parsing involves recursive function calls, with lower precedent calls occuring first, and higher precedent
calls occuring down the call chain.