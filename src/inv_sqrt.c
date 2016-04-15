#include <stdio.h>

float inv_sqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

int main()
{
    printf("%f\n", 1 / inv_sqrt(3));
    printf("%f\n", 1 / inv_sqrt(5));
    printf("%f\n", 1 / inv_sqrt(7));
    printf("%f\n", 1 / inv_sqrt(11));
    return 0;
}
