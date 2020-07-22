#include "helper.h"

extern void loader_init(int argc, char **argv);

extern void loader_run_test(int argc, char **argv);

extern void loader_clean_up(int argc, char **argv);

extern argp ap_loader;
extern argp ap_test;

static int merge_and_parse_arg(int argc, char **argv) {
    argp_child apc[] = {{&ap_loader, 0, "Loader Arguments"},
                        {&ap_test,   0, "Test Arguments"},
                        {nullptr}};

    argp ap = {nullptr, nullptr, nullptr, nullptr, apc};

    return argp_parse(&ap, argc, argv, 0, nullptr, nullptr);
}

int main(int argc, char **argv) {
    merge_and_parse_arg(argc, argv);

    loader_init(argc, argv);
    loader_run_test(argc, argv);
    loader_clean_up(argc, argv);
}
