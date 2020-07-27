//
// Created by qiyilang.wqyl on 2020/7/8.
//

#include "loader_base.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

static unsigned int warm_up_round = 5;
static unsigned int round_count = 16;

static argp_option ao[] = {
        {nullptr, 'w', "warm_up",     0, "set warm up round"},
        {nullptr, 'r', "round_count", 0, "set round count"},
        {nullptr}
};

static int arg_parser(int key, char *text, argp_state *input) {
    switch (key) {
        case 'w':
            warm_up_round = strtol(text, nullptr, 10);
            break;
        case 'r':
            round_count = strtol(text, nullptr, 10);
            break;
    }
    return 0;
}

argp ap_loader = {ao, arg_parser, nullptr, nullptr};

static long int get_duration_in_us(time_point<steady_clock> later, time_point<steady_clock> former) {
    return duration_cast<microseconds>(later - former).count();
}

static long int get_duration_in_ns(time_point<steady_clock> later, time_point<steady_clock> former) {
    return duration_cast<nanoseconds>(later - former).count();
}

void loader_init(int argc, char **argv) {
    static time_point<steady_clock> timer_a;
    static time_point<steady_clock> timer_b;

    //arg_parser(argc, argv);

    INFO("start test : %s", test_name)

    timer_a = steady_clock::now();
    test_init(argc, argv);
    timer_b = steady_clock::now();
    printf("test_init \t: %ld us\n", get_duration_in_us(timer_b, timer_a));

    DEBUG("%s", "start warming up...")
    for (int i = 0; i < warm_up_round; ++i) {
        timer_a = chrono::steady_clock::now();
        test_loop_body(argc, argv);
        timer_b = chrono::steady_clock::now();
        printf("warm up #%d \t: %ld us\n", i, get_duration_in_us(timer_b, timer_a));
    }
}

void loader_run_test(int argc, char **argv) {
    static time_point<steady_clock> timer_a;
    static time_point<steady_clock> timer_b;

    DEBUG("%s", "start running test...")
    DEBUG("%s", "result show in ns")
    for (int i = 0; i < round_count; ++i) {
        timer_a = chrono::steady_clock::now();
        test_loop_body(argc, argv);
        timer_b = chrono::steady_clock::now();
        printf("test #%d latency \t: %ld\n", i, get_duration_in_ns(timer_b, timer_a));
    }
}

void loader_clean_up(int argc, char **argv) {
    static time_point<steady_clock> timer_a;
    static time_point<steady_clock> timer_b;

    DEBUG("%s", "start cleanup")
    timer_a = chrono::steady_clock::now();
    test_cleanup(argc, argv);
    timer_b = chrono::steady_clock::now();
    printf("clean up \t: %ld us\n", get_duration_in_us(timer_b, timer_a));
}