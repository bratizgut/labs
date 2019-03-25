#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct graph{
    int n1;
    int n2;
    unsigned long weight;
    int OUT;
}old;

void swap(old *a, old *b){
	old temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void sort(int left, int right, old *s){
	int lp, rp, pivot;
	pivot = s[(left + right)/2].weight;
	lp = left;
	rp = right;
	while (lp <= rp){
		while (s[lp].weight < pivot)
			lp++;
		while (s[rp].weight > pivot)
			rp--;
		if (lp <= rp){
			swap(&s[lp], &s[rp]);
			lp++;
			rp--;
		}
	}
	if (rp > left)
		sort(left, rp, s);
	if (lp < right)
		sort(lp, right, s);
}

int main(){

    int N, M, *color, cur_color;
    FILE *in;
    old *GRAPH;

    in = fopen("in.txt", "r");

    fscanf(in, "%d", &N);
    if (fscanf(in, "%d", &M) == EOF) {
        printf("bad number of lines");
        exit(0);
    }

    if((N < 0) || (N > 5000)){
        printf("bad number of vertices");
        return 0;
    }

    if((M < 0) || (M > N*(N+1)/2)){
        printf("bad number of edges");
        return 0;
    }
	
	if((N == 0) && (M == 0)){
		printf("no spanning tree");
		return 0;
	}

    GRAPH = (old*)malloc(M * sizeof(old));
    color = (int*)malloc(N * sizeof(int));

    for(int i = 0; i < M; i++){
        if (fscanf(in, "%d %d %ld", &GRAPH[i].n1, &GRAPH[i].n2, &GRAPH[i].weight) != EOF){
        } else {
            printf("bad number of lines");
            exit(0);
        }
        if((GRAPH[i].n1 <= 0) || (GRAPH[i].n2 <= 0) || (GRAPH[i].n1 > N) || (GRAPH[i].n2 > N)){
            printf("bad vertex");
            exit(0);
        }
		if((GRAPH[i].weight < 0) || (GRAPH[i].weight > INT_MAX)){
			printf("bad length");
			return 0;
		}
		GRAPH[i].n1--;
		GRAPH[i].n2--;
	    	GRAPH[i].OUT = 0;
    }
    if(M > 0)	
    	sort(0, M-1, GRAPH);
    for(int i = 0; i < N; i++)
        color[i] = i;

    for(int i = 0; i < M; i++){
        if(color[GRAPH[i].n1] != color[GRAPH[i].n2]){
			cur_color = color[GRAPH[i].n2];		
            for(int j = 0; j < N; j++){
                if(color[j] == cur_color)
                    color[j] = color[GRAPH[i].n1];
			}
            GRAPH[i].OUT = 1;
        }
    }

    for(int i = 1; i < N; i++)
        if (color[i-1] != color[i]){
            printf("no spanning tree");
            return 0;
        }

    for(int i = 0; i < M; i++)
        if(GRAPH[i].OUT)
            printf("%d %d\n", GRAPH[i].n1 + 1, GRAPH[i].n2 + 1);

	free(color);
	free(GRAPH);

    return 0;
}
