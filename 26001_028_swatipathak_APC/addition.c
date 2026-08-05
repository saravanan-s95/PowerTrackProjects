#include "header.h"

Dlist* add(Dlist* head1,Dlist*tail1,Dlist*head2,Dlist* tail2,Dlist** head,Dlist** tail){
    // printf("I was called\n");
    *head=NULL;
    *tail=NULL;
    int carry=0;
    while(tail1!=NULL || tail2!=NULL || carry){
        int sum=0;
        if(tail1!=NULL){
            sum+=tail1->digit;
            tail1=tail1->prev;
        }
        if(tail2!=NULL){
            sum+=tail2->digit;
            tail2=tail2->prev;
        }
        sum+=carry;
        carry=sum/10000;
        sum=sum%10000;
        insertatfirst(head,tail,sum);
        
    }
    return *head;

}

Dlist* addition(char* str1,char*str2){
    int len1=strlen(str1);
    int len2=strlen(str2);
    int neg_flag1=0;
    int neg_flag2=0;
    if(str1[0]=='-'){
        neg_flag1=1;
    }
    else if(str1[0]=='+'){
        str1=str1+1;
    }
    if(str2[0]=='-'){
        neg_flag2=1;
    }
    else if(str2[0]=='+'){
        str2=str2+1;
    }
    //case 1:if both number is 0
    if(len1==1 && len2==1 && str1[0]=='0' && str2[0]=='0'){
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=0;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    //case 2:if one number is 0
    else if(len1==1 && str1[0]=='0'){
        Dlist* head2=NULL;
        Dlist* tail2=NULL;
        if(neg_flag2){
            createlist(&head2,&tail2,str2+1);
            head2->digit=(-1)*(head2->digit);

        }
        else{
            createlist(&head2,&tail2,str2);
        } 
        return head2;
    }
    else if(len2==1 && str2[0]=='0'){
        Dlist* head1=NULL;
        Dlist* tail1=NULL;
        if(neg_flag1){
            createlist(&head1,&tail1,str1+1);
            head1->digit=(-1)*(head1->digit);
        }
        else{
            createlist(&head1,&tail1,str1);
        }
        return head1;
    }
    //case 3: non is zero
    else{
        int flag1=0;
        int flag2=0;
        Dlist* head1=NULL;
        Dlist* tail1=NULL;
        Dlist* head2=NULL;
        Dlist* tail2=NULL;
        Dlist* head=NULL;
        Dlist* tail=NULL;
        // if(str1[0]=='-'){
        //     createlist(&head1,&tail1,str1+1);
        //     flag1=1;
        // }
        // else{
        //     createlist(&head1,&tail1,str1);
        // }
        // if(str2[0]=='-'){
        //     createlist(&head2,&tail2,str2+1);
        //     flag2=1;
        // }
        // else{
        //     createlist(&head2,&tail2,str2);
        // }
        //creating list of numbers
        createlist(&head1,&tail1,str1);
        createlist(&head2,&tail2,str2);
        //case 1: both negative
        if(neg_flag1 && neg_flag2){
            Dlist* res=add(head1,tail1,head2,tail2,&head,&tail);
            res->digit=(-1)*(res->digit);
            return res;
        }
        //case 2: one number is negative
        else if(neg_flag1||neg_flag2){
            // printf("I am add calling sub\n");
            Dlist* res;
            if(neg_flag1){
                char* s2=malloc(sizeof(char)*(len2+2));
                s2[0]='-';
                strcpy(s2+1,str2);
                res=subtraction(str1,s2);
            }
            else{
                res=subtraction(str1,str2+1);
            }
            return res;
        }
        //case 3: both positive
        else{
            add(head1,tail1,head2,tail2,&head,&tail);
            return head;
        }   
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