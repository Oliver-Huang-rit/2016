#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char *get_command(){
    printf(">> ");
    char *cmd = (char *)malloc(sizeof(char)*40);
    for (int i = 0; i < strlen(cmd); i++){
        *(cmd + i) = '\0';
    }
    for (int i = 0; i < 40; i++){
        *(cmd + i) = '\0';
    }
    char temp[40];
    for (int i = 0; i < strlen(temp); i++){
        temp[i] = '\0';
    }
    for (int i = 0; i < 40; i++){
        temp[i] = '\0';
    }
    fgets(temp, sizeof(char)*40, stdin);
    for (int i = 0; i < strlen(temp); i++){
        *(cmd + i) = temp[i];
    }
    for (int i = 0; i < strlen(cmd); i++){
        if (i >= 40){
            *(cmd + i) = '\0';
        }
        if (isalpha(*(cmd + i)) == 0 && *(cmd + i) != '|' && *(cmd + i) != ' '){
            *(cmd + i) = '\0';
        }
    }
    printf("\ncmd: %s\n", cmd);
    return cmd;
}

char ** pars_space(char *cmd){
    printf("input check: %s\n", cmd);
    int checker = 0;
    char *token;
    char **msg = (char**)malloc(sizeof(char**));
    token = strtok(cmd, " ");
    while (token != NULL){
        (msg[checker]) = (char *)malloc(sizeof(char*));
        strcpy((msg[checker]), token);
        token = strtok(NULL, " ");
        checker += 1;
    }
    return msg;
}

char ** parse_pipe(char *cmd){
    printf("input check: %s\n", cmd);
    int checker = 0;
    char *token;
    char **msg = (char**)malloc(sizeof(char**));
    token = strtok(cmd, "|");
    while (token != NULL){
        (msg[checker]) = (char *)malloc(sizeof(char*));
        strcpy((msg[checker]), token);
        token = strtok(NULL, "|");
        checker += 1;
    }
    return msg;
}

void handle_pipe_commands(char **args1, char **args2){
    int pipefd[2]; 
    pid_t p1, p2;

    if (pipe(pipefd) < 0){
        printf("Pipe could not be initialized\n");
        return;
    }

    p1 = fork();
    if (p1 < 0){
        printf("Forking failed\n");
        return;
    }if (p1 == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(args1[0], args1) < 0){
            printf("Could not execute command 1\n");
            exit(0);
        }
    }else{
        p2 = fork();
  
        if (p2 < 0){
            printf("Forking Failed\n");
            return;
        }
        if (p2 == 0){
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(args2[0], args2) < 0){
                printf("Could not execute command 2\n");
                exit(0);
            }
        }else{
            wait(NULL);
            wait(NULL);
        }
    }
}

void handle_special_commands(char **args){
    if (strcmp(*(args), "cd") == 0){
        printf("cd is requested\n");
    }else if (*(args), "pwd"){
        char path[40];
        if (getcwd(path, sizeof(path)) != NULL){
            printf("%s\n",path);
        }else{
            printf("Error when getting the path");
        }
    }
}

void handle_non_pipe_commands(char **args){
    pid_t pid = fork();

    if (pid == -1){
        printf("Failed forking child\n");
        return;
    }else if (pid == 0){
        if (execvp(args[0], args) < 0) {
            printf("Could not execute command\n");
        }
        exit(0);
    }else{
        wait(NULL);
    }
}

int main(){
    char *cmd = get_command();
    while (*(cmd) != 'e' && *(cmd + 1) != 'x' && *(cmd + 2) != 'i' && *(cmd + 4) != 't'){
        int sent = 0;
        for (int i = 0; i < strlen(cmd); i++){
            if (*(cmd + i) == '|'){
                sent = 1;
            }
        }
        if (sent == 0 && ((*(cmd) == 'p' && *(cmd + 1) == 'w' && *(cmd + 2) == 'd') || (*(cmd) == 'c' && *(cmd + 1) == 'd'))){
            printf("special\n");
            char **temp = pars_space(cmd);
            handle_special_commands(temp);
        }else if(sent == 0){
            printf("going to parse by white space\n");
            printf("check before inpit: %s\n", cmd);
            char **temp = pars_space(cmd);
            free(*(temp));
            free(temp);
        }else{
            printf("going to parse by |\n");
            printf("check before inpit: %s\n", cmd);
            char **temp = parse_pipe(cmd);
            char **fi = pars_space(*(temp));
            char **se = pars_space(*(temp + 1));
            free(*(temp));
            free(temp);
        }
        free(cmd);
        char *cmd = get_command();
    }
    free(cmd);
    return 0;
}