#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
//#include "populate.c"


void load_contacts_from_file(AddressBook *addressBook)
{
    // open file in read mode
    FILE *fptr = fopen("contacts.csv", "r");

    if (fptr == NULL)
    {
        // file not present first time
        addressBook->contactCount = 0;
        return;
    }

    // initialize count
    addressBook->contactCount = 0;

    // read contact count
    fscanf(fptr, "%d\n", &addressBook->contactCount);

    // read contacts from file
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%s\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    fclose(fptr);

    printf("Contacts loaded successfully from contacts.csv\n");
}
void listContacts(AddressBook *addressBook)
{
	//list all the contacts..
    printf("-------------------------------------------------------\n");
    printf("NAME        PHONE NUM           EMAIL \n");
    printf("--------------------------------------------------------\n");

    for(int i=0;i<addressBook -> contactCount ; i++)
    {
        printf("%s      %s      %s\n",addressBook ->contacts[i].name,addressBook ->contacts[i].phone,addressBook ->contacts[i].email);
    }


}


void createContact(AddressBook *addressBook)
{
    /* Define the logic to create a Contacts */
    //------NAME VALIDATION -----
int isvalidname(char name[])
{
    int i=0;
    while(name[i]!= '\0')
    {
        if(!((name[i] >='A' && name[i]<='Z') ||(name[i]>='a' && name[i]<='z')||name[i] ==' '))
        {
            return 0;
        }
        i++;
    }
    return 1;
}


// ----------PHONE VALIDATION----------
int isvalidphone(char phone[],AddressBook *addressBook)
{
    int i=0;
    //digits check
    while(phone[i]!='\0')
    {
        if(phone[i]<'0'|| phone[i]>'9')
        return 0;
        i++;
    }
    //length check
    if(i!=10)
    return 0;

    //uniqueness check
    for(int j=0;j<addressBook->contactCount;j++)
    {
        if(strcmp(phone,addressBook->contacts[j].phone)==0)
        return 0;
    }
    return 1;
}


//------EMAIL VALIDATION------
int isvalidemail(char email[],AddressBook *addressBook)
{
   int i, atIndex = -1;
    int len = strlen(email);

    // find '@'
    for(i = 0; i < len; i++)
    {
        if(email[i] == '@')
        {
            if(atIndex != -1)
                return 0; // multiple @
            atIndex = i;
        }
    }

    if(atIndex <= 0)
        return 0;

    // check ".com"
    if(len < 5 || strcmp(&email[len - 4], ".com") != 0)
        return 0;

    // check chars between @ and .com
    if((len - 4) - atIndex <= 1)
        return 0;

    // uniqueness
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(email, addressBook->contacts[i].email) == 0)
            return 0;
    }

    return 1;
}
  char name[25], phone[11], email[30];

    // STEP 1: NAME
    while(1)
    {
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        if(isvalidname(name))
            break;
        else
            printf("Invalid name! Use alphabets only.\n");
    }

    // STEP 2: PHONE
    while(1)
    {
        printf("Enter Phone: ");
        scanf("%s", phone);

        if(isvalidphone(phone, addressBook))
            break;
        else
            printf("Invalid phone! Must be 10 digits & unique.\n");
    }

    // STEP 3: EMAIL
    while(1)
    {
        printf("Enter Email: ");
        scanf("%s", email);

        if(isvalidemail(email, addressBook))
            break;
        else
            printf("Invalid email! Try again.\n");
    }

    // SAVE CONTACT
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);

    addressBook->contactCount++;

    printf("\n Contact saved successfully!\n\n");
}



