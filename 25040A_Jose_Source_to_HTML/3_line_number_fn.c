#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "main.h"

extern const char* data_keywords[];
extern const char* non_data_keywords[];
extern const char* ascii_char;

int line_count = 1;


//Function to get character-by-character from the input file, and print to html file with line-number
int get_character_from_file_number(void)
{
    int ch = 0;
    char str[100];

    fprintf(dst_fp, "%d\t", line_count);        //Print the first line number before the loop
    line_count++;                               //Increment line_count

    while((ch = fgetc(src_fp)) != EOF)
    {
        if(ch == '#')               //Pre-processor directive
        {
            print_pre_processor_to_html_file_number(str);
        }

        else if(ch == '/')          //Case of single-line or multi-line comments
        {
            print_comment_lines_to_html_file_number(str);
        }

        else if(ch == '"')          //String literals
        {
            print_string_literals_to_html_file();
        }

        else                        //If not any of the above, form tokens 
        {
            separate_token_number(ch);
        }
    }

    return SUCCESS;
}


//Function to convert the preprocessor directive to html_format with line-number
void print_pre_processor_to_html_file_number(char* str)
{
    str[0] = '\0';                              //To avoid garbage data, especially when reading a blank line
    fscanf(src_fp, "%[^\n]", str);              //If the first character read is #, then read the remaining characters
    
    int length = strlen(str);
    if(length > 0 && str[length-1] == '\r')     //If the last character read is \r, replace with \0
    {
        str[length-1] = '\0';
    }  

    fprintf(dst_fp, "<span style=\"color:purple;\">#");     //Print the '#' to html file with corresponding style

    for(int i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '<')               //If the character is '<', then write &lt; to the html file.
        {
            fprintf(dst_fp, "&lt;");
        }
        else if(str[i] == '>')          //If the character is '>', then write &gt; to the html file.
        {
            fprintf(dst_fp, "&gt;");
        }
        else                            //If any other characters, then write them as such.
        {
            fprintf(dst_fp, "%c", str[i]);
        }
    }

    fprintf(dst_fp, "</span>");         //Close the span tag
}


//Function to convert the comment_lines to html_format with line-number
void print_comment_lines_to_html_file_number(char* str)
{
    int ch1 = fgetc(src_fp);
    if(ch1 == '/')                          //Single-line comment (ie., both ch and ch1 are /)
    {
        str[0] = '\0';                      //To avoid garbage data, especially when reading a blank line
        fscanf(src_fp, "%[^\n]", str);      //Read the entire line
        
        int length = strlen(str);
        if(length > 0 && str[length-1] == '\r')     //If the last character read is \r, replace with \0
        {
            str[length-1] = '\0';
        }

        fprintf(dst_fp, "<span style=\"color:blue;\">//%s</span>", str);    //Print the comment to html file with corresponding style
    }

    else if(ch1 == '*')         //Multi-line comment
    {
        fprintf(dst_fp, "<span style=\"color:blue;\">/*");      //Print the '/*' to the html file with corresponding style

        int prev = 0;           //To track the characters read  (Stop reading when */ is present)
        int curr = 0;

        while((curr = fgetc(src_fp)) != EOF)
        {
            if(curr == '<')             //If the character is '<', then write &lt; to the html file.
            {
                fprintf(dst_fp, "&lt;");
            }
            else if(curr == '>')        //If the character is '>', then write &gt; to the html file.
            {
                fprintf(dst_fp, "&gt;");
            }
            else                        //If any other characters, then write them as such.
            {
                if(curr == '\n')        //If the character is \n, print the \n and then print the line number
                {
                    fprintf(dst_fp, "%c", curr);
                    fprintf(dst_fp, "<span style=\"color:black;\">%d\t</span>", line_count);        //Print line-number
                    line_count++;                   //Increment line-number
                }
                else
                {
                    fprintf(dst_fp, "%c", curr);
                }
            }

            if(prev == '*' && curr == '/')      //When '*' and '/' comes together, stop reading (ie., multi-line comment ends)
            {
                break;
            }

            prev = curr;
        }

        fprintf(dst_fp, "</span>");         //Close the span tag
    }

    else                        //If neither single line comment nor multi-line comment
    {
        if(ch1 != EOF)
        {
            fseek(src_fp, -1, SEEK_CUR);        //Move the pointer back by sinle position so that next loop starts from that character
        }
        fprintf(dst_fp, "<span style=\"color:firebrick;\">/</span>");       //print the first '/' as a normal character
    }
}



//Function to separate token based on space, special characters, operators etc and print with line-number
int separate_token_number(char ch)
{
    if(is_token_delimiter(ch) == TRUE)              //If the character is \0, \n, \t or space
    {
        if(ch == '\n')                              //If the character is \n, print the \n and then print the line number
        {
            fprintf(dst_fp, "%c", ch);              //Print the character as such to the html file
            fprintf(dst_fp, "%d\t", line_count);    //Print line-number
            line_count++;                           //Increment line_count
            return SUCCESS;
        }
        else
        {
            fprintf(dst_fp, "%c", ch);              //Print the character as such to the html file
            return SUCCESS;
        }
    }

    //If the character is a special character
    if( ch=='(' || ch==')' || ch=='{' || ch=='}' || ch==';' || 
        ch==',' || ch=='+' || ch=='-' || ch=='*' || ch=='/' || 
        ch=='%' || ch=='=' || ch=='!' || ch=='<' || ch=='>' || 
        ch=='|' || ch=='&')
    {
        if(ch == '<')               //If the character is '<', then write &lt; to the html file           
        {
            fprintf(dst_fp, "<span style=\"color:firebrick;\">&lt;</span>");
        }
        else if(ch == '>')          //If the character is '>', then write &gt; to the html file.
        {
            fprintf(dst_fp, "<span style=\"color:firebrick;\">&gt;</span>");
        }
        else                        //Otherwise, print as such to the html file
        {
            fprintf(dst_fp, "<span style=\"color:firebrick;\">%c</span>", ch);
        }

        return SUCCESS;
    }
    

    //If it doesnt fall under both the above mentioned cases
    char token[100]; 
    int j=0;    

    while(ch != EOF)        //Read each character 
    {
        
        if((is_token_delimiter(ch) == TRUE) || 
                ch=='(' || ch==')' || ch=='{' || ch=='}' || ch==';' || 
                ch==',' || ch=='+' || ch=='-' || ch=='*' || ch=='/' || 
                ch=='%' || ch=='=' || ch=='!' || ch=='<' || ch=='>' || 
                ch=='|' || ch=='&')
        {
            break;          //When a delimiter or special charcter is reached, break the loop
        }
        
        else        //If the character is something other than the above listed, then store each character to the token.
        {
            token[j] = ch;
            j++;
        }

        ch = fgetc(src_fp);
    }


    token[j] = '\0';            //Append the token with '\0' at the end

    categorizeToken(token);     //Categorize and print each token with corresponding style.

    if(ch != EOF)           //To identify the end of a token, the next character present after the token (in the file) is also read.
    {                       //To properly read from that character again, move one character backward.
        fseek(src_fp, -1, SEEK_CUR);
    }

    return SUCCESS;
}