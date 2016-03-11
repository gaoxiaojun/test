#include <stdio.h>

long long my_pow(int m, unsigned int n) {
    long long ans = 1;
    long long x = m;

    while (n > 0) {
        if (n & 1)
            ans *= x;
        x *= x;
        n >>= 1;
    }

    return ans;
}

int main() {
    printf("pow(2, 5) = %lld\n", my_pow(2, 5));
    printf("pow(3, 10) = %lld\n", my_pow(3, 10));
    return 0;
}