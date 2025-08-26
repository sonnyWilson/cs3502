#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main() {
FILE *logfile;
char message[100];
time_t current_time;
logfile = fopen("owltech.log", "a");
if (logfile == NULL){
printf("Error: Could not open log file\n");
return 1;
}
printf("Enter log message: ");
fgets(message, sizeof(message), stdin);
time(&current_time);
fprintf(logfile, "[%s] %s", ctime(&current_time), message);
fclose(logfile);
printf("Log entery saved successfully!\n");
printf("\n--- Current Log Contents ---\n");
system("cat owltech.log");
return 0;
}
