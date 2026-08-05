#include "header.h"
int main(int argc, char* argv[]){
    //file input : ./a.out test.c output.html
    if(argc<2 || argc>3){
        printf(RED"Error: Enter file name\n"RESET);
        printf(BLUE"Usage: <executable> filename  outputfile.html\n"RESET);
        printf(BLUE"example: ./a.out abc.c xyz.html\n"RESET); 
        return 0;
    }
    if(!validatefile(argv)){
        printf(RED"Error: Enter file name with valid extension\n"RESET);
        printf(BLUE"Usage: <executable> filename  outputfile.html\n"RESET);
        printf(BLUE"example: ./a.out abc.c xyz.html\n"RESET); 
        return 0; 
    }
    FILE* fin=fopen(argv[1],"r");
    FILE* fout=fopen(argv[2],"w");

    if(fin==NULL || fout==NULL){
        printf(RED"Error: opening the file"RESET);
        return 0;
    }
    html_begin(fout);
    char ch;
    while((ch=fgetc(fin))!=EOF){
        if(ch=='#'){
            handle_preprocessor(fin,fout,ch);
        }
        else if(ch>='0' && ch<='9'){
            handle_digits(fin,fout,ch);
        }
        else if(ch=='"'){
            handle_strings(fin,fout,ch);
        }
        else if(ch=='/'){
            handle_comments(fin,fout,ch);
        }
        else if(isalpha(ch)|| ch=='_'){
            // printf("%c",ch);
            // printf("I am identifier\n");
            handle_identifiers(fin,fout,ch);
        }
        else if(ch=='\''){
            handle_char_string(fin,fout,ch);
        }
        else{
            if(ch=='<'){
                fprintf(fout, "&lt;");
            }
            else if(ch=='>'){
                fprintf(fout, "&gt;");
            }
            else if(ch=='&'){
                fprintf(fout, "&amp;");
            }
            else{
                fputc(ch, fout);
            }
        }
    }
    html_end(fout);
    fclose(fin);
    fclose(fout);
    printf(MAGENTA"OUTPUT FILE SUCCESSFULLY GENERATED"RESET);
    return 0;
}

int validatefile(char**argv){
    if(strstr(argv[1],".c")==NULL)
        return 0;
    else if(strstr(argv[2],".html")==NULL)
        return 0;
    return 1;
}