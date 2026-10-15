#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"

int main(){
    char filename[256];

    printf("Enter file name: ");

    if (scanf("%255s", filename) != 1){
        perror("read filename");
        return -1;
    }

    int file = open(filename, O_RDONLY);
    if (file == -1){
        perror("open");
        return -1;
    }

    int pipe_fd[2]; // 0-read, 1-write
    if (pipe(pipe_fd) == -1){
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        return -1;
    }
    else if(pid == 0){
        if(dup2(file, STDIN_FILENO) < 0){
            perror("dup2 in");
            return -1;
        }
        if(dup2(pipe_fd[1], STDOUT_FILENO)<0){
            perror("dup2 out");
            return -1;
        }
        close(file);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        execl("./Child", "Child", NULL);
        perror("execl");
        return -1;
    }
    else{
        close(file);
        close(pipe_fd[1]);
        
        char buf[4096];
        ssize_t n;
        while ((n=read(pipe_fd[0], buf, sizeof(buf)))>0){
            if (write(STDOUT_FILENO, buf, n)!=n){
                perror("write");
                break;
            }
        }
        if(n < 0){
            perror("read");
        }
        close(pipe_fd[0]);
        int status;
        if(waitpid(pid, &status, 0) == -1){
            perror("waitpid");
            return -1;
        }
        if (WIFEXITED(status)){
            int code = WEXITSTATUS(status);
            if (code != 0){
                printf("Child process end with %d\n", code);
                return code;
            }
        }else{
            printf("Child process fall\n");
            return -1;
        }
    }
    return 0;
}