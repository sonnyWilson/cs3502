#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

	int main(int argc, char *argv[]) {
	int max_lines = -1;
	int verbose =0;
    char opt;
while ((opt= getopt(argc,argv "n:v")) != -1) {
switch(opt){
case 'n': max_lines = atoi(optarg); break;
case 'v': verbose = 1; break;
default: fprintf(stderr, "Usage: %s [-n max_lines] [-v]\n", argv[0]); exit(1);}}
clock_t start_time = clock();
int line_count = 0;
size_t char_count = 0;
while(max_lines == -1 || line_count < max_lines){
if (fgets(line, sizeof(line), stdin) == NULL){
 break;} line_count++; char_count += strlen(line); fflush(stdout);}}
 clock_t end_time = clock(0);
 double time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
 fprintf(stderr, "Total Lines 5d\n, Line_count");
 fprintf(stderr, "Total Characters %zu\n, char_count);
 if (time > 0){ fprintf(stderr, "Throughput: %.2f line/sec\n", (double)line_count / time);}
 return 0;}
