#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int filename(char *str);
void view(char *filename);
void edit(char *filename);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./a.out <file.mp3>\n");
        return 1;
    }

    char *str = argv[1];

    if(filename(str) == 0)
    {
        printf("Invalid MP3 file\n");
        return 1;
    }

    int option;

    do
    {
        printf("\nEnter the option:\n");
        printf("1. View\n");
        printf("2. Edit\n");
        printf("3. Exit\n");
        printf("Choice: ");
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

            case 3:
                printf("Exiting...\n");
                break;

            default:
                printf("Please enter a valid option\n");
        }

    } while(option != 3);

    return 0;
}
