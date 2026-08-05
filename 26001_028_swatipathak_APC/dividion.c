#include "header.h"


// void divi(Dlist*head1,Dlist*tail1,Dlist*head2,Dlist* tail2,Dlist** quot,Dlist** rem){
//     // printf("I am mini div\n");
//     Dlist* arr[1000][2]={0};
//     Dlist* bits_arr[1000][2]={0};
//     int idx=0;
//     arr[idx][0]=head2;
//     arr[idx][1]=tail2;
//     bits_arr[idx][0]=NULL;
//     bits_arr[idx][1]=NULL;
//     insertatfirst(&bits_arr[idx][0],&bits_arr[idx][1],1);
//     Dlist* dividend_head=head1;
//     Dlist* dividend_tail=tail1;
//     int com=compare(dividend_head,arr[idx][0]);
//     while(com!=2){
//         add(arr[idx][0],arr[idx][1],arr[idx][0],arr[idx][1],&arr[idx+1][0],&arr[idx+1][1]);
//         add(bits_arr[idx][0],bits_arr[idx][1],bits_arr[idx][0],bits_arr[idx][1],&bits_arr[idx+1][0],&bits_arr[idx+1][1]);
//         idx++;
//         com=compare(dividend_head,arr[idx][0]);
//     }
//     Dlist* head=NULL;
//     Dlist* tail=NULL;
//     insertatfirst(&head,&tail,0);
//     for(int i=idx-1;i>=0;i--){
//         // printf("I am stuck here\n");
//         com=compare(dividend_head,arr[i][0]);
//         if(com!=2){
//             sub(dividend_head,dividend_tail,arr[i][0],arr[i][1],&dividend_head,&dividend_tail);
//             add(bits_arr[i][0],bits_arr[i][1],head,tail,&head,&tail);
//         }
//     }
//     // printf(" iam out\n");
//     *rem=dividend_head;
//     *quot=head;
// }

/*Multiplying the divisor with 2 and storing the value in stack until it becomes larger than divident.
Once the divisor becomes larger than divident. pop the stack and if the number is less than divisor.
subtract the number from divident and add the bit value of it (which power 2 we optained that number similar to bit value concept) to res.
continue until the stack is empty the final res optained after addition is the ans */
void divi(Dlist*head1,Dlist*tail1,Dlist*head2,Dlist* tail2,Dlist** quot,Dlist** rem){
    // printf("I am mini div\n");
    stacknode* top=malloc(sizeof(stacknode));
    top->bit_head=NULL;
    top->bit_tail=NULL;
    top->curr_dividend_head=NULL;
    top->curr_dividend_tail=NULL;
    top->next=NULL;
    insertatfirst(&top->bit_head,&top->bit_tail,1);
    top->curr_dividend_head=head2;
    top->curr_dividend_tail=tail2;
    Dlist* dividend_head=head1;
    Dlist* dividend_tail=tail1;
    int com=compare(dividend_head,top->curr_dividend_head);
    while(com!=2){
        stacknode* new=malloc(sizeof(stacknode));
        new->bit_head=NULL;
        new->bit_tail=NULL;
        new->curr_dividend_head=NULL;
        new->curr_dividend_tail=NULL;
        new->next=top;
        add(top->curr_dividend_head,top->curr_dividend_tail,top->curr_dividend_head,top->curr_dividend_tail,&new->curr_dividend_head,&new->curr_dividend_tail);
        add(top->bit_head,top->bit_tail,top->bit_head,top->bit_tail,&new->bit_head,&new->bit_tail);
        top=new;
        com=compare(dividend_head,top->curr_dividend_head);
    }
    // stacknode* temp=top;
    // while(temp!=NULL){
    //     print_list(temp->curr_dividend_head);
    //     printf(" ");
    //     print_list(temp->bit_head);
    //     printf("\n");
    //     temp=temp->next;
    // }
    stacknode* del_node;
    Dlist* head=NULL;
    Dlist* tail=NULL;
    insertatfirst(&head,&tail,0);
    while(top!=NULL){
        int com=compare(dividend_head,top->curr_dividend_head);
        if(com!=2){
            Dlist* next_head=head;
            Dlist* next_tail=tail;
            dividend_head=sub(dividend_head,dividend_tail,top->curr_dividend_head,top->curr_dividend_tail,&dividend_head,&dividend_tail);
            add(head,tail,top->bit_head,top->bit_tail,&head,&tail);
        }
        del_node=top;
        top=top->next;
        free(del_node);
    }
    // printf(" iam out\n");
    *rem=dividend_head;
    *quot=head;
}

