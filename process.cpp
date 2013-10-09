#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"

Process::Process(int in, int out, int pid) {
    this->in = in;
    this->out = out;
    this->pid = pid;
    this->isError = false;
}

Process::Process(const char* filename, char* argv[]) {
    int fd1[2];     // Parent to child. [0] is for reading, [1] is for writing.
    int fd2[2];     // Child to parent.
    
    int child;      // Child pid.

    // Create pipes
    if (pipe(fd1) == -1) {
        return;
    }

    if (pipe(fd2) == -1) {
        return;
    }

    child = fork();

    if (child >= 0) {
        if (child == 0) {   // Child
            // Closing writing end of parent to child.
            close(fd1[1]);

            // Closing reading end of child to parent.
            close(fd2[0]);

            // Redirect stdout and stdin to pipe.
            dup2(fd2[1], STDOUT_FILENO);
            dup2(fd1[0], STDIN_FILENO);

            // Executing sub process
            execv(filename, argv);
        } else {            // Parent
            // Closing reading end of parent to child.
            close(fd1[0]);
            
            // Closing writing end of child to parent.
            close(fd2[1]);
            
            // Initialize child elements.
            this->out = fd2[0];
            this->in  = fd1[1];
            this->pid = child;
        }
    }
}

/**
 * This call will write to buffered stdin.
 * 
 * @param message
 * @param bufSz
 * @return 
 */
int Process::writeToStdin(char* message) {
    return write(in, message, strlen(message));
}

/**
 * This call will write to buffered stdin.
 * 
 * @param message
 * @param bufSz
 * @return 
 */
int Process::writeToStdin(const char* message) {
    return write(in, message, strlen(message));
}

/**
 * This call will write to buffered stdin.
 * 
 * @param message
 * @param bufSz
 * @return 
 */
int Process::writeToStdin(char* message, int bufSz) {
    return write(in, message, bufSz);
}

/**
 * This call will write to buffered stdin.
 * 
 * @param message
 * @param bufSz
 * @return 
 */
int Process::writeToStdin(const char* message, int bufSz) {
    return write(in, message, bufSz);
}

/**
 * This call will read from buffered stdout.
 * 
 * @param buffer
 * @param bufSz
 * @return 
 */
int Process::readFromStdout(char* buffer, int bufSz) {
    return read(out, buffer, bufSz);
}


/**
 * This call will close the stdin and stdout pipes.
 * 
 * @return 
 */
int Process::closePipes() {
    close(in);
    close(out);
    
    return 0;
}

/**
 * This call will block until this process dies.
 * 
 * @return 
 */
int Process::waitFor() {
    return waitpid(pid, NULL, 0);
}

/**
 * This call will block until this process dies or timeout is reached.
 * 
 * @return 
 */
int Process::waitFor(long timeout) {
    int ret = 0;
    int started = time(NULL);
    
    while ((ret = waitpid(pid, NULL, WNOHANG)) == 0 && (time(NULL) - started < timeout));
    
    return ret;
}

/**
 * This will result in a clean kill if the process handles
 * signals.
 * 
 * @return 
 */
int Process::cleanKill() {
    return kill(pid, SIGINT);
}

/**
 * This will always result in the death of this process.
 * 
 * @return 
 */
int Process::forceKill() {
    return kill(pid, SIGKILL);
}