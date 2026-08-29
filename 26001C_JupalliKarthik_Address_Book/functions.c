#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "colors.h"
#include <unistd.h>
// Validate name (only letters + max one space, no spaces at starting or at last)
int my_alpha(char *s)
{
    if(strlen(s)<4){
        printf(CYAN"Name must contain atleast 4 characters.\n"RESET);
        return 0;
    }
    if (s == NULL || s[0] == ' ' || s[0] == '\0')
        return 0;

    int spacect = 0;

    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == ' ')
        {
            spacect++;
            if (spacect > 1 || s[i + 1] == '\0') // at last checking spaces
                return 0;
        }
        else if (!isalpha(s[i]))
        {
            printf(CYAN"Name should contain only alphabets.\n"RESET);
            return 0;
        }
    }
    return 1;
}


// To check whether  all the digits or not
int my_dig( char *s)
{
    if (s == NULL || *s == '\0')
        return 0;

    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!isdigit(s[i]))
            return 0;
    }
    return 1;
}


// Validate phone
int valid_phone( char *phone)
{
    if (!phone) return 0;
    if (!my_dig(phone)){
        printf(CYAN"Symbols are not allowed in phone number.\n"RESET);
        return 0;
    }
    if (phone[0] < '6' || phone[0] > '9'){
        printf(CYAN"First digit must be between 6 and 9.\n"RESET);
        return 0;
    }
    if (strlen(phone) != 10){
        printf(CYAN"Phone number must contain exactly 10 digits.\n"RESET);
        return 0;
    }

    return 1;
}


// Check duplicate phone
int is_duplicate_phone(AddressBook *addressBook,  char *phone, int index)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (i == index) continue;
        if (strcmp(addressBook->contacts[i].phone, phone) == 0){
            printf(CYAN"The entered phone number is duplicate.\n"RESET);
            return 1;
        }
    }
    return 0;
}

//  Email validation
int validate_email(char *str)
{
    int pos = 0;
    int atCount = 0;
    int dotCount = 0;

    if(str[0] == '\0') {
        printf(CYAN "Email should not be empty.\n" RESET);
        return 0;
    }

    if(str[0] == '@' || str[0] == '.') {
        printf(CYAN "Email should not start with '@' or '.'.\n" RESET);
        return 0;
    }

    while(str[pos] != '\0')
    {
        if(str[pos] == ' ') {
            printf(CYAN "Email should not contain spaces.\n" RESET);
            return 0;
        }

        // Reject uppercase letters
        if(isupper(str[pos])) {
            printf(CYAN "Email should not contain uppercase letters.\n" RESET);
            return 0;
        }

        if(str[pos] == '@')
        {
            atCount++;
            if(pos == 0 || str[pos + 1] == '\0') {
                printf(CYAN "'@' cannot be at the beginning or end.\n" RESET);
                return 0;
            }
        }

        if(str[pos] == '.')
        {
            dotCount++;

            if(pos == 0 || str[pos + 1] == '\0') {
                printf(CYAN "'.' cannot be at the beginning or end.\n" RESET);
                return 0;
            }

            if(str[pos + 1] == '.') {
                printf(CYAN "Email should not contain consecutive dots '..'.\n" RESET);
                return 0;
            }
        }

        if(!(islower(str[pos]) || isdigit(str[pos]) ||
             str[pos] == '@' || str[pos] == '.' ||
             str[pos] == '_' || str[pos] == '-'))
        {
            printf(CYAN "Invalid character '%c' in email.\n" RESET, str[pos]);
            return 0;
        }

        pos++;
    }

    if(atCount != 1) {
        printf(CYAN "Email must contain exactly one '@'.\n" RESET);
        return 0;
    }

    if(dotCount < 1) {
        printf(CYAN "Email must contain at least one '.'.\n" RESET);
        return 0;
    }

    char *atPosition = strchr(str, '@');
    char *lastDot = strrchr(str, '.');

    if(lastDot < atPosition) {
        printf(CYAN "'.' must come after '@'.\n" RESET);
        return 0;
    }

    if(*(atPosition + 1) == '.') {
        printf(CYAN "'.' cannot be immediately after '@'.\n" RESET);
        return 0;
    }

    //Domain length check
    int length_between = lastDot - atPosition - 1;
    if(length_between < 5) {
        printf(CYAN "Domain name between '@' and '.' must be at least 5 characters.\n" RESET);
        return 0;
    }

    //Domain must contain only lowercase alphabets
    char *domainStart = atPosition + 1;
    for(int i = 0; i < length_between; i++)
    {
        if(!islower(domainStart[i])) {
            printf(CYAN "Domain name should contain only lowercase alphabets.\n" RESET);
            return 0;
        }
    }

    if(strcmp(lastDot, ".com") != 0) {
        printf(CYAN "Email must end with '.com'.\n" RESET);
        return 0;
    }
    return 1;
}
//  Checking for duplicate email
int is_duplicate_email(AddressBook *addressBook, char *email, int skipIndex)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (i == skipIndex) continue;
        if (addressBook->contacts[i].email &&strcmp(addressBook->contacts[i].email, email) == 0){
            printf(CYAN"The entered email is duplicate.\n"RESET);
            return 1;
        }

    }
    return 0;
}
//  To sort contacts by name
void sort_structure(AddressBook *addressBook)
{
    if (!addressBook) return;
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        int swapped = 0;//this is to optimize the code
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            //using bubble sort
            if (strcasecmp(addressBook->contacts[j].name,addressBook->contacts[j + 1].name) > 0)
            {
                Contact temp = addressBook->contacts[j];//using contact temp variable
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped==0) break;//if swapped =0 then all the contacts are already sorted.
    }
}

