#include "header.h"
int main(int argc, char* argv[]){
    if(argc!=4){
        printf("Invalid Input!\n");
        return 0;
    }
    int val=validatenum(argv[1]);
    if(val==FAILURE){
        printf("Enter valid number 1!\n");
        return 0;
    }
    val=validatenum(argv[3]);
    if(val==FAILURE){
        printf("Enter valid number 2!\n");
        return 0;
    }
    Dlist* res=NULL;
    switch(argv[2][0]){
        case '+':
            res=addition(argv[1],argv[3]);
            break;
        case '-':
            res=subtraction(argv[1],argv[3]);
            break;
        case 'x':
            res=multiplication(argv[1],argv[3]);
            break;
        case '/':
            res=division(argv[1],argv[3]);
            break;
        case '%':
            res=modulo(argv[1],argv[3]);
            break;
        default: 
            printf("Enter valid operator!\n");
            break;
    }
    if(res!=NULL){
        printf("res=");
        print_list(res);
    }
    return 0;
}

int validatenum(char* str){
    int len=strlen(str);
    int idx=0;
    if(str[0]=='-' || str[0]=='+')
        idx++;
    int num_flag=0;
    while(str[idx]!='\0'){
        if(str[idx]<'0' || str[idx]>'9')
            return FAILURE;
        num_flag=1;
        idx++;
    }
    if(num_flag)
        return SUCCESS;
    return FAILURE;
}

/* Author: Swati Pathak*/
/*Project: APC-Arbitary Precision Calculator*/
/*Description: Normally when we do any arithmetic operation there is limit for the number for 32 bits for int or 64 bits for long.
                In order to overcome this limitation we have made this project where the arithmetic operation doesnot get limited 
                due to size of int or long int. we can calculate any big number operation with this method.
                In this project I have used Linked list concept where each number is first converted to a list of node where each node 
                holds digit in the base of 10000 except the first node which can or canot be a base of 10000.
                In this project the I have implemented addition,multiplication,subtraction,division and modulo operation*/