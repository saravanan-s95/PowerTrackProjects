#include<stdio.h>
#include"source2html.h"

int main(int argc,char *argv[])
{
    FILE *src,*html;

    if(argc!=2)
    {
        printf("Usage: %s <source_file>\n",argv[0]);
        return 1;
    }

    src=fopen(argv[1],"r");
    if(src==NULL)
    {
        printf("Unable to open source file\n");
        return 1;
    }

    html=fopen("output.html","w");
    if(html==NULL)
    {
        
        printf("Unable to create output file\n");
        fclose(src);
        return 1;
    }

    html_begin(html);
    tokenize(src,html);
    html_end(html);

    fclose(src);
    fclose(html);

    printf("HTML file generated successfully.\n");

    return 0;
}