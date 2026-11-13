
/*
 * Project Name : Arbitrary Precision Calculator (APC)
 *
 * Description  : Performs arithmetic operations on large
 *                integers using linked lists to store and
 *                process digits beyond standard data types.
 *
 * Author       : Sheeba Insily R
 * Batch Number : 26013
 * File Name    : main.c
 *
 * Concepts Used:
 * - C Programming
 * - Linked Lists
 * - Dynamic Memory Allocation
 * - Pointers and Functions
 * - Command-Line Arguments
 */

#include "apc.h"

/* Compare two positive numbers */
static int compare_numbers(Dlist *head1, Dlist *head2)
{
    int count1 = 0;
    int count2 = 0;

    Dlist *temp1 = head1;
    Dlist *temp2 = head2;

    while (temp1 != NULL)
    {
        count1++;
        temp1 = temp1->next;
    }

    while (temp2 != NULL)
    {
        count2++;
        temp2 = temp2->next;
    }

    if (count1 > count2)
        return 1;

    if (count1 < count2)
        return -1;

    temp1 = head1;
    temp2 = head2;

    while (temp1 != NULL && temp2 != NULL)
    {
        if (temp1->data > temp2->data)
            return 1;

        if (temp1->data < temp2->data)
            return -1;

        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    return 0;
}


/* Free complete linked list */
static void free_number(Dlist **head, Dlist **tail)
{
    Dlist *temp;

    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    *tail = NULL;
}


/* Validate number */
static int is_valid_number(const char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
        return FAILURE;

    if (str[0] == '+' || str[0] == '-')
        i = 1;

    if (str[i] == '\0')
        return FAILURE;

    while (str[i] != '\0')
    {
        if (!isdigit((unsigned char)str[i]))
            return FAILURE;

        i++;
    }

    return SUCCESS;
}


/* Create linked list from number string */
static int create_list(const char *str,
                       Dlist **head,
                       Dlist **tail)
{
    int i = 0;

    *head = NULL;
    *tail = NULL;

    /* Skip sign */
    if (str[0] == '+' || str[0] == '-')
        i = 1;

    /* Skip leading zeroes */
    while (str[i] == '0' && str[i + 1] != '\0')
        i++;

    while (str[i] != '\0')
    {
        if (insert_at_last(head,
                           tail,
                           str[i] - '0') == FAILURE)
        {
            free_number(head, tail);
            return FAILURE;
        }

        i++;
    }

    return SUCCESS;
}


/* Print result with sign */
static void print_result(Dlist *headR, int negative)
{
    if (negative &&
        !(headR->data == 0 && headR->next == NULL))
    {
        printf("-");
    }

    print_list(headR);
}


int main(int argc, char *argv[])
{
    Dlist *head1 = NULL;
    Dlist *tail1 = NULL;

    Dlist *head2 = NULL;
    Dlist *tail2 = NULL;

    Dlist *headR = NULL;
    Dlist *tailR = NULL;

    char operator;

    int negative_result = 0;
    int comparison;

    /*
        Command-line format:

        ./a.out number1 operator number2

        Example:

        ./a.out 123 + 456
    */

    if (argc != 4)
    {
        printf("Usage: %s <number1> <operator> <number2>\n",
               argv[0]);

        printf("Example: %s 123456789 + 987654321\n",
               argv[0]);

        return FAILURE;
    }


    /* Validate first number */
    if (is_valid_number(argv[1]) == FAILURE)
    {
        printf("ERROR: Invalid first number\n");
        return FAILURE;
    }


    /* Validate second number */
    if (is_valid_number(argv[3]) == FAILURE)
    {
        printf("ERROR: Invalid second number\n");
        return FAILURE;
    }


    operator = argv[2][0];


    /* Validate operator */
    if (argv[2][1] != '\0' ||
        (operator != '+' &&
         operator != '-' &&
         operator != '*' &&
         operator != '/'))
    {
        printf("ERROR: Invalid operator\n");
        printf("Supported operators: +  -  *  /\n");

        return FAILURE;
    }


    /* Create first number list */
    if (create_list(argv[1],
                    &head1,
                    &tail1) == FAILURE)
    {
        printf("ERROR: Memory allocation failed\n");
        return FAILURE;
    }


    /* Create second number list */
    if (create_list(argv[3],
                    &head2,
                    &tail2) == FAILURE)
    {
        printf("ERROR: Memory allocation failed\n");

        free_number(&head1, &tail1);

        return FAILURE;
    }


    /* Compare magnitudes */
    comparison = compare_numbers(head1, head2);


    /*------------------------------------------------------
                        OPERATOR +
    ------------------------------------------------------*/

    if (operator == '+')
    {
        /*
            Positive + Positive
        */

        if (argv[1][0] != '-' &&
            argv[3][0] != '-')
        {
            if (addition(head1,
                         tail1,
                         head2,
                         tail2,
                         &headR,
                         &tailR) == FAILURE)
            {
                printf("ERROR: Addition failed\n");
                goto cleanup;
            }
        }

        /*
            Negative + Negative
        */

        else if (argv[1][0] == '-' &&
                 argv[3][0] == '-')
        {
            negative_result = 1;

            if (addition(head1,
                         tail1,
                         head2,
                         tail2,
                         &headR,
                         &tailR) == FAILURE)
            {
                printf("ERROR: Addition failed\n");
                goto cleanup;
            }
        }

        /*
            Negative + Positive
        */

        else if (argv[1][0] == '-' &&
                 argv[3][0] != '-')
        {
            if (comparison > 0)
            {
                negative_result = 1;

                if (subtraction(head1,
                                tail1,
                                head2,
                                tail2,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
            else
            {
                if (subtraction(head2,
                                tail2,
                                head1,
                                tail1,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
        }

        /*
            Positive + Negative
        */

        else
        {
            if (comparison >= 0)
            {
                if (subtraction(head1,
                                tail1,
                                head2,
                                tail2,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
            else
            {
                negative_result = 1;

                if (subtraction(head2,
                                tail2,
                                head1,
                                tail1,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
        }

        printf("Result: ");
        print_result(headR, negative_result);
    }


    /*------------------------------------------------------
                        OPERATOR -
    ------------------------------------------------------*/

    else if (operator == '-')
    {
        /*
            Positive - Positive
        */

        if (argv[1][0] != '-' &&
            argv[3][0] != '-')
        {
            if (comparison >= 0)
            {
                if (subtraction(head1,
                                tail1,
                                head2,
                                tail2,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
            else
            {
                negative_result = 1;

                if (subtraction(head2,
                                tail2,
                                head1,
                                tail1,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
        }

        /*
            Negative - Positive
        */

        else if (argv[1][0] == '-' &&
                 argv[3][0] != '-')
        {
            negative_result = 1;

            if (addition(head1,
                         tail1,
                         head2,
                         tail2,
                         &headR,
                         &tailR) == FAILURE)
            {
                printf("ERROR: Addition failed\n");
                goto cleanup;
            }
        }

        /*
            Positive - Negative
        */

        else if (argv[1][0] != '-' &&
                 argv[3][0] == '-')
        {
            if (addition(head1,
                         tail1,
                         head2,
                         tail2,
                         &headR,
                         &tailR) == FAILURE)
            {
                printf("ERROR: Addition failed\n");
                goto cleanup;
            }
        }

        /*
            Negative - Negative
        */

        else
        {
            if (comparison >= 0)
            {
                if (subtraction(head1,
                                tail1,
                                head2,
                                tail2,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
            else
            {
                negative_result = 1;

                if (subtraction(head2,
                                tail2,
                                head1,
                                tail1,
                                &headR,
                                &tailR) == FAILURE)
                {
                    printf("ERROR: Subtraction failed\n");
                    goto cleanup;
                }
            }
        }

        printf("Result: ");
        print_result(headR, negative_result);
    }


    /*------------------------------------------------------
                        OPERATOR *
    ------------------------------------------------------*/

    else if (operator == '*')
    {
        /*
            Negative result when exactly one
            number is negative.
        */

        if ((argv[1][0] == '-') !=
            (argv[3][0] == '-'))
        {
            negative_result = 1;
        }

        if (multiplication(head1,
                           tail1,
                           head2,
                           tail2,
                           &headR,
                           &tailR) == FAILURE)
        {
            printf("ERROR: Multiplication failed\n");
            goto cleanup;
        }

        printf("Result: ");
        print_result(headR, negative_result);
    }


    /*------------------------------------------------------
                        OPERATOR /
    ------------------------------------------------------*/

    else if (operator == '/')
    {
        /*
            Negative result when exactly one
            number is negative.
        */

        if ((argv[1][0] == '-') !=
            (argv[3][0] == '-'))
        {
            negative_result = 1;
        }

        printf("Result: ");

        /*
            Print negative sign first.
        */

        if (negative_result)
        {
            printf("-");
        }

        /*
            Print quotient and 20 decimal places.
        */

        if (division_decimal(head1,
                             tail1,
                             head2,
                             tail2,
                             20) == FAILURE)
        {
            goto cleanup;
        }

        goto cleanup;
    }


cleanup:

    free_number(&head1, &tail1);
    free_number(&head2, &tail2);
    free_number(&headR, &tailR);

    return SUCCESS;
}