# Day 6
Our goal is to fix some bugs and issues. And finally to start working on the parser.

# Issues
We have some design issues that we need to fix. Below we are listing those that we have noticed, 
1. The file that is being processed now, its name is not being saved anywher except argument parser. (priority: Very High) (**DONE**)
2. No way to compile multiple files (Priority: Low)
3. Files are very unorganized. Make them organized in meaningful folders. (Priority: Low)
4. Tokens are stored as `std::string` now. But, we need their actual representation like integers as `long long`, floats as `double`. (Priority: Very Very High) (**DONE**)
5. The Error Managing system is getting too messy. Have to clean it up. (Priority: Very Very high) (**DONE**)
