#include <stdio.h>
#include "header.h"

extern unsigned char sign1;
extern unsigned char sign2;

void Multiplication(char*argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    int result = 0;         //To store result-digit    
    int zero_shift = 0;     //To add zeroes on partial-products

    apc_digit* temp = tail1;    //To progress throught the operand1

    *res_head = NULL;           
    *res_tail = NULL;
    
    
    while(tail2)
    {

        int carry = 0;                  //To sotre carry-digit 
        apc_digit* temp_head = NULL;    //To store the partial-product
        apc_digit* temp_tail = NULL;

        for(int i=0; i<zero_shift; i++)     //Adding zeroes in partial-produt list
        {
            insert_to_dll(0, &temp_head, &temp_tail);
        }

        while(temp)
        {
 
            result = (((temp->digit) * (tail2->digit)) + carry);

            if(result > 9)
            {
                carry = result/10;
                result = result%10;
            }
            else
            {
                carry = 0;
            }
            insert_to_dll(result, &temp_head, &temp_tail);

            temp = temp->prev;
        }

        if(carry > 0)       //If carry is left, insert at the beginning
        {
            insert_to_dll(carry, &temp_head, &temp_tail);
        }

        if(*res_head == NULL)       //The result after multipying the first digit (the result_list is currently empty - no addition required), so directly assigned to result
        {
            *res_head = temp_head;
            *res_tail = temp_tail;
        }
        else                        //The further partial products need to be added
        {
            apc_digit* prev_result_head = NULL;
            apc_digit* prev_result_tail = NULL;

            prev_result_head = *res_head;
            prev_result_tail = *res_tail;

            *res_head = NULL;
            *res_tail = NULL;

            Add(prev_result_head, prev_result_tail, temp_head, temp_tail, res_head, res_tail);  //Add the previous result and the new partial-product

            free_list(prev_result_head);
            free_list(temp_head);
        }

        temp = tail1;
        tail2 = tail2->prev;
        zero_shift++;
    }


    if(sign1 == POSITIVE && sign2 == NEGATIVE)          //Print based on the sign of the operands
    {
        printf("Result is: -");
        print_dll_as_number(*res_head, *res_tail);
    }
    else if(sign1 == NEGATIVE && sign2 == POSITIVE)
    {
        printf("Result is: -");
        print_dll_as_number(*res_head, *res_tail);
    }
    else
    {
        printf("Result is: ");
        print_dll_as_number(*res_head, *res_tail);
    }
}
