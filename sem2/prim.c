#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct graph{
	int n1;
	int n2;
	int weight;
}GRAPH;

void swap(GRAPH *a, GRAPH *b){
	GRAPH temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void sort(int left, int right, GRAPH *s){
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

GRAPH *findmin(GRAPH *minB, GRAPH *graph_in, int *count, int N, int M){
	int min = INT_MAX;
	minB = NULL;
	for(int i = 0; i < M; i++)
		if ((((count[graph_in[i].n1] == 1) && (count[graph_in[i].n2] == 0)) ||
		((count[graph_in[i].n2] == 1) && (count[graph_in[i].n1] == 0))))
			if ((graph_in[i].weight <= min) && (graph_in[i].weight != -1))	
				return &graph_in[i];
	return minB;	
}

int prim(GRAPH *graph_in, int N, int M, FILE *out){
	int nodes = 1;
	GRAPH *minB = NULL;
	GRAPH *ptr;
	int *count = (int*)malloc((N + 1) * sizeof(int));
	for (int i = 0; i < N+1; i++){
		count[i] = 0;
	}
	count[graph_in[0].n1] = 1;
	while ((ptr = findmin(minB, graph_in, &count[0], N, M))){
		fprintf(out,"%d %d\n",ptr->n1,ptr->n2);
		ptr->weight = - 1;
		count[ptr->n1] = 1;
		count[ptr->n2] = 1;
		nodes++;
	}
	free(count);
	return nodes;
}

int main(){
	
	int N, M, indicate = 0;
    FILE *in, *out;
    GRAPH *graph_in;

    in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

    fscanf(in, "%d", &N);
    if (fscanf(in, "%d", &M) == EOF) {
        printf("bad number of lines");
        return 0;
    }

    if((N < 0) || (N > 5000)){
        printf("bad number of vertices");
        return 0;
    }

    if((M < 0) || (M > N*(N+1)/2)){
        printf("bad number of edges");
        return 0;
    }
	
	if(((N == 0) || (N > 1)) && (M == 0)){
		printf("no spanning tree");
		return 0;
	}
	
	if((N == 1) || (M == 0))
		return 0;
	
	do{
		graph_in = (GRAPH*)malloc(M * sizeof(GRAPH));

		for(int i = 0; i < M; i++){
			if (fscanf(in, "%d %d %d", &graph_in[i].n1, &graph_in[i].n2, &graph_in[i].weight) != EOF){
			} else {
				printf("bad number of lines");
				indicate = 1;
				break;
			}
			if((graph_in[i].n1 <= 0) || (graph_in[i].n2 <= 0) || (graph_in[i].n1 > N) || (graph_in[i].n2 > N)){
				printf("bad vertex");
				indicate = 1;
				break;
			}
			if((graph_in[i].weight < 0) || (graph_in[i].weight > INT_MAX)){
				printf("bad length");
				indicate = 1;
				break;
			}
		}
		if(indicate)
			break;
		if(M > 0)
			sort(0, M-1, graph_in);
		
		if (prim(graph_in, N, M, out) != N){
			fclose(out);
			out = fopen("out.txt","w");
			fprintf (out,"no spanning tree");
		}
		break;
	}while(1);
	
	fclose(out);
	free(graph_in);
	
	return 0;
}
