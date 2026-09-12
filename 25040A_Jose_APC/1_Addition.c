#include <stdio.h>
#include <stdlib.h>
#include "header.h"

extern unsigned char sign1;
extern unsigned char sign2; 

void Add(apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    int sum = 0;
    int carry = 0;

    while(tail1 && tail2)
    {
        sum = tail1->digit + tail2->digit + carry;
        
        if(sum > 9)     //If sum is greater than 9, then carry '1' exists
        {
            carry = 1;
            sum = sum%10;
        }
        else
        {
            carry = 0;
        }

        insert_to_dll(sum, res_head, res_tail);     //Insert the digit to the result_list

        tail1 = tail1->prev;
        tail2 = tail2->prev;

    }

    while(tail1 != NULL)    //When list2 finished
    {
        sum = tail1->digit + carry;
        if(sum > 9)
        {
            carry = 1;
            sum = sum%10;
        }
        else
        {
            carry = 0;
        }
        insert_to_dll(sum, res_head, res_tail);     //Insert the digit to the result_list

        tail1 = tail1->prev;

    }
    while(tail2 != NULL)        //When list1 has finished
    {
        sum = tail2->digit + carry;
        if(sum > 9)
        {
            carry = 1;
            sum = sum%10;
        }
        else
        {
            carry = 0;
        }
        insert_to_dll(sum, res_head, res_tail);     //Insert the digit to the result_list

        tail2 = tail2->prev;

    }

    if (carry == 1)         // If there is still a carry left, insert it at the very front (e.g., 80 + 20 = 100)
    {
        insert_to_dll(carry, res_head, res_tail);   //Insert the digit to the result_list
    }

    remove_leading_zeroes(res_head, res_tail);      //Remove any leading zeroes from the result list

}


//Function to select operation based on the sign of operands
void sign_based_addition_select(char* argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    if(sign1 == POSITIVE && sign2 == POSITIVE)
    {
        Add(head1, tail1, head2, tail2, res_head, res_tail);
        printf("Result is: ");
        print_dll_as_number(*res_head, *res_tail);
    }
    else if(sign1 == NEGATIVE &&  sign2 == NEGATIVE)
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
