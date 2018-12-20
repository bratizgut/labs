#include <stdio.h>
#include <string.h>

int shift[255];

void table(unsigned char str[255]){
    int t = strlen(str), i;
    for (i = 0; i<=255; i++)
        shift[i] = t;
    for(i = 0; i < t-1; i++)
        shift[str[i]] = t-i-1;
}

int check(unsigned char pattern[], unsigned char str[], long ind){
    int i = strlen(pattern) - 1;
    printf("%ld ", ind + (long)i);
    while((pattern[i] == str[i]) && (i > 0)){
        i--;
        printf("%ld ", ind + (long)i);
    }
    return(shift[str[strlen(pattern) - 1]]);
}

int main(void){
    unsigned char str[17] = {0},pattern[17] = {0};
    gets(pattern);
    table(pattern);
    int len = strlen(pattern);
    int j = len, i = 0, k = 0;
    long ind = 1;
    while (1){
        for (i = len - j; i < len; i++){
            if ((k = getchar()) == EOF)
                return 0;
            str[i] = k;
        }
        j = check(pattern, str, ind);
        ind += j;
        if (j < len)
            memmove(str, str+j, len-j);
    }
}
