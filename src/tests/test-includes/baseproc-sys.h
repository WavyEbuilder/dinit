#include <sys/types.h>
#include <iostream>

// Mock system functions for testing.

namespace bp_sys {

int pipe2(int pipefd[2], int flags);
int close(int fd);

inline int fcntl(int fd, int cmd, ...)
{
    // This is used for setting the CLOEXEC flag, we can just return 0:
    return 0;
}

inline int kill(pid_t pid, int sig)
{
    // No proper mock implemented yet:
    std::cout << "(kill; aborting)" << std::endl;
    abort();
    return 0;
}

}