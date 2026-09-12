#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

extern unsigned char sign1;
extern unsigned char sign2; 

void Subtract(char* argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    int diff = 0;
    int borrow = 0;

    if(find_how_to_subt(argv) == NORMAL_SUBT)       // Normal Subtraction (ie., List1 - List2)
    {
        while(tail1 && tail2)
        {
            diff = tail1->digit - tail2->digit - borrow;
            if(diff < 0)
            {
                diff = diff + 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            insert_to_dll(diff, res_head, res_tail);
            
            tail1 = tail1->prev;
            tail2 = tail2->prev;
        }

        while(tail1)    
        {
            diff = tail1->digit - borrow;
            if(diff < 0)
            {
                diff = diff + 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            insert_to_dll(diff, res_head, res_tail);
            tail1 = tail1->prev;
        }
        
        remove_leading_zeroes(res_head, res_tail);      

        
        if((sign1 == NEGATIVE && sign2 == POSITIVE) || (sign1 == NEGATIVE && sign2 == NEGATIVE))        // Print based on the sign of the operands
        {
            printf("Result is: -");
        }
        else
        {
            printf("Result is: ");
        }
        print_dll_as_number(*res_head, *res_tail);
    }


    else        // Swap and Subtract (ie., List2 - List1)
    {
        while(tail1 && tail2)
        {
            diff = tail2->digit - tail1->digit - borrow;
            if(diff < 0)
            {
                diff = diff + 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            insert_to_dll(diff, res_head, res_tail);
            
            tail1 = tail1->prev;
            tail2 = tail2->prev;
        }

        while(tail2)
        {
            diff = tail2->digit - borrow;
            if(diff < 0)
            {
                diff = diff + 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            insert_to_dll(diff, res_head, res_tail);
            tail2 = tail2->prev;
        }

        remove_leading_zeroes(res_head, res_tail);      

        
        if((sign2 == NEGATIVE && sign1 == POSITIVE) || (sign2 == POSITIVE && sign1 == POSITIVE))        // Print based on the sign of the operands
        {
            printf("Result is: -");
        }
        else
        {
            printf("Result is: ");
        }
        print_dll_as_number(*res_head, *res_tail);
    }
}


//Function to find normal_subtraction or swap_and_subtract (With respect to the number stored in argv)
int find_how_to_subt(char* argv[])
{
    
    char *num1;
    if (argv[1][0] == '-')          // If negative sign present for operand 1, skip the sign
    {
        num1 = argv[1] + 1;         //Store to new pointer num1
    }
    else
    {
        num1 = argv[1];
    }


    char *num2;
    if (argv[3][0] == '-')          // If negative sign present for operand 2, skip the sign
    {
        num2 = argv[3] + 1;         //Store to new pointer num2
    }
    else
    {
        num2 = argv[3];
    }


    //Compare
    if (strlen(num1) > strlen(num2))
    {
        //Normal subtraction (ie., list1 - list2)
        return NORMAL_SUBT;
    }
    else if (strlen(num1) < strlen(num2))
    {
        //swap and subtract (ie., list2 - list1)
        return SWAP_AND_SUBT;
    }
    else if(strlen(num1) == strlen(num2))
    {
        if (strcmp(num1, num2) >= 0)
        {
            //Normal subtraction (ie., list1 - list2)
            return NORMAL_SUBT;
        }
        else
        {
            //swap and subtract (ie., list2 - list1)
            return SWAP_AND_SUBT;
        }
    }
}


//Function to decide the operation based on sign of the operands
void sign_based_subtraction_select(char* argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    if(sign1 == POSITIVE && sign2 == NEGATIVE)
    {
        Add(head1, tail1, head2, tail2, res_head, res_tail);
        printf("Result is: ");
        print_dll_as_number(*res_head, *res_tail);
    }
    else if(sign1 == NEGATIVE && sign2 == POSITIVE)
    {
        Add(head1, tail1, head2, tail2, res_head, res_tail);
        printf("Result is: -");
        print_dll_as_number(*res_head, *res_tail);
    }
    else
    {
        Subtract(argv, head1, tail1, head2, tail2, res_head, res_tail);
    }
}