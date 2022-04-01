#include <stdio.h>
#include <stdlib.h>
#include <string.h>



unsigned int calcTerm(u_char a, u_char b)
{
   unsigned int res = (a << 8) + b;
//   printf("%x\n", res);
   return res * res;
}




unsigned int getDaysAmount(char *str)
{
    char *tok = strtok(str, ".");
    
    unsigned int res = atoi(tok);
    
    tok = strtok(NULL, ".");
    res += atoi(tok) * 30;
    
    tok = strtok(NULL, ".");
    res += atoi(tok) * 365;
//	printf("%x\n", res);  
  return res;
}





int main(int args, char *argv[])
{
    if (args < 6)
    {
        printf("Ошибка: Недостаточное количество элементов\n");
        return 1;
    }

    unsigned long sum = 0;
    for (int i = 1; i < 4; ++i)
    {
        int j = 0;
        while (argv[i][j] != 0)
        {
            sum += calcTerm(argv[i][j], argv[i][j+1]);
            j += 2;
        }
    }
    sum *= getDaysAmount(argv[4]); 
    int len = 1;
    for (unsigned long i = 10; i < sum; i *= 10)
        ++len;
        
/*    if (len < 3)
    {
        printf("Error: unexpected value\n");
        return 2;
    }
  */ 
    for (int i = 0; i < (len - 3) / 2; ++i)
        sum = sum / 10;
    
    int val = 0;
    for (int i = 0, mul = 1; i < 3; ++i)
    {
        val = val + ((sum % 10) * mul);
        sum = sum / 10;
        mul *= 10;
    }
	printf("%d\n", val);    
    val = val % atoi(argv[5])+ 1;
    printf("\nVariant = %d\n\n", val);
    
    return 0;
}
