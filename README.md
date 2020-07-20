# perf-test-for-io_uring

a framework for performance tests on io_uring.

# compile

```
mkdir cmake-build-debug
cd cmake-build-debug/
cmake ..
make all
```

# usage

compile out are several elf file, name in this rule:

>example: round_sqpoll-read
>
>   means this test is loaded by `round` loader, and test unit is `sqpoll-read`
>
>   loader determines how test runs, and test unit determines which test to run.
>   
>   `round` loader runs test for fixed rounds, and count time used.
>
>   `sqpoll-read` test unit tests the `read` functionality in `sqpoll` mode.
>
>   there are kinds of loaders and test units, and it is easy to add more.

run the elf file, and refer to detailed help via argument `-h`:

example: round_writev

```
$./round_writev -h
[INFO]  start test : /tmp/tmp.31XL3mGv3i/src/test/writev.cpp
usage:    [-s io_size]
          [-d io_depth]
          [-i file_size]
          [-h]
          -f filename
```

```
$./round_writev -i 536870912 -f /path/to/test_file
```

Besides, there are several tests does not need any argument to run. they will not process any argument passed.

# how test runs

a single test is assembled from two part, loader and test.

loader controls how to run test, and test defines which part to test.

the process of this framework's work can be showed in the following pseudo code:

```c
int main(){
    loader_init();
    loader_run_test();
    loader_cleanup();
}
```

and there are 3 function in each single test can be used in `loader_*()`:
```c
void test_init(int argc, char **argv);

void test_loop_body(int argc, char **argv);

void test_cleanup(int argc, char **argv);
```

literally, the `test_init()` and `test_cleanup()` should be executed once, while put `test_loop_body()` in a loop


# how to add single test

1. add a `.cpp` file in `/src/test/`, filename corresponds to the test
2. implement 3 functions and define a `char[]` test description in the `.cpp` added previously.
    - functions to be implemented:
        1. `void test_init(int argc, char **argv);`: run once, before `test_loop_body`, 
        2. `void test_loop_body(int argc, char **argv);`: run repeatedly, the main part of test
        3. `void test_cleanup(int argc, char **argv);`: run once, after `test_loop_body`
    - test description:
        1. `char test_name[]`, usually use `char test_name[] = __FILE__;`
   
# how to add loader

1. add a `.cpp` file in `/src/loader`
2. implement 3 functions in the `.cpp`
    - functions to be implemented:
        1. `void loader_init(int argc, char **argv);`
        2. `void loader_run_test(int argc, char **argv);`
        3. `void loader_clean_up(int argc, char **argv);`
    - all three functions will be executed once in `main()` as showed above
