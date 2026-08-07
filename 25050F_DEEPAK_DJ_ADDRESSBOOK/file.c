#include <stdio.h>
#include "file.h"
#include<unistd.h>
void saveContactsToFile(AddressBook *addressBook) {
 
    FILE *fp;
    if((fp=fopen("contacts.csv","w"))==NULL)
    {
        printf("FILE NOT FOUND");
        return;
    }
    for(int i=0;i<addressBook->contactCount;i++){
        fprintf(fp,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fp);
    for(int i=0;i<100;i++)
    {
        printf("\tsaving and exiting ------>%d%%\r",i);
        fflush(stdout);
        usleep(25000);
    }
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fp;
    if((fp=fopen("contacts.csv","r"))==NULL){
        printf("FILE NOT FOUND");
        return;
    }
    int i=0;
    while((fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email))==3){
       addressBook->contactCount++;
       i++;
    }
    fclose(fp);
}