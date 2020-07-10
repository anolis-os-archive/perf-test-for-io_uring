extern void loader_init(int argc, char **argv);

extern void loader_run_test(int argc, char **argv);

extern void loader_clean_up(int argc, char **argv);

int main(int argc, char **argv) {
    loader_init(argc, argv);
    loader_run_test(argc, argv);
    loader_clean_up(argc, argv);
}
