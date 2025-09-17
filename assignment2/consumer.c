#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int max_lines = -1;
    int verbose = 0;   
    int opt;            
    char line[4096];    

    
while ((opt = getopt(argc, argv, "n:vh")) != -1) {
switch (opt) {
case 'n':max_lines = atoi(optarg);
if (max_lines <= 0) {
fprintf(stderr, "Error: max_lines must be positive\n");
exit(1);}break;
case 'v':verbose = 1;break;
 case 'h':
 fprintf(stderr, "Usage: %s [-n max_lines] [-v] [-h]\n", argv[0]);
 fprintf(stderr, "  -n: Maximum number of lines to process\n");
 fprintf(stderr, "  -v: Enable verbose mode\n");
 fprintf(stderr, "  -h: Show this help message\n");
 exit(0);
 default:
 fprintf(stderr, "Usage: %s [-n max_lines] [-v] [-h]\n", argv[0]);exit(1);}}
    clock_t start = clock();
    int line_count = 0;
    size_t char_count = 0;
    while (max_lines == -1 || line_count < max_lines) {
    if (fgets(line, sizeof(line), stdin) == NULL) {
 break;}
 line_count++;
 char_count += strlen(line);
 if (verbose) {fprintf(stderr, "Line %d: %s", line_count, line);}
    }
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    fprintf(stderr, "Total Lines: %d\n", line_count);
    fprintf(stderr, "Total Characters: %zu\n", char_count);
    if (elapsed > 0) {
        fprintf(stderr, "Throughput: %.2f lines/sec\n", (double)line_count / elapsed);
    } else {
        fprintf(stderr, "Throughput: N/A (elapsed time too small)\n");
    }
    return 0;
}
