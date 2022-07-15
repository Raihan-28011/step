# step - A New Interpreted Language

step is basically merge of python and javascript. It's type system is dynamic.
In step, everything is object, even the primitives are also object.

# Currently Implemented Features

* literals
    - integers, strings, booleans
    ```python
        print(1203298, "\t'string literal'\t", true);
        # output: 1203298    'string literal'    true
    ```
* variables
    ```python
        stringVar = "string";
        print(stringVar, "\n");

        stringVar = true;
        print(stringVar, "\n");
    ```
* functions
    ```python
        def function() {    # Currently arguments are not supported
            print("Calling from function body\n");
        }
    ```
* Operators
    - `+`, `-`, `*`, `**`, `/`, `%`, `<`, `>`, `<=`, `>=`, `==`, `!=`, `||`, `&&`
