#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 1001

struct num{
	int nums[MAX];
	int top;
};

struct oper{
	char opers[MAX];
	int top;
};

void init1(struct num *nmb);
void push1(struct num *nmb, int sym);
int pop1(struct num *nmb);
int isempty1(struct num *nmb);
int nmbTop(struct num *nmb);

void init2(struct oper *op);
void push2(struct oper *op, char sym);
char pop2(struct oper *op);
int isempty2(struct oper *op);
int opTop(struct oper *op);


void poland(struct num *nmb, char *ex, struct oper *op);
void calc(struct num *nmb, char sym);
int prior(char sym);

int main(void){
	char ex[1000] = {0};
	struct num nmb;
	struct oper op;
	init1(&nmb);
	init2(&op);
	poland (&nmb, ex, &op);
	printf ("%d", nmbTop(&nmb));
	return 0;
}

void poland(struct num *nmb, char *ex, struct oper *op){
	char sym;
	int number;
	int i = 0, count = 0, inbracket = 0;
	while ((sym = fgetc(stdin)) != EOF){
		if (isdigit(sym) != 0){
			ex[i] = sym;
			++i;
			while (isdigit(sym = fgetc(stdin)) != 0){
				ex[i] = sym;
				++i;
			}
			number = atoi(ex);
			++inbracket;
			push1(nmb, number);
			memset(ex,0,i);
			i = 0;
		}
		if (sym == '('){
			inbracket = 0;
			push2(op,sym);
			++count;
			continue;
		}
		else if (sym == ')'){
			--count;
			if ((count >= 0)&&(inbracket != 0)){
				while (opTop(op) != '('){
					calc(nmb, pop2(op));
				}
			pop2(op);
			}
			else{
				printf ("syntax error");
				exit (0);
			}
		}
		else if ((sym == '+')||(sym == '-')||(sym == '*')||(sym == '/')){
			while ((!isempty2(op))&&(prior(opTop(op))>=prior(sym)))
				calc(nmb, pop2(op));
			push2(op, sym);
			continue;
		}
		else if ((sym != '\n')&&(sym != EOF)){
			printf ("syntax error");
			exit (0);
		}
	}
	if (!isempty1(nmb))
		while (!isempty2(op))
			calc(nmb,pop2(op));
	else{
		printf ("syntax error");
		exit (0);
	}
}
void calc(struct num *nmb, char sym){
	int a,b;
	if (sym == '('){
		printf ("syntax error");
		exit (0);
	}
	else{
		if (!isempty1(nmb))
			a = pop1(nmb);
		else{
			printf ("syntax error");
			exit (0);
		}
		if (!isempty1(nmb))
			b = pop1(nmb);
		else{
			printf ("syntax error");
			exit (0);
		}
		if (sym == '+')
			push1(nmb, b+a);
		else if (sym == '-')
			push1(nmb, b-a);
		else if (sym == '*')
			push1(nmb, b*a);
		else if (sym =='/')
			if (a != 0)
				push1(nmb, b/a);
			else{
				printf ("division by zero");
				exit (0);
			}
	}
}

void push1(struct num *nmb, int sym){
	if(nmb->top < MAX) {
    nmb->nums[nmb->top] = sym;
    nmb->top++;
	} 
}

void init1(struct num *nmb) {
  nmb->top = 0;
}

int pop1(struct num *nmb) {
  int sym;
  if((nmb->top) > 0) {
    nmb->top--;
    sym = nmb->nums[nmb->top];
    return(sym);
  }
  else 
    return 0;
}

int opTop(struct oper *op) {
    return( op->opers[op->top-1]);
}

int nmbTop(struct num *nmb) {
    return( nmb->nums[nmb->top-1]);
}
 
int isempty1(struct num *nmb) {
	if((nmb->top) == 0)
		return 1;
	else 
		return 0;
}

void push2(struct oper *op, char sym){
	if(op->top < MAX) {
    op->opers[op->top] = sym;
    op->top++;
	} 
}

void init2(struct oper *op) {
  op->top = 0;
}

char pop2(struct oper *op) {
  char sym;
  if((op->top) > 0) {
    op->top--;
    sym = op->opers[op->top];
    return(sym);
  }
  else 
    return 0;
}
 
int isempty2(struct oper *op) {
	if((op->top) == 0)
		return 1;
	else 
		return 0;
}

int prior(char sym){
	if ((sym == '(')||(sym == ')'))
		return 1;
	if ((sym == '+')||(sym =='-'))
		return 2;
	if ((sym == '*')||(sym == '/'))
		return 4;
}	
