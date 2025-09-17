#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig) {
    shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
    stats_flag = 1;
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    int buffer_size = 4096;
    char *filename = NULL;
    int opt;
    size_t total_bytes = 0;
    clock_t start_time = clock();
    
    struct sigaction sa = {0};
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);
    while ((opt = getopt(argc, argv, "f:b:h")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 'b':
                buffer_size = atoi(optarg);
                if (buffer_size <= 0) {
                    fprintf(stderr, "Error: buffer_size must be positive\n");
                    exit(1);
                }
                break;
            case 'h':
                fprintf(stderr, "Usage: %s [-f filename] [-b buffer_size][-h]\n", argv[0]);
                fprintf(stderr,"-f: Input file (default: stdin)\n");
                fprintf(stderr,"-b: Buffer size in bytes (default: 4096)\n");
                fprintf(stderr,"-h: Show this help message\n");
                exit(0);
            default:
                fprintf(stderr, "Usage: %s [-f filename] [-b buffer_size][-h]\n", argv[0]);
                exit(1);
        }}
    if (filename != NULL) {
        input = fopen(filename, "r");
        if (input == NULL) {
            perror("Failed to open input file");
            exit(1);
        }
    }
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("Failed to allocate buffer");
        if (input != stdin) fclose(input);
        exit(1); }
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, buffer_size, input)) > 0 &&
     !shutdown_flag) {
        if (fwrite(buffer, 1, bytes_read, stdout) != bytes_read) {
            perror("Failed to write to stdout");
            break; }
        total_bytes += bytes_read;
        if (stats_flag) {
            clock_t now = clock();
            double elapsed = ((double)(now - start_time)) / CLOCKS_PER_SEC;
            if (elapsed > 0) {
                fprintf(stderr, "Producer stats: %zu bytes, %.2f MB/s\n",
                        total_bytes, (total_bytes / 1024.0 / 1024.0) / elapsed);
            } else {
                fprintf(stderr, "Producer stats: %zu bytes, N/A MB/s(elapsed time too small)\n",
                        total_bytes);}
            stats_flag = 0;}}
    if (ferror(input)) {
        perror("Failed to read input");}
    free(buffer);
    if (input != stdin) {
        fclose(input);}
    return 0;}