Dlist* division(char* str1,char* str2){
    // printf("I am div\n");
    int len1=strlen(str1);
    int len2=strlen(str2);
    int neg_flag1=0;
    int neg_flag2=0;
    if(str1[0]=='-'){
        neg_flag1=1;
    }
    if(str2[0]=='-'){
        neg_flag2=1;
    }
    //case 1: if both number is 0 output is 1
    if(len1==1 && str1[0]=='0' && len2==1 && str2[0]=='0'){
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=1;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    //case 2: if divisor is 0 output is invalid
    else if(len2==1 && str2[0]=='0'){
        printf("Invalid Operation!\n");
        return NULL;
    }
    //case 3: if divident is 0 output is 0
    else if(len1==1 && str1[0]=='0'){
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=0;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    //case 4: both are nonzero value
    else{
        Dlist* head1=NULL;
        Dlist* head2=NULL;
        Dlist* tail1=NULL;
        Dlist* tail2=NULL;
        Dlist* quot=NULL;
        Dlist* rem=NULL;
        if(neg_flag1){
            createlist(&head1,&tail1,str1+1);
        }
        else{
            createlist(&head1,&tail1,str1);
        }
        if(neg_flag2){
            createlist(&head2,&tail2,str2+1);
        }
        else{
            createlist(&head2,&tail2,str2);
        }
        int big=compare(head1,head2);
        if(big==2){
            Dlist* res=malloc(sizeof(Dlist));
            res->digit=0;
            res->next=NULL;
            res->prev=NULL;
            return res;
        }
        else{
            // printf("i call\n");
            divi(head1,tail1,head2,tail2,&quot,&rem);
            if(neg_flag1 && neg_flag2){
                return quot;
            }
            else if(neg_flag1 || neg_flag2){
                quot->digit=(-1)*(quot->digit);
                return quot;
            }
            else{
                return quot;
            }
        }
    }
}

Dlist* modulo(char* str1,char* str2){//same as division
    int len1=strlen(str1);
    int len2=strlen(str2);
    int neg_flag1=0;
    int neg_flag2=0;
    if(str1[0]=='-')
        neg_flag1=1;
    if(str2[0]=='-')
        neg_flag2=1;
    if(len1==1 && str1[0]=='0'){
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=0;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    else if(len2==1 && str2[0]=='0'){
        Dlist* head=NULL;
        Dlist* tail=NULL;
        if(neg_flag1){
            createlist(&head,&tail,str1+1);
            head->digit=(-1)*(head->digit);
        }
        else{
            createlist(&head,&tail,str1);
        }
        return head;
    }
    else{
        Dlist* head1=NULL;
        Dlist* head2=NULL;
        Dlist* tail1=NULL;
        Dlist* tail2=NULL;
        Dlist* quot=NULL;
        Dlist* rem=NULL;
        if(neg_flag1){
            createlist(&head1,&tail1,str1+1);
        }
        else{
            createlist(&head1,&tail1,str1);
        }
        if(neg_flag2){
            createlist(&head2,&tail2,str2+1);
        }
        else{
            createlist(&head2,&tail2,str2);
        }
        int big=compare(head1,head2);
        if(big==2){
           return head1;
        }
        else{
            // printf("i call\n");
            divi(head1,tail1,head2,tail2,&quot,&rem);
            return rem;
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