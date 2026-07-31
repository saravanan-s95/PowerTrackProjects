#include "minishell.h"
char *external_command[155];
int input[20];
extern int index1;
void extract_external_command()
{
    FILE* fd = fopen("external_command.txt","r");
    char str[20];
    if(fd == NULL)
    {
        printf("Invalid in extract\n");
        return;
    }
    int i=0;
    while((fscanf(fd," %[^\n]",str))!=EOF)
    {
        
        external_command[i]=malloc(1*strlen(str)+1);
        int len = strlen(str);
        //printf("len is : %d\n",len);
        str[len-1] = '\0';
        // char *ptr = strrchr(str,'\n');
        // *ptr = '\0';

        strcpy(external_command[i],str);
      
        //int j=0;
        i++;
    }
    fclose(fd);
    external_command[i]=NULL;
    
}

void convert_input_string(char *str,char *input_string[])
{
    int i=0;
    index1 = 0;
    input[0]=0;
    //index1++;
    char *ptr = strtok(str," ");
    while(ptr!=NULL)
    {
        //`printf("shubham\n");
        input_string[i]=malloc(1*strlen(ptr)+1);
        if((strcmp(ptr,"|"))==0)
        {
            input[++index1]=i+1;
            input_string[i]=NULL;
        }
        else
        {
            strcpy(input_string[i],ptr);
        }
        i++;
        
        ptr=strtok(NULL," ");
    }
    input_string[i]=NULL;
    return;
}