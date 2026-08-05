#include "header.h"
//creating list of each number
void createlist(Dlist**head,Dlist**tail,char* str){
    if(str[0]=='+'|| str[0]=='-'){
        str=str+1;
    }
    int len=strlen(str);
    //each node hold number with base 10000
    while(len>=4){
        int idx=len-4;
        int digit=0;
        for(int i=idx;i<len;i++){
            digit=digit*10+(str[i]-'0');
        }
        // printf("%d ",digit);
        //calling insert at first as head of node shuld have starting values of number
        insertatfirst(head,tail,digit);
        len=len-4;
    }
    if(len!=0){//first node can or cannot have number with base of 10000
        int digit=0;
        for(int i=0;i<len;i++){
            digit=digit*10+(str[i]-'0');
        }
        insertatfirst(head,tail,digit);
    }
    // Dlist* temp=*head;
    // while(temp!=NULL){
    //     printf("%d ",temp->digit);
    //     temp=temp->next;
    // }
    // printf("\n");
}

/* Author: Swati Pathak*/
/*Project: APC-Arbitary Precision Calculator*/
/*Description: Normally when we do any arithmetic operation there is limit for the number for 32 bits for int or 64 bits for long.
                In order to overcome this limitation we have made this project where the arithmetic operation doesnot get limited 
                due to size of int or long int. we can calculate any big number operation with this method.
                In this project I have used Linked list concept where each number is first converted to a list of node where each node 
                holds digit in the base of 10000 except the first node which can or canot be a base of 10000.
                In this project the I have implemented addition,multiplication,subtraction,division and modulo operation*/