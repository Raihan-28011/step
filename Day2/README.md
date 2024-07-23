# Day 2
Our goal today is going to be building a very basic lexer and parser for the language produced by the following grammar,
```
expr ::= term '+' expr
       | term '-' expr
       | term

term ::= factor '*' term
       | factor '/' term
       | factor

factor ::= number
         | '(' expr ')'

number ::= inetger
         | float

integer ::= [0-9]([0-9]*_*[0-9])*

float ::= integer '.' integer ('E' | 'e')? ('+' | '-')? integer
```

***CAUTION: DO NOT GIVE ERRONEOUS FILES TO THE PROGRAM! ERROR SYSTEM IS NOT IMPLEMENTED YET.***

### Approach
The first thing we did was to create an `IReader` interface (or C++ abstract class), which worked as the base to build out the reader interface as a whole. We created a `FileReader` that reads `2KB` of data from a file at a time. This was done only to avoid reading big large files. Then we created `LineReader` that expects an `IReader` as the primary source of data. Then `LineReader` keeps reading data and splits them into lines and also saves them in a `std::vector`. Furthermore, we also created a `LexemeReader` which also expects an `IReader` as the primary source of data. It tokenizes the data as it reads data one by one. If you look closely, the reader interface actually follows the `Decorator` design pattern, almost similar to that of Java IO library.

### Tokenization or Lexation Logic
The logic behind lexation or tokenization is in [LexemeReader::process()](src/LexemeReader.cpp) method. We process lines one by one. As we do so, we are also creating a `std::vector<Step::Lexeme>`, which is a vector of all the tokens or lexemes. Here is the `while` loop that tokenizes the lines,
```c++
    std::size_t lexeme_begin = 0, forward = 0; 
    while (forward < line.length()) {
        skipws(line, forward);
        lexeme_begin = forward;
        char c = line.at(forward);
        switch (c) {
        case '\n':
            _lexemes.emplace_back("\\n", line_no++, ++forward, LexemeKind::NEWLINE);
            break;
        case '+':
            /* create lexeme */
        case '-':
            /* create lexeme */
        case '*':
            /* create lexeme */
        case '/':
            /* create lexeme */
        case '%':
        /* --- rest of the tokens --- */
        default:
            if (is_digit(c)) {
                process_float(line, forward, lexeme_begin, line_no);
            } else { 
                /* rest of the processing */
            }
            break;
        }
    }
```
You can also notice, we have a `process_float()` function that processes both integer and floats. You can check out the [LexemeReader::process_float()](src/LexemeReader.cpp) for more details. The summary of the function is as follows,
1. Keep moving forward until you run out of valid digits
2. Check if the encountered charact is '.' or not,
    i. If it is a '.' then process float.
    ii. If it is not, then this is a plain integer.

### Final Words
We have added test in the [tests/](tests/) folder. Tested the current working version of the step. And it is successfully working. We have completed our task to parse the grammar we mentioned earlier.

### Next Goal
Our next goal will be to add some way to handle errors. Currently if you give any erroneous code to the program, it will mark it as miscellaneous token.
