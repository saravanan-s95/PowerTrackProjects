#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

extern unsigned char sign1;
extern unsigned char sign2; 

void Division(char*argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail)
{
    if(strcmp(argv[3], "0") == 0)
    {
        printf("Error: Division with zero\n");
        return;
    }
    else if(strcmp(argv[1],"0") == 0)           //If the numerator is zero, then the result is zero.
    {
        printf("Result is: 0\n");
        return;
    }

    else if(compare_lists(head1, tail1, head2, tail2) == LIST_2_GREATER)        //If the denominator is greater than numerator, result is zero.
    {
        printf("Result is: 0\n");
        return;
    }
    
    else if(strcmp(argv[1], argv[3]) == 0)          //If numerator and denominator are equal, then the result is one.
    {
        printf("Result is: 1\n");
        return;
    }

    insert_to_dll(0, res_head, res_tail);   //Initialise result_dll with value 0 

    int ret = 0;

    apc_digit* count_head = NULL;           //Double-linked-list for storing count 
    apc_digit* count_tail = NULL;
    insert_to_dll(0, &count_head, &count_tail);     //Initialise count_dll with value 0


    apc_digit* count_incr_head = NULL;      //Double-linked-list for incrementing count (ie., to implement count++)
    apc_digit* count_incr_tail = NULL;
    insert_to_dll(1, &count_incr_head, &count_incr_tail);   //count_incr_dll with value 1 (fixed value)


    while(compare_lists(head1, tail1, *res_head, *res_tail) == LIST_1_GREATER)  //Perform addition as long as operand1 is greater than result
    {
        apc_digit* temp_head = NULL;        //A temporary list to store result, later to re-assign to result_list
        apc_digit* temp_tail = NULL;

        Add(head2, tail2, *res_head, *res_tail, &temp_head, &temp_tail);

        free_list(*res_head);   //Free the existing result_list

        *res_head = temp_head;  //Re-assign the new result 
        *res_tail = temp_tail;

        ret = compare_lists(head1, tail1, *res_head, *res_tail);
        if(ret == LIST_1_GREATER || ret == LIST_EQUAL)  //When operand1 is less than or equal to result, then increment count
        {
            //Increment count_dll
            apc_digit* count_temp_head = NULL;      //A temporary list to store count_incrementing result, later to re-assign to count_list
            apc_digit* count_temp_tail = NULL;

            Add(count_head, count_tail, count_incr_head, count_incr_tail, &count_temp_head, &count_temp_tail);
            
            free_list(count_head);          //Free the existing count_list
            count_head = count_temp_head;   //Re-assign the new count 
            count_tail = count_temp_tail;

        }
        if(ret == LIST_2_GREATER || ret == LIST_EQUAL)      //When the operand1 becomes equal or greater than result, break
        {
            break;
        }
    }


    if(sign1 == POSITIVE && sign2 == NEGATIVE)      //Print based on the sign of the operands
    {
        printf("Result is: -");
        print_dll_as_number(count_head, count_tail);
    }
    else if(sign1 == NEGATIVE && sign2 == POSITIVE)
    {
        printf("Result is: -");
        print_dll_as_number(count_head, count_tail);
    }
    else
    {
        printf("Result is: ");
        print_dll_as_number(count_head, count_tail);
    }
}


//Function to compare two list (ie., to check which list holds the greater number)
int compare_lists(apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2)
{
    int length1 = 0;
    int length2 = 0;
    apc_digit* temp = head1;
    
    while(temp)
    {
        length1++;
        temp = temp->next;
    }

    temp = head2;
    while(temp)
    {
        length2++;
        temp = temp->next;
    }

    if(length1 > length2)
    {
        return LIST_1_GREATER;
    }
    if(length1 < length2)
    {
        return LIST_2_GREATER;
    }
    if(length1 == length2)
    {
        while(head1 && head2)
        {
            if(head1->digit > head2->digit)
            {
                return LIST_1_GREATER;
            }
            if(head1->digit < head2->digit)
            {
                return LIST_2_GREATER;
            }
            if(head1->digit == head2->digit)
            {
                head1 = head1->next;
                head2 = head2->next;
            }
        }
    }

    return LIST_EQUAL;

}

//Function to compare the inputs via command-line arguments (To check which string holds the greater number)
int find_greater_operand(char* argv[])
{
    if(strlen(argv[1]) > strlen(argv[3]))
    {
        return GREATER_1;
    }

    else if(strlen(argv[1]) < strlen(argv[3]))
    {
        return GREATER_2;
    }

    else if(strlen(argv[1]) == strlen(argv[3]))
    {
        int i=0;
        while(argv[1][i]!='\0')
        {
            if(argv[1][i] > argv[3][i])
            {
                return GREATER_1;
            }
            else if(argv[1][i] < argv[3][i])
            {
                return GREATER_2;
            }
            else if(argv[1][i] == argv[3][i])
            {
                i++;
            }
        }
    }
}
