//
// Created by qiyilang.wqyl on 2020/7/8.
//

#ifndef IO_URING_PERF_HELPER_H
#define IO_URING_PERF_HELPER_H

#include <cstdint>

// debug macro, assign cmd return val to var and exit when cond is true
#define ERR_TEST(cmd, var, cond) do{var=(cmd);if(cond){ALERT("err "#cmd"with %lu",(uint64_t)var)exit(1);}}while(0);\

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

#endif //IO_URING_PERF_HELPER_H