void searchContact(AddressBook *addressBook)
{
    /* Define the logic to search a Contacts */
     int choice;
    char name[25], phone[11], email[30];
    int found = 0;

    printf("\nSearch Menu:\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone Number\n");
    printf("3. Search by Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            printf("Enter Name: ");
            scanf(" %[^\n]", name);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(name, addressBook->contacts[i].name) == 0)
                {
                    printf("\nContact Found:\n");
                    printf("Name  : %s\n", addressBook->contacts[i].name);
                    printf("Phone : %s\n", addressBook->contacts[i].phone);
                    printf("Email : %s\n", addressBook->contacts[i].email);
                    found = 1;
                }
            }

            if(!found)
                printf("Contact not found!\n");
            break;

        case 2:
            printf("Enter Phone: ");
            scanf("%s", phone);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(phone, addressBook->contacts[i].phone) == 0)
                {
                    printf("\nContact Found:\n");
                    printf("Name  : %s\n", addressBook->contacts[i].name);
                    printf("Phone : %s\n", addressBook->contacts[i].phone);
                    printf("Email : %s\n", addressBook->contacts[i].email);
                    found = 1;
                    break; // phone is unique → stop after found
                }
            }

            if(!found)
                printf("Contact not found!\n");
            break;

        case 3:
            printf("Enter Email: ");
            scanf("%s", email);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(email, addressBook->contacts[i].email) == 0)
                {
                    printf("\nContact Found:\n");
                    printf("Name  : %s\n", addressBook->contacts[i].name);
                    printf("Phone : %s\n", addressBook->contacts[i].phone);
                    printf("Email : %s\n", addressBook->contacts[i].email);
                    found = 1;
                    break; // email is unique
                }
            }

            if(!found)
                printf("Contact not found!\n");
            break;

        default:
            printf("Invalid choice!\n");
    }
} 

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
    int choice;
    char name[25], phone[11], email[30];
    int foundIndex = -1;

    printf("\nEdit Menu:\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone Number\n");
    printf("3. Search by Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // 🔍 SEARCH PART
    switch(choice)
    {
        case 1:
            printf("Enter Name: ");
            scanf(" %[^\n]", name);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(name, addressBook->contacts[i].name) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;

        case 2:
            printf("Enter Phone: ");
            scanf("%s", phone);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(phone, addressBook->contacts[i].phone) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;

        case 3:
            printf("Enter Email: ");
            scanf("%s", email);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(email, addressBook->contacts[i].email) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    if(foundIndex == -1)
    {
        printf("Contact not found!\n");
        return;
    }

    // ✅ DISPLAY FOUND CONTACT
    printf("\nContact Found:\n");
    printf("Name  : %s\n", addressBook->contacts[foundIndex].name);
    printf("Phone : %s\n", addressBook->contacts[foundIndex].phone);
    printf("Email : %s\n", addressBook->contacts[foundIndex].email);

    // ✏️ EDIT MENU
    int editChoice;
    printf("\nEdit Options:\n");
    printf("1. Edit Name\n");
    printf("2. Edit Phone\n");
    printf("3. Edit Email\n");
    printf("Enter your choice: ");
    scanf("%d", &editChoice);

    // 🔹 VALIDATION FUNCTIONS (reuse your logic)
    int isvalidname(char name[])
    {
        for(int i = 0; name[i] != '\0'; i++)
        {
            if(!((name[i]>='A' && name[i]<='Z') ||
                 (name[i]>='a' && name[i]<='z') ||
                 name[i]==' '))
                return 0;
        }
        return 1;
    }

    int isvalidphone(char phone[], AddressBook *addressBook)
    {
        int i;
        for(i = 0; phone[i] != '\0'; i++)
        {
            if(phone[i] < '0' || phone[i] > '9')
                return 0;
        }

        if(i != 10)
            return 0;

        for(int j = 0; j < addressBook->contactCount; j++)
        {
            if(j != foundIndex && strcmp(phone, addressBook->contacts[j].phone) == 0)
                return 0;
        }

        return 1;
    }

    int isvalidemail(char email[], AddressBook *addressBook)
    {
        int at = -1, len = strlen(email);

        for(int i = 0; i < len; i++)
        {
            if(email[i] == '@')
            {
                if(at != -1) return 0;
                at = i;
            }
        }

        if(at <= 0 || strcmp(&email[len-4], ".com") != 0)
            return 0;

        for(int i = 0; i < addressBook->contactCount; i++)
        {
            if(i != foundIndex && strcmp(email, addressBook->contacts[i].email) == 0)
                return 0;
        }

        return 1;
    }

    // 🔄 EDIT LOGIC
    switch(editChoice)
    {
        case 1:
            while(1)
            {
                printf("Enter New Name: ");
                scanf(" %[^\n]", name);

                if(isvalidname(name))
                {
                    strcpy(addressBook->contacts[foundIndex].name, name);
                    break;
                }
                else
                    printf("Invalid name! Try again.\n");
            }
            break;

        case 2:
            while(1)
            {
                printf("Enter New Phone: ");
                scanf("%s", phone);

                if(isvalidphone(phone, addressBook))
                {
                    strcpy(addressBook->contacts[foundIndex].phone, phone);
                    break;
                }
                else
                    printf("Invalid phone! Try again.\n");
            }
            break;

        case 3:
            while(1)
            {
                printf("Enter New Email: ");
                scanf("%s", email);

                if(isvalidemail(email, addressBook))
                {
                    strcpy(addressBook->contacts[foundIndex].email, email);
                    break;
                }
                else
                    printf("Invalid email! Try again.\n");
            }
            break;

        default:
            printf("Invalid edit option!\n");
            return;
    }

    printf("\nContact updated successfully!\n");
}

void deleteContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
 int choice;
    char name[25], phone[11], email[30];
    int index[100];   // to store matched indices
    int count = 0;

    printf("\nDelete Menu:\n");
    printf("1. Delete by Name\n");
    printf("2. Delete by Phone Number\n");
    printf("3. Delete by Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        // 🔹 DELETE BY NAME (multiple matches possible)
        case 1:
            printf("Enter Name: ");
            scanf(" %[^\n]", name);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(name, addressBook->contacts[i].name) == 0)
                {
                    printf("%d) %s | %s | %s\n", count + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);

                    index[count++] = i;
                }
            }

            if(count == 0)
            {
                printf("Contact not found!\n");
                return;
            }

            int serial;
            printf("Enter serial number to delete: ");
            scanf("%d", &serial);

            if(serial < 1 || serial > count)
            {
                printf("Invalid selection!\n");
                return;
            }

            int delIndex = index[serial - 1];

            // 🔥 SHIFTING LOGIC
            for(int i = delIndex; i < addressBook->contactCount - 1; i++)
            {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            }

            addressBook->contactCount--;
            printf("Contact deleted successfully!\n");
            break;

        // 🔹 DELETE BY PHONE (unique)
        case 2:
            printf("Enter Phone: ");
            scanf("%s", phone);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(phone, addressBook->contacts[i].phone) == 0)
                {
                    for(int j = i; j < addressBook->contactCount - 1; j++)
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }

                    addressBook->contactCount--;
                    printf("Contact deleted successfully!\n");
                    return;
                }
            }

            printf("Contact not found!\n");
            break;

        // 🔹 DELETE BY EMAIL (unique)
        case 3:
            printf("Enter Email: ");
            scanf("%s", email);

            for(int i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(email, addressBook->contacts[i].email) == 0)
                {
                    for(int j = i; j < addressBook->contactCount - 1; j++)
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }

                    addressBook->contactCount--;
                    printf("Contact deleted successfully!\n");
                    return;
                }
            }

            printf("Contact not found!\n");
            break;

        default:
            printf("Invalid choice!\n");
    }
}

void saveContactsToFile(AddressBook *addressBook)
{
    printf("Inside save function\n");

    FILE *fptr = fopen("contacts.csv", "w");

    if (fptr == NULL)
    {
        printf("File open failed\n");
        return;
    }

    fprintf(fptr, "%d\n", addressBook->contactCount);

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf("Saved successfully\n");
}