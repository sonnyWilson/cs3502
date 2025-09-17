#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(){
int pipe1[2];
int pipe2[2];
pid_t pid;
if (pipe(pipe1) == -1){ perror("pipe1 failed");
exit(1);}
if (pipe(pipe2) == -1){ perror("pipe2 failed");
exit(1);}
pid = fork();
if (pid == -1){ perror("fork failed");
exit(1);}
if (pid == 0){
close(pipe1[0]);
close(pipe2[1]);
exit(0);} else {
close(pipe1[0]);
close(pipe2[1]);
char buffer[256];
ssize_t bytes;
for( int i = 0; i < 3; i++){
snprintf(buffer, sizeof(buffer), "parent message%d",i+1);
write(pipe1[1], buffer, strlen(buffer));
bytes = read(pipe2[0],buffer, sizeof(buffer)-1);
if (bytes > 0) { buffer[bytes] = '\0';
printf("parent recived: %s\n", buffer);}}
close(pipe1[1]);
close(pipe2[0]); wait(NULL); } return 0;}
