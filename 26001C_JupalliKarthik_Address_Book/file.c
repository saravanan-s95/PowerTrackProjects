#include <stdio.h>
#include "contact.h"
#include "file.h"
//To save the contacts in a file
void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "w");//opening the file in write mode
    fprintf(fp,"#%d\n",addressBook->contactCount);//printing the contactcount in the format
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,%s,%s,%d,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email,
            addressBook->contacts[i].is_favourite,addressBook->contacts[i].fav_type);//printing the contacts i format in the file
    }

    fclose(fp);//closing the file after storing the structure values to the file
}
//To load contacts from file
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "r");//opening the file in read mode
    if (fp == NULL) {//checking whether file is opened or not
        printf("No file found. Starting fresh.\n");
        return;
    }
    fscanf(fp,"#%d\n",&addressBook->contactCount);//storing the value return back to contact count
    for(int i=0;i<addressBook->contactCount;i++){
        fscanf(fp,"%[^,],%[^,],%[^,],%d,%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,
            addressBook->contacts[i].email,&addressBook->contacts[i].is_favourite,addressBook->contacts[i].fav_type);
            //storing back all the contacts from file to then respective structure indexes
    }
    fclose(fp);//closing the file after loading from file to the structure
}