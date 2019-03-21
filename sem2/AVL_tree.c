#include <stdio.h>
#include <stdlib.h>

struct node{
	char height;
	int value;
	struct node* right;
	struct node* left;
};

char height(struct node* T){
	if (T == NULL)
	return 0;
	return T->height;
}

int getbalance(struct node* T){
	return height(T->right) - height(T->left);
}

void fixheight(struct node* T){
	char hl = height(T->left);
	char hr = height(T->right);
	if(hl > hr)
		T->height = hl + 1;
	else 
		T->height = hr + 1;
}

struct node* right(struct node* T){
	struct node* L = T->left;
	T->left = L->right;
	L->right = T;
	fixheight(T);
	fixheight(L);
	return L;
}

struct node* left(struct node* L){
	struct node* T = L->right;
	L->right = T->left;
	T->left = L;
	fixheight(L);
	fixheight(T);
	return T;
}

struct node* balance(struct node* T){
	fixheight(T);
	if (getbalance(T) == 2){
		if (getbalance(T->right) < 0)
			T->right = right(T->right);
	return left(T);
	} else if (getbalance(T) == -2){
		if (getbalance(T->left) > 0)
			T->left = left(T->left);
		return right(T);
	}
	return T;
}

struct node* NewNode(struct node* T, int k){
	if (!T){
		T = malloc(sizeof(struct node));
		T->height = 1, T->value = k, T->left = NULL, T->right = NULL;
		return T;
	}
	if (k < T->value){
		T->left = NewNode(T->left, k);
	} else {
		T->right = NewNode(T->right, k);
	}
	return balance(T);
}

void Tree_free(struct node* T){
	if (T->left){
		Tree_free(T->left);
	}
	if (T->right){
		Tree_free(T->right);
	}
	free(T);
}

int main(void){
	struct node* root = NULL;
	int N = 0, k = 0;
	scanf("%d", &N);

	if (N == 0){
		printf("0");
		return 0;
	}

	for (int i = 0; i < N; i++){
		scanf("%d", &k);
		root = NewNode(root, k);
	}
	printf("%d", height(root));
	Tree_free(root);
	return 0;
}
