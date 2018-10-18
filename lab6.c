#include <stdio.h>
#include <stdlib.h>

void sort(int *s, int left, int right) {
	int i = left, j = right, w = 0;
	int x = (left + right) / 2;
	do {
		while (s[i] < s[x]) i++;
		while (s[x] < s[j]) j--;
		if (i <= j) {
			w = s[i];
			s[i] = s[j];
			s[j] = w;
			i++;
			j--;
		}
	} while (i < j);
	if (left < j) sort(s, left, j);
	if (i < right) sort(s, i, right);
}

int main(void) {
	int N = 0, *s;
	scanf("%d", &N);
	s = (int*)malloc(N * sizeof(int));
	for (int i = 0; i<N; i++)
		scanf("%d", &s[i]);
	sort(s, 0, N - 1);
	for (int i = 0; i<N; i++)
		printf("%d ", s[i]);
	free(s);
	return 0;
}
