# Day 3 & 4 & 5
Our goal is going to build an Error Management System.

# Initial Approach
First thing we decided was to specialize each error type as a derived class of `Step::IError` interface (abstract class). Like, `Step::NoInpuFile`, `Step::NotValidFile`, `Step::UnrecognizedOption` etc. But, then we came across a similar pattern in some of this errors, so we decided to not to create class for each of them. We created a generic error class `Step::GenericError`, it just prints the pre-compilation errors. Then we have errors during lexing process. We plan to make them all concrete specialed classes of `Step::IError`. Though if found suitable and feasible, we can make some exceptions to that plan.

# Improved Approach
We just cleaned all the excess and unnecessary duplicate code by introducing new classes each time we add a new error. We made a `Step::PreCompilationError` class for all the pre-compilation errors (like, unrecognized command-line options, no input files, file could not be opened etc.). Then we made another class `Step::LexicalError` for all the lexical-analysis errors.

# Issues
We have some design issues that we need to fix. Below we are listing those that we have noticed, 
1. The file that is being processed now, its name is not being saved anywher except argument parser. (priority: Very High) (**DONE**)
2. No way to compile multiple files (Priority: Low)
3. Files are very unorganized. Make them organized in meaningful folders. (Priority: Low)
4. Tokens are stored as `std::string` now. But, we need their actual representation like integers as `long long`, floats as `double`. (Priority: Very Very High)
5. The Error Managing system is getting too messy. Have to clean it up. (Priority: Very Very high)
