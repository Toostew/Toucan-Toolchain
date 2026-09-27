this is the design/architecture doc for the lexer

tokenType:
    this is a special enum that stores the type of the token. There is no association with string or char literals yet,
    so the "linking" is done manually inside class constructor with addToken()

