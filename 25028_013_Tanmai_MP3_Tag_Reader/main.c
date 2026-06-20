#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int filename(char *str);
void view(char *filename);
void edit(char *filename);
int main()
{
    char str[100];
   // Read MP3 file name from user
    printf("Enter the file name with extension(.mp3):");
    scanf("%s",str);
    int option;
    do
    {
      // Check whether entered file is a valid MP3 file
    if(filename(str))
    {
       // Display menu
       printf("Enter the option:\n1.View\n2.Edit\n");
       scanf("%d",&option);
       switch(option)
       {
        case 1:
        printf("Viewing the data of mp3 file\n");
        view(str);
        break;
        case 2:
        printf("Editing the data of mp3 file\n");
        edit(str);
        break;
        default:
        printf("Please enter valid option\n");
      }
    }
    else
    {
       printf("its is not mp3 file\n");
    }
    } while (option !='3');     // Exit when user enters 3
}