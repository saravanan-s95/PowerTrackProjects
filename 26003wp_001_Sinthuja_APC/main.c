#include "apc.h"

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Usage: ./a.out <num1> <op> <num2>\n");
        return 1;
    }

    Dlist *head1 = NULL;
    Dlist *tail1 = NULL;

    Dlist *head2 = NULL;
    Dlist *tail2 = NULL;

    Dlist *res_head = NULL;
    Dlist *res_tail = NULL;

    int sign1 = get_sign(argv[1]);
    int sign2 = get_sign(argv[3]);

    char *num1 = skip_sign(argv[1]);
    char *num2 = skip_sign(argv[3]);

    create_list(&head1,&tail1,num1);
    create_list(&head2,&tail2,num2);

    char op = argv[2][0];

    int result_sign = 1;

    switch(op)
    {
        case '+':
        {
            if(sign1 == sign2)
            {
                addition(head1,tail1,
                         head2,tail2,
                         &res_head,&res_tail);

                result_sign = sign1;
            }
            else
            {
                int cmp = compare_lists(head1,head2);

                if(cmp >= 0)
                {
                    subtraction(head1,tail1,
                                head2,tail2,
                                &res_head,&res_tail);

                    result_sign = sign1;
                }
                else
                {
                    subtraction(head2,tail2,
                                head1,tail1,
                                &res_head,&res_tail);

                    result_sign = sign2;
                }
            }
            break;
        }

        case '-':
        {
            if(sign1 != sign2)
            {
                addition(head1,tail1,
                         head2,tail2,
                         &res_head,&res_tail);

                result_sign = sign1;
            }
            else
            {
                int cmp = compare_lists(head1,head2);

                if(cmp >= 0)
                {
                    subtraction(head1,tail1,
                                head2,tail2,
                                &res_head,&res_tail);

                    result_sign = sign1;
                }
                else
                {
                    subtraction(head2,tail2,
                                head1,tail1,
                                &res_head,&res_tail);

                    result_sign = -sign1;
                }
            }
            break;
        }

        case '*':
        {
            multiplication(head1,tail1,
                           head2,tail2,
                           &res_head,&res_tail);

            result_sign = sign1 * sign2;
            break;
        }

        case '/':
        {
            division(head1,tail1,
                     head2,tail2,
                     &res_head,&res_tail);

            result_sign = sign1 * sign2;
            break;
        }

        default:
            printf("Invalid Operator\n");
            return 1;
    }

    if(result_sign == -1)
        printf("-");

    print_list(res_head);
    printf("\n");

    return 0;
}