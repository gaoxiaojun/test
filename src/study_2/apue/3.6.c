#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("tmp.foo", O_RDWR);//|O_APPEND);
    write(fd, "tmp.foooooooooooooo", 18);
    lseek(fd, 0, SEEK_SET);
    write(fd, "hello\n", 6);
    return 0;
}
