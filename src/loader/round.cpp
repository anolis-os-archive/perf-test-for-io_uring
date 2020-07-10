//
// Created by qiyilang.wqyl on 2020/7/8.
//

#include "loader_base.h"
#include <iostream>
#include <chrono>
#include <getopt.h>

using namespace std;
using namespace chrono;

static unsigned int warm_up_round = 5;
static unsigned int round_count = 16;

static void arg_parser(int argc, char **argv) {
    int ret;

    optind = 1;
    while ((ret = getopt(argc, argv, "w:r:")) != -1) {
        switch (ret) {
            case 'w':
                warm_up_round = strtol(optarg, nullptr, 10);
                break;
            case 'r':
                round_count = strtol(optarg, nullptr, 10);
                break;
            default:
                break;
        }
    }
}

void loader_init(int argc, char **argv) {
    static time_point<steady_clock> timer_a;
    static time_point<steady_clock> timer_b;

    //arg_parser(argc, argv);

    INFO("start test : %s", test_name)

    timer_a = steady_clock::now();
    test_init(argc, argv);
    timer_b = steady_clock::now();
    INFO("test_init \t: %ld", duration_cast<microseconds>(timer_b - timer_a).count())

    DEBUG("%s", "start warming up...")
    for (int i = 0; i < warm_up_round; ++i) {
        timer_a = chrono::steady_clock::now();
        test_loop(argc, argv);
        timer_b = chrono::steady_clock::now();
        INFO("warm up #%d \t: %ld", i, duration_cast<microseconds>(timer_b - timer_a).count())
    }
}

void loader_run_test(int argc, char **argv) {
    static chrono::time_point <steady_clock> timer_a;
    static chrono::time_point <steady_clock> timer_b;

    DEBUG("%s", "start running test...")
    for (int i = 0; i < round_count; ++i) {
        timer_a = chrono::steady_clock::now();
        test_loop(argc, argv);
        timer_b = chrono::steady_clock::now();
        INFO("test #%d \t: %ld", i, duration_cast<microseconds>(timer_b - timer_a).count())
    }
}

void loader_clean_up(int argc, char **argv) {
    static chrono::time_point <steady_clock> timer_a;
    static chrono::time_point <steady_clock> timer_b;

    DEBUG("%s", "start cleanup")
    timer_a = chrono::steady_clock::now();
    test_cleanup(argc, argv);
    timer_b = chrono::steady_clock::now();
    INFO("clean up \t: %ld", duration_cast<microseconds>(timer_b - timer_a).count())
}