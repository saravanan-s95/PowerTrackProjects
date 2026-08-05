#include "header.h"
 Dlist* mul(Dlist* head1,Dlist* tail1,Dlist* head2,Dlist* tail2){
    Dlist* head=NULL;
    Dlist* tail=NULL;
    insertatfirst(&head,&tail,0);
    int shift=0;
    while(tail2!=NULL){
        Dlist* temp_head=NULL;
        Dlist* temp_tail=NULL;
        int carry=0;
        Dlist* list1=tail1;
        while(list1!=NULL || carry){
            int ans=0;
            if(list1!=NULL){
                ans=(list1->digit)*(tail2->digit);
                list1=list1->prev;
            }
            ans=ans+carry;
            carry=0;
            carry=ans/10000;
            ans=ans%10000;
            insertatfirst(&temp_head,&temp_tail,ans);
        }
        for(int i=0;i<shift;i++){
            insertatlast(&temp_head,&temp_tail,0);
        }
        shift++;
        Dlist* curr_tail=NULL;
        Dlist* curr_head=NULL;
        add(head,tail,temp_head,temp_tail,&curr_head,&curr_tail);
        head=curr_head;
        tail=curr_tail;
        tail2=tail2->prev;
    }
    return head;
 }

Dlist* multiplication(char* str1,char* str2){
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
        Dlist* res=malloc(sizeof(Dlist));
        res->digit=0;
        res->next=NULL;
        res->prev=NULL;
        return res;
    }
    else{
        Dlist* head1=NULL;
        Dlist* head2=NULL;
        Dlist* tail1=NULL;
        Dlist* tail2=NULL;
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
        Dlist* res=mul(head1,tail1,head2,tail2);
        if((neg_flag1 && neg_flag2) ||(!(neg_flag1) && !(neg_flag2))){
            return res;
        }
        else{
            res->digit=(-1)*(res->digit);
            return res;
        }
    }
}