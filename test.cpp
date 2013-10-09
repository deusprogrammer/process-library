#include <stdlib.h>
#include <stdio.h>

#include "process.h"

int main() {
    puts("Starting.");
    
    char buf[1024];
    Process* proc = new Process("./subprocess", NULL);

    puts("Created new process.");

    proc->writeToStdin("Testing", 1024);
    proc->readFromStdout(buf, 1024);

    printf("Got reply from subprocess: %s.\n", buf);
    
    proc->cleanKill();
    if (proc->waitFor(10) <= 0) {
        puts("Process wait timed out!");
        proc->forceKill();
        return 0;
    }
    
    puts("Process killed successfully!");
    
    return 0;
}