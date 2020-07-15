# perf-test-for-io_uring

a framework for performance tests on io_uring.

# compile

```shell script
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

```shell script
$./round_writev -h
[INFO]  start test : /tmp/tmp.31XL3mGv3i/src/test/writev.cpp
usage:    [-s io_size]
          [-d io_depth]
          [-i file_size]
          [-h]
          -f filename
```

```shell script
$./round_writev -i 536870912 -f /path/to/test_file
```

Besides, there are several tests does not need any argument to run.

