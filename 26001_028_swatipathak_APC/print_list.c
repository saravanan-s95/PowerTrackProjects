#include"header.h"
void print_list(Dlist* head){
    printf("%d",head->digit);
    head=head->next;
    while(head!=NULL){
        printf("%4.4d",head->digit);
        head=head->next;
    }
}