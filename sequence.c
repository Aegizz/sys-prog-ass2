#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute_command(char command[]) {
    //create new process
    pid_t pid = fork();
    if (pid < 0) {
        //check for fail and exit
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        //tokenize input and provide it to exec()
        char *token = strtok(command, " \n");
        char *params[100];
        int i = 0;
        while (token != NULL) {
            params[i++] = token;
            token = strtok(NULL, " \n");
        }
        params[i] = NULL;

        if (execvp(params[0], params) == -1){
            perror("execvp failed");
            exit(1);
        }
    } else {
        int status;
        //wait for process to end before return
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            exit(1);
        }
    }
    return 0;
}

int main(void){
    //grab file from stdin
    FILE *file = stdin;
    //init commands as null for command input
    char commands[1024];
    while (fgets(commands, 1024, file) != NULL){
        //read each command line by line and then execute the command from that line
        commands[strcspn(commands, "\n")] = 0;
        execute_command(commands);
    }
    return 0;
}
