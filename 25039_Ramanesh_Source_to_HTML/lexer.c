#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"source2html.h"

void tokenize(FILE *src,FILE *html)
{
    int ch;

    while((ch=fgetc(src))!=EOF)
    {
        if(isalpha(ch)||ch=='_')
        {
            char word[100];
            int i=0;

            do
            {
                word[i++]=ch;
                ch=fgetc(src);
            }
            while(isalnum(ch)||ch=='_');

            word[i]='\0';

            if(ch!=EOF)
                ungetc(ch,src);

            if(isKeyword(word))
                print_keyword(html,word);
            else
                print_identifier(html,word);
        }

        else if(isdigit(ch))
        {
            char num[50];
            int i=0;

            do
            {
                num[i++]=ch;
                ch=fgetc(src);
            }
            while(isdigit(ch));

            num[i]='\0';

            if(ch!=EOF)
                ungetc(ch,src);

            print_number(html,num);
        }

        else if(ch=='"')
        {
            char str[500];
            int i=0;

            str[i++]='"';

            while((ch=fgetc(src))!=EOF)
            {
                str[i++]=ch;

                if(ch=='"')
                    break;
            }

            str[i]='\0';

            print_string(html,str);
        }

        else if(ch=='#')
        {
            char pre[500];
            int i=0;

            pre[i++]='#';

            while((ch=fgetc(src))!=EOF && ch!='\n')
                pre[i++]=ch;

            pre[i]='\0';

            print_preprocessor(html,pre);

            if(ch=='\n')
                fputc('\n',html);
        }

        else if(ch=='/')
        {
            int next=fgetc(src);

            if(next=='*')
            {
                char comment[2000];
                int i=0;

                comment[i++]='/';
                comment[i++]='*';

                while((ch=fgetc(src))!=EOF)
                {
                    comment[i++]=ch;

                    if(ch=='*')
                    {
                        next=fgetc(src);

                        if(next==EOF)
                            break;

                        comment[i++]=next;

                        if(next=='/')
                            break;

                        ungetc(next,src);
                        i--;
                    }
                }

                comment[i]='\0';

                print_comment(html,comment);
            }
            else
            {
                if(next!=EOF)
                    ungetc(next,src);

                print_operator(html,"/");
            }
        }

        else if(strchr("+-*=<>!&|%^",ch))
        {
            char op[2];

            op[0]=ch;
            op[1]='\0';

            print_operator(html,op);
        }

        else
        {
            html_escape(html,ch);
        }
    }
}