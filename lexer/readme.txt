this is the design/architecture doc for the lexer

The main job of the lexer is to simply convert incoming text (which has been preprocessed so that includes, comment stripping, text replacements are processed)
into a set of standardized tokens. Everything from Reserved keywords, types, operators, logical comparators and more.
In my design the preprocessor and lexer are separate distinct programs you need to call. (to help reinforce the steps in the process)

My design heavily relies on istringstream to get text delimited by spaces, and I kinda designed most of my logic around that, so
for now quotations being used to delimit strings and allows spaces is not supported. In the future I should probably move away
from this, but for now it'll do.

TokenTable is the class that holds an unordered map of every single keyword. This is used to find keywords that occur within text,
which is used to generate a sequential vector of tokens as they appear in the text.

The parser is designed to be LL(k), wherein it parses tokens left to right, Left-most derivation, lookahead by k (for this design k is , since the grammar is quite simple)

The parser will expect certain formats for the incoming tokens, here are some examples:

    Reserved Keywords:
        For:
            FOR, LPAREN, [logic], RPAREN, LBRACE, [logic], RBRACE
        else:
            [must be trailing an RBRACE of a for loop] WHILE, LPAREN, [logic], RPAREN, LBRACE, [logic], RBRACE
        While:
            WHILE, LPAREN, [logic], RPAREN, LBRACE, [logic], RBRACE




tokenType:
    this is a special enum that stores the type of the token. There is no association with string or char literals yet,
    so the "linking" is done inside class constructor with addToken()

