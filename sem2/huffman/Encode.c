#include "Encode.h"
#include "Convert.h"

int print_bin(char *bin, FILE *fout) {
  int size = strlen(bin);
  int bin_pos = 0;
  int k = 0;
  size_t zip_pos = 0;
  unsigned char *zipped =
      (unsigned char *) malloc(sizeof(unsigned char) * (BUF_SIZE + 1));
  while (bin_pos < size) {
    char bin8[9];
    for (int i = 0; i < 8; i++, bin_pos++) {
      if (bin_pos >= size) {
        bin8[i] = '0';
        k++;
        continue;
      }
      bin8[i] = bin[bin_pos];
    }
    bin8[8] = '\0';
    unsigned char tmp;
    bin_to_char(bin8, &tmp);
    zipped[zip_pos] = tmp;
    zip_pos++;
  }
  fwrite(zipped, sizeof(unsigned char), zip_pos, fout);
  free(zipped);
  return k;
}

void make_dir(node *cur_node, char *cur_dir, int *cur_size) {
  if (cur_node->is_leaf) {
    cur_dir[*cur_size] = '0';
    (*cur_size)++;
    char bin_code[9];
    char_to_bin(cur_node->symbol, &bin_code[0]);
    for (int i = 0; i < 8; ++i) {
      cur_dir[*cur_size + i] = bin_code[i];
    }
    *cur_size += 8;
    return;
  }

  cur_dir[*cur_size] = '1';
  (*cur_size)++;
  if (cur_node->right->is_leaf) {
    cur_dir[*cur_size] = '0';
    (*cur_size)++;
    char bin_code[9];
    char_to_bin(cur_node->right->symbol, &bin_code[0]);
    for (int i = 0; i < 8; ++i) {
      cur_dir[*cur_size + i] = bin_code[i];
    }
    *cur_size += 8;
  } else {
    make_dir(cur_node->right, cur_dir, cur_size);
  }

  if (cur_node->left->is_leaf) {
    cur_dir[*cur_size] = '0';
    (*cur_size)++;
    char bin_code[9];
    char_to_bin(cur_node->left->symbol, &bin_code[0]);
    for (int i = 0; i < 8; ++i) {
      cur_dir[*cur_size + i] = bin_code[i];
    }
    *cur_size += 8;
  } else {
    make_dir(cur_node->left, cur_dir, cur_size);
  }
}

node *build_tree(node *mas, int symb_sum) {
  node *min1 = (mas[0].freq < mas[1].freq) ? &mas[0] : &mas[1];
  node *min2 = (mas[0].freq < mas[1].freq) ? &mas[1] : &mas[0];
  for (int i = 2; i < symb_sum; ++i) {
    if (mas[i].freq < min1->freq && mas[i].freq < min2->freq) {
      min2 = min1;
      min1 = &mas[i];
    } else if (mas[i].freq < min2->freq) {
      min2 = &mas[i];
    }
  }

  if (min2->freq == INF) {
    return min1;
  }

  mas[symb_sum].is_leaf = 0;
  mas[symb_sum].right = min2;
  mas[symb_sum].left = min1;
  mas[symb_sum].freq = min1->freq + min2->freq;
  symb_sum++;

  min1->freq = INF;
  min2->freq = INF;

  return build_tree(mas, symb_sum);
}

void get_code(node *cur_node, char cur_code[], int cur_pos) {
  if (cur_node->is_leaf) {
    if (cur_pos == 0) {
      strcpy(cur_node->code, "1\0");
      return;
    }
    cur_code[cur_pos] = '\0';
    strcpy(cur_node->code, cur_code);
    return;
  }

  cur_code[cur_pos] = '0';
  get_code(cur_node->left, cur_code, cur_pos + 1);
  cur_code[cur_pos] = '1';
  get_code(cur_node->right, cur_code, cur_pos + 1);
}

void zip(FILE *fin, FILE *fout) {
  node *nodes = (node *) malloc(sizeof(node) * 512);
  for (int i = 0; i < 256; ++i) {
    nodes[i].is_leaf = 0;
    nodes[i].left = NULL;
    nodes[i].right = NULL;
    nodes[i].symbol = (unsigned char) i;
    nodes[i].freq = 0;
    nodes[i].code = (char *) malloc(sizeof(char) * 32);
  }

  unsigned char
      *text = (unsigned char *) malloc(sizeof(unsigned char) * (BUF_SIZE));
  int to_zip_size = 0;

  while (1) {
    int fsize = fread(text, sizeof(unsigned char), BUF_SIZE, fin);
    if (fsize == 0) break;

    for (int i = 0; i < fsize; ++i) {
      nodes[(int) text[i]].freq++;
      nodes[(int) text[i]].is_leaf = 1;
      to_zip_size++;
    }
  }

  for (int i = 0; i < 256; ++i) {
    if (nodes[i].freq == 0) {
      nodes[i].freq = INF;
      continue;
    }
  }
  int symb_sum = 256;

  node *root = build_tree(&nodes[0], symb_sum);

  char start_code[101];
  get_code(root, start_code, 0);

  char *bin_zip = (char *) malloc(sizeof(char) * (8 * (BUF_SIZE + 1)) + 1);
  int code_size = 0;
  make_dir(root, bin_zip, &code_size);

  fseek(fin, 0, SEEK_SET);
  unsigned char tmp[3];
  fread(tmp, sizeof(unsigned char), 3, fin);

  int fsize = fread(text, sizeof(unsigned char), BUF_SIZE, fin);

  for (int i = 0; i < fsize; ++i) {
    for (int j = 0; j < strlen(nodes[text[i]].code); ++j) {
      bin_zip[code_size] = nodes[text[i]].code[j];
      bin_zip[code_size + 1] = '\0';
      code_size++;
      if (code_size == 8 * BUF_SIZE) {
        print_bin(bin_zip, fout);
        code_size = 0;
        bin_zip[0] = '\0';
      }
    }
    if (i == fsize - 1) {
      fsize = fread(text, sizeof(unsigned char), BUF_SIZE, fin);
      i = -1;
    }
  }

  tmp[0] = (unsigned char) (print_bin(bin_zip, fout) + '0');
  fwrite(tmp, sizeof(unsigned char), 1, fout);
  free(text);
  free(bin_zip);
}