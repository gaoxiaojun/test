#include "apue.h"
#include <fcntl.h>

int main()
{
    int fd = open("tmp.foo", O_WRONLY|O_CREAT);
    int fd2 = open("tmp.foo", O_WRONLY|O_CREAT);
    write(fd, "hello", 5);
    write(fd2, "HELLO", 5);
    write(fd, "hello", 5);
    exit(0);
}
