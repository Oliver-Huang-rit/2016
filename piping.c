#include<unistd.h>
#include<stdio.h>

int main(){
    int fd[2];

    pipe(fd);

    char msg1[] = "Hello World1";
    char msg2[] = "Hello World2";

    write(fd[1], msg1, 20);
    write(fd[1], msg2, 20);

    char buffer[20];
    read(fd[0], buffer, 40);
    printf("recived: %s\n", buffer);
    read(fd[0], buffer, 20);
    printf("recived: %s\n", buffer);
    return 0;
}