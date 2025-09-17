#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	FILE *input=stdin;
	int buffer_size = 4096;
	char *filename = NULL;
	char opt;
while ((opt = getopt(argc, argv,"f:b:")) != -1){

switch (opt){
case 'f': filename =optarg; break;
case 'b': buffer_size = atoi(optarg); break;
 default: fprintf(stderr, "Usage: %s [-f file] [-b size]\n", argv[0]);
  exit(1);
  }}

  if (filename != NULL){
  input = fopen(filename, "r");
  if (input == NULL){ perror("invalid");
  exit(1);
  }}

char *buffer = malloc(buffer_size);
size_t bytes_read;
while((bytes_read = fread(buffer, 1,buffer_size,input)) >0){
if (fwrite(buffer, 1, bytes_read,stdout)!= bytes_read){ perror("faild"); break;}
fflush(stdout);}
free(buffer); if (input!= stdin){
fclose(input);}
	return 0;
	}
