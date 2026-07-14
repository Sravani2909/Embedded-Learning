#include <stdio.h>

#define SET_BIT(REG,POS)      ((REG) | (1<<(POS)))
#define CLEAR_BIT(REG,POS)    ((REG) & ~(1<<(POS)))
#define TOGGLE_BIT(REG,POS)   ((REG) ^ (1<<(POS)))
#define READ_LSB(REG)         ((REG) & 1)
#define CHECK_BIT(REG,POS)    (((REG)>>(POS)) & 1)

int main()
{
    int num = 20;
    int pos = 3;

    printf("SET     = %d\n", SET_BIT(num,pos));
    printf("CLEAR   = %d\n", CLEAR_BIT(num,pos));
    printf("TOGGLE  = %d\n", TOGGLE_BIT(num,pos));
    printf("LSB     = %d\n", READ_LSB(num));
    printf("BIT %d  = %d\n", pos, CHECK_BIT(num,pos));

    printf("Original num = %d\n", num);

    return 0;
}
