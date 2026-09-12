#ifndef HEADER_H

#define HEADER_H

#define FAILURE 0
#define SUCCESS 1
#define MEMORY_NOT_ALLOCATED 2

#define NORMAL_SUBT     11
#define SWAP_AND_SUBT   12

#define GREATER_1       21
#define GREATER_2       22

#define LIST_1_GREATER  31
#define LIST_2_GREATER  32
#define LIST_EQUAL      33

#define POSITIVE        0
#define NEGATIVE        1

typedef struct node
{
    struct node* prev;
    int digit;
    struct node* next;
}apc_digit;


int validate_operands(char* operand);
int validate_operator(char* operator);

int print_dll(apc_digit* head1, apc_digit* tail1);
int print_dll_as_number(apc_digit* head, apc_digit* tail);

int convert_cla_to_dll(char* argv, apc_digit** head, apc_digit** tail);
void remove_leading_zeroes(apc_digit** head, apc_digit** tail);
int insert_to_dll(int value, apc_digit** res_head, apc_digit** res_tail);

void free_list(apc_digit* head);

int find_sign(char* operand1, char* operand2);

void Add(apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);
void sign_based_addition_select(char* argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);

void Subtract(char*argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);
int find_how_to_subt(char* argv[]);
void sign_based_subtraction_select(char* argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);

void Division(char*argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);
int find_greater_operand(char* argv[]);
int compare_lists(apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2);

void Multiplication(char*argv[], apc_digit* head1, apc_digit* tail1, apc_digit* head2, apc_digit* tail2, apc_digit** res_head, apc_digit** res_tail);


#endif

