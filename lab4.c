#include <stdio.h>
#include <string.h>

int check( int arr[], int len ){
	int i, checkarr[10] = {0};
	for ( i = 0; i < len; i++ ){
        checkarr[arr[i]] += 1;
        if( ( checkarr[arr[i]]>1 ) || ( arr[i] < 0 ) || ( arr[i] > 9 ) )
            return 1;
    }
	return 0;
}

void Sort( int a[], int n,int l )
{
    int j, K;
    do{
        K = 0;
        for ( j = l + 1; j < n; j++ )
            if ( a[j-1] > a[j] ) {
                int c = 0;
                c = a[j-1];
                a[j-1] = a[j];
                a[j] = c;
                K = j;
            }
        n = K;
    }while( n );
}

int Sortcheck( int arr[], int len ){
    int count=0;
    for( int i = 0; i<len-1; i++ ){
        if( arr[i]>arr[i+1] )
            count++;
    }
    if( count==len )
        return 0;
    return 1;
}

void Dijkstra( int arr[], int len, int N ){
	int i=0,check=0;
    while((i<N) && ( Sortcheck(arr, len) ) ){
        for (int j = len - 1; j >= 0; j--){
            for (int k = j - 1 ; k >= 0; k--) {
                if(arr[j] > arr[k]) {
                    int c = arr[j];
                    arr[j] = arr[k];
                    arr[k] = c;
                    Sort( arr,len,k+1 );
                    for ( int s = 0; s < len; s++ )
						printf("%d", arr[s]);
                    check += 1;
                    break;
                }
            }
            if(check){
                check -= 1;
                break;
            }
        }
        i++;
        printf("\n");
    }
}

int main(void){
	int arr[10], N, len;
	char str[11];
	scanf("%s", str);
	len = strlen(str);
	scanf("%d", &N);
	for (int i = 0; i < len; i++)
		arr[i] = str[i] - '0';
	if ( check(arr, len) ){
		printf("bad input");
		return 0;
	} else {
		Dijkstra( arr, len, N );
	}
	return 0;
}
