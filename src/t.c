#include <stdio.h>

char arr[] = {'a', 'b', 'c', 'd', 'e', 'f'};
char result[6];
void printR(char *arr, int n, char *result)
{
    if (n <= 0)
    {
        //putchar('\n');
        for (int i = 0; i < 6; ++i)
            putchar(result[i]);
        putchar('\n');
        return;
    }
    int i, j;
    for (i = 0; i < n; ++i)
    {
        //putchar(arr[i]);
        for (
            for (j = 0; j < n; ++j)
        {
            if (j != i)
                    ajust[j] = arr[j];
            }
        printR(ajust, n - 1);
    }
}

int main()
{
    printR(arr, sizeof(arr), result);
    return 0;
}
