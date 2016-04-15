#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

char *line = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n";

int main()
{
    int fd = open("letter_e", O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
    for (int i=0; i<8000; ++i)
    {
        write(fd, line, 73);
    }
    return 0;
}
