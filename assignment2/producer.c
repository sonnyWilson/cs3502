#include <studio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *arg[]) {
	FILE *input=stdin;
	int buffer_size = 4096;
	char opt;
while ((opt = getopt(argc,argv,"f:b:")) != -1){

switch (opt){
case 'f': filename=optarg; brake;
case 'b': buffer_size = atoi(optarg); brake;
 default: fprinf(stderr, "Usage: %s [-f file] [-b size]\n", argv[0]);
  exit(1);
  }}

  if (filename != NULL){
  input = fopen(filename, "r");
  if (input == NULL){ perror("invalid");
  exit(1);
  }}

char *bufffer = malloc(buffer_size);
size_t bytes_read;
while((bytes_read = fread(buffer, 1,buffer_size,input))>0{
if (fwrite(buffer, 1, bytes_read,stdout)!= bytes_read){ perror(faild) break;}
fflush(stdout);}
free(buffer); if (input!= stdin){
fclose(input);
	return 0;
	}
