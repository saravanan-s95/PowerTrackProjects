#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include<ctype.h>
#include "file.h"
#include "populate.h"
#define _GNU_SOURCE
int arr[100],size=0;
int validate_name(char *temp)
{
    int i=0;
    while(temp[i]!='\0')
    {
        if((temp[i]>='a' && temp[i]<='z') || (temp[i]>='A' && temp[i]<='Z') || (temp[i]>='0' && temp[i]<='9') || (temp[i]==' '))
        {
            i++;
        }
        else
        {
            printf("\tEntered name is invalid\n");
            return 1;
        }
    }
    return 0;
}

int validate_mobilenumber(char *temp)
{
    int i=0;
    while(temp[i]!='\0')
    {
        if(temp[i]>='0' && temp[i]<='9')
        {
            i++;
        }
        else
        {
            printf("\tEntered phone number is invalid\n");
            return 1;
        }
    }
    if(i!=10)
    {
        printf("\tEntered mobile number is invalid\n");
        return 1;
    }
    return 0;
}

int validate_email(char *temp)
{
      if(strstr(temp,".com")==NULL)
      {
        printf("\tEntered email is invalid \".com\" is misssing\n");
        return 1;
      }
      if(strchr(temp,'@')==NULL)
      {
        printf("\tEntered email is invalid \"@\" is misssing\n");
        return 1;
      }
    return 0;
}

int unique_phone(AddressBook *addressBook,char *temp)
{
    int i;
    for(i=0;i<addressBook->contactCount;i++){
        if(strcmp(addressBook->contacts[i].phone,temp)==0){
            printf("\tEnter a unique phone number\n");
            return 1;
        }
    }
    return 0;
}

