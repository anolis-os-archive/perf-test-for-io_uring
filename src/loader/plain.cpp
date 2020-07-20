//
// Created by qiyilang.wqyl on 2020/7/8.
//

#include "loader_base.h"
#include <iostream>

using namespace std;

void loader_init(int argc, char **argv) {
    cout << "plain init" << endl;
    test_init(argc, argv);
}

void loader_run_test(int argc, char **argv) {
    cout << "plain test running" << endl;
    test_loop_body(argc, argv);
    cout << "plain testing finished" << endl;
}

void loader_clean_up(int argc, char **argv) {
    cout << "plain test cleanup" << endl;
    test_cleanup(argc, argv);
}