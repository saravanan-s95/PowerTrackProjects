#ifndef MAIN_H
#define MAIN_H

#define SUCCESS 1
#define FAILURE 0

#define TRUE    1
#define FALSE   0

extern FILE* dst_fp;
extern FILE* src_fp;

typedef enum {
    DATA_KEYWORD,
    NON_DATA_KEYWORD,
    CONSTANT,
    UNKNOWN
} TokenType;

extern int line_print_mode;
extern char input_file[50];
extern char output_file[50];


int validate_inputs(int argc, char** argv);
int validate_input_filename(char* input_file_name);
int validate_output_filename(char* output_file_name);

void html_begin(FILE* dest_fp);
void html_end(FILE* dest_fp);


int get_character_from_file(void);
int separate_token(char ch);
void categorizeToken(char* token);

int is_token_delimiter(char ch);
int is_data_Keyword(const char* token);
int is_non_data_Keyword(const char* token);
int isConstant(const char* token);

void print_pre_processor_to_html_file(char* str);
void print_comment_lines_to_html_file(char* str);
void print_string_literals_to_html_file(void);


int get_character_from_file_number(void);
int separate_token_number(char ch);
void print_pre_processor_to_html_file_number(char* str);
void print_comment_lines_to_html_file_number(char* str);

#endif