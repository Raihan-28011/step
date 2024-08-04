# Day 9 & 10
Our goal is to write a bytecode virtual machine and integrate it with the parser.

# Choice of Virtual Machine
There are two kinds of virtual machine.
1. Stack Based
2. Register Based
For an initial experiment we implemented Stack based virtual machine. Later on, we might introduce register based virtual machine.

# Approach
TODO

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
