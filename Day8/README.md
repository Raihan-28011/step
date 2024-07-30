# Day 8
Our goal is to fix the grammar to make 

# Fixing the grammar
First thing first, we have to fix the grammar. There is nothing wrong with the grammar. But, in terms of programming language step, the grammar makes some operators right associative rather being left associative. Therefore, we have to introduce left recusrion and reduce it so that our LL(1) parser can parse it correctly.
```
expression ::= expression '+' term
             | expression '-' term
             | term

term ::= term '*' factor
       | term '/' factor
       | factor

# rest of the rules
    ...

```
We removed the rules that are unnecessary for this transformation. If you look closely, we have introduced left recursion. Now we have to reduce it to make it in CMK and also appropriate for LL(1) parser.
```
expression ::= term expression_suf

expression_suf ::= '+' term expression_suf
                 | '-' term expression_suf
                 | EMPTY
```
We can transform it further to simplify.
```
expression ::= term expression_suf

expression_suf ::= (('+' | '-') term)*
```
Simplifying further gets us,
```
expression ::= term (('+' | '-') term)*
```
Similarly,
```
expression ::= term (('+' | '-') term)*

term ::= factor (('*' | '/' | '%') factor)*
```
Now, the left recusrion has been reduced and the grammar is redefined such that LL(1) parser can parse it. Let's try to fix the parser to support this grammar now.

# Issues
We will keep listing issues below as we face them and notice them. Issues are written with their priorities. And also the status of the issue, if done, is updated here.
1. The file that is being processed now, its name is not being saved anywher except argument parser. (priority: Very High) (**DONE**)
2. No way to compile multiple files (Priority: Low)
3. Files are very unorganized. Make them organized in meaningful folders. (Priority: Low)
4. Tokens are stored as `std::string` now. But, we need their actual representation like integers as `long long`, floats as `double`. (Priority: Very Very High) (**DONE**)
5. The Error Managing system is getting too messy. Have to clean it up. (Priority: Very Very high) (**DONE**)
6. Grammer should introduce left recusrion to get left associativity. (Priority: Very Very high) (**DONE**)
