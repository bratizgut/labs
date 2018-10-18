#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

int shift[255];

void table(unsigned char pattern[MAXLINE]){
	int t = strlen(pattern), i, j=0;	
	for (i = 0; i<=255; i++)
		shift[i] = t;
	for(i = 0; i < t-1; i++)
		shift[pattern[i]] = t-i-1;
}

void pos(unsigned char pattern[MAXLINE],int i){
	unsigned char str[100] = {0};
	gets(str);
	int j, k, t = strlen(pattern), len = strlen(str);
	if ((len == 0) || (t == 0))
		exit(0);
	if (len < t)
		printf("%d ", t);
	for (k = t; k <= len; ){
		printf("%d ", k+i);
		if (pattern[t-1] == str[k-1]){
			for (j = t-2; (pattern[j] == str[k-t+j]) && (j>=0); j--){
				printf("%d ", k-t+j+1+i);
			}
			if (j < 0){
				k += shift[pattern[t-1]];	
			} else {
				printf("%d ", k-t+j+1+i);
				k += shift[pattern[t-1]];
			}
		} else {
				k+=shift[str[k-1]];
			}
	}
	i+=t;
	pos(pattern, i);
}

int main(void){
	unsigned char pattern[100];
	gets(pattern);
	table(pattern);
	pos(pattern, 0);
	return 0;	
}
