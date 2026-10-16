#include <stdio.h>
#include <string.h>
#define MAX_STUDENTS 5
#define SQUARE(x) ((x) * (x))

/* Student record
   used to demo the lexer */
typedef struct
{
    char name[20];
    int marks;
    float average;
} student_t;

enum grade { FAIL, PASS, DISTINCTION };

static int total_count = 0;   // global counter

int square_sum(int a, int b)
{
    return SQUARE(a) + SQUARE(b);
}

int main(void)
{
    int scores[MAX_STUDENTS] = {72, 85, 90, 64, 58};
    student_t s1;
    student_t *ptr = &s1;
    float avg = 0.0;
    char grade_char = 'A';
    const char *msg = "Results:\n";
    unsigned int flags = 0;
    int i;

    strcpy(s1.name, "Asha");
    ptr->marks = 0;
    printf("%s", msg);

    for (i = 0; i < MAX_STUDENTS; i++)
    {
        ptr->marks += scores[i];
        total_count++;
        if (scores[i] >= 90 && scores[i] <= 100)
            flags |= 1;
        else if (scores[i] < 60 || scores[i] == 0)
            flags <<= 1;
    }

    if (ptr != NULL)
        flags &= ~1;

    avg = (float)ptr->marks / MAX_STUDENTS;

    switch (grade_char)
    {
        case 'A':
            printf("Top grade\n");
            break;
        default:
            printf("Other\n");
    }

    int result = (avg > 60.5) ? PASS : FAIL;
    while (i-- > 0)
        total_count -= 1;

    printf("Average: %.2f, Result: %d, Flags: %u\n", avg, result, flags);
    printf("Sum: %d, Size: %zu\n", square_sum(3, 4), sizeof(scores) / sizeof(scores[0]));
    return 0;
}
