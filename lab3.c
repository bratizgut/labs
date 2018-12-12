#include <stdio.h>
#include <string.h>

void check(unsigned char word[], unsigned char str[],int ind) {
	long len = strlen(word);
    for(int i=0; i < len; i++){
        printf("%d ",ind + i - len + 1);
        if(word[i] != str[i])
            break;
    }
}

long hash(unsigned char word[]){
    int N = strlen(word);
    long sum = 0, s = 1;
    for(int i = 0; i < N; i++){
        sum += (word[i]%3)*s;
		s *= 3;
    }
    return sum;
}

int main(void) {
    unsigned char str[17] = {0},pattern[17] = {0};
    gets(pattern);
    long hash_pat = hash(pattern), s = 1;
    int count = 0,ind = 1;
    long hash_cur = 0, len = strlen(pattern);
    int k = 0,fsym = 0;
    printf("%ld ",hash_pat);
	for (int i = 0; i < len-1; i++)
		s *= 3;
    while ((k = getchar()) != EOF){
        str[count++] = k;

        if(ind == len){
            hash_cur=hash(str);
        }
        else if (ind > len){
            hash_cur = ((hash_cur - fsym%3)/3) + (k%3)*s;
        }

        if (count == len){
            if(hash_cur == hash_pat){
                check(pattern, str, ind);
            }
            fsym = str[0];
            memmove(str, str+1, len-1);
            count--;
        }
        ind++;
    }
    return 0;
}
