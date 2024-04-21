// file: TwoPipesTwoChildren.cpp
// author: Giridhar Nair
// date: 04/09/2024
// purpose: CS3377
// description: Exectues the ls -ltr | grep 3376 | wc -l command using two pipes and two children

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    int status;
    int childpid1, childpid2;
    char *ls_args[] = {"ls", "-ltr", NULL}; // Arguments for 'ls' command
    char *grep_args[] = {"grep", "3376", NULL}; // Arguments for 'grep' command
    char *wc_args[] = {"wc", "-l", NULL}; // Arguments for 'wc' command
    
    // Pipes for communication between processes
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    
    // Create first child process
    if ((childpid1 = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }
    
    // Code for first child process
    if (childpid1 == 0) {
        dup2(pipe1[1], 1); // Redirect stdout to the write end of pipe1
        
        // Close unnecessary pipe descriptors
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        
        // Execute 'ls' command
        execvp(ls_args[0], ls_args); 
        perror("execvp ls failed");
        exit(1);
    }

    // Create second child process
    if ((childpid2 = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }
    
    // Code for second child process
    if (childpid2 == 0) {
        dup2(pipe1[0], 0); // Redirect stdin to the read end of pipe1
        dup2(pipe2[1], 1); // Redirect stdout to the write end of pipe2
        
        // Close unnecessary pipe descriptors
        close(pipe1[1]);
        close(pipe2[0]);
        
        // Execute 'grep' command
        execvp(grep_args[0], grep_args); 
        perror("execvp grep failed");
        exit(1);
    } 

    // Close unnecessary pipe descriptors in parent process
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]);
    
    dup2(pipe2[0], 0); // Redirect stdin to the read end of pipe2
    
    close(pipe2[0]);
    
    // Execute 'wc' command
    execvp(wc_args[0], wc_args);
    perror("execvp wc failed");

    return(0);
}
