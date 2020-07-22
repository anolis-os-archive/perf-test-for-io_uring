//
// Created by qiyilang.wqyl on 2020/7/8.
//

#ifndef IO_URING_PERF_HELPER_H
#define IO_URING_PERF_HELPER_H

#include <cstdint>
#include <cstdlib>
#include <argp.h>

// debug macro, assign cmd return val to var and exit when cond is true
#define ERR_TEST(cmd, var, cond) do{var=(cmd);if(cond){ALERT("err "#cmd" with %ld",(int64_t)(var))exit(1);}}while(0);\

// print macro
#define DEBUG(str, ...) printf("[DEBUG] " str "\n",__VA_ARGS__);
#define INFO(str, ...)  printf("[INFO]  " str "\n",__VA_ARGS__);
#define ALERT(str, ...) printf("[ALERT] " str "\n",__VA_ARGS__);


inline uint64_t round_up_power_of2(uint64_t n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    n++;
    return n;
}

static char suffix_map[] = {'\0', 'K', 'M', 'G', 'T'};

static unsigned int scan_num(const char *str, unsigned int *p) {
    unsigned int res = 0;

    while (str[*p] >= '0' && str[*p] <= '9') {
        res *= 10;
        res += str[(*p)++] - '0';
    }
    return res;
}

static unsigned int get_ratio(char suffix) {
    unsigned int res = 1;

    for (char i : suffix_map) {
        if (suffix == i || suffix == i + 'a' - 'A') return res;
        else res *= 1024;
    }
    return -1;
}

static unsigned int human2size(const char *str) {
    unsigned int res = 0;
    unsigned int ratio;
    unsigned int t_res;
    unsigned int p = 0;

    while (str[p] != '\0') {
        t_res = 0;
        t_res += scan_num(str, &p);
        ERR_TEST(get_ratio(str[p]), ratio, ratio == -1)
        t_res *= ratio;
        res += t_res;
        if (str[p] == '\0') break;
        else p++;
    }
    return res;
}


#endif //IO_URING_PERF_HELPER_H
