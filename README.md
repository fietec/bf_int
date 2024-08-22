# BF_INT (Brainfuck Interpreter by cdm)
A small scale brainfuck interpreter written purely in c.
This project is meant to be an exercise for myself. Therefore be reminded that this intpreter is potentially flawed and slow.

## How to build
To build, simply compile ```bf_int.c``` with your favourite c compiler, e.g.:
```
gcc -o bf bf_int.c
```
## How to run
The comannd line interface of this interpreter is as simple as it gets. Simply run:
```
bf <brainfuck-file>
```

### Technical Details
The interpreter consists of two steps: Lexing and Interpreting
In the lexing step all valid brainfuck commands are collected and stored in dynamic array, implemented in ```dynarray.h```.
The resulting chain of commands is then in-time interpreted, meaning no command meta data was collected by the lexer.
Therefore the running of the code or this interpreter is slower compared to others, which follow= a more sophisticated approach of e.g. storing jump information in the lexing process.

If you still want to use this project of mine after all this, have fun and enjoy!