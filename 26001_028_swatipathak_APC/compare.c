#include "header.h"

int compare(Dlist* head1,Dlist* head2){
    Dlist* temp1=head1;
    Dlist* temp2=head2;
    int len1=0,len2=0;
    while(temp1!=NULL){
        len1++;
        temp1=temp1->next;
    }
    while (temp2!=NULL)
    {
        len2++;
        temp2=temp2->next;
    }
    if(len1>len2)//case 1: if len of list 1 is greater than list 1 then num1 is greater
        return 1;
    else if(len2>len1)//case 2: if len of list 2 is greater than list 2 then num1 is greater
        return 2;
    //case 3:both have same len than compare the number of each node of both list
    else{
        temp1=head1;
        temp2=head2;
        while(temp1!=NULL && temp2!=NULL){
            if((temp1->digit)>(temp2->digit))
                return 1;
            else if((temp1->digit)<(temp2->digit))
                return 2;
            else{
                temp1=temp1->next;
                temp2=temp2->next;
            }
        }
        return 0;//if both same return 0
    }
    
}

/* Author: Swati Pathak*/
/*Project: APC-Arbitary Precision Calculator*/
/*Description: Normally when we do any arithmetic operation there is limit for the number for 32 bits for int or 64 bits for long.
                In order to overcome this limitation we have made this project where the arithmetic operation doesnot get limited 
                due to size of int or long int. we can calculate any big number operation with this method.
                In this project I have used Linked list concept where each number is first converted to a list of node where each node 
                holds digit in the base of 10000 except the first node which can or canot be a base of 10000.
                In this project the I have implemented addition,multiplication,subtraction,division and modulo operation*/