// file: TwoPipesThreeChildren.cpp
// author: Giridhar Nair
// date: 04/09/2024
// purpose: CS3377
// description: Exectues the ls -ltr | grep 3376 | wc -l command using two pipes and three children

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int status;
    int childpid;
    char *ls_args[] = {"ls", "-ltr", NULL}; // Arguments for 'ls' command
    char *grep_args[] = {"grep", "3376", NULL}; // Arguments for 'grep' command

    // Create two pipes to send the output of "ls" to "grep"
    int pipe_ls_grep[2];
    pipe(pipe_ls_grep);

    // Fork the first child (to execute ls)
    if ((childpid = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }

    if (childpid == 0) {
        // Replace ls's stdout with write end of pipe_ls_grep
        dup2(pipe_ls_grep[1], 1);
        close(pipe_ls_grep[0]);
        close(pipe_ls_grep[1]);
        execvp(*ls_args, ls_args);
        exit(0);
    }

    // Create another pipe to send the output of grep
    int pipe_grep[2];
    pipe(pipe_grep);

    // Fork the second child (to execute grep)
    if ((childpid = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }

    if (childpid == 0) {
        // Replace grep's stdin with read end of pipe_ls_grep
        dup2(pipe_ls_grep[0], 0);
        
        // Replace grep's stdout with write end of pipe_grep
        dup2(pipe_grep[1], 1);
        close(pipe_ls_grep[0]);
        close(pipe_ls_grep[1]);
        close(pipe_grep[0]);
        close(pipe_grep[1]);
        execvp(*grep_args, grep_args);
        exit(0);
    }

    // Fork the third child (to execute the third command)
    if ((childpid = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }

    if (childpid == 0) {
        // Close unnecessary pipe ends
        close(pipe_ls_grep[0]);
        close(pipe_ls_grep[1]);
        close(pipe_grep[1]);

        // Replace stdin with read end of pipe_grep
        dup2(pipe_grep[0], 0);
        close(pipe_grep[0]);

        // Execute the third command (could be anything)
        execlp("wc", "wc", "-l", NULL);
        perror("Failed to execute wc");
        exit(1);
    }

    // Close all pipe ends in the parent process
    close(pipe_ls_grep[0]);
    close(pipe_ls_grep[1]);
    close(pipe_grep[0]);
    close(pipe_grep[1]);

    // Parent process does nothing but wait for children to finish
    for (int i = 0; i < 3; ++i) {
        wait(&status);
    }

    return 0;
}
