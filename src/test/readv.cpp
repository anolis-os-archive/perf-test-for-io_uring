//
// Created by qiyilang.wqyl on 2020/7/9.
//

#include "test_base.h"
#include <liburing.h>
#include <iostream>
#include <cstring>
#include <random>

char test_name[] = __FILE__;

using namespace std;

// io_uring instance
static io_uring *iu;

// test config
static unsigned int io_size = 4 * 1024;
static unsigned int io_depth = 16;

// associated file
static FILE *fp;
static char *file_name;
static unsigned int file_size = 16 * 1024 * 1024;

// vars for globally use
static int *offsets;
static iovec *iovs;


static argp_option ao[] = {
        {nullptr, 'd', "io_depth",  0, "set io_depth"},
        {nullptr, 'i', "io_size",   0, "set io_size"},
        {nullptr, 'f', "file_name", 0, "set file path for test"},
        {nullptr, 's', "file_size", 0, "set test file size"},
        {nullptr}
};

static int arg_parser(int key, char *text, argp_state *input) {
    switch (key) {
        case 'd':
            io_depth = strtol(text, nullptr, 10);
            break;
        case 'i':
            io_size = human2size(text);
            break;
        case 'f':
            file_name = static_cast<char *>(malloc(strlen(text) + 1));
            strcpy(file_name, text);
            break;
        case 's':
            file_size = human2size(text);
            break;
        default:
            break;
    }
    return 0;
}

argp ap_test = {ao, arg_parser, nullptr, nullptr};


void test_init(int argc, char **argv) {
    int ret;

    // prepare file
    DEBUG("filename = %s , file_size = %u", file_name, file_size)
    ERR_TEST(fopen(file_name, "w"), fp, fp == nullptr)
    ERR_TEST(fseek(fp, file_size, SEEK_SET), ret, ret != 0)
    ERR_TEST(fputc('\0', fp), ret, ret == EOF)
    ERR_TEST(fclose(fp), ret, ret == EOF)
    ERR_TEST(fopen(file_name, "r"), fp, fp == nullptr)

    // create io uring instance
    iu = new io_uring;
    ERR_TEST(io_uring_queue_init(2 * io_depth, iu, 0), ret, ret != 0)

    offsets = new int[io_depth];
    iovs = new iovec[io_depth];
    default_random_engine dre;
    uniform_int_distribution<int> uid(0, file_size);
    for (int i = 0; i < io_depth; ++i) {
        offsets[i] = uid(dre);
        iovs[i].iov_base = static_cast<char *>(malloc(io_size));
        iovs[i].iov_len = io_size;
    }
}

static void io_action() {
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;
    int ret;
    int io_count = 0;

    for (int j = 0; j < io_depth; ++j) {
        ERR_TEST(io_uring_get_sqe(iu), sqe, sqe == nullptr)
        io_uring_prep_readv(sqe, fp->_fileno, iovs, io_depth, offsets[j]);
    }
    ERR_TEST(io_uring_submit(iu), ret, ret <= 0)

    for (int i = 0; i < io_depth; ++i) {
        ERR_TEST(io_uring_wait_cqe(iu, &cqe), ret, ret < 0)
        io_uring_cqe_seen(iu, cqe);
        ++io_count;
    }
}

void test_loop_body(int argc, char **argv) {
    (void) argc;
    (void) argv;
    io_action();
}

void test_cleanup(int argc, char **argv) {
    (void) argc;
    (void) argv;
    io_uring_queue_exit(iu);
    fclose(fp);
}