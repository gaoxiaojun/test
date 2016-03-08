#include <stdio.h>

int loop_fibonacci(int n)
{
    if (n == 1 || n == 2) return 1;
    else if (n > 2)
    {
        int f1 = 1, f2 = 1;
        for (int i = 0; i < n - 2; ++i)
        {
            int tmp = f2;
            f2 += f1;
            f1 = tmp;
        }
        return f2;
    }
    return 0;
}

int recursive_fibonacci(int n)
{
    if (n == 1 || n == 2)
        return 1;
    else
        return recursive_fibonacci(n - 1) + recursive_fibonacci(n - 2);
}

int main()
{
    printf("%d\n", loop_fibonacci(10));
    printf("%d\n", recursive_fibonacci(10));
}