//  Search contact
void search_contact(char *key, AddressBook *addressBook, int type)//1.arg ->user input,2.arg->including address book,3.arg->type
//if user enters 1->name 2->moblie number 3->email
{
    int flag = 0;
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        char *check = NULL;//to store the different fields in same name
        if (type == 1)
            check = addressBook->contacts[i].name;
        else if (type == 2)
            check = addressBook->contacts[i].phone;
        else
            check = addressBook->contacts[i].email;
        if (strcasestr(check, key))//checking with strcasestr
        {
            if (flag==0)
            {
                printf(CYAN "-------------------------------------------------------------------------------------------------\n" RESET);
                printf(YELLOW "| %-4s | %-25s | %-15s | %-40s |\n" RESET,"ID", "Name", "Phone No", "Email ID");
                printf(CYAN "-------------------------------------------------------------------------------------------------\n" RESET);
                flag = 1;
            }
            printf(GREEN "| %-4d | %-25s | %-15s | %-40s |\n" RESET,i + 1,addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,addressBook->contacts[i].email);
            printf(CYAN "-------------------------------------------------------------------------------------------------\n" RESET);
        }
    }
    if (flag==0)
        printf(RED "Contact Not Found.\n" RESET);
}
//  Safer input search
int index_list[100];//declaring an array to store the indexes with size of 100 because the max contact count is 100
int search_and_select(AddressBook *addressBook)
{
    char ch[50];
    int count = 0;
    printf("Enter name/phone/email to search: ");//all fields are strings so taking only one input
    scanf(" %s",ch);
    getchar();
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((addressBook->contacts[i].name &&
             strcasestr(addressBook->contacts[i].name, ch)) ||
            (addressBook->contacts[i].phone &&
             strcasestr(addressBook->contacts[i].phone, ch)) ||
            (addressBook->contacts[i].email &&
             strcasestr(addressBook->contacts[i].email, ch)))//checking all the conditions with strcasestr if any one case is true then it list the contacts
        {
            if(count==0){
                printf(CYAN "\n-------------------------------------------------------------------------------------------------\n" RESET);
                printf(YELLOW "| %-4s | %-25s | %-15s | %-40s |\n" RESET,"ID", "Name", "Phone No", "Email ID");
                printf(CYAN "-------------------------------------------------------------------------------------------------\n" RESET);

            }
            printf(GREEN "| %-4d | %-25s | %-15s | %-40s |\n" RESET,count + 1,addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,addressBook->contacts[i].email);
            printf(CYAN "-------------------------------------------------------------------------------------------------\n" RESET);
            if (count < 100)
                index_list[count++] = i;//after listing storing that index values in index_list at the count position
        }
    }

    if (count == 0)
        printf(RED "No matching contacts found!\n" RESET);

    return count;
}

// Checking whether the address book is empty or not
int is_empty(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf(RED "\n📂 Address Book is Empty!\n" RESET);
        printf(YELLOW "👉 Please add contacts first.\n" RESET);
        return 1;
    }
    return 0;
}

// Loading animation
void welcome_and_loading()//For styling
{
    printf(CYAN BOLD "\n");
    printf("=============================================================================\n");
    printf("                    CONTACT MANAGEMENT SYSTEM\n");
    printf("=============================================================================\n");
    printf(RESET);

    printf(YELLOW "Starting Application...\n\n" RESET);

    printf(BOLD GREEN "[Loading");
    fflush(stdout);

    for (int i = 0; i <= 100; i++)
    {
        printf("\r" BOLD GREEN "[Loading....... %-3d%%]" RESET, i);
        fflush(stdout);
        usleep(30000); // faster
    }

    printf(GREEN " ✔\n" RESET);
}
