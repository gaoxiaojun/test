#include <unistd.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
int main()
{
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int fd = creat("hole.txt", RWRWRW);
    write(fd, "hello", 5);
    lseek(fd, 80000, SEEK_END);
    write(fd, "world", 5);
    return 0;
}
