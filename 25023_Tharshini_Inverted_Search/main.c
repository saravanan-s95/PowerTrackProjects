/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - MAIN FILE*/ 
                   
#include "inverted.h"
#include<string.h>

int main(int argc,char *argv[])
{
    //file list pointer
    Inputfile *file_list=NULL;

    //validate the command-line arguments
    int ret=validate_args(argc,argv,&file_list);
    if(ret!=SUCCESS)
    {
        printf("ERROR: Invalid arguments!\n");
        return FAILURE;
    }

    //hash table initialization
    Htable table[SIZE];
    for(int i=0;i<SIZE;i++)
    {
       table[i].index=i;
       table[i].head=NULL;
    }
    
int choice;
char filename[MAXFILE];
char word[MAXWORD];
char option;
int dataBase_created=0;

do
{
    printf("\n---------------INVERTED SEARCH MENU---------------\n");
    printf("1.Create Database\n");
    printf("2.Update Database\n");
    printf("3.Search a Word\n");
    printf("4.Display Database\n");
    printf("5.Save Database to File\n");
    printf("6.Exit\n");
    printf("---------------------------------------------------\n");
    printf("Enter your choice: ");
    if(scanf("%d",&choice)!=1)
    {
      printf("ERROR: Invalid input!\n");
      return FAILURE;
    }

    switch(choice)
    {
        case 1:
        if(dataBase_created)
        {
          printf("Database already created!\n");
          break;
        }
        if(create_database(table,file_list)==SUCCESS)
        {
          dataBase_created=1;
          printf("Database created successfully.\n");
        }
        else
        {
          printf("ERROR: Failed to create database!\n");
        }
        break;

        case 2:
          printf("Enter the file name to update: ");
          scanf("%s",filename);

          if(update_database(table,filename,&file_list)!=SUCCESS)
          {
            printf("ERROR: Failed to update database!\n");
          }
          break;

        case 3:
          printf("Enter the word to search: ");
          scanf("%s",word);
          if(search_database(table,word)==SUCCESS)
          {
            printf("Search completed successfully.\n");
          }
          break;

        case 4:
          display_database(table);
          break;

        case 5:
          printf("Enter file name to save(eg., backup.txt): ");
          scanf("%s",filename);
          ret=save_database(table,file_list,filename);
          if(ret==SUCCESS)
            printf("Database saved successfully.\n");
          else if(ret==FILE_REPEATED)
            printf("ERROR: File already exists! Use another name.\n");
          else if(ret==FILE_NOT_FOUND)
            printf("ERROR: Invalid file name!\n");
          else
            printf("ERROR: Failed to save database!\n");
          break;

        case 6:
          printf("Inverted Search is signing off...\n");
          free_database(table);
          free_filelist(&file_list);
          return SUCCESS;

        default:
          printf("ERROR: Invalid choice! Try again.\n");
    }
    printf("Do you want to continue? (y/n): ");
    scanf(" %c",&option);
}while(option=='y'||option=='Y');
printf("Program closed...Thank you for using Inverted Search!\n");

//cleanup
free_database(table);
free_filelist(&file_list);

return SUCCESS;
}


