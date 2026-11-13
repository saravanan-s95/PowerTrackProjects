#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include <ctype.h>

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
    int choice;

    printf("Choose sorting criteria:\n1. Sort by Name\n2. Sort by Phone Number\n3. Sort by Email\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:

            for (int i = 0; i <= addressBook->contactCount - 1; i++)
            {
                for (int j = i + 1; j < addressBook->contactCount; j++)
                {
                    if (strcmp(addressBook->contacts[i].name,
                               addressBook->contacts[j].name) > 0)
                    {
                        Contact temp = addressBook->contacts[i];
                        addressBook->contacts[i] = addressBook->contacts[j];
                        addressBook->contacts[j] = temp;
                    }
                }
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                printf("Contact %d: Name: %s, Phone: %s, Email: %s\n",
                       i + 1,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);
            }
            break;

        case 2:

            for (int i = 0; i < addressBook->contactCount - 1; i++)
            {
                for (int j = i + 1; j < addressBook->contactCount; j++)
                {
                    if (strcmp(addressBook->contacts[i].phone,
                               addressBook->contacts[j].phone) > 0)
                    {
                        Contact temp = addressBook->contacts[i];
                        addressBook->contacts[i] = addressBook->contacts[j];
                        addressBook->contacts[j] = temp;
                    }
                }
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                printf("Contact %d: Name: %s, Phone: %s, Email: %s\n",
                       i + 1,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);
            }
            break;

        case 3:

            for (int i = 0; i < addressBook->contactCount - 1; i++)
            {
                for (int j = i + 1; j < addressBook->contactCount; j++)
                {
                    if (strcmp(addressBook->contacts[i].email,
                               addressBook->contacts[j].email) > 0)
                    {
                        Contact temp = addressBook->contacts[i];
                        addressBook->contacts[i] = addressBook->contacts[j];
                        addressBook->contacts[j] = temp;
                    }
                }
            }

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                printf("Contact %d: Name: %s, Phone: %s, Email: %s\n",
                       i + 1,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);
            }
            break;
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
     loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

/* validate Name */
int valid_name(char *name)
{
    int i = 0;

    while (name[i] != '\0')
    {
        if ((name[i] >= 'A' && name[i] <= 'Z') ||
            (name[i] >= 'a' && name[i] <= 'z') ||
            (name[i] == ' '))
        {
            i++;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}
/* validate phone number */

int valid_phone(AddressBook *addressBook, char *phone)
{
    int i;

    if(strlen(phone) != 10)
        return 0;

    for(i = 0; phone[i] != '\0'; i++)
    {
        if(!isdigit(phone[i]))
            return 0;
    }

    for(i = 0; i < 8; i++)
    {
        if(phone[i] == phone[i + 1] &&
           phone[i] == phone[i + 2])
        {
            return 0;
        }
    }

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(phone, addressBook->contacts[i].phone) == 0)
        {
            return 0;
        }
    }

    return 1;
}

/* validate email */

int valid_email(AddressBook *addressBook, char *email)
{
    int i;
    int at_count = 0;
    int len = strlen(email);

    /* Check uppercase letters and count '@' */
    for(i = 0; email[i] != '\0'; i++)
    {
        if(email[i] >= 'A' && email[i] <= 'Z')
        {
            return 0;
        }

        if(email[i] == '@')
        {
            at_count++;
        }
    }

    /* Exactly one '@' */
    if(at_count != 1)
    {
        return 0;
    }

    /* Must end with .com */
    if(len < 4 || strcmp(email + len - 4, ".com") != 0)
    {
        return 0;
    }

    /* Check duplicate email */
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(email, addressBook->contacts[i].email) == 0)
        {
            return 0;
        }
    }

    return 1;
}




void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    char name[50];

do
{
    printf("Enter the name : ");
    scanf(" %[^\n]", name);

    if (valid_name(name) == 0)
    {
        printf("Invalid name. Enter again.\n");
    }

} while (valid_name(name) == 0);

strcpy(addressBook->contacts[addressBook->contactCount].name, name);

    char phone[20];

do
{
    printf("Enter the phone no : ");
    scanf("%s", phone);

    if (valid_phone(addressBook, phone) == 0)
    {
        printf("Invalid phone number. Enter again.\n");
    }

} while (valid_phone(addressBook, phone) == 0);

strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);

    char email[50];

do
{
    printf("Enter the email id : ");
    scanf("%s", email);

    if(valid_email(addressBook, email) == 0)
    {
        printf("Invalid email. Enter again.\n");
    }

} while(valid_email(addressBook, email) == 0);

strcpy(addressBook->contacts[addressBook->contactCount].email, email);

    addressBook->contactCount++;
}

int searchContact(AddressBook *addressBook)
{
    /* Define the logic for search */
    int choice = 0;

    printf("1. Search by name.\n2. Search by phone no.\n3. Search by email.\nEnter the choice : ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
        {
            char name[50];

            printf("Enter the name : ");
            scanf(" %[^\n]", name);

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(name, addressBook->contacts[i].name) == 0)
                {
                    printf("Name: %s, Phone: %s, Email: %s\n",
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    return i;
                }
            }

            break;
        }

        case 2:
        {
            char phone[20];

            printf("Enter the phone no : ");
            scanf(" %s", phone);

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(phone, addressBook->contacts[i].phone) == 0)
                {
                    printf("Name: %s, Phone: %s, Email: %s\n",
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    return i;
                }
            }

            break;
        }

        case 3:
        {
            char email[50];

            printf("Enter the email id : ");
            scanf(" %s", email);

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(email, addressBook->contacts[i].email) == 0)
                {
                    printf("Name: %s, Phone: %s, Email: %s\n",
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    return i;
                }
            }

            break;
        }
        default:
            break; 
    }
    printf("Contact not found\n");
    return -1;
}

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */

    int ret = searchContact(addressBook);
    if (ret == -1)
    {
        return;
    }
    printf("1. Edit name\n2. Edit phone.\n3. Edit mail id.\nEnter the choice : ");

    int choice = 0;
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
        {
            char name[50];

            do
            {
                printf("Enter the name : ");
                scanf(" %[^\n]", name);

                if(valid_name(name) == 0)
                {
        
                    printf("Invalid name. Enter again.\n");
                }
            }while(valid_name(name) == 0);

            strcpy(addressBook->contacts[ret].name, name);
            break;
        }
        case 2:
        {
            char phone[20];

            do
            {
                printf("Enter the phone no : ");
                scanf("%s", phone);

                if(valid_phone(addressBook, phone) == 0)
                 {
                     printf("Invalid phone number. Enter again.\n");
                 }

            }while(valid_phone(addressBook, phone) == 0);

            strcpy(addressBook->contacts[ret].phone, phone);
            break;
        }
        case 3:
        {
        char email[50];

        do
        {
              printf("Enter the email id : ");
              scanf("%s", email);

            if(valid_email(addressBook, email) == 0)
            {
            printf("Invalid email. Enter again.\n");
            }

        }while(valid_email(addressBook, email) == 0);

         strcpy(addressBook->contacts[ret].email, email);
         break;
    }

        default:
          break;
        
}

    printf("Name: %s, Phone: %s, Email: %s\n",
           addressBook->contacts[ret].name,
           addressBook->contacts[ret].phone,
           addressBook->contacts[ret].email);
}

void deleteContact(AddressBook *addressBook)
{
    /* Define the logic for deletecontact */

    int ret = searchContact(addressBook);
    if (ret == -1)
    {
        return;
    }
    for (int i = ret; i < addressBook->contactCount - 1; i++)
    {
    addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--;
}
