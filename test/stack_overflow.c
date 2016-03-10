#include <stdio.h>
#include <stdlib.h>

// Have we invoked this function?
void why_here(void)
{
    printf("why u r here?!\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int buf[1];
    printf("size int:%ld\n", sizeof(int));
    printf("size ptr:%ld\n", sizeof(char *));
    buf[4] = (int)why_here;
    return 0;
}
