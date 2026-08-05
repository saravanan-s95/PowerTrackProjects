#include "header.h"

int iskeyword(char* word){
    for(int i=0;i<19;i++){
        if(strcmp(keywords_data[i],word)==0){
            return 1;
        }
    }
    return 0;
}
int is_nondatakeyword(char* word){
    for(int i=0;i<13;i++){
        if(strcmp(keywords_non_data[i],word)==0){
            return 1;
        }
    }
    return 0;
}
int issymbol(char ch){
    for(int i=0;i<9;i++){
        if(symbols[i]==ch){
            return 1;
        }
    }
    return 0;
}
void html_begin(FILE* fout){
    fprintf(fout,"<!DOCTYPE html>\n");
    fprintf(fout,"<html lang=\"en-US\">\n"); // Fixed bracket
    fprintf(fout,"<head>\n");
    fprintf(fout,"<title>sourcecode_to_html</title>\n");
    fprintf(fout,"<meta charset=\"UTF-8\">\n");
    // Standardize styles internally to prevent broken links
    fprintf(fout,"<style>\n"
                 "body{background-color:#131212; color:#d8d1d1;}\n" // Dark theme helps see bright hexes
                 ".preprocessor{color:#86c2f3;}\n"
                 ".header{color:#a51699;}\n"
                 ".keyword_data{color:#16b102;}\n"
                 ".keyword_non_data{color:#e6c456;}\n" // Match name exactly
                 ".comment{color:#08a858;}\n"
                 ".string{color:#a51699;}\n"
                 ".char{color:#a51699;}\n"
                 ".number{color:#a51699;}\n"
                 ".char{color:#b00b9f;}\n"
                 "</style>\n");
    fprintf(fout,"</head>\n");
    fprintf(fout,"<body>\n<pre>\n");
}
void html_end(FILE* fout){
    fprintf(fout, "</pre></body></html>");
}
void handle_preprocessor(FILE* fin,FILE* fout,char ch){
    // printf("I was called");
    char word[100];
    int idx=0;
    fprintf(fout, "<span class=\"preprocessor\">#");
    // word[idx++]=ch;
    ch=fgetc(fin);
    while(ch!=EOF && ch!=' ' && ch!='<' && ch!='"'){
        // printf("I am while 1\n");
        word[idx++]=ch;
        ch=fgetc(fin);
    }
    // printf("I exited\n");
    word[idx]='\0';
    fprintf(fout, "%s</span>", word);
    fprintf(fout, "<span class=\"header\">");
    ungetc(ch,fin);
    while(ch!='\n'&&ch!=EOF){
        // printf("I was called2\n");
        ch=fgetc(fin);
        if(ch=='<'){
            fprintf(fout,"&lt");
        }
        else if(ch=='>'){
            fprintf(fout,"&gt");
        }
        else{
            fputc(ch,fout);
        }
    }
    fprintf(fout, "</span>");
    // printf("I exited2\n");
}

void handle_identifiers(FILE* fin,FILE* fout,char ch){
    char word[100];
    int idx=0;
    word[idx++]=ch;
    ch=fgetc(fin);
    while(isalnum(ch) || ch=='_'){
        word[idx++]=ch;
        ch=fgetc(fin);
    }
    word[idx]='\0';
    // ungetc(ch,fin);
    if(iskeyword(word)){
        fprintf(fout, "<span class=\"keyword_data\">%s</span>", word);
    }
    else if(is_nondatakeyword(word)){
        fprintf(fout, "<span class=\"keyword_non_data\">%s</span>", word);
    }
    else{
            fprintf(fout, "%s", word);
    }
    ungetc(ch,fin);
}
void handle_comments(FILE* fin,FILE* fout,char ch){
    char word[100];
    int idx=0;
    char next=fgetc(fin);
    if(next=='/'){
        fprintf(fout, "<span class=\"comment\">//");
        while((ch = fgetc(fin)) != '\n' && ch != EOF)
        	fputc(ch, fout);
        fprintf(fout, "</span>\n");
    }
    else if(next=='*'){
        fprintf(fout, "<span class=\"comment\">/*");
        while(1){
            ch=fgetc(fin);
            next=fgetc(fin);
            if(ch=='*' && next=='/'){
                break;
            }
            fputc(ch,fout);
            fputc(next,fout);
        }
        fputc(ch,fout);
        fputc(next,fout);
        fprintf(fout, "</span>");
    }
    else{
        fputc(ch,fout);
        if(next!=EOF){
            ungetc(next,fin);
        }     
    }
}

void handle_digits(FILE* fin,FILE* fout,char ch){
    char word[100];
    int idx=0;
    word[idx++]=ch;
    ch=fgetc(fin);
    while(ch>='0' && ch<='9'){
        word[idx++]=ch;
        ch=fgetc(fin);
    }
    word[idx]='\0';
    ungetc(ch,fin);
    fprintf(fout, "<span class=\"number\">%s</span>", word);
}

void handle_strings(FILE* fin, FILE* fout,char ch){
    char word[100];
    int idx=0;
    // word[idx++]=ch;
    while((ch=fgetc(fin))!=EOF){
        word[idx++]=ch;
        if(ch=='"'&& word[idx-2]!='\\'){
            break;
        }
        if(ch=='\n'){
            break;
        }
    }
    // word[idx++]=ch;
    word[idx]='\0';
    fprintf(fout, "<span class=\"string\">\"");
    fprintf(fout,"%s",word);
    fprintf(fout, "</span>");
}
void handle_char_string(FILE* fin, FILE* fout,char ch){
    char word[100];
    int idx=0;
    // word[idx++]=ch;
    while((ch=fgetc(fin))!=EOF){
        word[idx++]=ch;
        if(ch=='\''&& word[idx-2]!='\\'){
            break;
        }
        else if(word[idx-2]=='\'' && (ch==')' || ch=='\n')){
            ungetc(ch,fin);
            break;
        }
        if(ch=='\n'){
            break;
        }
    }
    // word[idx++]=ch;
    word[idx]='\0';
    fprintf(fout, "<span class=\"char\">\'");
    fprintf(fout,"%s",word);
    fprintf(fout, "</span>");
}


