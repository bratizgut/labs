#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void check(int b1, int b2, char s[]){
	int t=0;
	if ((b1 < 2) || (b1 > 16) || (b2 < 2) || (b2 > 16)){
		printf("bad input");
		exit(0);
	}
	for (int i = 0; i < strlen(s); i++){
		if (s[i] == '.'){
			t = t + 1;
			if (t > 1){
				printf("bad input");
				exit(0);
			}
			continue;
		}
		if (((s[i] - '0' >= b1) && (b1 < 10)) || ((s[i] - 'A' >= b1) && (b1 >= 10)) || (s[i] - '0' < 0)){
			printf("bad input");
			exit(0);
		}
	}
}

void out(int num){
	if (num >= 10){
		printf("%c", num-10+'A');
	} else {
		printf("%d", num);
	}
}

int TenTob2(int b2, unsigned long long num){
	if (num < b2){
		out(num);
		return 0;
	}
	TenTob2(b2, num / b2);
	out(num % b2);
	return 0;
}

void b1Tob2(int b1, int b2, char s[14]){
	unsigned long long num = 0, count = 1;
	for (int i= strlen(s) - 1; i >= 0; i--){
		if (s[i] >= 'A'){
			num = num + (s[i] + 10 - 'A') * count;
			count *= b1;
		} else {
			num = num + (s[i] - '0') * count;
			count *= b1;
		}
	}
	TenTob2(b2, num);
}

void b1Tob2Dot(int b1,int b2,char s[14]){
	double num = 0, count;
	int d, i = 0;
	count = (float)b1;
	if ((s[0] == '0') && strlen(s) == 1){
		printf("0");
		exit(0);
	}
	for (i = 0; i <= strlen(s) - 1; i++){
		if (s[i] >= 'A'){
			num = num + (s[i] + 10 - 'A')/count;
			count *= (float)b1;
		} else {
			num = num + (s[i] - '0')/count;
			count *= (float)b1;
		}
	}
	i=0;
	while ((num - trunc(num) != 0.0) && (i != 12)){
		num *= b2;
		d = trunc(num);
		out(d);
		num = num - (double) d;
		++i;
	}
}

int main(void){
	char s[14] = {'\0'}, s1[14] = {'\0'}, s2[14] = {'\0'};
	int b1, b2, t = 0, i = 0;
	
	
	scanf("%d %d ", &b1, &b2);
	gets(s);
	for (int j=0; s[j] != '\0'; j++)
		if (s[j] >='a')
			s[j]=s[j]- 'a' + 'A';
	check(b1, b2, s);
	
	
	while (s[i] != '\0'){
		if (s[i] == '.'){
			t = 1;
			break;
		}
		++i;
	}
	
	if (t){
			for (t=0; t < i; t++){
				s1[t] = s[t];
			}
			for (t = i + 1; s[t] != '\0'; t++){
				s2[t-i-1] = s[t];
			}
			if (strlen(s1) * strlen(s2) == 0){
				printf("bad input");
				exit(0);
			}
			b1Tob2(b1, b2, s1);
			printf(".");
			b1Tob2Dot(b1, b2, s2);
	} else {
		b1Tob2(b1, b2, s);
	}
	return 0;
}
