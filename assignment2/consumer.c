#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig) {
    shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
    stats_flag = 1;
}

int main(int argc, char *argv[]) {
    int max_lines = -1;  // -1 means unlimited
    int verbose = 0;
    char opt;
    char line[4096];
    int line_count = 0;
    size_t char_count = 0;
    while ((opt = getopt(argc, argv, "n:vh")) != -1) {
        switch (opt) {
            case 'n':
                max_lines = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [-n max_lines] [-v]\n", argv[0]);
                exit(1);
        }
    }
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);

    clock_t start_time = clock();
    while ((max_lines == -1 || line_count < max_lines) && !shutdown_flag) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line_count++;
        char_count += strlen(line);
        if (verbose) {
            printf("%s", line);
            fflush(stdout);
        }
        if (stats_flag) {
            clock_t now = clock();
            double elapsed = ((double)(now - start_time)) / CLOCKS_PER_SEC;
            if (elapsed > 0) {
    fprintf(stderr, "Consumer stats: %d lines, %.2f lines/sec\n", line_count,
                        (double)line_count / elapsed);
            }
            stats_flag = 0;}}
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    if (elapsed_time > 0) {
        double throughput_lines_s = (double)line_count / elapsed_time;
        fprintf(stderr, "Lines: %d\n", line_count);
        fprintf(stderr, "Characters: %zu\n", char_count);
fprintf(stderr, "Time: %.2f sec, Throughput: %.2f lines/sec\n", elapsed_time, throughput_lines_s);}
    return 0;
}
