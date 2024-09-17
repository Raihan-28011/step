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
        def function() {
            print("Calling from function body\n");
        }
    ```

* Recursion
    ```python
        def fact(n) {
            if (n <= 1)
                return 1;
            return fact(n-1) * n;
        }

        print(fact(15)); # outpu: 1307674368000
    ```

* arrays
    ```python
        array = [232, 43434, [9323, "nested array"]];
        print(array);  # output: [232, 43434, [9323, 'nested array']]
    ```

* nested subscript and function calls
    ```python
        def func() {
            return "success";
        }

        def func2() {
            return [100, 200, func, [10, 92843, 121]];
        }

        array = [1, 2, "raihan kabir", [10, 20], func];
        print(array, "\n", array[3][0], "\n", array[4](), "\n", func2()[2](), "\n");
        # output:
        # [1, 2, 'raihan kabir', [10, 20], {Object::Function (0x1fcacf0)}]
        # 10
        # success
        # success
    ```

* Operators
    - Arithmetic operators: `+`, `-`, `*` `**`, `/`, `%`
    - Relational operators: `<`, `>`, `<=`, `>=`
    - Equality operators: `==`, `!=`
    - Logical Operators: `||`, `&&`
    - Subscript operator: `[]`
    - Function call operator: `()`
