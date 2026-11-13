#include "apc.h"

/*----------------------------------------------------------
                    Helper Functions
----------------------------------------------------------*/

static void remove_leading_zeros(Dlist **head, Dlist **tail)
{
    while (*head != NULL &&
           (*head)->data == 0 &&
           *head != *tail)
    {
        Dlist *temp = *head;

        *head = (*head)->next;
        (*head)->prev = NULL;

        free(temp);
    }
}


static int compare_lists(Dlist *head1, Dlist *head2)
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

    while (temp1 != NULL)
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


static void free_list(Dlist **head, Dlist **tail)
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


static int copy_list(Dlist *head,
                     Dlist **new_head,
                     Dlist **new_tail)
{
    Dlist *temp = head;

    *new_head = NULL;
    *new_tail = NULL;

    while (temp != NULL)
    {
        if (insert_at_last(new_head,
                           new_tail,
                           temp->data) == FAILURE)
        {
            free_list(new_head, new_tail);
            return FAILURE;
        }

        temp = temp->next;
    }

    return SUCCESS;
}


/*----------------------------------------------------------
                  Subtraction Helper
----------------------------------------------------------*/

static int subtract_lists(Dlist *head1,
                          Dlist *tail1,
                          Dlist *head2,
                          Dlist *tail2,
                          Dlist **headR,
                          Dlist **tailR)
{
    Dlist *temp1 = tail1;
    Dlist *temp2 = tail2;

    int borrow = 0;

    *headR = NULL;
    *tailR = NULL;

    while (temp1 != NULL)
    {
        int digit1 = temp1->data - borrow;
        int digit2 = 0;

        if (temp2 != NULL)
            digit2 = temp2->data;

        if (digit1 < digit2)
        {
            digit1 += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        if (insert_at_first(headR,
                            tailR,
                            digit1 - digit2) == FAILURE)
        {
            free_list(headR, tailR);
            return FAILURE;
        }

        temp1 = temp1->prev;

        if (temp2 != NULL)
            temp2 = temp2->prev;
    }

    remove_leading_zeros(headR, tailR);

    return SUCCESS;
}


/*----------------------------------------------------------
                  Insert At First
----------------------------------------------------------*/

int insert_at_first(Dlist **head,
                    Dlist **tail,
                    int data)
{
    Dlist *new = malloc(sizeof(Dlist));

    if (new == NULL)
        return FAILURE;

    new->data = data;
    new->prev = NULL;
    new->next = NULL;

    if (*head == NULL)
    {
        *head = new;
        *tail = new;

        return SUCCESS;
    }

    new->next = *head;
    (*head)->prev = new;

    *head = new;

    return SUCCESS;
}


/*----------------------------------------------------------
                  Insert At Last
----------------------------------------------------------*/

int insert_at_last(Dlist **head,
                   Dlist **tail,
                   int data)
{
    Dlist *new = malloc(sizeof(Dlist));

    if (new == NULL)
        return FAILURE;

    new->data = data;
    new->prev = NULL;
    new->next = NULL;

    if (*head == NULL)
    {
        *head = new;
        *tail = new;

        return SUCCESS;
    }

    new->prev = *tail;
    (*tail)->next = new;

    *tail = new;

    return SUCCESS;
}


/*----------------------------------------------------------
                  Delete At First
----------------------------------------------------------*/

int delete_at_first(Dlist **head,
                    Dlist **tail)
{
    Dlist *temp;

    if (*head == NULL)
        return FAILURE;

    temp = *head;

    if (*head == *tail)
    {
        *head = NULL;
        *tail = NULL;

        free(temp);

        return SUCCESS;
    }

    *head = (*head)->next;
    (*head)->prev = NULL;

    free(temp);

    return SUCCESS;
}


/*----------------------------------------------------------
                  Delete At Last
----------------------------------------------------------*/

int delete_at_last(Dlist **head,
                   Dlist **tail)
{
    Dlist *temp;

    if (*head == NULL)
        return FAILURE;

    temp = *tail;

    if (*head == *tail)
    {
        *head = NULL;
        *tail = NULL;

        free(temp);

        return SUCCESS;
    }

    *tail = (*tail)->prev;
    (*tail)->next = NULL;

    free(temp);

    return SUCCESS;
}


/*----------------------------------------------------------
                    Print List
----------------------------------------------------------*/

void print_list(Dlist *head)
{
    Dlist *temp = head;

    while (temp != NULL)
    {
        printf("%d", temp->data);
        temp = temp->next;
    }

    printf("\n");
}


/*----------------------------------------------------------
                       ADDITION
----------------------------------------------------------*/

int addition(Dlist *head1,
             Dlist *tail1,
             Dlist *head2,
             Dlist *tail2,
             Dlist **headR,
             Dlist **tailR)
{
    Dlist *temp1 = tail1;
    Dlist *temp2 = tail2;

    int carry = 0;

    *headR = NULL;
    *tailR = NULL;

    while (temp1 != NULL ||
           temp2 != NULL ||
           carry != 0)
    {
        int digit1 = 0;
        int digit2 = 0;
        int sum;

        if (temp1 != NULL)
            digit1 = temp1->data;

        if (temp2 != NULL)
            digit2 = temp2->data;

        sum = digit1 + digit2 + carry;

        carry = sum / 10;
        sum = sum % 10;

        if (insert_at_first(headR,
                            tailR,
                            sum) == FAILURE)
        {
            free_list(headR, tailR);
            return FAILURE;
        }

        if (temp1 != NULL)
            temp1 = temp1->prev;

        if (temp2 != NULL)
            temp2 = temp2->prev;
    }

    remove_leading_zeros(headR, tailR);

    return SUCCESS;
}


/*----------------------------------------------------------
                     SUBTRACTION
----------------------------------------------------------*/

int subtraction(Dlist *head1,
                Dlist *tail1,
                Dlist *head2,
                Dlist *tail2,
                Dlist **headR,
                Dlist **tailR)
{
    int comparison;

    *headR = NULL;
    *tailR = NULL;

    comparison = compare_lists(head1, head2);

    if (comparison == 0)
    {
        if (insert_at_last(headR,
                           tailR,
                           0) == FAILURE)
        {
            return FAILURE;
        }

        return SUCCESS;
    }

    if (comparison < 0)
    {
        return subtract_lists(head2,
                              tail2,
                              head1,
                              tail1,
                              headR,
                              tailR);
    }

    return subtract_lists(head1,
                          tail1,
                          head2,
                          tail2,
                          headR,
                          tailR);
}


/*----------------------------------------------------------
                MULTIPLICATION HELPER
----------------------------------------------------------*/

static int multiply_lists(Dlist *head1,
                          Dlist *tail1,
                          Dlist *head2,
                          Dlist *tail2,
                          Dlist **headR,
                          Dlist **tailR)
{
    int position = 0;

    Dlist *temp2 = tail2;

    *headR = NULL;
    *tailR = NULL;

    if (insert_at_last(headR,
                       tailR,
                       0) == FAILURE)
    {
        return FAILURE;
    }

    while (temp2 != NULL)
    {
        int carry = 0;
        int digit2 = temp2->data;

        Dlist *temp1 = tail1;

        Dlist *partialHead = NULL;
        Dlist *partialTail = NULL;

        while (temp1 != NULL)
        {
            int product;
            int digit;

            product = temp1->data * digit2 + carry;

            digit = product % 10;
            carry = product / 10;

            if (insert_at_first(&partialHead,
                                &partialTail,
                                digit) == FAILURE)
            {
                free_list(&partialHead,
                          &partialTail);

                free_list(headR, tailR);

                return FAILURE;
            }

            temp1 = temp1->prev;
        }

        if (carry != 0)
        {
            if (insert_at_first(&partialHead,
                                &partialTail,
                                carry) == FAILURE)
            {
                free_list(&partialHead,
                          &partialTail);

                free_list(headR, tailR);

                return FAILURE;
            }
        }

        for (int i = 0; i < position; i++)
        {
            if (insert_at_last(&partialHead,
                               &partialTail,
                               0) == FAILURE)
            {
                free_list(&partialHead,
                          &partialTail);

                free_list(headR, tailR);

                return FAILURE;
            }
        }

        Dlist *newResultHead = NULL;
        Dlist *newResultTail = NULL;

        if (addition(*headR,
                     *tailR,
                     partialHead,
                     partialTail,
                     &newResultHead,
                     &newResultTail) == FAILURE)
        {
            free_list(&partialHead,
                      &partialTail);

            free_list(headR, tailR);

            return FAILURE;
        }

        free_list(headR, tailR);
        free_list(&partialHead, &partialTail);

        *headR = newResultHead;
        *tailR = newResultTail;

        temp2 = temp2->prev;
        position++;
    }

    remove_leading_zeros(headR, tailR);

    return SUCCESS;
}


/*----------------------------------------------------------
                   MULTIPLICATION
----------------------------------------------------------*/

int multiplication(Dlist *head1,
                   Dlist *tail1,
                   Dlist *head2,
                   Dlist *tail2,
                   Dlist **headR,
                   Dlist **tailR)
{
    if ((head1 != NULL &&
         head1->data == 0 &&
         head1 == tail1) ||
        (head2 != NULL &&
         head2->data == 0 &&
         head2 == tail2))
    {
        *headR = NULL;
        *tailR = NULL;

        if (insert_at_last(headR,
                           tailR,
                           0) == FAILURE)
        {
            return FAILURE;
        }

        return SUCCESS;
    }

    return multiply_lists(head1,
                          tail1,
                          head2,
                          tail2,
                          headR,
                          tailR);
}


/*----------------------------------------------------------
                    INTEGER DIVISION
----------------------------------------------------------*/

int division(Dlist *head1,
             Dlist *tail1,
             Dlist *head2,
             Dlist *tail2,
             Dlist **headR,
             Dlist **tailR)
{
    Dlist *remainderHead = NULL;
    Dlist *remainderTail = NULL;

    Dlist *divisorHead = NULL;
    Dlist *divisorTail = NULL;

    Dlist *temp;

    *headR = NULL;
    *tailR = NULL;

    if (head2 == NULL ||
        (head2->data == 0 &&
         head2 == tail2))
    {
        printf("ERROR: Division by zero\n");
        return FAILURE;
    }

    if (copy_list(head2,
                  &divisorHead,
                  &divisorTail) == FAILURE)
    {
        return FAILURE;
    }

    temp = head1;

    while (temp != NULL)
    {
        int quotientDigit = 0;

        if (insert_at_last(&remainderHead,
                           &remainderTail,
                           temp->data) == FAILURE)
        {
            free_list(&remainderHead,
                      &remainderTail);

            free_list(&divisorHead,
                      &divisorTail);

            return FAILURE;
        }

        remove_leading_zeros(&remainderHead,
                             &remainderTail);

        while (compare_lists(remainderHead,
                             divisorHead) >= 0)
        {
            Dlist *newRemHead = NULL;
            Dlist *newRemTail = NULL;

            if (subtract_lists(remainderHead,
                               remainderTail,
                               divisorHead,
                               divisorTail,
                               &newRemHead,
                               &newRemTail) == FAILURE)
            {
                free_list(&remainderHead,
                          &remainderTail);

                free_list(&divisorHead,
                          &divisorTail);

                free_list(headR, tailR);

                return FAILURE;
            }

            free_list(&remainderHead,
                      &remainderTail);

            remainderHead = newRemHead;
            remainderTail = newRemTail;

            quotientDigit++;
        }

        if (insert_at_last(headR,
                           tailR,
                           quotientDigit) == FAILURE)
        {
            free_list(&remainderHead,
                      &remainderTail);

            free_list(&divisorHead,
                      &divisorTail);

            free_list(headR, tailR);

            return FAILURE;
        }

        temp = temp->next;
    }

    remove_leading_zeros(headR, tailR);

    free_list(&remainderHead,
              &remainderTail);

    free_list(&divisorHead,
              &divisorTail);

    return SUCCESS;
}


/*----------------------------------------------------------
                  DECIMAL DIVISION
----------------------------------------------------------*/

int division_decimal(Dlist *head1,
                     Dlist *tail1,
                     Dlist *head2,
                     Dlist *tail2,
                     int decimal_places)
{
    Dlist *remainderHead = NULL;
    Dlist *remainderTail = NULL;

    Dlist *divisorHead = NULL;
    Dlist *divisorTail = NULL;

    Dlist *temp;

    int quotientDigit;
    int decimal_count = 0;

    /* Division by zero */
    if (head2 == NULL ||
        (head2->data == 0 &&
         head2 == tail2))
    {
        printf("ERROR: Division by zero\n");
        return FAILURE;
    }

    /* Copy divisor */
    if (copy_list(head2,
                  &divisorHead,
                  &divisorTail) == FAILURE)
    {
        return FAILURE;
    }

    /*
       Calculate integer part.

       We do NOT print leading zero quotient
       digits.

       Example:

       10 / 3

       First quotient digit = 0
       Second quotient digit = 3

       We print only 3.
    */

    temp = head1;

    int started = 0;

    while (temp != NULL)
    {
        quotientDigit = 0;

        if (insert_at_last(&remainderHead,
                           &remainderTail,
                           temp->data) == FAILURE)
        {
            free_list(&remainderHead,
                      &remainderTail);

            free_list(&divisorHead,
                      &divisorTail);

            return FAILURE;
        }

        remove_leading_zeros(&remainderHead,
                             &remainderTail);

        while (compare_lists(remainderHead,
                             divisorHead) >= 0)
        {
            Dlist *newRemHead = NULL;
            Dlist *newRemTail = NULL;

            if (subtract_lists(remainderHead,
                               remainderTail,
                               divisorHead,
                               divisorTail,
                               &newRemHead,
                               &newRemTail) == FAILURE)
            {
                free_list(&remainderHead,
                          &remainderTail);

                free_list(&divisorHead,
                          &divisorTail);

                return FAILURE;
            }

            free_list(&remainderHead,
                      &remainderTail);

            remainderHead = newRemHead;
            remainderTail = newRemTail;

            quotientDigit++;
        }

        /*
           Don't print leading zero quotient digits.

           But if the entire quotient is zero,
           print one zero.

           Example:

           1 / 8 -> 0.125
        */

        if (quotientDigit != 0 || started)
        {
            printf("%d", quotientDigit);
            started = 1;
        }

        temp = temp->next;
    }

    /*
       If integer part was zero,
       print zero before decimal point.
    */

    if (!started)
    {
        printf("0");
    }

    /*
       Generate decimal digits.
    */

    if (remainderHead != NULL &&
        !(remainderHead->data == 0 &&
          remainderHead == remainderTail))
    {
        printf(".");

        while (decimal_count < decimal_places)
        {
            quotientDigit = 0;

            /*
               Multiply remainder by 10.

               Example:

               1 / 8

               remainder = 1

               1 * 10 = 10
            */

            if (insert_at_last(&remainderHead,
                               &remainderTail,
                               0) == FAILURE)
            {
                free_list(&remainderHead,
                          &remainderTail);

                free_list(&divisorHead,
                          &divisorTail);

                return FAILURE;
            }

            remove_leading_zeros(&remainderHead,
                                 &remainderTail);

            while (compare_lists(remainderHead,
                                 divisorHead) >= 0)
            {
                Dlist *newRemHead = NULL;
                Dlist *newRemTail = NULL;

                if (subtract_lists(remainderHead,
                                   remainderTail,
                                   divisorHead,
                                   divisorTail,
                                   &newRemHead,
                                   &newRemTail) == FAILURE)
                {
                    free_list(&remainderHead,
                              &remainderTail);

                    free_list(&divisorHead,
                              &divisorTail);

                    return FAILURE;
                }

                free_list(&remainderHead,
                          &remainderTail);

                remainderHead = newRemHead;
                remainderTail = newRemTail;

                quotientDigit++;
            }

            printf("%d", quotientDigit);

            decimal_count++;

            /*
               Stop if remainder becomes zero.
            */

            if (remainderHead != NULL &&
                remainderHead->data == 0 &&
                remainderHead == remainderTail)
            {
                break;
            }
        }
    }

    printf("\n");

    free_list(&remainderHead,
              &remainderTail);

    free_list(&divisorHead,
              &divisorTail);

    return SUCCESS;
}