#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Encode.h"
#include "Decode.h"

int main() {
  FILE *fin = fopen("in.txt", "rb");
  FILE *fout = fopen("out.txt", "wb");

  fseek(fin, 0, SEEK_END);
  int file_size = ftell(fin);
  fseek(fin, 0, SEEK_SET);
  if (file_size <= 3) goto END;

  unsigned char type[3];
  fread(type, sizeof(unsigned char), 3, fin);

  if (type[0] == 'c') {
    zip(fin, fout);
  }

  if (type[0] == 'd') {
    unzip(fin, fout);
  }
  END:
  fclose(fin);
  fclose(fout);
  return 0;
}
