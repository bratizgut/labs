#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

struct stack{
    int elem[1001];
    int top;
};

void Push(struct stack *stk, int S){
    if(stk->top < 1001){
        stk->elem[stk->top] = S;
        stk->top++;
    } else {
        printf("Stcak full");
    }
}

void stkPrint(struct stack *stk) {
    int i;
    i=stk->top;
    if(stk->top == 0) return;
    do {
        i--;
        printf("%d ", stk->elem[i]);
    }while(i>0);
}

void Sort(int ind, int** arr, int* checked, int N, struct stack *out){
	
    if(checked[ind] == 1){
        printf("impossible to sort %d", ind);
        exit(0);
    } else if(checked[ind] == 2)
        return;
	
    checked[ind] = 1;
	
    for(int i = 1; i < arr[ind][0] + 1; i++){
        if(arr[ind][i] != 0)
            Sort(arr[ind][i]-1, arr, checked, N, out);
    }
	
    Push(out, ind+1);
    checked[ind] = 2;

}

int main(void){

    int N = 0, M = 0, a = 0, b = 0, S = 0;
	FILE *in;
	
	in = fopen("in.txt", "r");
	
    struct stack *out;
    out = (struct stack*)malloc(sizeof(struct stack));
    out->top = 0;
    fscanf(in, "%d", &N);
    if (fscanf(in, "%d", &M) == EOF) {
        printf("bad number of lines");
        exit(0);
    }
	 
    if((N < 0) || (N > 1000)){
        printf("bad number of vertices");
        return 0;
    }
	
    if((M < 0) || (M > N*(N+1)/2)){
        printf("bad number of edges");
        return 0;
    }
	
	S = (N < M) ? N : M;
	
	int **arr;
    arr = (int **)malloc(N * sizeof(int*));
    for(int i = 0; i < N; i++){
        arr[i] = (int *)malloc((S+1) * sizeof(int));
	}
	
	int *checked;
    checked = (int *)malloc(N * sizeof(int));
	
    for(int i = 0; i < N; i++)
        arr[i][0] = 0;
    for(int i = 0; i < N; i++){
        checked[i] = 0;
		arr[i][0] = 0;
    }
	
    for(int i = 0; i < M; i++){
        if (fscanf(in, "%d%d", &a, &b) != EOF){
		} else {
            printf("bad number of lines");
            exit(0);
        }
        if((a <= 0) || (b <= 0) || (a > N) || (b > N)){
            printf("bad vertex");
            exit(0);
        }
		
        arr[a-1][0]++;
        arr[a-1][arr[a-1][0]] = b;
    }
	
    for(int i = 0; i < N; i++)
        if(checked[i] == 0){
            Sort(i, arr, checked, N, out);
        }

    stkPrint(out);
	
	free(out);
	for(int i = 0; i < N; i++)
		free(arr[i]);
	free(arr);
	free(checked);
	fclose(in);

    return 0;
}
