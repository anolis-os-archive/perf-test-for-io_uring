//
// Created by qiyilang.wqyl on 2020/7/9.
//

#include "test_base.h"
#include <liburing.h>
#include <iostream>

char test_name[] = __FILE__;

using namespace std;


// io_uring instance
static io_uring *iu;

static unsigned int io_depth = 16;

static argp_option ao[] = {
        {"io-depth", 'd', "io_depth", 0, "set io_depth"},
        {nullptr}
};

static int arg_parser(int key, char *text, argp_state *input) {
    switch (key) {
        case 'd':
            io_depth = strtol(text, nullptr, 10);
            break;
        default:
            break;
    }
    return 0;
}

argp ap_test = {ao, arg_parser, 0, 0};

void test_init(int argc, char **argv) {
    int ret;

    (void) argc;
    (void) argv;

    // create io uring instance
    iu = new io_uring;
    ERR_TEST(io_uring_queue_init(2 * io_depth, iu, 0), ret, ret != 0)
}

void test_loop_body(int argc, char **argv) {
    int ret;
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;

    (void) argc;
    (void) argv;

    for (int i = 0; i < io_depth; ++i) {
        ERR_TEST(io_uring_get_sqe(iu), sqe, sqe == nullptr)
        io_uring_prep_nop(sqe);
    }
    ERR_TEST(io_uring_submit(iu), ret, ret <= 0)

    for (int i = 0; i < io_depth; ++i) {
        ERR_TEST(io_uring_wait_cqe(iu, &cqe), ret, ret < 0)
        io_uring_cqe_seen(iu, cqe);
    }
}

void test_cleanup(int argc, char **argv) {
    (void) argc;
    (void) argv;
    io_uring_queue_exit(iu);
}