#include <stdio.h>

int func(int n)
{
    int sum = 0, i;

    for (i=1; i<=n; ++i)
    {
        sum += i;
    }

    return sum;
}

int main(int argc, const char *argv[])
{
    //printf("Happy to use gdb!\n");
    for (int i = 0; i < argc; ++i)
        printf("%s ", argv[i]);
    printf("\n");

    int i;
    int result = 0;

    for (i=1; i<=100; ++i)
    {
        result += i;
    }

    printf("result[1-100] = %d\n", result);
    printf("result[1-250] = %d\n", func(250));

    return 0;
}
