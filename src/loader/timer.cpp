//
// Created by qiyilang.wqyl on 2020/7/16.
//

#include "loader_base.h"
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <zconf.h>

static unsigned *count_arr;
static unsigned round_count = 0;

static bool mark = true;

pthread_t thread_alter;
pthread_t thread_run;

pthread_rwlock_t lock_mark;
pthread_rwlock_t lock_res;

static void *clocking(void *arg) {
    for (int this_round = 0; this_round < *(int *) arg; ++this_round) {
        sleep(1);

        pthread_rwlock_rdlock(&lock_res);
        count_arr[this_round] = round_count;
        pthread_rwlock_unlock(&lock_res);
    }

    pthread_rwlock_wrlock(&lock_mark);
    mark = !mark;
    pthread_rwlock_unlock(&lock_mark);
    return nullptr;
}

static void *run_io(void *args) {
    (void) args;

    while (true) {
        pthread_rwlock_rdlock(&lock_mark);
        if (!mark) break;
        pthread_rwlock_unlock(&lock_mark);

        test_loop_body(0, nullptr);

        pthread_rwlock_wrlock(&lock_res);
        ++round_count;
        pthread_rwlock_unlock(&lock_res);
    }
    return nullptr;
}


void loader_init(int argc, char **argv) {
    int ret;

    INFO("start test : %s", test_name)

    test_init(argc, argv);

    ERR_TEST(pthread_rwlock_init(&lock_mark, nullptr), ret, ret != 0)
    ERR_TEST(pthread_rwlock_init(&lock_res, nullptr), ret, ret != 0)
}

void loader_run_test(int argc, char **argv) {
    int ret;
    int warm_time_len = 5;
    int run_time_len = 10;
    int ttl_time_len;
    int this_round;

    count_arr = static_cast<unsigned int *>(malloc((run_time_len + warm_time_len) * sizeof(unsigned int)));

    ttl_time_len = warm_time_len + run_time_len;
    ERR_TEST(pthread_create(&thread_alter, nullptr, clocking, &ttl_time_len), ret, ret != 0)
    ERR_TEST(pthread_create(&thread_run, nullptr, run_io, nullptr), ret, ret != 0)

    sleep(1);

    INFO("%s", "warm up")
    for (this_round = 0; this_round < warm_time_len; ++this_round) {
        sleep(1);
        printf("IOPS: %d\n", count_arr[this_round] - (this_round == 0 ? 0 : count_arr[this_round - 1]));
    }

    INFO("%s", "warm up finish")

    for (; this_round < warm_time_len + run_time_len; ++this_round) {
        sleep(1);
        printf("IOPS: %d\n", count_arr[this_round] - count_arr[this_round - 1]);
    }
}

void loader_clean_up(int argc, char **argv) {
    int ret;

    ERR_TEST(pthread_rwlock_destroy(&lock_mark), ret, ret != 0)
    ERR_TEST(pthread_rwlock_destroy(&lock_res), ret, ret != 0)
}