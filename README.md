# perf-test-for-io_uring

a framework for performance testing on io_uring.

# compile

## dependencies

1. [liburing](https://github.com/axboe/liburing)
2. [CMake](https://cmake.org/)
3. pthread

## build

```
cmake -S . -B cmake-build-debug/
make -C cmake-build-debug/ all
```

# usage

## quick launch

```
bash ./test.sh
```

this command builds and runs all perf units with configs in `test.sh`

## detailed

compile outputs are several elf file, name in the following rule:

>example: round_sqpoll-read
>
>   means this perf unit is loaded by `round` loader, and test unit is `sqpoll-read`
>
>   loader determines how test runs, and test unit determines which functionality to perf.
>   
>   `round` loader runs test for fixed rounds, and count time used.
>
>   `sqpoll-read` test unit tests the `read` functionality in `sqpoll` mode.
>
>   there are kinds of loaders and test units, and it is easy to add more depends as wish.

run the elf file, and refer to detailed help via argument `-?`:

example: round_writev

```
$./round_write -?
Usage: round_write [OPTION...]

 Test Arguments
  -d io_depth                set io_depth
  -f file_name               set file path for test
  -i io_size                 set io_size
  -s file_size               set test file size

 Loader Arguments
  -r round_count             set round count
  -w warm_up                 set warm up round

  -?, --help                 Give this help list
      --usage                Give a short usage message

```

and config the runnning via arguments:

```
$./round_write -r 2 -w 1 -d 16 -f "/home/qiyilang.wqyl/nvme/test_file" -i 4K -s 500M
```

# how test runs

as mentioned in ep. `usage`, a perf unit is assembled from two part, loader and test.

loader determines how test runs, and test unit determines which functionality to perf.

loader contains 3 functions named in `loader_*()`, and `main()` use loader functions in such way:
```c
int main(){
    loader_init();
    loader_run_test();
    loader_cleanup();
}
```

and there are 3 function in each test unit, which can be used in `loader_*()`:
```c
void test_init(int argc, char **argv);

void test_loop_body(int argc, char **argv);

void test_cleanup(int argc, char **argv);
```

literally, the `test_init()` and `test_cleanup()` should be executed once, while put `test_loop_body()` in a loop

thus, the key to implement a perf unit is to add a test unit or a loader or both.

# how to add test unit

there are 3 functions, 1 `char[]` for description and 1 `argp` for argument handling to be implemented.

1. add a `.cpp` file in `/src/test/`, filename corresponds to the test
2. implement components:
    - functions to be implemented:
        1. `void test_init(int argc, char **argv);`: run once, before `test_loop_body`, 
        2. `void test_loop_body(int argc, char **argv);`: run repeatedly, the main part of test
        3. `void test_cleanup(int argc, char **argv);`: run once, after `test_loop_body`
    - `char[]` test description:
        1. `char test_name[]`, usually use `char test_name[] = __FILE__;`
    - `argp` arg handler
        1. `argp ap_test`, filling the fields depends on the args the **test unit** needed.

# how to add loader

there are 3 functions and 1 `argp` for argument handling to be implemented in loader.

1. add a `.cpp` file in `/src/loader`
2. implement components:
    - functions to be implemented:
        1. `void loader_init(int argc, char **argv);`
        2. `void loader_run_test(int argc, char **argv);`
        3. `void loader_clean_up(int argc, char **argv);`
    - `argp` arg handler
        1. `argp ap_loader`, filling the fields depends on the args the **loader** needed.
