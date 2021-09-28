#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

char * const* argvec[] = {"./cmd.exe", "Hello", "Bye", NULL};

void spawn_process(){
    int pid = fork();
    if(pid < 0){
        printf("forkfailed.\n");
        eixt(1);
    }else if (pid == 0){
        execvp(argvec[0], argvec);
        printf("Should not see this\n");
    }
}

int main(int argc, char *args[]){
    printf("Original Process pid = %d\n", getpid());

    spawn_process();
    //execlp(argvec[0], argvec[1], argvec[2], NULL);

    printf("you should not be here");
/*
    if (execvp(argvec[0], argvec) < 0){
        printf("Exec Fails.\n");
        exit(1);
    }
    printf("main ends.\n");*/
    return 0;

}