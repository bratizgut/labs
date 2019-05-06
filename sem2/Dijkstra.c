#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef struct len{
  long long length;
  int way;
} data;

void Dijkstra(int **graph, int *check, data *gData, int N){
  int minindex;
  long long min, tmp;
  do
  {
    minindex = 6000;
    min = LONG_LONG_MAX;
    for(int i = 0; i < N; i++){
      if((gData[i].length < min) && (!(check[i]))){
        min = gData[i].length;
        minindex = i;
      }
    }
    if(minindex != 6000){
      for(int i = 0; i < N; i++){
        if(graph[minindex][i] > 0){
          tmp = min + graph[minindex][i];
          if(tmp < gData[i].length){
            gData[i].length = tmp;
			gData[i].way = minindex;
          }
        }
      }
      check[minindex]++;
    }
  } while (minindex < 6000);
}

int OUT(data *gData, int N){
  int flag = 0;
  for (int i = 0; i < N; i++) {
      if (gData[i].length != LONG_LONG_MAX) {
        if (gData[i].length >= INT_MAX) flag++;
        if (gData[i].length > INT_MAX) {
          printf("INT_MAX+ ");
        } else printf("%lld ", gData[i].length);
      } else printf("oo ");
    }
    return flag;
}

int main(){
  int **graph, *check, N, M, start, end;
  data *gData;

  scanf("%d", &N);
  if (scanf("%d %d\n%d", &start, &end, &M) == EOF) {
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

  if(start < 1 || start > 5000 || end < 1 || end > 5000){
	printf("bad vertex");
	return 0;
  }
  
  graph = (int**)malloc(N * sizeof(int*));
  for(int i = 0; i < N; i++)
    graph[i] = calloc(N, sizeof(int));
  gData = (data *)malloc(N * sizeof(data));
  check = calloc(N, sizeof(int));
  for(int i = 0; i < N; i++) {
    gData[i].length = LONG_LONG_MAX;
	gData[i].way = 0;
  }
  
  int indicate = 0;
  for(int i = 0; i < M; i++){
    long tmp;
    int a, b;
    if (scanf("%d %d %ld", &a, &b, &tmp) == EOF) {
      printf("bad number of lines");
      indicate++;
	  break;
    }
    if (a < 1 || a > 5000 || b < 1 || b > 5000) {
    printf("bad vertex");
    indicate++;
	  break;
    }
    if (tmp < 0 || tmp > INT_MAX) {
      printf("bad length");
      indicate++;
	  break;
    }
    graph[a-1][b-1] = tmp;
    graph[b-1][a-1] = tmp;
  }
  if(indicate)
	goto fin;

  gData[start-1].length = 0;
  Dijkstra(graph, check, gData, N);
  int flag = OUT(gData, N);
  printf("\n");

  if(gData[end-1].length == LONG_LONG_MAX){
    printf("no path");
    return 0;
  }
  if(gData[end-1].length > INT_MAX && flag > 2){
    printf("overflow");
    return 0;
  }
  int i = end-1;
  printf("%d ", end);
  while(i != start-1){
	printf("%d ", gData[i].way+1); 
	i = gData[i].way;
  }

  fin:
    free(check);
	free(gData);
	for(int i = 0; i < N; i++)
	  free(graph[i]);
	free(graph);

  return 0;
}
