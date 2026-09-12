#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

extern unsigned char sign1;
extern unsigned char sign2;

//Function to validate the operands given via CLA
int validate_operands(char* operand)
{
    if(!(operand[0] == '+' || operand[0] == '-' || isdigit(operand[0])))
    {
        printf("Invalid Operand\n");
        return FAILURE;
    }
    return SUCCESS;
}


//Function to validate the operators given via CLA
int validate_operator(char* operator)
{
    if(operator[0] == '+' || operator[0] == '-' || operator[0] == '*' || operator[0] == '/')
    {
        return SUCCESS;
    }
    return FAILURE;
}


//Print the DLL
int print_dll(apc_digit* head, apc_digit* tail)
{
    while(head != NULL)
    {
        printf("%d --> ", head->digit);
        head = head->next;
    }
    printf("NULL\n");

    return SUCCESS;
}


//Print DLL as a single number
int print_dll_as_number(apc_digit* head, apc_digit* tail)
{
    while(head != NULL)
    {
        printf("%d", head->digit);
        head = head->next;
    }
    printf("\n");

    return SUCCESS;
}


//Function to find the sign of the operands
int find_sign(char* operand1, char* operand2)
{
    if(operand1[0] == '+')
    {
        sign1 = POSITIVE;
    }
    else if(operand1[0] == '-')
    {
        sign1 = NEGATIVE;
    }

    if(operand2[0] == '+')
    {
        sign2 = POSITIVE;
    }
    else if(operand2[0] == '-')
    {
        sign2 = NEGATIVE;
    }
    return SUCCESS;
}


//To convert the input operands (given via CLA) to linked-list
int convert_cla_to_dll(char* argv, apc_digit** head, apc_digit** tail)
{

    int i=0;
    while(argv[i] != '\0')
    {
       
        if(argv[i] == '+' || argv[i] == '-')
        {
            i++;
            continue;
        }        
        char ch = argv[i];

        apc_digit* new_node = malloc(sizeof(apc_digit));
        if(new_node == NULL)
        {
            printf("Node memory not allocated\n");
            return MEMORY_NOT_ALLOCATED;
        }

        new_node->prev = NULL;
        new_node->next = NULL;
        new_node->digit = ch - '0';

        if((*head) == NULL)
        {
            (*head) = new_node;
            (*tail) = new_node;
        }
        else
        {
            new_node->prev = (*tail);
            (*tail)->next = new_node;
            (*tail) = new_node;
        }

        i++;
    }

    return SUCCESS;
}


//Function to remove leading zeroes from the list
void remove_leading_zeroes(apc_digit** head, apc_digit** tail)
{
    if(*head == NULL)
    {
        return;
    }
    
    apc_digit* temp = *head;

    while(temp)
    {
        if(*head == *tail)
        {
            return;
        }

        if((temp->digit) == 0)
        {
            // delete node
            apc_digit* temp1 = temp;
            (*head) = (*head)->next;
            (*head)->prev = NULL;
            free(temp1);
            temp = (*head);
        }
        else
        {
            break;
        }
    }

    return;
}


//Function to insert a new digit to the dll
int insert_to_dll(int value, apc_digit** res_head, apc_digit** res_tail)
{

    apc_digit* new_node = malloc(sizeof(apc_digit));
    if(new_node == NULL)
    {
        printf("Node not created\n");
        return FAILURE;
    }
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->digit = value;

    if((*res_head) == NULL)
    {
        (*res_head) = new_node;
        (*res_tail) = new_node;
    }
    else
    {
        new_node->prev = NULL;
        new_node->next = *res_head;
        (*res_head)->prev = new_node; 
        *res_head = new_node;
    }
    
}


//Function to free a list
void free_list(apc_digit* head)
{
    apc_digit* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}