//
// Created by qiyilang.wqyl on 2020/7/10.
//

#include <liburing.h>
#include <cstring>
#include <unistd.h>
#include "test_base.h"
#include "iostream"

char test_name[] = __FILE__;

using namespace std;


// io_uring instance
static io_uring *iu;

// associated file
static int fd;
static char file_name[] = "/tmp/io_uring_perf/io_uring_perf-XXXXXX";


static void show_help() {
    cout <<
         "usage:    " << endl <<
         "          [-h]" << endl;
}

static void arg_parser(int argc, char **argv) {
    int ret;

    optind = 1;
    while ((ret = getopt(argc, argv, "s:d:i:f:h")) != -1) {
        switch (ret) {
            case 'h':
                show_help();
                exit(1);
            default:
                break;
        }
    }
}

void test_init(int argc, char **argv) {
    int ret;

    arg_parser(argc, argv);

    // prepare file
    ERR_TEST(mkstemp(file_name), fd, fd == -1)

    // create io uring instance
    iu = new io_uring;
    ERR_TEST(io_uring_queue_init(8, iu, 0), ret, ret != 0)
}

void test_loop_body(int argc, char **argv) {
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;
    int ret;

    (void) argc;
    (void) argv;

    ERR_TEST(io_uring_get_sqe(iu), sqe, sqe == nullptr)
    io_uring_prep_fsync(sqe, fd, 0);
    ERR_TEST(io_uring_submit(iu), ret, ret <= 0)
    ERR_TEST(io_uring_wait_cqe(iu, &cqe), ret, ret < 0)
    io_uring_cqe_seen(iu, cqe);
}

void test_cleanup(int argc, char **argv) {
    (void) argc;
    (void) argv;

    io_uring_queue_exit(iu);
    unlink(file_name);
}