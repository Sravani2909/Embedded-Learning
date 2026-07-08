#include <stdio.h>

int main() {
    unsigned int rev = 0;
    unsigned int num = 13;

    for (int i = 0; i < 8; i++) {
        rev <<= 1;
        rev |= (num & 1);
        num >>= 1;
    }

    printf("%u\n", rev);   // 176
    return 0;
}
