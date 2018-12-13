#include <stdio.h>
#include <stdlib.h>

void swap(int k, int s, int *arr){
	int b = arr[k];
	arr[k] = arr[s];
	arr[s] = b;
}

void down(int *arr, int N){
	int i = 0, j = 0;
	while ((2 * i + 1) < N){
        if(arr[2 * i + 1 ]>arr[j])
            j=2 * i + 1;
        if ((2 * i + 2 < N)&&(arr[ 2 * i + 2]>arr[j]))
            j= 2 * i + 2;
        if (i == j)
            break;
        swap(i, j, arr);
        i = j;
   }
}

void up(int *arr, int i){
	while((i > 0) && ((arr[(i - 1)/2]) < arr[i])){
        swap(i, (i - 1)/2, arr);
        i = (i - 1)/2;
    }
}

int main(void){
	long N;
	int	*arr, i;
	scanf("%ld", &N);
	if (N <= 0)
		return 0;
	arr = (int*)malloc(N * sizeof(int));
	for (i = 0; i < N; i++){
		scanf("%d", &arr[i]);
		up(arr, i);
	}
	int l = N;
	for (i = 0; i < N; i++){
		swap(0, l-1, arr);
		down(arr, l - 1);
		l--;
	}
	for (int i = 0; i < N; i++)
		printf("%d ", arr[i]);
	free(arr);
	return 0;
}
