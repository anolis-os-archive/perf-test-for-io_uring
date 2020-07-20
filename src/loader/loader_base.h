//
// Created by qiyilang.wqyl on 2020/7/8.
//

#ifndef IO_URING_PERF_LOADER_BASE_H
#define IO_URING_PERF_LOADER_BASE_H

#include "../helper.h"

extern void test_init(int argc, char **argv);

extern void test_loop_body(int argc, char **argv);

extern void test_cleanup(int argc, char **argv);

extern char test_name[];


void loader_init(int argc, char **argv);

void loader_run_test(int argc, char **argv);

void loader_clean_up(int argc, char **argv);


#endif //IO_URING_PERF_LOADER_BASE_H
