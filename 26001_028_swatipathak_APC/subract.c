#include "header.h"

Dlist* sub(Dlist* head1,Dlist* tail1,Dlist* head2,Dlist*tail2,Dlist**head,Dlist**tail){
    *head=NULL;
    *tail=NULL;
    int borrow=0;
    while(tail1!=NULL || borrow>0){
        int ans=0;
        if(tail1!=NULL){
            ans+=tail1->digit;
            tail1=tail1->prev;
        }
        ans=ans-borrow;
        borrow=0;
        if(tail2!=NULL){
            ans=ans-tail2->digit;
            tail2=tail2->prev;
        }
        if(ans<0){
            ans=ans+10000;
            borrow=1;
        }
        insertatfirst(head,tail,ans);
    }
    while((*head)->digit==0 && (*head)->next!=NULL){
        (*head)=(*head)->next;
        free((*head)->prev);
        (*head)->prev=NULL;
    }
    if((*head)->next==NULL){
        *tail=*head;
    }
    return *head;
}
Dlist* subtraction(char* str1,char* str2){
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
    if(len1==1 && len2==1 && str1[0]=='0' && str2[0]=='0'){
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=0;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    else if(len1==1 && str1[0]=='0'){
        Dlist* head2=NULL;
        Dlist* tail2=NULL;
        if(neg_flag2){
            createlist(&head2,&tail2,str2+1);
        }
        else{
            createlist(&head2,&tail2,str2);
            head2->digit=(-1)*(head2->digit);
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
    else{
        Dlist* head1=NULL;
        Dlist* head2=NULL;
        Dlist* tail1=NULL;
        Dlist* tail2=NULL;
        Dlist* head=NULL;
        Dlist* tail=NULL;
        Dlist* res;
        // if(neg_flag1){
        //     createlist(&head1,&tail1,str1+1);
        // }
        // else{
        //     createlist(&head1,&tail1,str1);
        // }
        // if(neg_flag2){
        //     createlist(&head2,&tail2,str2+1);
        // }
        // else{
        //     createlist(&head2,&tail2,str2);
        // }
        createlist(&head1,&tail1,str1);
        createlist(&head2,&tail2,str2);
        int big=compare(head1,head2);
        if(big==0){
            if((neg_flag1 && neg_flag2) || (!(neg_flag1) && !(neg_flag2))){
                res=malloc(sizeof(Dlist));
                res->digit=0;
                res->next=NULL;
                res->prev=NULL;
                return res;
            }
            else if(neg_flag1){
                res=add(head1,tail1,head2,tail2,&head,&tail);
                res->digit=(-1)*(res->digit);
                return res;
            }
            else if(neg_flag2){
                res=add(head1,tail1,head2,tail2,&head,&tail);
                return res;
            }
        }
        else if(big==1){
            if(!(neg_flag1) && !(neg_flag2)){
                res=sub(head1,tail1,head2,tail2,&head,&tail);
                return res;
            }
            else if(neg_flag1 && neg_flag2){
                res=sub(head1,tail1,head2,tail2,&head,&tail);
                res->digit=(-1)*(res->digit);
                return res;
            }
            else if(neg_flag1){
                res=add(head1,tail1,head2,tail2,&head,&tail);
                res->digit=(-1)*(res->digit);
                return res;
            }
            else if(neg_flag2){
                res=add(head1,tail1,head2,tail2,&head,&tail);
                return res;
            }
        }
        else if(big==2){
            if(!(neg_flag1)&&!(neg_flag2)){
                res=sub(head2,tail2,head1,tail1,&head,&tail);
                res->digit=(-1)*(res->digit);
                return res;
            }
            else if(neg_flag1&&neg_flag2){
                res=sub(head2,tail2,head1,tail1,&head,&tail);
                return res;
            }
            else if(neg_flag2){
                res=add(head2,tail2,head1,tail1,&head,&tail);
                return res;
            }
            else if(neg_flag1){
                res=add(head2,tail2,head1,tail1,&head,&tail);
                res->digit=(-1)*(res->digit);
                return res;
            }
        }
    }
    
}