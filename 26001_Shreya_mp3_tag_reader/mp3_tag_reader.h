#ifndef TAG_READER_H
#define TAG_READER_H
#define SUCCESS 1
#define FAILURE 0

int check_version(char*);
void view_content(char*filename);
int edit_content(char*option,char*new_value,char*filename);
#endif