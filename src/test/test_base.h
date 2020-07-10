//
// Created by qiyilang.wqyl on 2020/7/8.
//

#ifndef IO_URING_PERF_TEST_BASE_H
#define IO_URING_PERF_TEST_BASE_H

#include "../helper.h"

void test_init(int argc, char **argv);

void test_loop(int argc, char **argv);

void test_cleanup(int argc, char **argv);

#endif //IO_URING_PERF_TEST_BASE_H
