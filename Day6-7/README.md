# Day 6 & 7
Our goal is to fix some bugs and issues. And finally to start working on the parser.

# Approach for Parser
We planned to write a recursive descent parser (LR(1)), which is very fairly easy to write. We also changed the grammar to make it more standard. Here is the changed grammar.
```
expression ::= term '+' expression
             | term '-' expression
             | term

term ::= factor '*' term
       | factor '/' term
       | factor

factor ::= literal
         | '(' expression ')'

literal ::= inetger
         | float

integer ::= [0-9]([0-9]*_*[0-9])*

float ::= integer '.' integer ('E' | 'e')? ('+' | '-')? integer
```
Let's talk about a little bit about the grammar. This grammar has no left recursion. We eliminated all the left recursions following Chomsky Normal Form. But this has a very crucial and inherent flaw. The flaw is, it makes operators right associative, which is not conventional approach. For example, let's consider the parse tree generated for the expression, `1 + 2 + 3 * 4 / 5`,
```
BinaryOperator:
op    -> +
left  -> 1
right -> BinaryOperator:
        op    -> +
        left  -> 2
        right -> BinaryOperator:
                 op    -> *
                 left  -> 3
                 right -> BinaryOperator:
                          op    -> /
                          left  -> 4
                          right -> 5
```
The evaluation order of the expression will be this, `1 + (2 + (3 * (4 / 5)))`. But, what we wanted was `(1 + 2) + ((3 * 4) / 5)`. This expression clerly shows the left associativity of the operators. Therefore, eliminating left recursion also introduces right associativity rather left associativity. We have to fix this issue.

# Issues
We have some design issues that we need to fix. Below we are listing those that we have noticed, 
1. The file that is being processed now, its name is not being saved anywher except argument parser. (priority: Very High) (**DONE**)
2. No way to compile multiple files (Priority: Low)
3. Files are very unorganized. Make them organized in meaningful folders. (Priority: Low)
4. Tokens are stored as `std::string` now. But, we need their actual representation like integers as `long long`, floats as `double`. (Priority: Very Very High) (**DONE**)
5. The Error Managing system is getting too messy. Have to clean it up. (Priority: Very Very high) (**DONE**)
6. Grammer should introduce left recusrion to get left associativity. (Priority: Very Very high)
