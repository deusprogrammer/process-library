#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigIntHandler(int sig, siginfo_t *si, void *unused) {
    perror("Received sigint, shutting down.");
    sleep(5);
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    exit(0);
}

int setupSigHandler(int sig, void (*function)(int, siginfo_t*, void*)) {
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = function;
    if (sigaction(sig, &sa, NULL) == -1) {
        return -1;
    }
    
    return 0;
}

int main() {
    char buf[1024];
    int nBytes = 0;
    
    if (setupSigHandler(SIGINT, sigIntHandler) == -1) {
        return 1;
    }
        
    while ((nBytes = read(STDIN_FILENO, buf, 1024)) != -1) {
        write(STDOUT_FILENO, "ECHO", 1024);
    }

    return 0;
}
