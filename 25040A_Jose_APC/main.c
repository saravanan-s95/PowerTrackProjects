#include <stdio.h>
#include <stdlib.h>
#include "header.h"

unsigned char sign1;
unsigned char sign2;

int main(int argc, char* argv[])
{
    if(argc != 4)       //Validating no.of arguments
    {
        printf("Invalid no. of arguments\n");
        return FAILURE;
    }

    if(validate_operands(argv[1]) == FAILURE || validate_operands(argv[3]) == FAILURE)      //Validating operands
    {
        printf("Validation of Operands Failure\n");
        return FAILURE;
    }
    printf("Validation of Operands Success\n");

    if(validate_operator(argv[2]) == FAILURE)       //Validating operator
    {
        printf("Validation of Operator Failure\n");
        return FAILURE;
    }
    printf("Validation of Operator Success\n");

    if(find_sign(argv[1], argv[3]) == SUCCESS)      //Finding whether the operands are positive or negative
    {
        if(sign1 == POSITIVE) {printf("\nOperand1 is positive number\n");}
        if(sign1 == NEGATIVE) {printf("\nOperand1 is negative number\n");}
        if(sign2 == POSITIVE) {printf("Operand2 is positive number\n");}
        if(sign2 == NEGATIVE) {printf("Operand2 is negative number\n");}
    }

    apc_digit* head1 = NULL;
    apc_digit* tail1 = NULL;
    if(convert_cla_to_dll(argv[1], &head1, &tail1) == SUCCESS)      //Converting first operand to double-linked-list
    {
        remove_leading_zeroes(&head1, &tail1);
    }

    apc_digit* head2 = NULL;
    apc_digit* tail2 = NULL;
    if(convert_cla_to_dll(argv[3], &head2, &tail2) == SUCCESS)      //Converting second operand to double-linked-list
    {
        remove_leading_zeroes(&head2, &tail2);
        printf("\n");
    }

    apc_digit* res_head = NULL;
    apc_digit* res_tail = NULL;

    switch(argv[2][0])          //Select operation to be done based on operator
    {
        case '+': sign_based_addition_select(argv, head1, tail1, head2, tail2, &res_head, &res_tail);
                    break;

        case '-': sign_based_subtraction_select(argv, head1, tail1, head2, tail2, &res_head, &res_tail);
                    break;

        case '*': Multiplication(argv, head1, tail1, head2, tail2, &res_head, &res_tail);
                    break;

        case '/': Division(argv, head1, tail1, head2, tail2, &res_head, &res_tail);
                    break;
        
        default: printf("Invalid Operator\n");
                    return FAILURE;

    }

    return SUCCESS;
}
