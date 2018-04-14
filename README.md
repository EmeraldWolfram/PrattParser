# PrattParser

What is needed in a compiler?
--------------
Before going into the detail, lets start with how a compiler work.
There were 3 phase in the Compiler:
- Phase 1: Tokenizing the source file
- Phase 2: Parsing token with precedence (What this repo is doing)
- Phase 3: Executing with the Processor provided instruction code (x86 or ARM or your own custom designed RTL processor)

Computer is dumb, there will do the first thing the see instead of using precedence control.
For example, 
  2+3*4
We human know that we have to do multiply first (3 * 4) = 12 then follow by 2 + 12 = 14.
But computer will do the firsr expression without the help of compiler meaning 2 + 3 = 5 then follow by 5 * 4 = 20

Therefore, a parser is needed to check operator precedence and link up a Token Tree

A Tokenizer will parse a file into a Serial Chain of Token in FIFO (First In First Out) manner.
This project assumed that the Tokenizer was there and the Tokenizer provide us two API:
- getToken()   - Return caller the head Token and remove from the Serial Chain
- peepToken()  - Return the pointer to the head Token but remain the Token in the Serial Chain

Detail Implementation
-------
The parser(0) function is a recursive function where it will call back itself to continue until the end of live symbol were found.
In this project '$' was used as the End of Live (EoL) Token.

PrattParser rely on 2 recursive function pointer to do the job (what u guys call method in class for higher level language like JAVA)
- nud()
- led()

nud() will 
return the value if it is not an expression like (integer, double or float)
or
call back parser() if it is an operator symbol like (+, -, >>, / ...) with it current operator binding power (example parse(20))

Whereas, led() function was used to assign the executing method to the token if it is an (Operator Token)
*Hint: The number of instruction set provided by the processor >= execute method to be implement.


Usage Model
-------------
This PrattParser provide one function:
Token* parser(int prevBindingPower);

Always provide the argument to the parser as 0
Eg.
  parser(0) 
and it will form the tree and return the pointer to the top Token in the Token Tree

Please override all the execute function in the execute.c file as I didn't put in any assembly instruction.
I simply implemented the code by printing the token to be execute wrapping in bracket only. 
