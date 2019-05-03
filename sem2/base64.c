#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

typedef struct Arguments_t{
    char e;
    char d;
    char i;
    char f;
    char er;
    int N;
}Arguments;

void help(){
    printf("\n\n\n\n");
    printf("A short manual\n");
    printf("-e is for encoding\n");
    printf("-d is for decoding\n");
    printf("-i used only with -d if used, symbols not from Base64 alphabet ar ignored\n");
    printf("-f + option(N) puts \"\\n\" every N symbols. Used only with -e");
    return;
}

char Val(char s){
    for(int i = 0; i < 64; i++)
        if(table[i] == s)
            return i;
    return -1;
}

void OUT(char str[3], int i, FILE *out){
    fputc(table[(str[0] & 0xFC) >> 2], out);
    fputc(table[((str[0] & 0x03) << 4) + ((str[1] & 0xF0) >> 4)], out);
    switch (i){
        case 1:
            fputc('=', out);
            fputc('=', out);
            break;
        case 2:
            fputc(table[((str[1] & 0x0F) << 2) + ((str[2] & 0xC0) >> 6)], out);
            fputc('=', out);
            break;
        default:
            fputc(table[((str[1] & 0x0F) << 2) + ((str[2] & 0xC0) >> 6)], out);
            fputc(table[(str[2] & 0x3F)], out);
    }

}

void OUT1(char str[4], int i, FILE *out){
    fputc(((Val(str[0]) & 0x3F) << 2) + ((Val(str[1]) & 0x30) >> 4), out);
    if(i>2)
        fputc(((Val(str[1]) & 0x0F) << 4) + ((Val(str[2]) & 0x3C) >> 2), out);
    if(i>3)
        fputc(((Val(str[2]) & 0x03) << 6) + ((Val(str[3]) & 0x3F) >> 0), out);
}

void encode(FILE *in, FILE *out){
    char str[3] = {0};
    int check = 1, i;

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

void decode(FILE *in, FILE *out, char flag){
    char str[4] = {0};
    int check = 1, i = 0;

    while (check) {
        for (i = 0; i < 4; i++) {
            str[i] = (fgetc(in));
            if((Val(str[i]) == -1) && flag){
                i--;
                continue;
            } else if(Val(str[i]) == -1)
                return;
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
    char *optString = "edif:", opt;

    output = fopen(out, "wb");
    if(!(input = fopen(in, "rb"))){
        printf("File %s does not exist!", in);
        fclose(output);
        return 0;
    }
    Arguments *args = malloc(sizeof(Arguments));
    args->e = 0;
    args->d = 0;
    args->i = 0;
    args->f = 0;
    args->er = 0;
    args->N = 0;

    while((opt = getopt(argc, argv, optString)) != -1){
        switch (opt){
            case ('e'):{
                args->e = 1;
                break;
            }
            case ('d'):{
                args->d = 1;
                break;
            }
            case ('i'):{
                args->i = 1;
                break;
            }
            case ('f'):{
                if (optarg != 0){
                    args->f = 1;
                    sscanf (optarg,"%d", &args->N);
                }
                break;
            }
            default:{
                args->er = 1;
                break;
            }
        }
    }
    if (args->er == 1){
        printf ("Unknown key(s).\n");
        help();
        fclose(input);
        free(args);
        return 0;
    }
    if ((args->e == 1) && (args->i == 1)){
        printf ("-e and -i can't be together.\n");
        help();
        fclose(input);
        return 0;
    }
    if ((args->d == 1) && (args->f == 1)){
        printf ("-d and -f can't be together.\n");
        help();
        fclose(input);
        free(args);
        return 0;
    }
    if ((args->f == 1) && (args->N == 0)){
        printf ("-f requires number.\n");
        help();
        fclose(input);
        free(args);
        return 0;
    }
    if ((args->d == 0) && (args->e == 0)){
        printf ("Choose option to continue.\n");
        help();
        fclose(input);
        free(args);
        return 0;
    }
    if ((args->d == 1) && (args->e == 1)){
        printf ("Choose only one option to continue.\n");
        help();
        fclose(input);
        free(args);
        return 0;
    }

    if(args->e)
        encode(input, output);
    if(args->d)
        decode(input, output, args->i);

    if(args->f){
        fclose(output);
        fclose(input);
        FILE *input = fopen(out, "r");
        FILE *output = fopen("temp.bin", "w");
        char tmp;
        int count = 0;
        while((tmp = fgetc(input)) != EOF){
            if(count == args->N){
                fprintf(output, "\n");
                count = 0;
            }
            count++;
            fputc(tmp, output);
        }
        fclose(input);
        fclose(output);
        remove(out);
        rename("temp.bin", out);
    }

    fclose(input);
    fclose(output);
    free(args);

    return 0;
}
