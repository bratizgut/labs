#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Val(char s){
    for(int i = 0; i < 64; i++)
        if(table[i] == s)
            return i;
}

void OUT(char str[3], int i, FILE *out){
    fprintf(out, "%c", table[(str[0] & 0xFC) >> 2]);
    fprintf(out ,"%c", table[((str[0] & 0x03) << 4) + ((str[1] & 0xF0) >> 4)]);
    switch (i){
        case 1:
            fprintf(out, "==");
            break;
        case 2:
            fprintf(out, "%c", table[((str[1] & 0x0F) << 2) + ((str[2] & 0xC0) >> 6)]);
            fprintf(out, "=");
            break;
        default:
            fprintf(out, "%c", table[((str[1] & 0x0F) << 2) + ((str[2] & 0xC0) >> 6)]);
            fprintf(out, "%c", table[(str[2] & 0x3F)]);
    }

}

void OUT1(char str[4], int i, FILE *out){
    fprintf(out, "%c", ((Val(str[0]) & 0x3F) << 2) + ((Val(str[1]) & 0x30) >> 4));
    if(i>2)
        fprintf(out, "%c", ((Val(str[1]) & 0x0F) << 4) + ((Val(str[2]) & 0x3C) >> 2));
    if(i>3)
        fprintf(out, "%c", ((Val(str[2]) & 0x03) << 6) + ((Val(str[3]) & 0x3F) >> 0));
}

void encode(FILE *in, FILE *out){
    char str[3] = {0};
    int check = 1, i = 0;

    while (check) {
        for (i = 0; i < 3; i++) {
            str[i] = (fgetc(in));
            if(str[i] == EOF) {
                str[i] = 0;
                check = 0;
                break;
            }
        }
        if(i > 0)
            OUT(str, i, out);
    }
}

void decode(FILE *in, FILE *out){
    char str[4] = {0};
    int check = 1, i = 0;

    while (check) {
        for (i = 0; i < 4; i++) {
            str[i] = (fgetc(in));
            if(str[i] == '=' || str[i] == EOF) {
                str[i] = 0;
                check = 0;
                break;
            }
        }
        if(i > 0)
            OUT1(str, i, out);
    }
}

int main(int argc,char *argv[]){
    FILE *input, *output;
    char *in = argv[argc - 2], *out = argv[argc - 1];

    output = fopen(out, "wb");
    if(!(input = fopen(in, "rb"))){
        printf("file does not exist!");
        fclose(output);
        return 0;
    }
	decode(input, output);
	fclose(input);
	fclose(output);
    
//АРГУМЕНТЫ ИЗ КОМАНДНОЙ СТРОКИ
    return 0;
}
