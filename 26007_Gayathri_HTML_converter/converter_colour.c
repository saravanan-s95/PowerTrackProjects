#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char *w)
{
    char *k[] = {"int","char","if","else","return","include","define"};   //keywords
     
    for(int i = 0;i < 7;i++)        
    if(strcmp(w, k[i]) == 0)  //compare strings
        return 1;
    return 0;
}

int main()
{
    FILE *src = fopen("input.c","r");       //src file open in read mode         
    FILE *dest = fopen("output_colour.html","w");  //output file in write mode

    //write the starting HTML + CSS header to the file and add some colurs 
    fprintf(dest, "<html><head><style>body{font-family:monospace; font-size:16px;} .k{color:blue; font-weight:bold;} .s{color:red;} .h{color:green;} .c{color:gray;}</style></head><body><pre>\n");

    //variable to store one character read from src
    char ch;

    //read the source file  till EOF
    while((ch = fgetc(src)) != EOF)  
    {
        // check less than symbol
        if(ch == '<')              
        { 
            //start a green span for header and write safe HTML &lt =>less than
            //<span class = 'h > = header style, &lt < in html
            fprintf(dest,"<span class='h' > &lt;");
        }
        //check greater than
        else if(ch =='>')
        { 
            //write html &gt, for  > close the green
            fprintf(dest,"&gt;</span>"); 
        }
        //check aprasan
        else if(ch == '&')
        {
            //& is used HTML entities like &lt, we escape to &amp
            fprintf(dest,"&amp;"); 
        }

        //check for double quotes
        else if(ch == '"')
        {
            //start a red span for string
            fprintf(dest,"<span class='s'>\"");

            //read span for reading
            while((ch = fgetc(src)) != '"' && ch != EOF)
            {
                if(ch == '<')        //lesser than
                    fprintf(dest,"&lt;");
                else if(ch == '>')          //greater than
                    fprintf(dest,"&gt;");
                else if(ch == '&')          //ampersent
                    fprintf(dest,"&amp;");
                else 
                //normal char write as it is
                    fputc(ch,dest);
            }
            //close the string and red span
            fprintf(dest,"\"</span>");
        }

        //check if char is '\'
        else if(ch == '\'')
        {
            
            //start red span for char
            fprintf(dest,"<span class ='s'>'");

            while((ch=fgetc(src))!='\'' && ch!=EOF) 
            //read till next single quote
                fputc(ch,dest);

            //close red span    
            fprintf(dest,"'</span>");
        }

        //check if char or #
        else if(isalpha(ch) || ch == '#'){
            char word[100]; 
            int k = 0;

            //word start with #
            int isHash = (ch == '#');

            //if not
            if(!isHash) 
                word[k++] = ch;  //save first letter
            while(isalnum(ch = fgetc(src)) || ch == '_') 
                word[k++] = ch;

            //end the word with null
            word[k] = '\0';

            //go back 1 step 
            fseek(src,-1,SEEK_CUR);

            //check if char is a keyword
            if(isKeyword(word))
            {

                //if hash
                if(isHash) 
                    fprintf(dest,"<span class = 'k'>#%s</span>",word); // #include => blue + bold
                else 
                    fprintf(dest,"<span class = 'k'>%s</span>",word); //int => blue + bold
            } 

            //not a keyword 
            else 
            {
                if(isHash) 
                    fprintf(dest,"#%s",word); //#define not keyword
                else 
                    fprintf(dest,"%s",word);  //normal word
            }
        }
        else 
            fputc(ch,dest);
    }

    //write and close a file
    fprintf(dest,"</pre></body></html>");

    //close src file
    fclose(src); 

    //close dest file
    fclose(dest);
   
    return 0;
}