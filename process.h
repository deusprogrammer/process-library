#include <sys/types.h>

#ifndef PROCESS_H
#define	PROCESS_H

class Process {
private:
    bool isError;
    char error[1024];
    int in;
    int out;
    pid_t pid;
public:
    Process(int in, int out, pid_t pid);
    Process(const char* filename, char* argv[]);
    
    int writeToStdin(char* message);
    int writeToStdin(const char* message);
    int writeToStdin(char* message, int bufSz);
    int writeToStdin(const char* message, int bufSz);
    
    int readFromStdout(char* buffer, int bufSz);
    
    int closePipes();
    int waitFor();
    int waitFor(long timeout);
    int cleanKill();
    int forceKill();
};

#endif	/* PROCESS_H */