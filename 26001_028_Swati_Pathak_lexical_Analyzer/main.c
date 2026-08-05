#include "header.h"

int main(int argc, char* argv[]){
    // printf("Hello");
    if(argc<2){
        printf(RED"Error! enter valid C file!\n"RESET);
        return 0;
    }
    // printf("hello\n");
    FILE* fin=fopen(argv[1],"r");
    if(fin==NULL){
        printf(RED"Error opening file!\n"RESET);
        return 0;
    }
    // printf("Hello\n");
    char word[100];
    int i=0;
    int line=1;
    char ch,next;
    int curly=0,round=0;
    // printf("Hello\n");
    // ch=fgetc(fin);
    while((ch=fgetc(fin))!=EOF){
        if(ch=='\n'){
            line++;
        }
        if(ch == '#'){
			word[i++] = ch;
		    while((ch = fgetc(fin)) != '\n'){
				word[i++] = ch;
		    }
		    word[i] = '\0';
		    printf(BLUE"Line %2d : %-23s :\t%s\n"RESET, line, "Preprocessor directive", word);
		    i = 0;
			line++;
		    continue;
	    }
        else if(ch=='/'){
            next=fgetc(fin);
            if(next=='/'){
                while((ch=fgetc(fin))!='\n');
                line++;
                continue;
            }
            else if(next=='*'){
                while(1){
                    ch=fgetc(fin);
                    next=fgetc(fin);
                    if(ch=='*' && next=='/'){
                        break;
                    }
                }
                continue;
            }
            else{
                ungetc(next,fin);
                printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "operator", ch);
                continue;
            }
        }
        else if(ch=='\''){
            i=0;
            word[i++]=ch;
            char prev=0;
            while(1){
                ch=fgetc(fin);
                if(ch=='\'' && prev!='\\'){
                    break;
                }
                else if(prev=='\'' && (ch=='\n'||ch==')')){
                    ungetc(ch,fin);
                    ch='\0';
                    break;
                }
                else if(ch==';'){
                    char next=fgetc(fin);
                    if(next=='\''){
                        ungetc(next,fin);
                        prev=ch;
                        word[i++]=ch;
                        continue;
                    }
                    printf(RED"Error: string literals not ended\n"RESET,line);
                    return 0;
                }
                prev=ch;
                word[i++]=ch;
            }
            word[i++]=ch;
            word[i]='\0';
            printf(BLUE"Line %2d : %-23s :\t%s\n"RESET, line, "String literals",word);
            i=0;
        }
        else if(ch=='"'){
            i=0;
            word[i++]=ch;
            while((ch=fgetc(fin))!='"'){
                if(ch==';'){
                    printf(RED"Error: %dstring literals not ended\n"RESET);
                    return 0;
                }
                word[i++]=ch;
            }
            word[i++]=ch;
            word[i]='\0';
            printf(BLUE"Line %2d : %-23s :\t%s\n"RESET, line, "String literals", word);
            continue;
            i=0;
        }
        else if(ch>='0' && ch<='9'){
            i=0;
            word[i++]=ch;
            ch=fgetc(fin);
            while(ch>='0' && ch<='9'){
                word[i++]=ch;
                ch=fgetc(fin);
            }
            word[i]='\0';
            printf(BLUE"Line %2d : %-23s :\t%s\n"RESET, line, "numeric constant", word);
            i=0;
            ungetc(ch,fin);
            continue;
        }
        else if(isalnum(ch)|| ch=='_'){
            i=0;
            // printf("%c",ch);
            word[i++]=ch;
            ch=fgetc(fin);
            // printf("%c",ch);
            while(isalnum(ch)||(ch=='_')){
                // printf("%c",ch);
                word[i++]=ch;
                ch=fgetc(fin);
                if(isalnum(ch)==0&&ch!='_'){
                    break;
                }
            }
            // printf("i executed\n");
            word[i]='\0';
            // printf("%s\n",word);
            if(iskeyword(word)){
                printf(BLUE"Line %2d : %-23s :\t" "%s\n"RESET, line, "Keyword",word);
            }
            else if(is_nondatakeyword(word)){
                printf(BLUE"Line %2d : %-23s :\t %s\n"RESET, line, "non-data keyword",word);
            }
            else{
                printf(BLUE"Line %2d : %-23s :\t %s\n"RESET, line, "identifier",word);
            }
            i=0;
            ungetc(ch,fin);
            continue;
        }
        else if(isoperator(ch)){
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "operator",ch);
            continue;
            
        }
        else if(ch=='('){
            round++;
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "round braces opening",ch);
        }
        else if(ch==')'){
            round--;
            if(round<0){
                printf(RED"Error: ')' braces not balanced"RESET);
                return 0;
            }
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "round braces closing",ch);
        }
        else if(ch=='{'){
            curly++;
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "curly braces opening",ch);
        }
        else if(ch=='}'){
            curly--;
            if(curly<0){
                printf(RED"Error: '}' braces not balanced"RESET);
                return 0;
            }
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "curly braces closing",ch);
        }
        else if(issymbol(ch)){
            printf(BLUE"Line %2d : %-23s :\t%c\n"RESET, line, "symbol",ch);
        }
    }
    fclose(fin);
    if(curly>0){
        printf(RED"Error: '{' braces not balanced"RESET);
        return 0;
    }
    if(round>0){
        printf(RED"Error: '(' braces not balanced"RESET);
        return 0;
    }
    printf(GREEN"Successfully parsed"RESET);
    return 0;
}