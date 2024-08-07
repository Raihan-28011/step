# Day 11 & 12
Our goal today is to add more rules to the grammar, parse them and generate bytecodes for them.

# New Instructions and Operators
We have implemented relational operators and `icmp` and `fcmp` instructions. To use the result of these two instructions and the result of relational operators, we introduced 6 new instructions, `pushtp`, `pushfp`, `pushtz`, `pushfz`, `pushtn` and `pushfn`. We also implemented `Boolean` object to support booleans. And incorported two new keywords, `true` and `false`.

# Todos
1. Add parsing errors or syntax errors.
2. Add approach section for virtual machine.

# Issues
We will keep listing issues below as we face them and notice them. Issues are written with their priorities. And also the status of the issue, if done, is updated here.
1. The file that is being processed now, its name is not being saved anywher except argument parser. (priority: Very High) (**DONE**)
2. No way to compile multiple files (Priority: Low)
3. Files are very unorganized. Make them organized in meaningful folders. (Priority: Low)
4. Tokens are stored as `std::string` now. But, we need their actual representation like integers as `long long`, floats as `double`. (Priority: Very Very High) (**DONE**)
5. The Error Managing system is getting too messy. Have to clean it up. (Priority: Very Very high) (**DONE**)
6. Grammer should introduce left recusrion to get left associativity. (Priority: Very Very high) (**DONE**)
