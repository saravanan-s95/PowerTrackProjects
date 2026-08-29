/*
Name : Jupalli Karthik
Date : 22-04-2026
Title : Address book
Description : The Address Book Project is used to manage contact details by allowing users to 
              add, view, search, edit, and delete contacts. It also saves the data in a file 
              so that information is not lost when the program is closed. 
*/

#include <stdio.h>
#include <unistd.h>
#include "contact.h"
#include "colors.h"
#include "file.h"

int main()
{
    welcome_and_loading();
    AddressBook addressBook;
    initialize(&addressBook);//The intialize the value of contactcount
    loadContactsFromFile(&addressBook);//To load contacts from file to the struct
    //Printing menu
    int choice;//declaring a variable to take input from user
        do 
        {
            printf(BOLD_MAGENTA "\n📒 ADDRESS BOOK MENU 📒\n" RESET);
            printf(CYAN "-----------------------------------------\n" RESET);
            printf(YELLOW "1. Create contact\n" );
            printf("2. Search contact\n");
            printf("3. Edit contact\n");
            printf("4. Delete contact\n");
            printf("5. List all contacts\n");
            printf("6. Favorite contacts\n");
            printf("7. Save and Exit\n" RESET);
            printf(CYAN "-----------------------------------------\n" RESET);
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) 
            {
                case 1:
                    createContact(&addressBook);//To create contact 
                    break;

                case 2:
                    searchContact(&addressBook);//To search contact 
                    break;

                case 3:
                    editContact(&addressBook);//To edit contact 
                    break;

                case 4:
                    deleteContact(&addressBook);//To delete contact 
                    break;

                case 5:
                    listContacts(&addressBook);//To list contacts 
                    break;

                case 6:
                    favoritecontacts(&addressBook);//To list favorite contacts
                    break;

                case 7:
                    printf(GREEN "\n💾 Saving and Exiting...\n" RESET);
                    saveContactsToFile(&addressBook);//To save contacts to the file
                    break;

                default:
                    printf(RED "❌ Invalid choice. Try again.\n" RESET);
            }

        } while (choice != 7);

    return 0;
}