int unique_email(AddressBook *addressBook,char *temp)
{
    int i;
    for(i=0;i<addressBook->contactCount;i++){
        if(strcmp(addressBook->contacts[i].email,temp)==0)
        {
            printf("\tEnter a unique email\n");
            return 1;
        }
    }
    return 0;
}

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
     Contact temp;
     printf("\t1. Sort by name\n");
     printf("\t2. Sort by phone\n");
     printf("\t3. Sort by email\n");
     printf("\tEnter your choice: ");
     int sortChoice;
     scanf("%d", &sortChoice);
     switch(sortChoice)
     {
       case 1:
        printf("\tSORTING BY NAME\n");
        for(int i=0;i<addressBook->contactCount-1;i++)
        {
            for(int j=0;j<addressBook->contactCount-i-1;j++)
            {
                if(strcmp(addressBook->contacts[j].name,addressBook->contacts[j+1].name)>0)
                {
                    temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }
        }
        printf("\t------------------------------------------------------------------------------------------\n");
        printf("\t|Index\t\tName                Phone number\t\tEmail                    |\n");
        printf("\t------------------------------------------------------------------------------------------\n");
        for(int i=0;i<addressBook->contactCount;i++)
        {
            printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
         break;
       case 2: 
        printf("\tSORTING BY PHONE NUMBER\n");
        for(int i=0;i<addressBook->contactCount-1;i++)
        {
            for(int j=0;j<addressBook->contactCount-i-1;j++)
            {
                if(strcmp(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone)>0)
                {
                    temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }
        }
        printf("\t------------------------------------------------------------------------------------------\n");
        printf("\t|Index\t\tName                Phone number\t\tEmail                    |\n");
        printf("\t------------------------------------------------------------------------------------------\n");
        for(int i=0;i<addressBook->contactCount;i++)
        {
            printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);

        }
        break;
       case 3:
        printf("\tSORTING BY EMAIL\n");
        for(int i=0;i<addressBook->contactCount-1;i++)
        {
            for(int j=0;j<addressBook->contactCount-i-1;j++)
            {
                if(strcmp(addressBook->contacts[j].email,addressBook->contacts[j+1].email)>0)
                {
                    temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }
        }
        printf("\t------------------------------------------------------------------------------------------\n");
        printf("\t|Index\t\tName                Phone number\t\tEmail                    |\n");
        printf("\t------------------------------------------------------------------------------------------\n");
        for(int i=0;i<addressBook->contactCount;i++)
        {
            printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
        break;
       default:
             printf("\n\t*** INVALID SORT CHOICE CRITERIA ***\n");
             break;
     }   
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    char temp[50];
    printf("\tMENU CREATE CONTACT\n");
    do{
      printf("\tEnter the name : ");
      scanf(" %[^\n]",temp);
    }while(validate_name(temp));
    strcpy(addressBook->contacts[addressBook->contactCount].name,temp);
    do{
        printf("\tEnter the phone number : ");
        scanf(" %[^\n]",temp);
    }while(validate_mobilenumber(temp) | unique_phone(addressBook,temp));
    strcpy(addressBook->contacts[addressBook->contactCount].phone,temp);
    do{
       printf("\tEnter the email : ");
       scanf(" %[^\n]",temp); 
    }while(validate_email(temp) | unique_email(addressBook,temp));
    strcpy(addressBook->contacts[addressBook->contactCount].email,temp); 
    printf("\tContact created sucessfully");
    addressBook->contactCount++;
}

int searchContact(AddressBook *addressBook)
{
    char temp[20];
    int count =0;
    /* Define the logic for search */
    int opt;
    printf("\tMENU SEARCH CONTACT\n");
    printf("\tPress 1 for search by name\n \tPress 2 for search by phone number \n \tpress 3 for search by email\n\t:");
    scanf("%d",&opt);
    switch(opt){
        case 1:
           printf("\tSearching by name\n");
           printf("\tEnter the name \n\t:");
           scanf(" %[^\n]",temp);
           for(int i=0;i<addressBook->contactCount;i++)
           {
              if(strcasestr(addressBook->contacts[i].name,temp)!=NULL){
                arr[size]=i+1;
                size++;
                count++;
                 printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); 
              } 
           }
           if(!count){ 
            printf("\tContact not found\n");
            return 0;
        }
           
           break;
        case 2:
           printf("\tSearching by phone number\n");
           printf("\tEnter the phone number \n\t:");
           scanf(" %[^\n]",temp);
           for(int i=0;i<addressBook->contactCount;i++)
           {
              if(strcasestr(addressBook->contacts[i].phone,temp)!=NULL){
                arr[size]=i+1;
                size++;
                count++;
                 printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
      
              }
           }
           if(!count){
             printf("\tContact not found\n");
             return 0;
            }
           break;
        case 3:
           printf("\tSearching by email\n");
           printf("\tEnter the email \n\t:");
           scanf(" %[^\n]",temp);
           for(int i=0;i<addressBook->contactCount;i++)
           {
              if(strcasestr(addressBook->contacts[i].email,temp)!=NULL){
                 arr[size]=i+1;
                 size++;
                 count++;
                 printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email); 
              }
           }
           if(!count){
             printf("\tContact not found\n");
             return 0;
            }
           break;
        default:
            printf("\tINVALID INPUT\n");
            return 0;
            break;
    }
    return 1;
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    char temp[20];
    int index,flag=1,opt;
    printf("\tMENU EDIT CONTACT\n");
    if(searchContact(addressBook)){
      label:
        printf("\tEnter the index you want to edit\n\t:");
        scanf("%d",&index);
        for(int i=0;i<size;i++){
            if(index==arr[i]) flag=0;
        }
        if(flag){
            printf("\t You have entered an invalid index\n");
            goto label;
        }
      edit:
        printf("\tPress 1 to edit name\n\tPress 2 to edit phone number\n\tPress 3 to edit email\n\t:");
        scanf("%d",&opt);
        switch(opt)
        {
             case 1:
                   do
                   {
                    printf("\tEnter the name you want to edit\n\t:");
                    scanf(" %[^\n]",temp);
                   }while(validate_name(temp));
                   strcpy(addressBook->contacts[index-1].name,temp);
                   printf("\tNAME EDITED SUCCESSFULLY\n");
                   break;
            case 2:
                do
                {
                   printf("\tEnter the phone number you want to edit\n\t:");
                   scanf(" %[^\n]",temp);
                   }while(validate_mobilenumber(temp) | unique_phone(addressBook,temp));
                   strcpy(addressBook->contacts[index-1].phone,temp);
                   printf("\tPHONE NUMBER EDITED SUCCESSFULLY\n");
                   break;
            case 3:
                do
                {
                   printf("\tEnter the email you want to edit\n\t:");
                   scanf(" %[^\n]",temp);
                   }while(validate_email(temp) | unique_email(addressBook,temp));
                   strcpy(addressBook->contacts[index-1].email,temp);
                   printf("\tEMAIL EDITED SUCCESSFULLY\n");
                   break;
            default: 
                 printf("\tEnter the correct option\n");
                 goto edit;
                 break;     
        }
         
    }
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    int index,flag=1;
    char opt;
    printf("\tMENU DELETE CONTACT\n");
    if(searchContact(addressBook))
    {
        start:
        printf("\tEnter the index you want to delete.\n\t:");
        scanf("%d",&index);
        for(int i=0;i<size;i++){
            if(index==arr[i])
            {
                flag=0;
            }
        }
        if(flag)
        {
           printf("\tINVALID INDEX");
           goto start;
        }
        label1:
        printf("\tContact delete confirmation\n");
        printf("\t%-5d\t\t%-20s%-10s  \t\t%-40s\n",index,addressBook->contacts[index-1].name,addressBook->contacts[index-1].phone,addressBook->contacts[index-1].email);
        printf("\tpress 'Y or 'y' for yes\n\tpress 'N' or 'n' for no\n\t:");
        scanf(" %c",&opt);
        switch(opt){
            case 'y':
            case'Y':{
                for(int i=index-1;i<addressBook->contactCount-1;i++){
                    addressBook->contacts[i]=addressBook->contacts[i+1];
                }
                addressBook->contactCount--;
                printf("\tContact deleted Sucessfully\n");
                break;
            }
            case 'n':
            case 'N':{
                return;
            }
            default: printf("\tEnter the correct index\n");
                     goto label1;
        }

    }

}
