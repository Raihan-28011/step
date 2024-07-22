# Day 1
Our goal today is to build an ArgumentParser to parse command line arguments.

### Key Features
1. Convert C-style argv to C++ `std::vector<std::string>>`.
2. Add new rules/options through method.
3. Parse arguments in arbitrary order.
4. Generate help message from passed rules/options.
5. Generate error and warning messages for invalid/wrong arguments and argument values.

### Design Decisions
I had to write it such that I don't have to make modifications to the implementation every when I want to add a new option. Therefore, I decided to design a generic parser, where I can add options without doing any modifications to the actual implementation. The interface is inspired from the python package `argparse`. I love the interface of that package, so I took it.

### Parse Logic
The primary logic of the parsing lies in this following code fragment,
```c++
    /* more code */
    while (index < _argv.size()) {
        auto arg = _argv.at(index);
        auto rule = std::find_if(_rules.begin(), _rules.end(), [&arg](ArgumentRule const &a) {
            return a._option == arg || a._long_option == arg;
        });

    /* more code */
```
If the argument is present in the predefined rules, we will move on and add it to valid options. Otherwise, this is an invalid option further argument parsing is unnecessary. To see the full implementation, go to [ArgumentParser.cpp:65](src/cmd/ArgumentParser.cpp).
