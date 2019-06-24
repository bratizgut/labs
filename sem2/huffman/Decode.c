#include "Decode.h"
#include "Encode.h"
#include "Convert.h"

void decode(FILE *fout, char *bin, int *cur_pos, node *root,
            int end_pos, int last_byte, FILE *fin,
            unsigned char *zip_text) {
  node *cur_node = root;
  unsigned char *out =
      (unsigned char *) malloc(sizeof(unsigned char) * (8 * (BUF_SIZE + 1)));

  size_t out_pos = 0;

  for (; *cur_pos < end_pos; ++(*cur_pos)) {
    if (end_pos - (*cur_pos) == last_byte && end_pos < 8 * (BUF_SIZE - 1)) {
      fwrite(out, sizeof(unsigned char), out_pos, fout);
      break;
    }

    if (root->is_leaf) {
      out[out_pos] = root->symbol;
      out[out_pos + 1] = '\0';
      out_pos++;
    } else {
      if (bin[*cur_pos] == '1') {
        cur_node = cur_node->right;
      } else {
        cur_node = cur_node->left;
      }

      if (cur_node->is_leaf) {
        out[out_pos] = cur_node->symbol;
        out[out_pos + 1] = '\0';
        out_pos++;
        cur_node = root;
      }
    }

    if (*cur_pos == end_pos - 1) {
      fwrite(out, sizeof(unsigned char), out_pos, fout);
      int fsize =
          fread(zip_text, sizeof(unsigned char), (size_t) (BUF_SIZE - 1), fin);
      if (fsize == 0) break;
      char bin_code[9];
      char_to_bin((unsigned char) (last_byte + '0'), &bin_code[0]);
      for (int j = 0; j < 9; ++j) {
        bin[j] = bin_code[j];
      }
      end_pos = 8;

      for (int i = 0; i < fsize - 1; ++i) {
        char_to_bin(zip_text[i], &bin_code[0]);
        for (int j = 0; j < 9; ++j) {
          bin[end_pos + j] = bin_code[j];
        }
        end_pos += 8;
      }
      last_byte = zip_text[fsize - 1] - '0';

      *cur_pos = -1;
      out_pos = 0;
    }
  }
}

node *build_unzip_tree(const char *bin, int *cur_pos, node *cur_node,
                       int *free_node_pos, node *nodes) {
  if (bin[*cur_pos] == '0') {
    cur_node->is_leaf = 1;
    (*cur_pos)++;
    char bin8[9];
    for (int i = 0; i < 8; i++, (*cur_pos)++) {
      bin8[i] = bin[*cur_pos];
    }
    bin8[8] = '\0';
    bin_to_char(bin8, &cur_node->symbol);
  } else {
    cur_node->is_leaf = 0;
    (*cur_pos)++;
    cur_node->right = &nodes[*free_node_pos];
    (*free_node_pos)++;
    build_unzip_tree(bin, cur_pos, cur_node->right, free_node_pos, nodes);
    cur_node->left = &nodes[*free_node_pos];
    (*free_node_pos)++;
    build_unzip_tree(bin, cur_pos, cur_node->left, free_node_pos, nodes);

  }
  return 0;
}

void unzip(FILE *fin, FILE *fout) {
  unsigned char *zip_text =
      (unsigned char *) malloc(sizeof(unsigned char) * (BUF_SIZE + 1));
  char *bin_zip = (char *) malloc(sizeof(char) * (8 * (BUF_SIZE + 1) + 1));

  bin_zip[0] = '\0';
  int unzip_size = 0;
  int last_byte = 0;

  int fsize = fread(zip_text, sizeof(unsigned char), BUF_SIZE, fin);
  for (int i = 0; i < fsize - 1; ++i) {
    char bin_code[9];
    char_to_bin(zip_text[i], &bin_code[0]);
    for (int j = 0; j < 9; ++j) {
      bin_zip[unzip_size + j] = bin_code[j];
    }
    unzip_size += 8;
  }
  last_byte = zip_text[fsize - 1] - '0';

  if (unzip_size == 0) return;

  int cur_pos = 0;
  node *nodes = (node *) calloc(512, sizeof(node));

  int free_node_pos = 1;
  build_unzip_tree(bin_zip, &cur_pos, &nodes[0], &free_node_pos, &nodes[0]);
  decode(fout, bin_zip, &cur_pos, &nodes[0],
         unzip_size, last_byte, fin, zip_text);
  free(bin_zip);

}