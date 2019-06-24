#include "Convert.h"
#include "Encode.h"

void char_to_bin(unsigned char c, char *bin_code) {
  unsigned int k = 128;
  for (int i = 0; i < 8; ++i) {
    bin_code[i] = (bool) (c & k) ? (char) '1' : (char) '0';
    k >>= 1u;
  }
  bin_code[8] = '\0';
}

void bin_to_char(const char *bin, unsigned char *c) {
  int res = 0;
  unsigned int N = 128;
  for (int i = 0; i < 8; ++i) {
    res += (int) N * (bin[i] - '0');
    N >>= 1u;
  }
  *c = (unsigned char) res;
}