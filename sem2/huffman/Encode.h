#ifndef HUFFMAN__ENCODE_H_
#define HUFFMAN__ENCODE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INF (int)1e9
#define BUF_SIZE (int)1e4

typedef short bool;

typedef struct node {
  struct node *right;
  struct node *left;
  int freq;
  bool is_leaf;
  unsigned char symbol;
  char *code;
}node;

void zip(FILE *fin, FILE *fout);
#endif //HUFFMAN__ENCODE_H_
